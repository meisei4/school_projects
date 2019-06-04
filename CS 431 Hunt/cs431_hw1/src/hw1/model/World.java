package hw1.model;

public interface World {
	public static final char WALL='#', EMPTY='.', ROBOT='R', GOAL='G', UNKNOWN='?';
	public World copy();
	public char[][] getApriori();
	public char get(Location location);
	public boolean isEmpty(Location location);
	public boolean isWall(Location location);
	public boolean isRobot(Location location);
	public Location robotLocation();
	public Location goalLocation();
	public boolean hasReachedGoal();	
	public boolean canMove(Direction dir);
	public void move(Direction d);
	public Integer getWidth();
	public Integer getHeight();
	public Direction getHeading();
	public boolean isGoal(Location location);
	public void applyAction(Action action);
	public String getName();
}
