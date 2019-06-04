
//Ian Edward Neville
import java.io.*;
import java.util.*;
public class H1 <T extends Comparable <? super T>> {

	private class Node {
		T data;
		Node next;
		Node(T d, Node n){
			data = d;
			next = n;
		}
	}
	
	private Node head;

	public H1(){
		head = null;
	}

	public void insert(T d){
		if(head == null){
			head = new Node(d, null);
		}else{
			if(head.data.compareTo(d) > 0){
				Node enteringNode = new Node(d,head);
				head = enteringNode;
				return;
			}
			Node temp = head;
			while(temp.next != null && temp.next.data.compareTo(d) < 0){
				temp = temp.next;
			}
			Node enteringNode = new Node(d, temp.next);
			temp.next = enteringNode;
		}
	}

	private String create(Node n){
		if(n.next == null){
			return "" + n.data;
		}
		return n.data + "," + create(n.next);

	}

	public String toString() {
		return create(head);

	}
	public static void main(String args[]){
		try{
			if (args.length != 1){
				throw new ArgumentsErrorException();
			}
			BufferedReader fin = new BufferedReader(new FileReader(args[0]));
			String line;
			while((line = fin.readLine()) != null){
				Scanner s1 = new Scanner(line);
				s1.useDelimiter("[:,]");
				if(s1.next().equals("String")){
					H1<String> ladder = new H1<String>();
					while(s1.hasNext()){
						ladder.insert(s1.next());
					}
					System.out.println(ladder.toString());
				}else{
					H1<Integer> ladder = new H1<Integer>();
					while(s1.hasNextInt()){
						ladder.insert(s1.nextInt());
					}
					System.out.println(ladder.toString());
				}
			}

		}catch(FileNotFoundException e1){
			System.out.println("File not found");
		}catch(ArgumentsErrorException e2){
			System.out.println("Must have exactly 1 argument");
		}catch(IOException e2){
			System.out.println("IO exception");
		}

	}
}
