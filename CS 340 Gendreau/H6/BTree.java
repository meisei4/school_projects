import java.util.*;
import java.io.*;
public class BTree {
	long root;
	long free;
	Stack<BTreeNode> path;
	RandomAccessFile f;
	int order;
	//the max number of keys in a node is order-1
	int blockSize;
	
	private class BTreeNode {
		private int count;
		private int[] keys; 
		private long[] keyAddr;
		private long[] children;
		private long neighbor;
		private long nodeAddr;
		//PRE: count < order
		private BTreeNode( int count, int[] keys, long[] keyAddr, long[] children, long neighbor, long addr){
			this.keys = keys;
			this.count = count;
			this.children = children;
			this.keyAddr = keyAddr;	
			this.neighbor = neighbor;
			this.nodeAddr = addr;
		}
		
		private BTreeNode(long addr) throws IOException{
			f.seek(addr);
			nodeAddr = addr;
			count = f.readInt(); 
			
			this.keys = new int[order-1];
			this.keyAddr = new long[order-1];
			this.children = new long[order];
			this.neighbor = 0L;
			for(int i = 0; i < order-1; i++){
				keys[i] = f.readInt();
			}
			if(isLeaf()){
				for(int j = 0; j < order-1; j++){
					keyAddr[j] = f.readLong();
				}
				this.neighbor = f.readLong();
			}else {
				for(int k = 0; k < order; k++){
					children[k] = f.readLong();
				}
			}
		}
		
		//write the keys to the file
		//if this is a leaf remember keyAddresses are written as well
		private void writeKeys() throws IOException{

			for(int i = 0; i < order-1; i++){
				f.writeInt(keys[i]);
			}
			if(this.isLeaf()){
				for(int i = 0; i < order-1; i++){
					f.writeLong(keyAddr[i]);
				}
			}
		}
		
		private int size(){
			return Math.abs(count);
		}
		//writes children to file
		//use for non-Leaves only
		private void writeChildren() throws IOException{
			for(int i = 0; i < order; i++){
				f.writeLong(children[i]);	
			}
		}

		//writes the node to addr in the file
		//uses writeKeys() and writeLong()
		private void writeNode(long addr) throws IOException{
			f.seek(addr);
			nodeAddr = addr;
			if(this.isLeaf()){
				f.writeInt(count);
				writeKeys();
				f.writeLong(this.neighbor);
				//link the neighbor leaves
			}else{
				f.writeInt(count);
				writeKeys();
				writeChildren();
			}
		}
		
		private boolean isLeaf(){
			return count < 0;
		}
		
		private boolean isRoot(){
			return this.nodeAddr == root;
		}
		//for splitting children 
		private int tempIndex = 0;
		//return: temporary array for nodes that experience overflow (count == order)
		//key: key to be inserted
		//adds the key to the keys in this node
		//sorts the keys again (They will always be sorted)
	
		private int[] tempArrayKeys(int key, long[] keyaddrs, long addr){
			int[] temp = new int[order];
			for(int i = 0; i < keys.length; i++){
				temp[i] = keys[i];
			}
			int temp1 = 0;
			long tempaddr1 = 0;
			int i = 0;
			for(i = 0; i < this.size(); i++){
				if(temp[i] > key){
					temp1 = temp[i];
					tempaddr1 = keyaddrs[i];
					temp[i] = key;
					keyaddrs[i] = addr;
					break;
				}
			}
			tempIndex = i;
			if(i == this.size()){
				temp[this.size()] = key;
				keyaddrs[this.size()] = addr;
			}else{
				for(i = i; i < this.size(); i++){
					int temp2 = temp[i+1];
					long tempaddr2 = keyaddrs[i+1];
					temp[i+1] = temp1;
					keyaddrs[i+1] = tempaddr1;
					temp1 = temp2;
					tempaddr1 = tempaddr2;
				}
			}
			return temp;	
		}
		
		//return: temporary array containing all of the children of a node experiencing overflow (count == order childrenSize == order+1)
		//addr: the address of the newly added key in the DBtable
		//key: the key added
		//is used in splitting nonleaf nodes only
		private long[] tempArrayChildren(long childAddr) throws IOException{
			long[] tempChild = new long[order+1];
			for(int i = 0; i  < children.length; i++){
					tempChild[i] = children[i];
			}
			int index = tempIndex;
			long temp1 = tempChild[index];
			tempChild[index] = childAddr;
			for(int i = index; i < this.size() + 1; i++){
				long temp2 = tempChild[i+1];
				tempChild[i+1] = temp1;
				temp1 = temp2;
			}
			return tempChild;
		}
		
