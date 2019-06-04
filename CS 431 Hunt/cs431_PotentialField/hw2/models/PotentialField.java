package hw2.models;

public interface PotentialField extends HasPosition {
    public AbstractVector2D getPotential(double x, double y);
    public double getX();
    public double getY();
    public void setX(double x);
    public void setY(double y);
    public DropoffFunction getDropoffFunction();
    public String getLabel();
}

