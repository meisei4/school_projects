package hw2.models;


public abstract class AbstractVector2D implements Vector2D {
	/**
	 * This method creates a new vector that is the sum of this Vector2D and 
	 * the right-hand-side (rhs) Vector2D.
	 * 
	 * @param rhs the other vector. 
	 */
    public Vector2D add(Vector2D rhs) {
        return new CartesianVector2D(getX() + rhs.getX(), getY() + rhs.getY());
    }

	/**
	 * This method creates a new vector that is the difference between this Vector2D and 
	 * the right-hand-side (rhs) Vector2D.
	 * 
	 * @param rhs the other vector. 
	 */
    public Vector2D subtract( Vector2D rhs ) {
    	return new CartesianVector2D( getX() - rhs.getX(), getY() - rhs.getY());
    }
}
