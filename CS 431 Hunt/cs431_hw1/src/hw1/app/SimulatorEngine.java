package hw1.app;

import java.io.IOException;
import java.util.Random;

import hw1.model.Action;
import hw1.model.Direction;
import hw1.model.EgoCentricDirection;
import hw1.model.Location;
import hw1.model.RobotPilot;
import hw1.model.Sensors;
import hw1.model.SimulatorState;
import hw1.model.World;

public class SimulatorEngine {
	private World world;
	private RobotPilot robot;
	private Random random;
	private int cost = 0;
	private int sensorQuality;
	private int sensorRange;
	
	public void setQuality( int q ) {
		this.sensorQuality = q;
	}
	
	public void setRange( int r ) {
		this.sensorRange = r;
	}
	
	public String getSummary() {
		return robot.getClass().getName() + ":" + sensorQuality + ":" + sensorRange + ":" + world.getName();
	}
	
	public static final int MAX_COST = 40000;

	private int cost(Action action) {
		Direction d = world.getHeading();
		switch (action) {
		case FORWARD:
			return 3 + (world.canMove(d) ? 0 : 5);
		case REVERSE:
			return 4 + (world.canMove(d.back()) ? 0 : 5);
		case TURN_LEFT:
			return 2;
		case TURN_RIGHT:
			return 2;
		case SIT:
			return 1;
		}

		return Integer.MAX_VALUE;
	}

	public Direction toGlobal(EgoCentricDirection d) {
		Direction dir = world.getHeading();
		switch (d) {
		case FRONT:
			return dir;
		case BACK:
			return dir.back();
		case LEFT:
			return dir.left();
		case RIGHT:
			return dir.right();
		}
		return null;
	}

	private class SimulatorSensors implements Sensors {
		private double front, back, left, right;
		private int range, quality;

		public SimulatorSensors(int range, int quality) {
			this.range = range;
			this.quality = quality;
			Location loc = world.robotLocation();
			Direction dir = toGlobal(EgoCentricDirection.FRONT);
			front = getDistance(dir, loc);

			dir = toGlobal(EgoCentricDirection.BACK);
			back = getDistance(dir, loc);

			dir = toGlobal(EgoCentricDirection.LEFT);
			left = getDistance(dir, loc);

			dir = toGlobal(EgoCentricDirection.RIGHT);
			right = getDistance(dir, loc);
		}

		public int getRange() {
			return this.range;
		}

		public double getQuality() {
			return this.quality;
		}

		private double getDistance(Direction d, Location loc) {
			double distance = 0;

			while ((world.isEmpty(loc) || world.isGoal(loc) || world.isRobot(loc)) && distance < range) {
				loc = loc.get(d);
				distance += 1;
			}

			distance = Math.min(distance, range) / (double) range; // normalize.  distance is now in [0, 1]
			double error = random.nextGaussian(); 
			error = error * ((100 - getQuality()) / 100.0); // scale the error downward by quality
			if (error < 0) {
				error = Math.max(-.95, error); // truncate if necessary
			} else {
				error = Math.min(.95, error);
			}
			
			error = ( error / 3 ) / range; // scale down by range

			//double temp = Math.abs(error);
			//maxError = Double.isNaN(maxError) ? temp : Math.max(maxError, temp);
			return distance + error;
		}

		public double front() {
			return front;
		}

		public double left() {
			return left;
		}

		public double right() {
			return right;
		}

		public double back() {
			return back;
		}

		public String toString() {
			return "[range=" + getRange() + ", stdev=" + getQuality() + ", front=" + front + ", back=" + back + ", left=" + left
					+ ", right=" + right + "]";
		}
	}

	public SimulatorEngine(World world, RobotPilot pilot, int quality, int range) throws IOException {
		this.world = world;
		this.robot = pilot;
		this.sensorQuality = quality;
		this.random = new Random();
		this.sensorRange = range;
		Location robotLocation = world.robotLocation();
		Location goalLocation = world.goalLocation();
		robot.init( goalLocation.row - robotLocation.row, goalLocation.col - robotLocation.col);
	}

	public SimulatorState move() {
		if (world.hasReachedGoal())
			return new SimulatorState(world, cost, null, null);

		SimulatorSensors sensors = new SimulatorSensors(sensorRange, sensorQuality);
		Action action = robot.act(sensors);		
//		System.out.println("BEFORE MOVING");
//		Location loc = world.robotLocation();
//		System.out.println("FRONT: " + sensors.front());
//		System.out.println("RIGHT: " + sensors.right());
//		System.out.println("BACK: " + sensors.back() );
//		System.out.println("LEFT:" + sensors.left() );
//		loc = loc.get(Direction.NORTH).get(Direction.WEST );
//		Location temp = loc;
//		for( int row=0; row < 3; row ++ ) {
//			for( int i = 0; i < 3; i ++ ) { 
//				System.out.print(world.get(temp));
//				temp = temp.get(Direction.EAST);
//			}
//			System.out.println();
//			loc = loc.get(Direction.SOUTH);
//			temp = loc;
//		}		
//		System.out.println(action + "\n\n\n");
		
		cost += cost(action);
		world.applyAction(action);
		//System.out.println(action + ":" + cost + ":" + world);
		return new SimulatorState(world, cost, action, sensors);
	}

	public int getCost() {
		return cost;
	}

	public void setCost(int cost) {
		this.cost = cost;
	}

	public boolean shouldTerminate() {
		return world.hasReachedGoal() || cost >= MAX_COST;
	}

	public static class PilotThread extends Thread {
		public int localCost;
		public long lastTime;
		public long lastDelta;
		public SimulatorEngine engine;
		public boolean stop = false;
		public int maxTime = 0;
		public static int MAX_TIME = 120000;
		public static int MAX_COST = 50000;
		public String message = "NORMAL";

		public PilotThread(SimulatorEngine engine) {
			lastTime = lastDelta = localCost = 0;
			this.engine = engine;
		}
		
		public String summary() {
			return engine.getSummary() +  ":" + localCost + ":" + maxTime + ":" + message;
		}
		
		public int getTimeForLastMove() {
			return (int)(System.currentTimeMillis() - lastTime);
		}
		
		public boolean shouldTerminate() {
			return engine.world.hasReachedGoal() || localCost >= this.MAX_COST;
		}
			
		public void run() {
			lastTime = System.currentTimeMillis();
			while (!shouldTerminate() ) {
				Action action = engine.robot.act(engine.new SimulatorSensors(engine.sensorRange, engine.sensorQuality));				
				localCost += engine.cost(action);
				maxTime = Math.max(maxTime, getTimeForLastMove());
				lastTime = System.currentTimeMillis();
				if(maxTime >= MAX_TIME) {
					message = "STOPPED";
					break;
				}
				engine.world.applyAction(action);
			}
			
			if(!engine.world.hasReachedGoal()) {
				localCost = (int)(1.25 * this.MAX_COST);			
			}			
		}
		
		public Class<?> getPilotClass() {
			return engine.robot.getClass();
		}		
	}

	public PilotThread getPilotThread() {
		return new PilotThread(this);
	}
		
	public int simulate() {
		while (!shouldTerminate()) {
			Action action = robot.act(new SimulatorSensors(sensorRange, sensorQuality));
			cost += cost(action);
			world.applyAction(action);

		}
		return cost;
	}
}
