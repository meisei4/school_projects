package hw2.models;

public class LinearDropOff implements DropoffFunction{
	private double la, max;
	public LinearDropOff(double la, double max) {
		this.la = la;
		this.max = max;
	}
	
	public double getMagnitude(double distance) {
		return Math.max(0, max + la * distance);
	}

	public String toString(){
		return 	"Linear" + 
				"\nLA = " + la +
				"\nMAX = " + max;
	}
}
