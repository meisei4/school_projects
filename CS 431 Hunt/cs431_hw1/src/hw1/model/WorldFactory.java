package hw1.model;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class WorldFactory {
	public static World createBoundedGridWorld(File f) throws FileNotFoundException, FileFormatException {
		Scanner scanner = new Scanner(f);
		int height = scanner.nextInt();
		int width = scanner.nextInt();
		boolean hasApriori = scanner.nextBoolean();
		String validChars = "[" + World.EMPTY + World.GOAL + World.ROBOT + World.WALL + "]";
		BoundedGridWorld world = new BoundedGridWorld(height, width);
		for (int r = 0; r < height; r++) {
			for (int c = 0; c < width; c++) {	
				world.setCell(new Location(r, c), scanner.findWithinHorizon(validChars, 5).charAt(0));
			}		
		}
		

		for (int r = 0; r < height; r++) {
			for (int c = 0; c < width; c++) {	
				if( hasApriori ) {
					world.setApriori(new Location(r, c), scanner.findWithinHorizon(validChars, 5).charAt(0));
				} else {
					world.setApriori( new Location(r,  c), '?' );
				}
			}		
		}

		
		world.setHeading(Direction.NORTH);
		world.setName(f.getName());
		scanner.close();
		return world;
	}

}
