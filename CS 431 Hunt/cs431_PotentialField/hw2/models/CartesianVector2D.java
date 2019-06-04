package hw2.models;
import java.lang.Math;
public class CartesianVector2D extends AbstractVector2D{
	private double x, y;
	

	public CartesianVector2D(double x, double y) {
		this.x = x;
		this.y = y;
	}
	
	public Vector2D add(Vector2D rhs) {
		double x2 = this.getX() + rhs.getX();
		double y2 = this.getY() + rhs.getY();
		return new CartesianVector2D(x2, y2);
	}
	
	public Vector2D subtract(Vector2D rhs) {
		double x2 = this.getX() - rhs.getX();
		double y2 = this.getY() - rhs.getY();
		return new CartesianVector2D(x2, y2);
	}
	
	public double getMagnitude() {
		return Math.sqrt(x * x + y * y);
	}
	
	public double getOrientation() {
		return Math.atan2(y, x);
	}
	
	public double getX() {
		return x;
	}

	public double getY() {
		return y;
	}
	public String toString(){
		return "X = " + getX() + " Y = " + getY() + 
				"\nORIENTATION = " + getOrientation() + 
				"\nMAGNITUDE  = " + getMagnitude() + "\n";
	}
}
