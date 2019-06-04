package hw1.app;

import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.io.File;
import java.net.URL;
import java.net.URLClassLoader;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.List;
import java.util.Vector;

import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JSlider;

import hw1.model.Direction;
import hw1.model.RobotPilot;
import hw1.model.SimulatorState;
import hw1.model.World;
import hw1.model.WorldFactory;

class SimulatorApp {
	private RobotPilot robot;
	private SimulatorEngine simulator;
	private World world;
	private World cloneWorld;

	private WorldPanel worldPanel;
	private JButton moveButton, runButton, loadWorldButton;
	private JSlider speedSlider;
	private JComboBox<RobotPilot> pilotCombo;
	private JComboBox<Integer> qualityCombo;
	private JComboBox<Integer> rangeCombo;
	private JFrame frame;
	private JFileChooser chooser = new JFileChooser();
	private RobotRunner runner = new RobotRunner();
	private DataFrame dataFrame = new DataFrame();

	public static final int MAX_DEPTH = 6;
	public int depth = 0;

	public boolean isClassFile(File f) {
		return f.getAbsolutePath().endsWith(".class");
	}

	public void loadPilots(File root, ClassLoader loader, List<RobotPilot> pilots) {
		if (root == null || depth > MAX_DEPTH) {
			return;
		}

		if (root.isDirectory()) {
			for (File f : root.listFiles()) {
				try {
					URLClassLoader newLoader = null;
					newLoader = new URLClassLoader(new URL[] { new URL(root.toURI().toString()) }, loader);

					depth++;
					loadPilots(f, newLoader, pilots);
					depth--;
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		} else if (isClassFile(root)) {
			Class<?> newClass = null;
			Enumeration<String> names = new ClassNameEnumeration(root);
			while (newClass == null && names.hasMoreElements()) {
				String className = names.nextElement();
				try {
					newClass = loader.loadClass(className);
					if (RobotPilot.class.isAssignableFrom(newClass)) {
						pilots.add((RobotPilot) newClass.newInstance());
					}
				} catch (Error e) {
				} catch (Exception e2) {
				}
			}
		}
	}

	public static final class ClassNameEnumeration implements Enumeration<String> {
		ArrayList<String> names = new ArrayList<String>();
		int index;

		public String toClassName(String name) {
			int dot = name.lastIndexOf('.');
			if (dot < 0) {
				return null;
			}
			if (!name.substring(dot).equals(".class")) {
				return null;
			}
			return name.substring(0, dot);
		}

		public ClassNameEnumeration(File f) {
			String name = toClassName(f.getName());
			if (name == null) {
				index = 0;
				return;
			} else {
				names.add(name);
			}

			f = new File(f.getParent());
			while (f != null && f.getParent() != null) {
				name = f.getName() + "." + name;
				names.add(name);
				f = new File(f.getParent());
			}

			index = 0;
		}

		@Override
		public boolean hasMoreElements() {
			return index != names.size();
		}

		@Override
		public String nextElement() {
			return names.get(index++);
		}
	}

	private JComboBox<RobotPilot> getPilotChooser() {
		File f = new File( SimulatorApp.class.getResource("").getPath() );
		f = f.getParentFile(); // takes us out of the 'app' into the main package 'robotpilot'
		List<RobotPilot> pilots = new ArrayList<RobotPilot>();
		loadPilots(f, this.getClass().getClassLoader(), pilots);
		Vector<RobotPilot> v = new Vector<>(pilots);
		return new JComboBox<RobotPilot>(v);
	}

	public String getMoveMessage(Direction dir) {
		switch (dir) {
		case NORTH:
			return "moving North";
		case SOUTH:
			return "moving South";
		case EAST:
			return "moving East";
		case WEST:
			return "moving West";
		default:
			return "Invalid move";
		}
	}

	public int getPathLength() {
		return simulator.getCost();
	}

	private class RobotRunner implements Runnable {
		private boolean shouldStop = false;

		public void setShouldStop(boolean f) {
			shouldStop = f;
		}

		public void run() {
			shouldStop = false;
			try {
				while (!shouldStop && robot != null && world != null && simulator != null && !simulator.shouldTerminate()) {					
					SimulatorState state = simulator.move();
					dataFrame.setSimState(state);					
					worldPanel.repaint();
					try {
						Thread.sleep(speedSlider.getValue());
					} catch (Exception exc) {
					}
				}
			} catch (Exception e) {
				JOptionPane.showMessageDialog(frame, "your pilot messed up:\n" + getStackTrace(e), "Pilot Generated Exception",
						JOptionPane.ERROR_MESSAGE);
			}
			if (runButton.getText().equalsIgnoreCase("stop"))
				runButton.doClick();
		}
	}

	private class RunRobot implements ActionListener {
		public void actionPerformed(ActionEvent ev) {
			if (ev.getActionCommand().equalsIgnoreCase("Run")) {
				runner.setShouldStop(false);
				runButton.setText("Stop");
				moveButton.setEnabled(false);
				pilotCombo.setEnabled(false);
				loadWorldButton.setEnabled(false);
				simulator.setCost(0);
				new Thread(runner).start();
			} else {
				runner.setShouldStop(true);
				runButton.setText("Run");
				moveButton.setEnabled(true);
				pilotCombo.setEnabled(true);
				loadWorldButton.setEnabled(true);
			}
		}
	}

	private class MoveRobot implements ActionListener {
		public void actionPerformed(ActionEvent ev) {
			if (robot == null || world.hasReachedGoal()) {
				return;
			}

			try {
				// before the move
				SimulatorState state = simulator.move();				
				dataFrame.setSimState(state);
				// after the move
			} catch (Exception e) {
				JOptionPane.showMessageDialog(worldPanel, "your pilot messed up:\n" + getStackTrace(e), "Pilot Generated Exception",
						JOptionPane.ERROR_MESSAGE);
			}
			worldPanel.repaint();
		}
	}
	
	private class QualityChanged implements ItemListener {
		public void itemStateChanged( ItemEvent ev ) {
			int quality = (int)qualityCombo.getSelectedItem();
			simulator.setQuality( quality );
		}
	}
	
	private class RangeChanged implements ItemListener {
		public void itemStateChanged( ItemEvent ev ) {
			int range = (int)rangeCombo.getSelectedItem();
			simulator.setRange( range );
		}
	}

	private class LoadPilot implements ItemListener {
		public void itemStateChanged(ItemEvent ev) {
			robot = (RobotPilot) pilotCombo.getSelectedItem();
			
			if (world == null) {
				return;
			}

			try {
				world = cloneWorld.copy();
				worldPanel.setWorld( world );
			} catch(Exception e) {
				JOptionPane.showMessageDialog(worldPanel, "Error loading world.  Probably an incorrect file format\n" + getStackTrace(e), "Error",
						JOptionPane.ERROR_MESSAGE );
			}
			
			try {
				int quality = (int)qualityCombo.getSelectedItem();
				int range = (int)rangeCombo.getSelectedItem();
				simulator = new SimulatorEngine(world, robot, quality, range);
			} catch (Exception e) {
				JOptionPane.showMessageDialog(worldPanel, "your pilot messed up:\n" + getStackTrace(e), "Pilot Generated Exception",
						JOptionPane.ERROR_MESSAGE);
			}
			worldPanel.repaint();
		}
	}
	
	

	private String getStackTrace(Exception e) {
		StringBuffer buffer = new StringBuffer();
		buffer.append(e);
		buffer.append('\n');
		for (StackTraceElement element : e.getStackTrace()) {
			buffer.append(element);
			buffer.append('\n');
		}
		return buffer.toString();
	}

	private class LoadWorld implements ActionListener {
		public void actionPerformed(ActionEvent ev) {
			int returnVal = chooser.showOpenDialog(frame);
			if (returnVal == JFileChooser.APPROVE_OPTION) {
				try {
					world = WorldFactory.createBoundedGridWorld(chooser.getSelectedFile());
					cloneWorld = world.copy();
				} catch(Exception e) {
					JOptionPane.showMessageDialog(worldPanel, "Error loading world.  Probably an incorrect file format\n" + getStackTrace(e), "Error",
							JOptionPane.ERROR_MESSAGE );
					return;
				}
				
				try {
					int quality = (int)qualityCombo.getSelectedItem();
					int range = (int)rangeCombo.getSelectedItem();
					simulator = new SimulatorEngine(world, robot, quality, range);
					worldPanel.setWorld(world);
					worldPanel.repaint();
				} catch (Exception e) {
					JOptionPane.showMessageDialog(worldPanel, "your pilot messed up:\n" + getStackTrace(e), "Pilot Generated Exception",
							JOptionPane.ERROR_MESSAGE);
				}
			}
		}
	}

	public static void main(String[] args) {
		new SimulatorApp();
	}
	

	public SimulatorApp() {
		frame = new JFrame("Silly Robot World");
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		world = null;
		worldPanel = new WorldPanel(null);
		frame.add(worldPanel, BorderLayout.CENTER);

		JPanel buttons = new JPanel(new GridLayout(1, 5));
		loadWorldButton = new JButton("Load World");
		loadWorldButton.addActionListener(new LoadWorld());
		buttons.add(loadWorldButton);

		pilotCombo = getPilotChooser();
		LoadPilot p = new LoadPilot();
		pilotCombo.addItemListener(p);
		buttons.add(pilotCombo);
		p.itemStateChanged(null);

		qualityCombo = new JComboBox<Integer>(new Integer[]{0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100});
		qualityCombo.setSelectedIndex(10);
		qualityCombo.addItemListener( new QualityChanged() );
		buttons.add(qualityCombo);
		
		rangeCombo = new JComboBox<Integer>(new Integer[]{1, 2, 3, 4, 5, 6, 7, 8});
		rangeCombo.setSelectedIndex( 3);
		rangeCombo.addItemListener(new RangeChanged());
		buttons.add(rangeCombo);
		
		moveButton = new JButton("Move");
		moveButton.addActionListener(new MoveRobot());
		buttons.add(moveButton);

		runButton = new JButton("Run");
		runButton.addActionListener(new RunRobot());
		buttons.add(runButton);

		speedSlider = new JSlider(10, 500, 75);
		frame.add(speedSlider, BorderLayout.SOUTH);

		frame.add(buttons, BorderLayout.NORTH);
		frame.setBounds(10, 10, 800, 800);
		frame.setVisible(true);
		
		//dataFrame.setBounds(10, 10, 250, 400);
		dataFrame.pack();
		dataFrame.setVisible(true);
	}
}
