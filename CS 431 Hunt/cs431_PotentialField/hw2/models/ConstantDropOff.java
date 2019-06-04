 package hw2.models;

public class ConstantDropOff implements DropoffFunction{
	private double k;
	public ConstantDropOff(double k) {
		this.k = k;
	}
	
	public double getMagnitude(double distance) {
		return k;
	}
	
	public String toString(){
		return 	"CONSTANT" +
				"\nK = " + k;
	}
}
