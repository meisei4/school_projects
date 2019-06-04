import java.io.*;
public class HuffmanInputStream extends BitInputStream {

	private String tree;
	private int totalChars;
	private int bread;
	private int count = 0;
	private int index = 0;
	private int[] bitString = new int[8];
	public HuffmanInputStream(String filename) throws IOException {
	//add additional private variables and methods as needed
		super(filename);
		try {
			tree = d.readUTF();
			totalChars = d.readInt();
			bread = d.readUnsignedByte();
			for(int i = 7; i >= 0; i--){
				bitString[i] = bread % 2;
				bread = bread/2;
			}
		}catch (IOException e) {
			
		}
		
	}
	
	public int readBit() throws IOException {
		int bit = bitString[index];
		index++;
		count++;
		 if(count == 8){
			bread = d.readUnsignedByte();
			for(int i = 7; i >= 0; i--){
				bitString[i] = bread % 2;
				bread = bread/2;
			}
			count = 0;
			index = 0;
		}
		return bit;
		
	}

	public String getTree() { 
		return tree;
	}

	public int totalChars() {  
		return totalChars;
	}
	
	public void close() throws IOException { 
			d.close();
	}
}