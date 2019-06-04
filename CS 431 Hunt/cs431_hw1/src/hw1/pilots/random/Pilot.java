package hw1.pilots.random;

import hw1.model.*;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import hw1.model.Action;
import hw1.model.RobotPilot;
import hw1.model.Sensors;

public class Pilot implements RobotPilot {
	private double quality, range;
	private Sensors sense;
	private Location goal, cur;
	private Direction face = Direction.NORTH;
	private char[][] map;
	
	@Override
	public void init(int deltaRowToGoal, int deltaColToGoal) {
		map = new char[1000][1000];
		cur = new Location(500, 500);
		goal = new Location(cur.getRow() + deltaRowToGoal, cur.getCol() + deltaColToGoal);
		map[cur.getRow()][cur.getCol()] = 'R';
		map[goal.getRow()][goal.getCol()] = 'G';
		for(int i = 0; i < map.length; i++){
			for(int j = 0; j < map[i].length; j++){
				if(map[i][j] == '0'){
					map[i][j] = '?';
				}
			}
		}
	}
	
	@Override
	public Action act(Sensors sensors) {
		sense = sensors;
		quality = sensors.getQuality();
		range = sensors.getRange();
		List<Location> endPath = shortestPath(cur, goal);
		Location loc1 = endPath.remove(0);
		Location loc2 = endPath.get(0);
		cur = loc2;
		return motion(face, loc1, loc2);
	}
	
	//write the map down
	//looks around depending on starting orientation 
	private void look(){
		map[cur.getRow()][cur.getCol()] = 'x';
		double f = 0, l = 0, b = 0, r = 0;
		for(int i = 1; i < Math.abs(quality - 100); i++){
			f = f + sense.front();
			r = r + sense.right();
			b = b + sense.back();
			l = l + sense.left();
		}
		double favg = f/Math.abs(quality - 100);
		double ravg = f/Math.abs(quality - 100);
		double bavg = f/Math.abs(quality - 100);
		double lavg = f/Math.abs(quality - 100);
		//distances from barrier noticed
		int df = (int) (range * favg);
		int dr = (int) (range * ravg);
		int db = (int) (range * bavg);
		int dl = (int) (range * lavg);

		if(face == Direction.NORTH){
			for(int i = 0; i < range; i++){
				if(i == df){
					map[cur.getRow() - i][cur.getCol()] = '#';
				}else{
					map[cur.getRow() - i][cur.getCol()] = '.';
				}
			}
			for(int i = 0; i < range; i++){
				if(i == dr){
					map[cur.getRow()][cur.getCol() - i] = '#';
				}else{
					map[cur.getRow()][cur.getCol() -i] = '.';
				}
			}
			for(int i = 0; i < range; i++){
				if(i == db){
					map[cur.getRow() + i][cur.getCol()] = '#';
				}else{
					map[cur.getRow() + i][cur.getCol()] = '.';
				}
			}
			for(int i = 0; i < range; i++){
				if(i == dl){
					map[cur.getRow()][cur.getCol() + i] = '#';
				}else{
					map[cur.getRow()][cur.getCol() + i] = '.';
				}
			}
		}else if(face == Direction.SOUTH){
			for(int i = 0; i < range; i++){
				if(i == db){
					map[cur.getRow() - i][cur.getCol()] = '#';
				}else{
					map[cur.getRow() - i][cur.getCol()] = '.';
				}
			}
			for(int i = 0; i < range; i++){
				if(i == dr){
					map[cur.getRow()][cur.getCol() - i] = '#';
				}else{
					map[cur.getRow()][cur.getCol() -i] = '.';
				}
			}
			for(int i = 0; i < range; i++){
				if(i == df){
					map[cur.getRow() + i][cur.getCol()] = '#';
				}else{
					map[cur.getRow() + i][cur.getCol()] = '.';
				}
			}
			for(int i = 0; i < range; i++){
				if(i == dl){
					map[cur.getRow()][cur.getCol() + i] = '#';
				}else{
					map[cur.getRow()][cur.getCol() + i] = '.';
				}
			}
		}else if(face == Direction.EAST){
			for(int i = 0; i < range; i++){
				if(i == dr){
					map[cur.getRow() - i][cur.getCol()] = '#';
				}else{
					map[cur.getRow() - i][cur.getCol()] = '.';
				}
			}
			for(int i = 0; i < range; i++){
				if(i == db){
					map[cur.getRow()][cur.getCol() - i] = '#';
				}else{
					map[cur.getRow()][cur.getCol() -i] = '.';
				}
			}
			for(int i = 0; i < range; i++){
				if(i == dl){
					map[cur.getRow() + i][cur.getCol()] = '#';
				}else{
					map[cur.getRow() + i][cur.getCol()] = '.';
				}
			}
			for(int i = 0; i < range; i++){
				if(i == df){
					map[cur.getRow()][cur.getCol() + i] = '#';
				}else{
					map[cur.getRow()][cur.getCol() + i] = '.';
				}
			}
		}else if(face == Direction.WEST){
			for(int i = 0; i < range; i++){
				if(i == dl){
					map[cur.getRow() - i][cur.getCol()] = '#';
				}else{
					map[cur.getRow() - i][cur.getCol()] = '.';
				}
			}
			for(int i = 0; i < range; i++){
				if(i == df){
					map[cur.getRow()][cur.getCol() - i] = '#';
				}else{
					map[cur.getRow()][cur.getCol() -i] = '.';
				}
			}
			for(int i = 0; i < range; i++){
				if(i == dr){
					map[cur.getRow() + i][cur.getCol()] = '#';
				}else{
					map[cur.getRow() + i][cur.getCol()] = '.';
				}
			}
			for(int i = 0; i < range; i++){
				if(i == db){
					map[cur.getRow()][cur.getCol() + i] = '#';
				}else{
					map[cur.getRow()][cur.getCol() + i] = '.';
				}
			}
		}
		
	}