		//splits a non leaf node
		//creates a new node containing all children and keys to the left of the mid key(excluding the mid key)
		//writes new node to free list
		//updates free list
		//updates this node's count
		//rewrites this node to the same address
		//returns the mid value of the overflowed array
		private int splitNonLeaf(int key, long childAddr, long addr) throws IOException{
			int split = (order-1)/2;
			long[] tempAddrs = new long[order];
			for(int i = 0; i < keyAddr.length; i++){
				tempAddrs[i] = keyAddr[i];
			}
			int[] temp = tempArrayKeys(key, tempAddrs, addr);
			long[] tempChild = tempArrayChildren(childAddr);
			for(int i = 0; i < this.size(); i++){
				keys[i] = temp[i];
				keyAddr[i] = tempAddrs[i];
				children[i] = tempChild[i];
			}
			children[this.size()] = tempChild[this.size()];
			int[] end = new int[order-1];
			long[] endAddr = new long[order-1];
			int mid = temp[split];
			int j = 0;
			//key counter
			for(int i = split + 1; i < temp.length; i++){
				end[j] = temp[i];
				endAddr[j] = tempAddrs[i];
				j++;
			}
			long[] endChildren = new long[order];
			int k = 0;
			for(int i = split + 1; i < tempChild.length; i++){
				endChildren[k] = tempChild[i];
				k++;
			}
			BTreeNode node = new BTreeNode(j, end, endAddr, endChildren, 0, childAddr);
			node.writeNode(childAddr);
			this.neighbor = node.nodeAddr;
			this.count = split;
			this.writeNode(this.nodeAddr);
			return mid;
		}
		
		private boolean splitRoot(int key, long addr, long childAddr) throws IOException{
			int split = (order -1)/2;
			long[] tempAddrs = new long[order];
			for(int i = 0; i < keyAddr.length; i++){
				tempAddrs[i] = keyAddr[i];
			}
			int[] temp = tempArrayKeys(key, tempAddrs, addr);
			for(int i = 0; i < this.size(); i++){
				keys[i] = temp[i];
				keyAddr[i] = tempAddrs[i];
			}
			long [] tempChild = tempArrayChildren(children[split]);
			long[] endChildren = new long[order];
			int k = 0;
			for(int i = split + 1; i < tempChild.length; i++){
				endChildren[k] = tempChild[i];
				k++;
			}
			int[] end = new int[order-1];
			long[] endAddr = new long[order-1];
			int mid = temp[split];
			int j = 0;
			if(this.isLeaf()){
				for(int i = split; i < temp.length; i++){
					end[j] = temp[i];
					endAddr[j] = tempAddrs[i];
					j++;
				}
				long tempAddr = getFree();
				BTreeNode node = new BTreeNode(-1 * j, end, endAddr, endChildren, this.neighbor, tempAddr);
				node.writeNode(tempAddr);
				this.neighbor = node.nodeAddr;
				long[] childs = new long[order];
				childs[0] = root;
				childs[1] = node.nodeAddr;
				int[] tempKeys = new int[order-1];
				tempKeys[0] = mid;
				long[] tempKeyAddrs = new long[order-1];
				tempKeyAddrs[0] = endAddr[0];
				root = getFree();
				BTreeNode rootNode = new BTreeNode(1, tempKeys, tempKeyAddrs, childs, 0L, root);
				this.count = -1 * (split);
				this.neighbor = node.nodeAddr;
				this.writeNode(this.nodeAddr);
				node.writeNode(node.nodeAddr);
				rootNode.writeNode(root);
				return true;
			}else{
				long[] childs = new long[order];
				childs[0] = root;
				childs[1] = childAddr;
				int[] tempKeys = new int[order-1];
				tempKeys[0] = mid;
				long[] tempKeyAddrs = new long[order-1];
				tempKeyAddrs[0] = tempAddrs[split];
				root = getFree();
				BTreeNode rootNode = new BTreeNode(1, tempKeys, tempKeyAddrs, childs, 0L, root);
				this.count = split;
				this.neighbor = childAddr;
				this.writeNode(this.nodeAddr);
				rootNode.writeNode(root);
				return true;
			}
			
		}
		
