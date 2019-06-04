package hw1.model;

public class Location {
	public int row, col;

	public Location(int r, int c) {
		row = r;
		col = c;
	}
	
	public Location get(Direction dir) {
		switch(dir) {
		case NORTH : return north();
		case SOUTH : return south();
		case EAST : return east();
		case WEST : return west();
		}
		return null;
	}
			
	public String toString() {
		return "(" + row + "," + col + ")";
	}
	
	public Location north() {
		return new Location(row-1, col);
	}
	
	public Location south() {
		return new Location(row+1, col);
	}
	
	public Location east() {
		return new Location(row, col+1);
	}
	
	public Location west() {
		return new Location(row, col-1);
	}
		
	public int getRow() {
		return row;
	}

	public void setRow(int row) {
		this.row = row;
	}

	public int getCol() {
		return col;
	}

	public void setCol(int col) {
		this.col = col;
	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + col;
		result = prime * result + row;
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		Location other = (Location) obj;
		if (col != other.col)
			return false;
		if (row != other.row)
			return false;
		return true;
	}
}
