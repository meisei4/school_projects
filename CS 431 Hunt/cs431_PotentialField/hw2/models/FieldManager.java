package hw2.models;
import java.awt.geom.Point2D;
import java.util.Collection;
import java.util.List;

public interface FieldManager {
	/**
	 * @return A collection of all fields in the environment
	 */
    public Collection<PotentialField> getFields();
    
    /**
     * Computes the potential at position (x,y) in the environment taking into
     * consideration all fields.
     * 
     * @param x the x-coordinate of the position where the force is <i>felt</i> or <i>applied</i>.
     * @param y the y-coordinate of the position where the force is <i>felt</i> or <i>applied</i>.
     * @return the potential exerted by all fields at position (x,y)
     */
    public Vector2D getPotential(double x, double y);
    /**
     * Computes the potential at position (x,y) in the environment taking into
     * consideration all fields that are in view of (x,y).
     * 
     * @param x the x-coordinate of the position where the force is <i>felt</i> or <i>applied</i>.
     * @param y the y-coordinate of the position where the force is <i>felt</i> or <i>applied</i>.
     * @param radiusOfView the radius of the circular field of view centered at (x,y)
     * @return the potential exerted by all fields in view at position (x,y)
     * @throws Exception 
     */
    public Vector2D getPotential( double x, double y, double radiusOfView ) throws Exception;
    

    /**
     * Computes the path a robot at position (x,y) will take assuming the robot can see
     * a distance of <code>radiusOfView</code> units away.  The path will be no longer than
     * <code>totalPathLength</code>.  The path is given by an ordered-list of 
     * positions that the robot takes.  Each position denotes a point at which the robot
     * senses the environment and makes a decision about how to move.  
     * Each vector force is scaled by an amount <code>percentageOfField</code>.
     * 
     * @param startX the x-coordinate of the starting position of the robot
     * @param startY the y-coordinate of the starting position of the robot
     * @param percentageOfField a scaling factor that is applied to every vector force in the environment.  
     *        This simulates something like sensing-rate.  Lower values denote <i>better</i> sensing rates.
     * @param radiusOfView the range of the robots sensors
     * @param totalPathLength the maximum length of the robot path returned.
     * @return
     */
    public List<Point2D> getRobotPath(double startX, double startY, double percentageOfField, double radiusOfView, double totalPathLength);
    
    /**
     * 
     * @return the number of fields in the environment
     */
    public int getNumberOfFields();
    
    /**
     * Returns a {@link Collection} of fields in view of the robot at position (x, y).
     * @param x the x-coordinate of the position of the robot
     * @param y the y-coordinate of the position of the robot
     * @param radiusOfView the radius-of-view of the robot.
     * @return The number of fields in view of the robot at position (x, y) where the robot has a <code>radiusOfView</code>.
     */
    public Collection<PotentialField> getFieldsInView(double x, double y, double radiusOfView);
}
