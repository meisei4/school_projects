package hw2.models;

public class RandomPotentialField extends AbstractPotentialField{
	private double change;
	
	public RandomPotentialField(double x, double y, double change, DropoffFunction drop) {
		super(x, y, drop);
		this.change = change;
	}
	
	public AbstractVector2D getPotential(double x, double y){
		double xd = x - getX();
		double yd = y - getY();
		double distance = Math.sqrt(xd*xd + yd*yd);
		double mag = getDropoffFunction().getMagnitude(distance);
		double orient = Math.random() * change;
		double scaledX = mag * Math.cos(orient);
		double scaledY = mag * Math.sin(orient);
		CartesianVector2D potential = new CartesianVector2D(scaledX, scaledY);
		return potential;
	}
	
	public String getLabel() {
		return "RANDOM";
	}

	public String toString(){
		return 	"TYPE: " + getLabel() + 
				"\nDROPOFF: " + getDropoffFunction().toString() + 
				"\nCHANGE: " + change + 
				"\nX = " + getX() + ", Y = " + getY();
	}
}
