import java.io.*;

public abstract class BitInputStream {

	protected DataInputStream d;

	public BitInputStream(String filename) {
		try {
			d = new DataInputStream(new FileInputStream(filename));
		}
		catch (IOException e) {
		}
	}

	public abstract int readBit() throws IOException;

	public abstract void close() throws IOException;

}