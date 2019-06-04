package hw2.models;

import java.awt.geom.Point2D;
import java.util.ArrayList;
import java.util.Collection;
import java.util.List;

public class WorkingFieldManager implements FieldManager {
	private ArrayList<PotentialField> fields = new ArrayList<PotentialField>();
	
	public void addField(PotentialField field){
		fields.add(field);
	}
	
	public Collection<PotentialField> getFields() {
		return fields;
	}

	public Vector2D getPotential(double x, double y) {
		Vector2D potential = new CartesianVector2D(0, 0);
		for(int i = 0; i < fields.size(); i++){
			PotentialField temp = fields.get(i);
			Vector2D tempV = temp.getPotential(x, y);
			potential = potential.add(tempV);
		}
		return potential;
	}

	public Vector2D getPotential(double x, double y, double radiusOfView) {
		Vector2D potential = new CartesianVector2D(0, 0);
		for(int i = 0; i < fields.size(); i++){
			PotentialField temp = fields.get(i);
			System.out.println(temp.toString());
			double distance = Math.sqrt((x - temp.getX()) * (x - temp.getX()) + 
										(y - temp.getY()) * (y - temp.getY())); 
			if(distance <= radiusOfView){
				potential = potential.add(temp.getPotential(x, y));
			}
		}
		return potential;
	}

	public List<Point2D> getRobotPath(double startX, double startY, double percentageOfField, double radiusOfView, double totalPathLength) {
		ArrayList<Point2D> path = new ArrayList<Point2D>();
		double pathlen = 0;
		double x = startX;
		double y = startY;
		path.add(new Point2D.Double(x, y));
		while(pathlen <= totalPathLength){
			 Vector2D pathSegment = getPotential(x, y, radiusOfView);
			 double step = pathSegment.getMagnitude() * percentageOfField;
			 if(step < 0.000001){
				 break;
			 }
			 pathlen = pathlen + step;
			 x = x + Math.cos(pathSegment.getOrientation()) * step;
			 y = y + Math.sin(pathSegment.getOrientation()) * step;
			 path.add(new Point2D.Double(x, y));
		}
		return path;
	}
	
	public int getNumberOfFields() {
		return fields.size();
	}
	
	public Collection<PotentialField> getFieldsInView(double x, double y, double radiusOfView) {
		ArrayList<PotentialField> view = new ArrayList<PotentialField>();
		for(int i = 0; i < fields.size(); i++){
			PotentialField temp = fields.get(i);
			double distance = Math.sqrt((x - temp.getX()) * (x - temp.getX()) + 
										(y - temp.getY()) * (y - temp.getY())); 
			if(distance <= radiusOfView){
				view.add(temp);
			}
		}
		return view;
	}
	
	public String toString(){
		String str = "";
		for(int i = 70; i < 120; i+=20){
			for(int j = 190; j < 250; j+=20){
				str = str + getPotential(i, j).toString() + "\n";
			}
		}
		return str;
	}
}
