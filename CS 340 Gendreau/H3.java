
import java.util.*;


import java.io.*;
public class H3 {
	int keySize;
	int dataSize;
	RandomAccessFile list;
	long head;
	long free;

	private class Node{
		private int[] key;
		private char[] data;
		private long next;
		
		
		private Node(int[] k, char[] d, long n){
			key = k;
			data = d;
			next = n;
		}

		//returns if k1 is greater than k2
		private int compareKey(int[] key){
			if(Arrays.equals(this.key, key)){
				return 0;
			}
			int i = 0;
			while(this.key[i] == key[i]){
				i++;
			}
			return this.key[i] - key[i];
				
		}
		
		private Node(long addr) throws IOException{
			list.seek(addr);
			key = new int[keySize];
			for(int i = 0; i < keySize; i++){
				key[i] = list.readInt();
			}
			data = new char[dataSize];
			for(int j = 0; j < dataSize; j++){
				data[j] = list.readChar();
			}
			next = list.readLong();
		}
	
		private void writeNode(long addr) throws IOException{
			list.seek(addr);
			for(int i = 0;i < keySize;i++){
				list.writeInt(key[i]);
	
			}
			for(int i = 0;i < data.length;i++){
				if(data[i] != '\u0000'){
				//character for empty
					list.writeChar(data[i]);
				}
				else{
					list.writeChar(' ');
				}
			}
			list.writeLong(next);
		}
	}
	
	public H3(int ks, int ds, String filename) throws IOException{
		File path = new File(filename);
		if(path.exists()){
			path.delete();
		}
		list = new RandomAccessFile(filename, "rw");
		//"rw" is read write mode
		keySize = ks;
		dataSize = ds;
		head = 0;
		free = 0;
		list.writeLong(head);
		list.writeLong(free);
		list.writeInt(ks);
		list.writeInt(ds);
	}
	
	//creates an H3E based off inputed file
	public H3(String filename) throws IOException{
		list = new RandomAccessFile(filename, "rw");
		list.seek(0);
		head = list.readLong();
		free = list.readLong();
		keySize = list.readInt();
		dataSize = list.readInt();
	}
	
	private long getFree() throws IOException{
		long addr = 0;
		Node temp;
		if(free == 0){
			addr = list.length();
		}else{
			addr = free;
			temp = new Node(free);
			free = temp.next;
		}
		return addr;
	}
	
	public void insert(int[] key, char d[]) throws IOException{
		Node temp;
		long addr = getFree();
		if(head == 0){
			//when the list is empty
			//inserting head
			Node headNode = new Node(key, d, 0);
			headNode.writeNode(addr);
			head = addr;
		}else {
			Node headNode = new Node(head);
			if(headNode.compareKey(key) >= 0){
				//inserting new head
				Node enteringNode = new Node(key, d, head);
				enteringNode.writeNode(addr);
				head = addr;
				return;	
			}
			long tempAddr = head;
			temp = new Node(head);
			while(temp.next != 0){
				temp = new Node(tempAddr);
				Node tempNode2 = new Node(temp.next);
				if(tempNode2.compareKey(key) >= 0){
					Node enteringNode = new Node(key, d, temp.next);
					temp.next = addr;
					temp.writeNode(tempAddr);
					enteringNode.writeNode(addr);
					return;
				}
				tempAddr = temp.next;
				temp = new Node(temp.next);
			}
			temp.next = addr;
			Node enteringNode = new Node(key, d, 0);
			temp.writeNode(tempAddr);
			enteringNode.writeNode(addr);
		}
	}

	public void remove(int key[]) throws IOException{
		long addr;	
	 	long addr1;	
	 	Node temp;	
	 	Node temp1;

	 	while(head != 0 && Arrays.equals((temp = new Node(head)).key, key)){	
			addr = temp.next;	
			addFree(head,temp);	
			head = addr;	
	 	}
		if(head == 0){	
	 		return;	
	 	}
	 	temp = new Node(head);	
	 	addr = head;	
	 	while(temp.next != 0) {	
	 		
			temp1 =	new Node(temp.next);
			addr1 =	temp.next;	

			if(Arrays.equals(temp1.key,key)){	
			 	temp.next = temp1.next;	
				addFree(addr1, temp1);	
				
			}else{	
				temp.writeNode(addr);	
				temp = temp1;	
				addr = addr1;	
			}	
	 	}	
	 	temp.writeNode(addr);	
		
	}
	private void addFree(long newFree,Node temp) throws IOException{
		temp.next = free;
		temp.writeNode(newFree);
		free = newFree;
	}
	
	public String[] find(int key[]) throws IOException {
		long addr = head;
		Node temp = new Node(head);
		LinkedList<String> strings = new LinkedList<String>();
		while(addr != 0){
			list.seek(addr);
			if(temp.compareKey(key) == 0){
				for(int i = 0; i < keySize; i++){
					list.readInt();
				}
				char[] str = new char[dataSize];
				for(int j = 0; j < dataSize; j++){
					str[j] = list.readChar();
				}
			    String realString = new String(str);
				strings.add(realString);
			}
			addr = temp.next;
			temp = new Node(temp.next);
		}
		
		String[] result = strings.toArray(new String[strings.size()]);
		if(result.length == 0){
			return null;
		}
		return result;
	}
	

	public void print() throws IOException{
		list.seek(head);
		long addr = head;
		Node temp;
		while(addr != 0){
			temp = new Node(addr);
			String s = "";
			for(int i = 0;i<temp.data.length;i++){
				s += temp.data[i];
			}
			for(int i = 0;i<temp.key.length;i++){
				System.out.print(""+temp.key[i]+"  ");
			}
			System.out.print(s);
			System.out.println("");
			addr = temp.next;
		}
		System.out.println();
	}

	public void close() throws IOException{
		list.seek(0);
		list.writeLong(head);
		list.writeLong(free);
		list.close();
	}
}
