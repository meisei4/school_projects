package hw1.model;

class BoundedGridWorld implements World {
	private char[][] apriori;
	private char[][] cells;
	private Location robotLocation;
	private Location goalLocation;
	private Direction robotHeading;
	private String name;
	
	private BoundedGridWorld() {
	}
	
	public World copy() {
		BoundedGridWorld result = new BoundedGridWorld();
		result.apriori = new char[ getHeight() ][ getWidth() ];
		result.cells = new char[ getHeight() ][ getWidth() ];
		
		for( int r=0; r<getHeight(); r++ ) {
			for( int c=0; c<getWidth(); c++) {
				result.apriori[r][c] = apriori[r][c];
				result.cells[r][c] = cells[r][c];
			}
		}
		
		result.robotLocation = robotLocation;
		result.goalLocation = goalLocation;
		result.robotHeading = robotHeading;
		result.name = name;
		return result;
	}
	
	public String getName() {
		return name;
	}
	
	public void setName(String name) {
		this.name = name;
	}
	
	public void setHeading(Direction heading) {
		robotHeading = heading;
	}
	
	public Direction getHeading() {
		return this.robotHeading;
	}
	
	public boolean canDo(Action action) {
		if(action == Action.FORWARD) {
			return canMove(robotHeading);
		} else if(action == Action.REVERSE) {
			return canMove(robotHeading.back());
		} else {
			return true;
		}
	}
	
	@Override
	public void applyAction(Action action) {
		if(canDo(action)) {
			switch(action) {
				case TURN_LEFT: robotHeading = robotHeading.left(); break;
				case TURN_RIGHT: robotHeading = robotHeading.right(); break;
				case FORWARD: move(robotHeading); break;
				case REVERSE: move(robotHeading.back()); break;
				case SIT: break;				
			}
		}
	}

	public char[][] getApriori() {
		int halfWidth = Math.max( this.robotLocation.col, getWidth() - this.robotLocation.col );
		int halfHeight = Math.max( this.robotLocation.row, getHeight() - this.robotLocation.row );
		char[][] result = new char[ halfHeight * 2 + 1 ][ halfWidth * 2 + 1];
		int row = this.robotLocation.row - halfHeight;
		int col = this.robotLocation.col - halfWidth;
		for( int r = 0; r < halfHeight * 2 + 1; r++ ){
			for( int c = 0; c < halfWidth * 2 + 1; c++ ) {
				try {
					result[r][c] = apriori[ row + r][col + c];
				} catch(Exception e) {
					result[r][c] = UNKNOWN;
				}
			}
		}
		
		return result;
	}
	
	BoundedGridWorld(int rows, int cols) {
		cells = new char[rows][cols];
		apriori = new char[rows][cols];
		robotHeading = Direction.NORTH;
	}
	
	
	public void setApriori( Location loc, char value ) {
		if( value == ROBOT || value == GOAL ) {
			value = EMPTY;
		}
		apriori[loc.row][loc.col] = value;
	}
	
	public void setCell(Location loc, char value) {
		if(value == ROBOT) {
			robotLocation = loc;
		} else if(value == GOAL) {
			goalLocation = loc;
		}
		
		cells[loc.row][loc.col]= value; 
	}

	@Override
	public char get(Location loc) {
		try {
			return cells[loc.row][loc.col];
		} catch(Throwable t) {
			return World.WALL;
		}
	}
	
	@Override
	public boolean isGoal(Location location) {
		return get(location) == GOAL;
	}

	@Override
	public boolean isEmpty(Location location) {
		return get(location) == EMPTY;
	}

	@Override
	public boolean isWall(Location location) {
		return get(location) == WALL;
	}
	
	@Override	
	public boolean isRobot(Location location) {
		return get(location) == ROBOT;
	}
	
	@Override
	public Location robotLocation() {
		return robotLocation;
	}
	
	public boolean hasReachedGoal() {
		return robotLocation.equals(goalLocation);
	}
	
	public Integer getWidth() {
		return cells[0].length;
	}
	
	public Integer getHeight() {
		return cells.length;
	}
	
	public String toString() {
		StringBuilder result = new StringBuilder();
		int height = getHeight();
		int width = getWidth();
		result.append(height + " " + width);
		for (int row = 0; row < height; row++) {
			for (int col = 0; col < width; col++) {
				result.append(get(new Location(row, col)));
			}
			result.append('\n');
		}
		return result.toString();
	}

	@Override
	public Location goalLocation() {
		return goalLocation;
	}
	
	@Override
	public boolean canMove(Direction dir) {
		Location loc = robotLocation().get(dir);
		return isEmpty(loc) || isGoal(loc);
	}

	@Override
	public void move(Direction dir) {
		if(canMove(dir)) {
			Location loc = robotLocation.get(dir);
			this.setCell(robotLocation, EMPTY);
			this.setCell(loc, ROBOT);
		}
	}

}
