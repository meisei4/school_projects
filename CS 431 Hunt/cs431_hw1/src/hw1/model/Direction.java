package hw1.model;

public enum Direction {
	NORTH, SOUTH, EAST, WEST;
	
	public final Direction left() {
		switch(this) {
		case NORTH: return WEST;
		case SOUTH: return EAST;
		case EAST: return NORTH;
		case WEST: return SOUTH;			
		}
		return null;
	}
	
	public final Direction right() {
		switch(this) {
		case NORTH: return EAST;
		case SOUTH: return WEST;
		case EAST: return SOUTH;
		case WEST: return NORTH;			
		}
		return null;
	}	
	
	public final Direction back() {
		switch(this) {
		case NORTH: return SOUTH;
		case SOUTH: return NORTH;
		case EAST: return WEST;
		case WEST: return EAST;			
		}
		return null;
	}
}
