import java.util.*;

public class HuffmanTree {
	
	private class Node {
		private Node left;
		private char data;
		private Node right;

		private Node(Node L, char d, Node R) {
			left = L;
			data = d;
			right = R;
		}
		private boolean isLeaf(){
			return !hasLeft() || !hasRight();
		}
		private boolean hasLeft(){
			return left != null;
		}
		
		private boolean hasRight(){
			return right != null;
		}
	}

	private Node root;
	private Node current; //this value is changed by the move methods

	
	public HuffmanTree() {
		root = null;
		moveRoot();
	}
	
	//single node tree 
	//USE IN ENCODE
	public HuffmanTree(char d) {
		root = new Node(null, d, null);
		moveRoot();
	}
	
	//Assumes t represents a post order representation of the tree as discussed in class
	//nonLeaf is the char value of the data in the non-leaf nodes
	//USE FOR DECODING
	public HuffmanTree(String t, char nonLeaf) {
		//BASED OFF OF THE EXPRESSION TREE ALGORITHM
		//REPLACING OPERATORS WITH THE NONLEAF CHARACTER
		Stack<Node> leaves = new Stack<Node>();
		for(int i = 0; i < t.length(); i++){
			char token = t.charAt(i);
			if(token != nonLeaf){
				leaves.push(new Node(null, token, null));
			}else{
				Node x = leaves.pop();
				Node y = leaves.pop();
				leaves.push(new Node(y, nonLeaf, x));
			}
		}
		root = leaves.pop();
		moveRoot();
	}
	
	
	//USE THIS IN ENCODE
	//makes a new tree where b1 is the left subtree and b2 is the right subtree
	//d is the data in the root
	public HuffmanTree(HuffmanTree b1, HuffmanTree b2, char d) {
		root = new Node(b1.root, d, b2.root);
		moveRoot();
	}

	//use the methods moveRoot through current to help with the decoding process
	//the move methods change the value of current
	public void moveRoot() {
		current = root;
	}

	public void moveLeft() {
		current = current.left;
	}

	public void moveRight() {
		current = current.right;
	}
	
	//returns true if current references a leaf other wise returns false
	public boolean atLeaf() {
		return current.isLeaf();
	}
	
	//returns the data value in the node referenced by current
	public char current() {
		return current.data;
	}

	
	//uses this in the encoding process
	//the iterator returns the path (a series of 0s and 1s) to each leaf
	//it will be easier to construct all the paths when the iterator is created
	public class PathIterator implements Iterator<String> {
		ArrayList<String> paths = new ArrayList<String>();
		private Iterator<String> itr;
		
		private void getPath(ArrayList<String> paths, Node n, String path){
			if(n.isLeaf()){
				//reaches leaf
				//add the character found at the end of path to the code
				//add code to List of paths
				paths.add(n.data + path);
			}else {

				//appends a "0" when going to the left
				getPath(paths, n.left, path + "0");

				//appends a "1" to the path when going to the right
				getPath(paths, n.right, path + "1");
			}
		}
		
		public PathIterator() {
			getPath(paths, root, "");
			itr = paths.iterator();
		}

		public boolean hasNext() {
			return itr.hasNext();
		}

		public String next() {
			return itr.next();
		}

		public void remove() {
		//optional method not implemented
		}
	}
	
	//return a new iterator object
	public Iterator<String> iterator() {
		PathIterator pathItr = new PathIterator();
		return pathItr;
	}
	
	//returns a string representation of the tree using the format discussed in class
	public String toString() {
		return toPostOrder(root);
	}
	//Recursive postOrder traversal
	private String toPostOrder(Node n){
		if(n == null){
			return "";
		}else{
			return toPostOrder(n.left) + toPostOrder(n.right) + n.data;
		}
	}

}

