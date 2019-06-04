package hw2.models;

public class TangentialPotentialField extends AbstractPotentialField{
	private String wise;
	public TangentialPotentialField(double x, double y, String wise, DropoffFunction drop) {
		super(x, y, drop);
		this.wise = wise;
	}
	
	public AbstractVector2D getPotential(double x, double y) {
		double orient;
		double xd = x - getX();
		double yd = y - getY();
		double distance = Math.sqrt(xd*xd + yd*yd);
		double mag = getDropoffFunction().getMagnitude(distance);
		if(wise == "CCW"){
			//
			orient = Math.PI/2 + Math.atan2(y - getY(), x - getX());
		}else{
			orient = Math.atan2(y - getY(), x - getX()) - Math.PI/2;
		}
		double scaledX = mag * Math.cos(orient);
		double scaledY = mag * Math.sin(orient);
		CartesianVector2D potential = new CartesianVector2D(scaledX, scaledY);
		return potential;
	}

	public String getLabel() {
		return "TANGENT";
	}
	
	public String toString(){
		return 	"TYPE: " + getLabel() + 
				"\nDROPOFF: " + getDropoffFunction().toString() + 
				"\nWISE: " + wise + 
				"\nX = " + getX() + ", Y = " + getY();
	}
}