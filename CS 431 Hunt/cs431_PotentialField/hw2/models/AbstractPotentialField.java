package hw2.models;

import java.awt.geom.Point2D;

public abstract class AbstractPotentialField implements PotentialField{
	private double x, y;
	private DropoffFunction drop;
	/** method creates a potential field
	 * 
	 * @param x = x coordinate of field source
	 * @param y = y coordinate of field source
	 * @param drop = drop off function for field
	 */
	public AbstractPotentialField(double x, double y, DropoffFunction drop){
		this.x = x;
		this.y = y;
		this.drop = drop;
	}
	
	public double getX() {
		return x;
	}
	
	public double getY() {
		return y;
	}
	
	public void setX(double x){
		this.x = x;
	}
	
	public void setY(double y){
		this.y = y;
	}
	
	public DropoffFunction getDropoffFunction() {
		return drop;
	}
	
	public Point2D getPosition() {
		return new Point2D.Double(this.x, this.y);
	}

	public void setPosition(Point2D pt) {
		setY(pt.getY());
		setX(pt.getX());
	}
	
	public String toString(){
		return "X: " + x + " Y: " + y;
	}
}
