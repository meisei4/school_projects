package hw1.app;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import hw1.model.RobotPilot;
import hw1.model.World;
import hw1.model.WorldFactory;

public class PilotTester {
	private static Class<?> pilotsAll[] = {  hw1.pilots.random.Pilot.class };
	private static Class<?> pilots[] = { hw1.pilots.random.Pilot.class };

	public static List<Integer> getCost(Map<Class<?>, List<Integer>> costs, Class<?> clzz) {
		if (costs.containsKey(clzz)) {
			return costs.get(clzz);
		} else {
			List<Integer> result = new ArrayList<Integer>();
			costs.put(clzz, result);
			return result;
		}
	}

	public static Integer sum(List<Integer> values) {
		Integer total = 0;
		for (Integer val : values) {
			total += val;
		}
		return total;
	}

	public static void runTests(List<SimulatorEngine.PilotThread> threads) {

		while (!threads.isEmpty()) {
			Thread t = threads.remove(0);
			t.start();

		}
	}
	
//	public static class FairRunner extends Thread {
//		List<SimulatorEngine.PilotThread> threads;
//		List<SimulatorEngine.PilotThread> running; 
//		public FairRunner(List<SimulatorEngine.PilotThread> threads, int n) {
//			List<SimulatorEngine.PilotThread> copy = new ArrayList<>();
//			running = new ArrayList<>();
//			Collections.shuffle(copy);
//			for(int i=0; i<n; i++) {
//				SimulatorEngine.PilotThread pt = copy.remove(0);
//				pt.start();
//				running.add(pt);
//			}
//			this.threads = copy;
//		}
//		
//		public synchronized void threadFinished() {
//			if(!threads.isEmpty()) {
//				SimulatorEngine.PilotThread pt = threads.remove(0);
//				pt.start();
//				running.add(pt);
//			}
//		}
//		
//		public void run() {
//			while(!threads.isEmpty() || !running.isEmpty()) {
//				for(SimulatorEngine.PilotThread t : running) {					
//					if (t.isAlive() && t.getTimeForLastMove() > t.MAX_TIME && !t.holdsLock(this)) {
//						t.localCost = 12500;
//						t.message = "PRE-EMPTED";
//						t.stop();
//						
//						System.out.print("killed: " + t.summary());						
//					}
//				}
//			}
//		}
//	}

	public static void main(String[] args) throws InstantiationException, IllegalAccessException, IOException {
		String testFilePrefix = "d://Teaching/cs431/RobotPilotTests/HuntTest";
		String testFileSuffix = ".dat";
		Map<Class<?>, List<Integer>> results = new HashMap<>();
		List<SimulatorEngine.PilotThread> threads = new ArrayList<SimulatorEngine.PilotThread>();

		for (Class<?> clzz : pilotsAll) {
			for (int testNumber = 1; testNumber <= 12; testNumber++) {
				for (int quality = 100; quality >= 40; quality -= 20) {
					for (int range = 2; range <= 10; range += 2) {
						RobotPilot pilot = null;
						try {
							String testNumberStr = String.format("%02d", testNumber);
							String filename = testFilePrefix + testNumberStr + testFileSuffix;
							World world = WorldFactory.createBoundedGridWorld(new File(filename));

							try {
								pilot = ((Class<RobotPilot>) clzz).newInstance();
							} catch (Throwable t) {
								t.printStackTrace();
								System.out.println("failed to create pilot for: " + clzz.getName());
								continue;
							}

							SimulatorEngine engine = new SimulatorEngine(world, pilot, quality, range);
							SimulatorEngine.PilotThread thread = engine.getPilotThread();
							thread.setPriority(Thread.MIN_PRIORITY);
							threads.add(thread);						
						} catch (Throwable t) {
							t.printStackTrace();
							System.out.println("failed to create thread for : " + clzz.getName());
						}
					}
				}
			}
		}

		Thread.currentThread().setPriority(Thread.MAX_PRIORITY);		
		Collections.shuffle(threads);
		for(int i=0; i<threads.size(); i++) {			
			Thread t = threads.get(i);
			t.start();
			if((i+1) % 8 == 0) {
				try {
					Thread.currentThread().sleep(500);
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}			
		}
		
		List<SimulatorEngine.PilotThread> active = new LinkedList<>();
		active.addAll(threads);
		while (!active.isEmpty()) {
			for (int i = 0; !active.isEmpty() && i < 20; i++) {
				SimulatorEngine.PilotThread t = active.remove(0);
				if (t.isAlive() && t.getTimeForLastMove() > t.MAX_TIME) {
					t.localCost = (int)(1.25 *  t.MAX_COST);
					t.message = "PRE-EMPTED";
					t.stop();
					System.out.print("interuppted: " + t.summary());
					System.out.println("  " + active.size() + " threads left to complete.");
				} else if (t.isAlive()) {
					active.add(active.size(), t);
				}
			}
			try {
				Thread.sleep(50);
			} catch(Throwable th) {
				
			}
		}
		
		for(Thread t : threads) {
			try {
				t.join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
								
		File fout = new File("d://pilots_out.txt");
		PrintWriter out = new PrintWriter(new FileWriter(fout));
		for( SimulatorEngine.PilotThread thread : threads) {
			out.println(thread.summary());
		}
		out.close();
	}
}
