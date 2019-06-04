package hw2.ui;
import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.io.IOException;
import javax.swing.*;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import hw2.models.FieldManager;
import hw2.models.FieldManagerFactory;

class FieldDemo extends JFrame {
	protected FieldPanel fieldPanel;
	protected JSlider radiusOfViewSlider, pathLengthSlider, percentSlider;

	class SliderListener implements ChangeListener {
		public void stateChanged(ChangeEvent e) {
			RobotComponent r = fieldPanel.getRobot();
			r.setRadius(radiusOfViewSlider.getValue());
			
			r.setPathLength(pathLengthSlider.getValue());
			
			r.setPercent(percentSlider.getValue() / 100.0);
			fieldPanel.repaint();
		}
	}

	public FieldDemo(String filename) throws IOException {
		super("Potential Fields Demo");
		getContentPane().setLayout(new BorderLayout());
		FieldManager manager = FieldManagerFactory.getFieldManager(filename);
		fieldPanel = new FieldPanel(manager);
		fieldPanel.setRobot(new RobotComponent(200, 200, .1, 800, 400));
		getContentPane().add(fieldPanel, BorderLayout.CENTER);

		ChangeListener sliderListener = new SliderListener();
		// make sliders
		JPanel sliders = new JPanel();
		sliders.setBorder(BorderFactory.createEtchedBorder());
		sliders.setLayout(new GridLayout(3, 2));
		
		sliders.add(new JLabel("Robot Path Length"));
		pathLengthSlider = new JSlider(0, 5000, 1000);
		pathLengthSlider.addChangeListener(sliderListener);
		sliders.add(pathLengthSlider);
		
		sliders.add(new JLabel("Robot Radius Of View"));
		radiusOfViewSlider = new JSlider(1, 5000, 500);
		radiusOfViewSlider.addChangeListener(sliderListener);
		sliders.add(radiusOfViewSlider);
		sliders.add(new JLabel("Frequency of Observation"));
		percentSlider = new JSlider(0, 1000, 10);
		percentSlider.addChangeListener(sliderListener);
		sliders.add(percentSlider);

		getContentPane().add(sliders, BorderLayout.SOUTH);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}

	public static void main(String[] args) throws IOException {
		String file = args.length == 0 ? "fields1.dat" : args[0];
		FieldDemo demo = new FieldDemo(file);
		demo.setSize(500, 500);
		demo.setVisible(true);
	}
}