		//splits a leaf node
		//writes a new node to the file at the free list
		//updates free list
		//updates count
		//updates neighbor
		//rewrites this node to the file
		//returns the mid value of the overflowed array
		private int splitLeaf(int key, long addr) throws IOException{
			int split = (order - 1)/2;
			long[] tempAddrs = new long[order];
			for(int i = 0; i < keyAddr.length; i++){
				tempAddrs[i] = keyAddr[i];
			}
			int[] temp = tempArrayKeys(key, tempAddrs, addr);
			for(int i = 0; i < this.size(); i++){
				keys[i] = temp[i];
				keyAddr[i] = tempAddrs[i];
			}
			int[] end = new int[order-1];
			long[] endAddr = new long[order-1];
			int mid = temp[split];
			int j = 0;
			for(int i = split; i < temp.length; i++){
				end[j] = temp[i];
				endAddr[j] = tempAddrs[i];
				j++;
			}
			long tempAddr = getFree();
			BTreeNode node = new BTreeNode(-1 * j, end, endAddr, this.children, this.neighbor, tempAddr);
			this.neighbor = tempAddr;
			this.count = -1 * split;
			node.writeNode(node.nodeAddr);
			this.writeNode(this.nodeAddr);
			return mid;
		}
		
		private boolean isFull(){
			return this.size() == order - 1;
		}
		//Updates the children of a nonleaf
		//childAddr is the address (in the Btree) of the most recently created node 
		//childAddr is written into the Children array 
		private void updateChildren(int index, long childAddr){
			long temp1 = children[index];
			children[index] = childAddr;
			for(int i = index; i < this.size() + 1; i++){
				long temp2 = children[i+1];
				children[i+1] = temp1;
				temp1 = temp2;
			}
		}
		
		//this inserts the new key and keyAddr into the array maintaining order
		//keys must not be full!
		private int updateKeys(int key, long addr) throws IOException{
			for(int i = 0; i < this.size(); i++){
				keyAddr[i] = search(keys[i], root, false);
			}
			int temp1 = 0;
			long addr1 = 0;
			int i = 0;
			for(i = 0; i < this.size(); i++){
				if(keys[i] > key){
					temp1 = keys[i];
					addr1 = keyAddr[i];
					keys[i] = key;
					keyAddr[i] = addr;
					break;
				}
			}
			int index = i;
			if(index == this.size()){
				keys[this.size()] = key;
				keyAddr[this.size()] = addr;
			}else{
				for(i = index; i < this.size(); i++){
					int temp2 = keys[i+1];
					long addr2 = keyAddr[i+1];
					keys[i+1] = temp1;
					keyAddr[i+1] = addr1;
					temp1 = temp2;
					addr1 = addr2;
				}
				
			}
			return index;
		}
		
		//PRE: !isFull()
		//key: key inserted
		//keyAddr: address of key in DB is written to Leaf Nodes
		//childAddr: the address in the BTree of the 
		//child addr is used only when nonleaves are being updated
		
		private void updateNodes(int key, long keyAddr, long childAddr) throws IOException{
			int index = updateKeys(key, keyAddr) + 1;
			System.out.println("" + this.keys[0]);
			if(this.isLeaf()){
				this.count--;
			}else{
				updateChildren(index, childAddr);
				this.count++;
			}
			this.writeNode(this.nodeAddr);
		}
	}
	
	public BTree(String filename, int bsize) throws IOException{
		File tree = new File(filename);
		if(tree.exists()){
			tree.delete();
		}
		f = new RandomAccessFile(tree, "rw");
		f.seek(0);	
		f.writeLong(0);
		f.writeLong(0);
		f.writeInt(bsize);
		blockSize = bsize;
		order = bsize/12;
		free = 0;
		root = free;
		path = new Stack<BTreeNode>();
	}
	
	public BTree(String filename) throws IOException{
		File tree = new File(filename);
		f = new RandomAccessFile(tree, "rw");
		f.seek(0);
		root = f.readLong();
		free = f.readLong();
		blockSize = f.readInt();
		order = blockSize/12;
	}
	
	public int getBlockSize(){
		return this.blockSize;
	}

	private long getFree() throws IOException{
		long addr = 0;
		if(free == 0){
			addr = f.length();
		}else{
			addr = free;
			f.seek(addr);
			free = f.readLong();
		}
		return addr;
	}
	

	private void printData(BTreeNode node) throws IOException{ 
		System.out.println();
		System.out.println("COUNT at BTreeAddr [" + node.nodeAddr + "]:    " + node.count);
		System.out.print("KEYS at BTreeAddr [" + node.nodeAddr + "]:         ");
		for(int i = 0; i < node.size(); i++){
			System.out.print(node.keys[i] + "      ");
		}
		System.out.println();
		System.out.print("KEYADDRS at BTreeAddr [" + node.nodeAddr + "]:     ");
		for(int i = 0; i < node.size(); i++){
			System.out.print(node.keyAddr[i] + "      ");
		}
		
		System.out.println();
		if(!node.isLeaf()){
			System.out.print("CHILDRENADDRS at BTreeAddr [" + node.nodeAddr + "]:      ");
			for(int i = 0; i < node.size() + 1; i++){
				System.out.print(node.children[i] + "      ");
			}
			System.out.println();
			System.out.println(">>>>>>CHILDREN TREES<<<<<<<<<");
			for(int i = 0; i < node.size() + 1; i++){
				printData(new BTreeNode(node.children[i]));
			}
			System.out.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
			
		}
		
		
		
		
	}
	
