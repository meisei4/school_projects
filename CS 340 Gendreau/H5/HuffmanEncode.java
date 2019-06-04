import java.io.*;
import java.util.*;

public class HuffmanEncode {
	
	
	private static ArrayList<Integer> freq = new ArrayList<Integer>();
	private static ArrayList<Character> chars = new ArrayList<Character>();
	private static int totalChars = 0;
	private static String[] codes = new String[128];
	private static H4<Integer> huffHeap;
	private static char defChar = (char)128;
	private static HuffmanTree encodingTree;
	private static File input;
	private static File output;
	private static HuffmanOutputStream houto;
	
	
	//creates parallel char and freq lists
	private static void MakeLists(String in) throws IOException  {
		input = new File(in);
		BufferedReader fin = new BufferedReader(new FileReader(input));
		int tokenInt;
		char tokenChar;
		while((tokenInt = fin.read()) != -1) {
			tokenChar = (char)tokenInt;
			totalChars++;
			if(chars.contains(tokenChar)){
				int index = chars.indexOf(tokenChar);
				freq.set(index, freq.get(index) + 1);
			}else{
				chars.add(tokenChar);
				freq.add(1);
			}
		}
	}
	
	private static void MakeHeap(){
		huffHeap = new H4<Integer>(chars.size(), 4); 
		for(int i = 0; i < chars.size(); i++){
			huffHeap.insert(freq.get(i), new HuffmanTree(chars.get(i)));
		}
	}
	
	//This method uses the algorithm for making the HuffmanTree
	//uses the 1 heap from MakeHeap method to take out Character data and insert Huffman subtrees
	private static void MakeHeapIntoTree(){
		HuffmanTree tree = new HuffmanTree();
		while(huffHeap.getSize() > 1){
			Integer xFreq = huffHeap.getMinKey();
			HuffmanTree x =  (HuffmanTree) huffHeap.getMinData();
			huffHeap.removeMin();
			Integer yFreq = huffHeap.getMinKey();
			HuffmanTree y = (HuffmanTree) huffHeap.getMinData();
			huffHeap.removeMin();
			tree = new HuffmanTree(x, y, defChar);
			//insertion on the left following ordering:
//																(root)
//																		 /  \
//																        /    \
//										  (lesser)  (greater)
//			newrootKey => tree1Key + tree2Key
			huffHeap.insert(xFreq + yFreq, tree);
		}
		encodingTree = tree;
	}
	
	
	
	private static void write(String in, String out) throws IOException{
		houto = new HuffmanOutputStream(out, encodingTree.toString(), totalChars);
		Iterator<String> itr = encodingTree.iterator();
		while(itr.hasNext()){
			String code = itr.next();
			codes[code.charAt(0)] = code.substring(1);
			//System.out.println(code.charAt(0) + codes[code.charAt(0)]);
		}
		BufferedReader fin = new BufferedReader(new FileReader(in));
		int tokenInt;
		char tokenChar;
		while((tokenInt = fin.read()) != -1) {
			tokenChar = (char)tokenInt;
			//System.out.println(codes[tokenChar]);
			for(int i = 0; i < codes[tokenChar].length(); i++){
				int bit = Integer.parseInt("" + codes[tokenChar].charAt(i));
				//parse the chars into 1's and 0's
				houto.writeBit(bit);
			}
		}
		houto.close();
	}
		
	//Implements the Huffman encoding algorithm
	public HuffmanEncode(String in, String out) throws IOException {
			MakeLists(in);
			//initializes char and frequency lists (parallel)
			MakeHeap();
			MakeHeapIntoTree();	
			write(in, out);
	}
		
	//args[0] is the name of the file whose contents should be compressed
	//args[1] is the name of the output file that will hold the compressed content if the input file
	public static void main(String args[]) {
		try {
			new HuffmanEncode(args[0], args[1]);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}