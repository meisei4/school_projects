package hw2.models;

public interface DropoffFunction{
	/**
	 * This method returns the magnitude of the field
	 * at a distance <code>distance</code> from the
	 * position of the field.
	 * 
	 * @param distance the distance between the field and the location of the force
	 * @return
	 */
    public double getMagnitude(double distance);
    
}
