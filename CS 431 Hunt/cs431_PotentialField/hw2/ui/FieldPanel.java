package hw2.ui;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.event.*;
import java.awt.geom.Point2D;
import java.util.Iterator;
import javax.swing.JPanel;

import hw2.models.FieldManager;
import hw2.models.PotentialField;
import hw2.models.Vector2D;

public class FieldPanel extends JPanel {
    protected int deltaX, deltaY;
    protected RobotComponent robot;
    private FieldManager fieldManager;    

    public RobotComponent getRobot() {
	return robot;
    }

    public void setRobot(RobotComponent r) {
	robot = r;
	add(robot);
	MouseMover mouser = new MouseMover();
	robot.addMouseListener(mouser);
	robot.addMouseMotionListener(mouser);
    }

    public void incrementDelta() {
	deltaX++;
	deltaY++;
    }

    public void decrementDelta() {	
	deltaX--;
	deltaY--;
    }

    private class KeyController implements KeyListener {
	public void keyPressed(KeyEvent e) {
	    if(e.getKeyChar() == '+') {
		incrementDelta();
	    } else if(e.getKeyChar() == '-') {
		decrementDelta();
	    }
	    repaint();
	}
	public void keyReleased(KeyEvent e) {
	}
	public void keyTyped(KeyEvent e) {
	}
    }

    protected class MouseMover implements MouseMotionListener, MouseListener {
	protected int mouseStartX, mouseStartY;
	protected int compStartX, compStartY;

	public void mouseMoved(MouseEvent e) {
	}

	public void mouseDragged(MouseEvent e) {
	    int dx = e.getX() - mouseStartX;
	    int dy = e.getY() - mouseStartY;
	    e.getComponent().setLocation(compStartX + dx,
					 compStartY + dy);
	    
	    mouseStartX = e.getX() - dx;
	    mouseStartY = e.getY() - dy;
	    compStartX = e.getComponent().getX();
	    compStartY = e.getComponent().getY();
	    repaint();
	}

	public void mousePressed(MouseEvent e) {
	    mouseStartX = e.getX();
	    mouseStartY = e.getY();
	    compStartX = e.getComponent().getX();
	    compStartY = e.getComponent().getY();
	}

        @Override
	public void mouseReleased(MouseEvent e) {
	}
        @Override
	public void mouseClicked(MouseEvent e) {
	}
        @Override
	public void mouseEntered(MouseEvent e) {
	}
        @Override
	public void mouseExited(MouseEvent e) {
	}
    }


    public void addField(PotentialFieldComponent newField){ 
        MouseMover mouser = new MouseMover();
	newField.addMouseListener(mouser);
	newField.addMouseMotionListener(mouser);
	add(newField);      
    }
    
    public FieldPanel(FieldManager manager) {
	setFocusable(true);
	setLayout(null);
	deltaX = 50;
	deltaY = 50;
        this.fieldManager = manager;
        for(PotentialField f : fieldManager.getFields()) {
            addField(new PotentialFieldComponent(f));
        }
	addKeyListener(new KeyController());
    }

    public void paintComponent(Graphics gin) {
	Graphics2D g = (Graphics2D)gin;

	// clear background
	g.setColor(Color.white);
	g.fillRect(0, 0, getWidth(), getHeight());

	// samle and paint field values
	g.setColor(Color.blue);
	for(int row=0; row<getHeight(); row+=deltaX) { 
	    for(int col=0; col<getWidth(); col+=deltaY) { 
                Vector2D vector = fieldManager.getPotential(col, row);
		g.drawLine(col, row, (int)(vector.getX() + col), (int)(vector.getY() + row));
		g.fillOval(col-2, row-2, 4,4);
	    }
	}

	// paint the path
	java.util.List robotPath = fieldManager.getRobotPath(robot.getCenterX(), 
							     robot.getCenterY(),
							     robot.getPercentage(),
							     robot.getRadius(),
							     robot.getPathLength());

	Iterator points = robotPath.iterator();
	Point2D fromPoint = null;
	if(points.hasNext()) {
	    fromPoint = (Point2D)points.next();
	}
	g.setColor(Color.black);
	g.drawOval((int)(robot.getCenterX() - robot.getRadius()),
		   (int)(robot.getCenterY() - robot.getRadius()),
		   (int)(robot.getRadius() * 2),
		   (int)(robot.getRadius() * 2));
		 

	g.setColor(Color.green);
	while(points.hasNext()) {
	    Point2D toPoint = (Point2D)points.next();
	    g.drawLine((int)fromPoint.getX(), (int)fromPoint.getY(), (int)toPoint.getX(), (int)toPoint.getY());
	    fromPoint = toPoint;
	}
    }

    public void setDelta(int dx, int dy) {
	deltaX = dx;
	deltaY = dy;
    }
}
