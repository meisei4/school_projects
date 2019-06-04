package hw2.models;

public class ExponentialDropOff implements DropoffFunction{
	private double ea, eb;
	public ExponentialDropOff(double ea, double eb) {
		this.ea = ea;
		this.eb = eb;
	}
	
	public double getMagnitude(double distance) {
		return ea * Math.pow(Math.E, -1 * eb * distance);
	}
	
	public String toString(){
		return 	"EXPONENTIAL" + 
				"\nEA = " + ea +
				"\nEB = " + eb; 
	}
}
