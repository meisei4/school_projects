package hw1.model;

public class FileFormatException extends Exception {
	public FileFormatException( String msg ) {
		super(msg);
	}
	
	public FileFormatException() {
		super();
	}
}
