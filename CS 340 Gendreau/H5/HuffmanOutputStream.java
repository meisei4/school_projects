import java.io.*;
public class HuffmanOutputStream extends BitOutputStream {
    private int count;
    private int bread;

    //String tree is postorderTraveral of huffman tree
    public HuffmanOutputStream(String filename, String tree, int totalChars) {
        super(filename);
        count = 0;
        bread = 0;
        try {
            d.writeUTF(tree);
            d.writeInt(totalChars);
        }catch (IOException e) {
        	
        }
    }
    
    //PRE bit == 0 || bit == 1
    //Writes a bit to the file
    //Bits must be packed 8 to a byte before writing to the file	
    public void writeBit(int bit) throws IOException {
    	if(bit == 0){
    		bread = bread * 2;
    		count++;
    	} else {
    		bread = bread * 2 + 1;
    		count++;
    	}
    	if(count == 8){
    		d.writeByte(bread);
    		bread = 0;
    		count = 0;
    	}
    }


    public void close() throws IOException {
    	if(count != 0){
    		for(int i = count; i < 8; i++){
    			bread = bread * 2;
    		}
    		d.writeByte(bread);
    	}
    	d.close();
        //be sure to write any partial byte the the file before closing
    }
}