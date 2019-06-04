package hw2.ui;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import javax.swing.JComponent;

import hw2.models.DropoffFunction;
import hw2.models.PotentialField;

public class PotentialFieldComponent extends JComponent {
    public static final int WIDTH = 20, HEIGHT=20;
    protected PotentialField field;

    public double getCenterX() {
	return field.getX();
    }

    public double getCenterY() {
	return field.getY();
    }

    public double distance(double x, double y) {
	double dx = getCenterX()-x;
	double dy = getCenterY()-y;
	return Math.sqrt(dx*dx + dy*dy);
    }

    public DropoffFunction getFunction() {
	return field.getDropoffFunction();
    }

    PotentialFieldComponent(PotentialField f) {
	field = f;
	setBounds((int)(getCenterX()-WIDTH/2), 
		  (int)(getCenterY()-HEIGHT/2),
		  WIDTH, 
		  HEIGHT);
    }

    public void paintComponent(Graphics gin){ 
	Graphics2D g = (Graphics2D)gin;
	g.setColor(Color.red);
	g.fillOval(0, 0, getWidth(), getHeight());
	g.setColor(Color.black);
	g.drawString(field.getLabel(), 1, 15);
    }

    public void setLocation(int x, int y) {
	double dx = x - getX();
	double dy = y - getY();
	field.setX(getCenterX() + dx);
	field.setY(getCenterY() + dy);
	super.setLocation(x,y);
    }
}
