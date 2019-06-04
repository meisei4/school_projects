package hw2.models;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.Scanner;

//<FILE> → { <PFIELD> <EOL> } 
//<PFIELD> → <UNIFORM> | <ATTRACTIVE> | <REPULSIVE> | <TANGENTIAL> | <RANDOM> 
//<UNIFORM> → UNIFORM <XLOC> <YLOC> <ORIENTATION> <DROPOFF> 
//<ATTRACTIVE> → ATTRACTIVE <XLOC> <YLOC> <DROPOFF> 
//<REPULSIVE> → REPULSIVE <XLOC> <YLOC> <DROPOFF> 
//<TANGENTIAL> → TANGENTIAL <XLOC> <YLOC> [CCW | CW] <DROPOFF> 
//<RANDOM> → RANDOM <XLOC> <YLOC> <CHANGE_INTERVAL> <DROPOFF> 
//<DROPOFF> → <LINEAR> | <CONSTANT> | <EXPONENTIAL> | <HANNING> 
//<LINEAR> → LINEAR <LA> <MAX> 
//<CONSTANT> → CONSTANT <K> 
//<EXPONENTIAL> → EXPONENTIAL <EA> <EB> 
//<HANNING> HANNING <MAX> <A> 
//<EOL> →‘\n’

public class FieldManagerFactory{

	public static FieldManager getFieldManager(String filename) throws FileNotFoundException {
		WorkingFieldManager man = new WorkingFieldManager();
		Scanner s1 = new Scanner(new BufferedReader(new FileReader(filename)));
		while(s1.hasNextLine()){
			String field = s1.nextLine();
			Scanner s2 = new Scanner(field);
			String type = s2.next().toUpperCase();
			PotentialField pField;
			switch(type){
			case "UNIFORM": 	
				pField = new UniformPotentialField(s2.nextDouble(), s2.nextDouble(), s2.nextDouble(), dropOff(s2.next().toUpperCase(), s2));
				break;
			case "ATTRACTIVE":	
				pField = new AttractivePotentialField(s2.nextDouble(), s2.nextDouble(), dropOff(s2.next().toUpperCase(), s2));
				break;
			case "REPULSIVE": 
				pField = new RepulsivePotentialField(s2.nextDouble(), s2.nextDouble(), dropOff(s2.next().toUpperCase(), s2));
				break;
			case "TANGENTIAL": 
				pField = new TangentialPotentialField(s2.nextDouble(), s2.nextDouble(), s2.next(), dropOff(s2.next().toUpperCase(), s2));
				break;
			case "RANDOM": 
				pField = new RandomPotentialField(s2.nextDouble(), s2.nextDouble(), s2.nextDouble(), dropOff(s2.next().toUpperCase(), s2));
				break;
			default:           
				throw new RuntimeException("File Format Error");
			}
			man.addField(pField);
		}
		return man;
	}

	public static DropoffFunction dropOff(String str, Scanner sc){
		DropoffFunction drop;
		switch(str.toUpperCase()){
		case "LINEAR":	 	
			drop = new LinearDropOff(sc.nextDouble(), sc.nextDouble());
			break;
		case "CONSTANT": 	
			drop = new ConstantDropOff(sc.nextDouble());
			break;
		case "EXPONENTIAL": 
			drop = new ExponentialDropOff(sc.nextDouble(), sc.nextDouble());
			break;
		case "HANNING": 	
			drop = new HanningDropOff(sc.nextDouble(), sc.nextDouble());
			break;
		default:          
			throw new RuntimeException("File Format Error");
		}
		return drop;
	}
}



