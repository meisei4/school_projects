import java.util.*;	
public class SymbolTable {	

	private	class Node {	
		private	String	key;	
		private	LinkedList<Integer>	lines;	
		//used	for	a	list	of	line	numbers	on	which	the	key	was	found		
		private	Node next;	
		private	Node(String	k, int lineNum,	Node x) {	
			key	= k;	
			lines =	new	LinkedList<Integer>();	
			lines.addLast(lineNum);	
			next = x;	
		}	

		private	void addLineNum(int	lineNum) {	
			if(lines.indexOf(lineNum) < 0){
				lines.add(lineNum);
			}
		}	
	
		public String toString(){
			String str = key + " ";
			String lineNums = "";
			for(int i = 0; i < lines.size(); i++){
				lineNums = lineNums + " " + lines.get(i);
			}
			return str + lineNums;
		}
	}	
	private int size;
	private	Node[] table;	
	private	int	tableSize;	
	public SymbolTable(int s){	
		table = new Node[s];
		tableSize = s;
	}	
	
	private	int	hash(String	k)	{	
		return	k.hashCode()%tableSize;	
	}	

	public boolean insert(String k, int lineNum) {	
		Node node = new Node(k, lineNum, null);
		int code = hash(k);
		if(!find(k)){
			Node temp = table[code];
			table[code] = node;
			node.next = node;
			size++;
			return true;
		}else{
			Node temp = table[code];
			while(temp.key != k){
				temp = temp.next;
			}
			temp.addLineNum(lineNum);
			return false;
		}
		//if	k	is	not	in	the	table	add	it	to	the	table	with	lineNum	as	the	first	line	number	on		
		//which	k	is	found	and	return	true	
		//if	k	is	in	the	table,	add	lineNum	to	the	list	of	line	numbers	and	return	false	
	}	
	
	public boolean find(String k) {	
		int code = hash(k);
		if(table[code] == null){
			return false;
		}else{
			Node node = table[code];
			while(node != null){
				if(node.key.equals(k)){
					return true;
				}
				node = node.next;
			}
			return false;
		}
	}	

	public LinkedList<Integer> getLines(String k) {	
		if(find(k)){
			int code = hash(k);
			Node temp = table[code];
			while(temp.key != k){
				temp = temp.next;
			}
			return temp.lines;
		}
		return null;
		//if	k	is	in	the	table	return	the	line	numbers	on	which	k	is	found	
		//if	k	is	not	in	the	table	return	null	
	}	

	public class STIterator	implements Iterator<String>	{	
		Node node;
		int tableIndex;
		int hashIndex;
		//An	iterator	that	iterates	through	the	keys in	the	table	
		public STIterator() {	
			node = table[0];
			tableIndex = 0;
			hashIndex = 0;
		}	

		public boolean hasNext() {
			if(hashIndex < size){
				return true;
			}
			return false;
		}	

		public String next() {	
			//PRE:	hasNext()	
			if(node.next == null){
				while(tableIndex < tableSize && table[tableIndex] == null){
					tableIndex++;
				}
				hashIndex++;
				return table[tableIndex].toString();
			}else{
				hashIndex++;
				return node.next.toString();
			}
		}	

		public void remove() {	
			
		}
	}
	public boolean delete(String k)	{	
		int code = hash(k);
		if(table[code] == null){
			return false;
		}else{
			Node node = table[code];
			Node temp = node.next;
			while(node != null){					
				if(node.key == k){
					node = temp;
					return true;
				}
				node = node.next;
				temp = node.next;
			}

		}
		return false;
	}	
	
	public Iterator<String> iterator() {	
			return new STIterator();
	}	
	
	public static void main(String args[]){
		SymbolTable table = new SymbolTable(5);
		
	}
}
	