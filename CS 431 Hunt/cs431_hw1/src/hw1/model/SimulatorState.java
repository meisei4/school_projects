package hw1.model;

public class SimulatorState {
	private World world;
	private Integer cost;
	private Action lastAction;
	private Sensors sensors;
	
	public SimulatorState(World world, Integer cost, Action lastAction, Sensors sensors) {
		super();
		this.world = world;
		this.cost = cost;
		this.lastAction = lastAction;
		this.sensors = sensors;
	}
	
	public World getWorld() {
		return world;
	}
	public void setWorld(World world) {
		this.world = world;
	}
	public Integer getCost() {
		return cost;
	}
	
	public void setCost(Integer cost) {
		this.cost = cost;
	}
	
	public Action getLastAction() {
		return lastAction;
	}
	
	public void setLastAction(Action lastAction) {
		this.lastAction = lastAction;
	}
	
	public Sensors getSensors() {
		return sensors;
	}
	
	public void setSensors(Sensors sensors) {
		this.sensors = sensors;
	}
}
