package hw2.ui;
import java.awt.Color;
import java.awt.Graphics;
import javax.swing.JComponent;

public class RobotComponent extends JComponent {
    public static final int WIDTH = 25, HEIGHT=25;    
    public double percent, totalPath, radius;

    public double getCenterX() {
	return getX() + getWidth()/2;
    }

    public double getCenterY() {
	return getY() + getHeight()/2;
    }

    RobotComponent(double x, double y, double delta, double total, double radius) {
	setBounds((int)x, (int)y, WIDTH, HEIGHT);
	percent = delta;
	totalPath = total;
	this.radius = radius;
    }

    public void setPercent(double p) {
	percent = p;
    }

    public void setPathLength(double p) {
	totalPath = p;
    }

    public void setRadius(double r) {
	radius = r;
    }

    public double getPercentage() {
	return percent;
    }

    public double getPathLength() {
	return totalPath;
    }

    public double getRadius() {
	return radius;
    }

    public void paintComponent(Graphics g) {
	g.setColor(Color.green);
	g.fillRect(0, 0, getWidth(), getHeight());
    }
}
