package hw2.models;

/**
 *  Represents an immutable 2-dimensional vector at some location.
 */
public interface Vector2D {
	/**
	 * This method creates a new vector that is the sum of this Vector2D and 
	 * the right-hand-side (rhs) Vector2D.
	 * 
	 * @param rhs the other vector. 
	 */
    public Vector2D add(Vector2D rhs);
	/**
	 * This method creates a new vector that is the difference between this Vector2D and 
	 * the cright-hand-side (rhs) Vector2D.
	 * 
	 * @param rhs the other vector. 
	 */
    public Vector2D subtract( Vector2D rhs );
    
    /**
     * 
     * @return The magnitude of this vector
     */
    public double getMagnitude();
    
    /**
     * 
     * @return The angle (ie orienation) of this vector given in radians.
     */
    public double getOrientation();
    
    /**
     * 
     * @return The x-coordinate of this vector in Cartesian space.  Assumes an origin of (0, 0).
     */
    public double getX();
    
    /**
     * 
     * @return The y-coordinate of this vector in Cartesian space.  Assumes an origin of (0, 0).
     */
    public double getY();
}
