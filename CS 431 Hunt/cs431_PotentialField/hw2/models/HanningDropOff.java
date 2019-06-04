package hw2.models;

public class HanningDropOff implements DropoffFunction{
	private double max, a;
	public HanningDropOff(double max, double a) {
		this.max = max;
		this.a = a;
	}
	
	public double getMagnitude(double distance) {
		if(distance <= max){
			return a * (1 - Math.cos(Math.PI * (max - distance)/max))/2;
		}
		return 0;
	}
	
	public String toString(){
		return 	"Hanning" +
				"\nMAX = " + max +
				"\nA = " + a; 
	}
}