	//given a set of locations and an orientation return the action 
	//thats required to get from loc1 to loc2
	private Action motion(Direction orient, Location loc1, Location loc2){
		if(loc1.getRow() > loc2.getRow()){
			if(orient == Direction.EAST){
				face = face.right();
				return Action.TURN_RIGHT;
			}else if(orient == Direction.WEST){
				face = face.left();
				return Action.TURN_LEFT;
			}else if(orient == Direction.NORTH){
				return Action.REVERSE;
			}else{
				return Action.FORWARD;
			}
		}else if(loc1.getRow() > loc2.getRow()){
			if(orient == Direction.EAST){
				face = face.left();
				return Action.TURN_LEFT;
			}else if(orient == Direction.WEST){
				face = face.right();
				return Action.TURN_RIGHT;
			}else if(orient == Direction.SOUTH){
				return Action.REVERSE;
			}else{
				return Action.FORWARD;
			}
		}else if(loc1.getCol() > loc2.getCol()){
			if(orient == Direction.NORTH){
				face = face.left();
				return Action.TURN_LEFT;
			}else if(orient == Direction.SOUTH){
				face = face.right();
				return Action.TURN_RIGHT;
			}else if(orient == Direction.EAST){
				return Action.REVERSE;
			}else{
				return Action.FORWARD;
			}
		}else if(loc1.getCol() < loc2.getCol()){
			if(orient == Direction.SOUTH){
				face = face.left();
				return Action.TURN_LEFT;
			}else if(orient == Direction.NORTH){
				face = face.right();
				return Action.TURN_RIGHT;
			}else if(orient == Direction.WEST){
				return Action.REVERSE;
			}else{
				return Action.FORWARD;	
			}
		}
		return Action.SIT;
		
	}
	
	private double distanceFromGoal(Location loc){
		double a = Math.sqrt(Math.pow(loc.row, 2) + Math.pow(loc.col, 2));
		double b = Math.sqrt(Math.pow(goal.row, 2) + Math.pow(goal.col, 2));
		return Math.abs(a - b);
	}
	
	//checks map to see if location is wall
	private boolean isWall(Location loc){
		return map[loc.getRow()][loc.getCol()] == '#';
	}
	
//	private boolean beenTraversed(Location loc){
//		return map[loc.getRow()][loc.getCol()] == 'x';
//	}
//	
	//remove the locations that result in a wall
	private ArrayList<Location> cleanAdjacents(ArrayList<Location> adj){
		for(int i = 0; i < adj.size(); i++){
			if(isWall(adj.get(i))){
				adj.remove(adj.get(i));
			}
		}
		return adj;
	}
	
	//return the best adjacent location given a location
	private Location bestLocation(Location last) {
		look();
		Location n = last.north(), s = last.south(), e = last.east(), w = last.west();
		ArrayList<Location> adj = new ArrayList<Location>();
		adj.add(e);
		adj.add(w);
		adj.add(n);
		adj.add(s);
		cleanAdjacents(adj);
		if(adj.size() == 1){
			return adj.get(0);
		}else if(adj.size() == 2){
			double x1 = distanceFromGoal(adj.get(0));
			double x2 = distanceFromGoal(adj.get(1));
			if(x1 > x2){
				return adj.get(1);
			}else{
				return adj.get(0);
			}
		}else if(adj.size() == 3){
			double a = distanceFromGoal(adj.get(0));
			double b = distanceFromGoal(adj.get(1));
			double c = distanceFromGoal(adj.get(2));
			double max = Math.min(Math.min(a, b), c);
			if(max == a){
				return adj.get(0);
			}else if(max == b){
				return adj.get(1);
			}else{
				return adj.get(2);
			}
		}else{
			double a = distanceFromGoal(adj.get(0));
			double b = distanceFromGoal(adj.get(1));
			double c = distanceFromGoal(adj.get(2));
			double d = distanceFromGoal(adj.get(3));
			double max = Math.min(Math.min(Math.min(a, b), c), d);
			if(max == a){
				return adj.get(0);
			}else if(max == b){
				return adj.get(1);
			}else if(max == c){
				return adj.get(2);
			}else {
				return adj.get(3);
			}		
		}
	}

	//turns right and continues down path recursively 
	//Direction is used to spin clockwise checking NORTH first in shortestpath method
	private ArrayList<Location> developPath(ArrayList<Location> path){
		ArrayList<Location> apath = path;
		Location last = path.get(path.size());
		Location next = bestLocation(last);
		apath.add(next);
		return apath;
	}
	
	private List<Location> shortestPath(Location start, Location end){
		ArrayList<Location> loc = new ArrayList<Location>();
		ArrayList<ArrayList<Location>> que = new ArrayList<ArrayList<Location>>();
		loc.add(start);
		que.add(loc);
		goal = end;
		while(true){
			ArrayList<Location> path = que.remove(0);
			ArrayList<Location> apath = developPath(path);
			if(apath.get(apath.size()).equals(goal)){
				return apath;
			}else{
				que.add(apath);
			}
		}
	}	
}

