import java.io.*;
import java.util.*;

public class HuffmanDecode {
	String tree;
	int totalChars;
	private int writtenChars;
	HuffmanInputStream intou;
	HuffmanTree haffu;
	
	//implements the Huffman Decode Algorithm
	//Add private methods and instance variables as needed
	public HuffmanDecode(String in, String out) throws IOException {
		BufferedWriter writer = new BufferedWriter(new FileWriter(out));
		intou = new HuffmanInputStream(in);
		tree = intou.getTree();
		totalChars = intou.totalChars();
		haffu = new HuffmanTree(tree, (char)128);
		writtenChars = 0;
		try{
			//for some reason it writes more characters than needed at the very end (only about 3)
			while(writtenChars < totalChars){
				int bit = intou.readBit();
				if(bit == 0){
					haffu.moveLeft();
				} else {
					haffu.moveRight();
				}
				if(haffu.atLeaf()){
					writer.write(haffu.current());
					writtenChars++;
					haffu.moveRoot();
				}
			}
		}catch(EOFException e){
			
		}
		//System.out.println(haffu.toString());
		//System.out.println(outp);
		intou.close();
		writer.close();
	}
	public static void main(String args[]) {
	//args[0] is the name of a file created by Huffman Encode
	//args[1] is the name of the output file for the uncompressed file
		try {
			new HuffmanDecode(args[0], args[1]);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		
	}

}