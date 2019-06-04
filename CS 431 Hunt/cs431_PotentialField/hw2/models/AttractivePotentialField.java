package hw2.models;

public class AttractivePotentialField extends AbstractPotentialField{

	public AttractivePotentialField(double x, double y, DropoffFunction drop) {
		super(x, y, drop);
	}

	public AbstractVector2D getPotential(double x, double y){
		double xd = x - getX();
		double yd = y - getY();
		double distance = Math.sqrt(xd*xd + yd*yd);
		double mag = getDropoffFunction().getMagnitude(distance);
		double orient = Math.atan2(y - getY(), x - getX()) + Math.PI;
		double scaledX = mag * Math.cos(orient);
		double scaledY = mag * Math.sin(orient);
		CartesianVector2D potential = new CartesianVector2D(scaledX, scaledY);
		return potential;
	}
	
	public String getLabel() {
		return "ATTRACTIVE";
	}
	
	public String toString(){
		return "TYPE: " + getLabel() + 
				"\nDROPOFF: " + getDropoffFunction().toString() + 
				"\nX = " + getX() + ", Y = " + getY();
	}
}