	//k: key to be inserted 
	//addr: address of the key being inserted in the DBTable
	private boolean split = false;
	private int mid = 0;
	private long midAddr = 0;
	private long tempAddr = 0;
	public boolean insert(int k, long addr) throws IOException{
		System.out.println("INSERTING KEY: " + k + "   ADDR: " + addr);
		if(root == 0){
			long[] tempKeyAddr = new long[order-1];
			tempKeyAddr[0] = addr;
			int[] tempKeys = new int[order-1];
			tempKeys[0] = k;
			root = getFree();
			BTreeNode rootNode = new BTreeNode(-1, tempKeys, tempKeyAddr, new long[order], 0, root);
			rootNode.writeNode(root); 
			printData(rootNode);
			return true;
		}
		BTreeNode current;
		path = new Stack<BTreeNode>();
		if(search(k) == 0){
			while(!path.empty()){
				current = path.pop();
				if(current.isLeaf()){
					if(current.isFull()){
						System.out.println("at count = " + current.size() + " >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>SPLIT!-------------------------------------------");
						if(current.isRoot()){
							current.splitRoot(k, addr, 0);
							split = false;
							printData(new BTreeNode(root));
							return true;
						}
						mid = current.splitLeaf(k, addr);
						midAddr = current.keyAddr[current.size()];
						tempAddr = current.neighbor;
						split = true;
						printData(new BTreeNode(root));
					}else{
						current.updateNodes(k, addr, 0);
						split = false;
						printData(new BTreeNode(root));
						return true;
					}
				}else if(split == true){
					if(current.isFull()){
						if(current.isRoot()){
							split = false;
							
							current.splitRoot(k, addr, tempAddr);
							printData(new BTreeNode(root));
							return true;
						}
						mid = current.splitNonLeaf(mid, getFree(),  addr);
						midAddr = current.keyAddr[current.size()];
						tempAddr = current.neighbor;
						split = true;
					}else{
						current.updateNodes(mid, midAddr, tempAddr);
						printData(new BTreeNode(root));
						split = false;
						
					}
				}else{
					current.updateNodes(k, addr, 0);
					printData(new BTreeNode(root));
					return true;
				}
			}
		}
		return false;
	}
	
	//pre: low <= high
	public LinkedList<Long> search(int low, int high) throws IOException{
		LinkedList<Long> range = new LinkedList<Long>();
		path = new Stack<BTreeNode>();
		this.search(low);
		BTreeNode leaf = path.pop();
		int start = 0;
		while(leaf.keys[start] < low){
			start++;
		}
		while(leaf.neighbor != 0){
			for(int j = start; j < leaf.size(); j++){
				if(leaf.keys[j] <= high){
					range.add(leaf.children[j]);
				}else{
					return range;
				}
			}
			leaf = new BTreeNode(leaf.neighbor);
		}
		//iterate through the final leaf
		for(int i = 0; i < leaf.size(); i++){
			if(leaf.keys[i] <= high){
				range.add(leaf.keyAddr[i]);
			}else{
				return range;
			}
		}
		return range;
	}
	
	
	//freelist update with remove
	//two different pieces:
	//1) what is the address of the next free space im going to use (item of free list or extending file)
	//don't remove or extend file yet
	//2)if insert returns true
	//update the free list by adding the new key (make it used)
	private long search(int k, long nodeAddr, boolean push) throws IOException{
		BTreeNode node = new BTreeNode(nodeAddr);
		if(push){
			path.push(node);
		}
		if(node.isLeaf()){
			for(int i = 0; i < node.size(); i++){
				if(node.keys[i] == k){
					return node.keyAddr[i];
				}
			}
			return 0;
		}else{
			for(int i = 0; i < node.size(); i++){
				if(node.keys[i] > k){
					return search(k, node.children[i], true);
				}
			}
			return search(k, node.children[node.size()], true);
		}
	}
	
	public long search(int k) throws IOException{
		return search(k, root, true);
	}		

	public void close() throws IOException{
		f.seek(0);
		f.writeLong(root);
		f.close();
	}
} 
	

