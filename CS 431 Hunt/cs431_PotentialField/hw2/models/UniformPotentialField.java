package hw2.models;

public class UniformPotentialField extends AbstractPotentialField{
	double orientation;
	
	public UniformPotentialField(double x, double y, double orientation, DropoffFunction drop) {
		super(x, y, drop);
		this.orientation = orientation;
	}

	public AbstractVector2D getPotential(double x, double y) {
		double xd = x - getX();
		double yd = y - getY();
		double distance = Math.sqrt(xd*xd + yd*yd);
		double mag = getDropoffFunction().getMagnitude(distance);
		double scaledX = mag * Math.cos(orientation);
		double scaledY = mag * Math.sin(orientation);
		CartesianVector2D potential = new CartesianVector2D(scaledX, scaledY);
		return potential;
	}
	
	public String getLabel() {
		return "UNIFORM";
	}

	public String toString(){
		return 	"TYPE: " + getLabel() + 
				"\nDROPOFF: " + getDropoffFunction().toString() + 
				"\nORIENTATION: " + orientation + 
				"\nX = " + getX() + ", Y = " + getY();
	}
}