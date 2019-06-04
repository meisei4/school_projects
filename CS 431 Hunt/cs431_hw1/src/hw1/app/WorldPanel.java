package hw1.app;

import java.awt.*;
import java.awt.geom.Point2D;
import java.util.HashMap;
import java.util.Map;

import javax.swing.*;

import hw1.model.Direction;
import hw1.model.Location;
import hw1.model.World;

/**
 *
 * @author Kenny Hunt
 */
@SuppressWarnings("serial")
public class WorldPanel extends JPanel {
    private World world;
    public static final int DEFAULT_CELL_SIZE = 10;    
    private Map<Direction, Point2D.Double> offsets;
    
    /** Creates a new instance of WorldPanel */
    public WorldPanel(World w) {
        setWorld(w);
        setBorder(BorderFactory.createEtchedBorder());
        
        offsets = new HashMap<Direction, Point2D.Double>();
        offsets.put(Direction.NORTH, new Point2D.Double(0, -1));
        offsets.put(Direction.SOUTH,  new Point2D.Double(0, 1));
        offsets.put(Direction.EAST,  new Point2D.Double(1, 0));
        offsets.put(Direction.WEST, new Point2D.Double(-1, 0));
    }
    
    public void setWorld(World w) {
        world = w;
        repaint();
    }
    
    public void setCellSize(double s) {
        repaint();
    }
    
    public void zoom(double scale) {
        setCellSize(DEFAULT_CELL_SIZE * scale);
    }
    
    public Color toColor(char value) {
    	switch(value) {
    	case World.EMPTY : return Color.white;
    	case World.GOAL : return Color.red;
    	case World.ROBOT : return Color.blue;
    	case World.WALL : return Color.black;
    	default : return Color.yellow;
    	}
    }
    
    public void drawGrid(Graphics gin) {
        double cellWidth = cellWidth();
        double cellHeight = cellHeight();

       gin.setColor(Color.lightGray);
        for(int i=0; i<world.getWidth(); i++) {
        	int x = (int)(Math.round(i * cellWidth));
        	gin.drawLine(x, 0, x, getHeight());
    	}
        
        for(int i=0; i<world.getHeight(); i++) {
        	int y = (int)(Math.round(i * cellHeight));
        	gin.drawLine(0, y, getWidth(), y);
    	}    
    }
    
    public double cellHeight() {
    	return getHeight() / (double)world.getHeight();
    }
    
    public double cellWidth() {
    	return getWidth() / (double)world.getWidth();
    }
    
    public void paintHeading(Graphics gin) {
    	Location bot = world.robotLocation();
    	Direction heading = world.getHeading();
    	double cellWidth = cellWidth();
    	double cellHeight = cellHeight();
        int cwRounded = (int)Math.round(cellWidth);
        int chRounded = (int)Math.round(cellHeight);    	
    	int length = (int)Math.max(cwRounded, chRounded);
    	length *= 2;
    	
    	int cx = (int)(bot.col * cellWidth + .5 * cellWidth);
    	int cy = (int)(bot.row * cellHeight + .5 * cellHeight);
    	
    	int endX = (int)(offsets.get(heading).x * length + cx);
    	int endY = (int)(offsets.get(heading).y * length + cy);
    	gin.setColor(Color.red);
    	gin.drawLine(cx, cy, endX, endY);    	    	
    }
    
    public void paintComponent(Graphics gin) {
        Graphics2D g = (Graphics2D)gin;
        g.setColor(Color.GRAY);
        g.fillRect(0, 0, getWidth(), getHeight());
        if(world == null) return;
        double cellWidth = cellWidth();
        double cellHeight = cellHeight();
        int cwRounded = (int)Math.round(cellWidth);
        int chRounded = (int)Math.round(cellHeight);
        for(int x=0; x<world.getWidth(); x++){ 
            for(int y=0; y<world.getHeight(); y++) {
            	Location loc = new Location(y, x);
                char cellValue = world.get(loc);
                Color cellColor = toColor(cellValue);
                g.setColor(cellColor);
                g.fillRect((int)Math.round(x*cellWidth), (int)Math.round(y*cellHeight), cwRounded, chRounded);
            }
        }
        drawGrid(gin);
        paintHeading(gin);
    }
}
