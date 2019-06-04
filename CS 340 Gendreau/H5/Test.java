import java.util.*;
import java.io.*;

public class Test {


    public static void main(String[] args) throws IOException{
    	char i = (char)128;

    	String tree = "US" + i + "O!" + i + i + "IC" + i + "A" + i + i + " 4" + i + "3F" + i + "N" + i + i + i;
////////////////////////////////////INPUT OUTPUT STREAM TESTS////////////////////////////////////////////////////
//   	  File test = new File("/home/anne/test");
//        HuffmanOutputStream o = new HuffmanOutputStream(test.getName(), tree , test.length());
//        o.writeBit(1);
//        o.writeBit(1);
//        o.writeBit(1);
//        o.writeBit(0);
//        o.writeBit(0);
//        o.writeBit(1);
//        o.writeBit(0);
//        o.writeBit(1);
//        o.close();
//        HuffmanInputStream is = new HuffmanInputStream(test.getName());
//        for(int j = 0; j < test.length(); j++){
//        	System.out.println(is.readBit());
//        }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
///////////////////////////////HUFFMAN TREE TO STRING AND ITERATOR TEST//////////////////////////////////////////  	
        HuffmanTree testTree = new HuffmanTree(tree, i);
        System.out.println(testTree.toString());
        Iterator<String> testItr = testTree.iterator();
        while(testItr.hasNext()){
        	System.out.println(testItr.next());
        }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////ENCODING TEST/////////////////////////////////////////////////////
    	
    	
    	
    	
    	
    	
    	
    }
}

