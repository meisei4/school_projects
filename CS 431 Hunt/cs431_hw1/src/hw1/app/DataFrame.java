package hw1.app;

import java.awt.GridLayout;

import javax.swing.JFrame;
import javax.swing.JLabel;

import hw1.model.SimulatorState;

@SuppressWarnings("serial")
public class DataFrame extends JFrame {
	private SimulatorState simState;
	private JLabel cost = new JLabel();
	private JLabel[] sensors = new JLabel[4];
	private JLabel action = new JLabel();
	
	public DataFrame() {
		setLayout(new GridLayout(6, 2));
		JLabel label = new JLabel("Cost");
		add(label);
		add(cost);
		
		label = new JLabel("front");
		add(label);
		sensors[0] = new JLabel("");
		add(sensors[0]);
		
		label = new JLabel("right");
		add(label);
		sensors[1] = new JLabel("");
		add(sensors[1]);
		
		label = new JLabel("back");
		add(label);
		sensors[2] = new JLabel("");
		add(sensors[2]);
			
		label = new JLabel("left");
		add(label);
		sensors[3] = new JLabel("");
		add(sensors[3]);
		
		label = new JLabel("action");
		add(label);
		add(action);
		
	}
	
	
	public SimulatorState getSimState() {
		return simState;
	}

	public void setSimState(SimulatorState simState) {
		this.simState = simState;
		if(this.simState != null) {
			cost.setText(simState.getCost().toString());
						
			Double number = simState.getSensors().front();
			number = simState.getSensors() == null ? 0 : number;
			sensors[0].setText(number.toString());			
			
			number = simState.getSensors().right();
			number = simState.getSensors() == null ? 0 : number;			
			sensors[1].setText(number.toString());			
			
			number = simState.getSensors().back();
			number = simState.getSensors() == null ? 0 : number;			
			sensors[2].setText(number.toString());			
			
			number = simState.getSensors().left();
			number = simState.getSensors() == null ? 0 : number;			
			sensors[3].setText(number.toString());

			String act = simState.getLastAction() == null ? "none" : simState.getLastAction().name();
			action.setText(act);
		}
		
		
		repaint();		
	}		
		
}
