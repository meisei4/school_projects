
import java.io.*;
import java.util.*;

public class H2 {

	
	private class VertexNode{
		private String name;
		private VertexNode nextV;
		private EdgeNode edges;
		private  int inDegree;
		
		private VertexNode(String n, VertexNode v){
			name = n;
			nextV = v;
			edges = null;
			inDegree = 0;
		}
	}
	
	private class EdgeNode{
		private VertexNode vertex1;
		private VertexNode vertex2;
		private EdgeNode nextE;
		
		private EdgeNode(VertexNode v1, VertexNode v2, EdgeNode e){
			vertex1 = v1;
			vertex2 = v2;
			nextE = e;
		}
	}
	
	private VertexNode vertices;
	private int numVertices;
	
	public H2(){
		vertices = null;
		numVertices = 0;
	}
	
	//insert method from H1 where list is sorted upon insertion
	/**private void insert(String s){
		if(vertices == null || vertices.name.compareTo(s) > 0){
			vertices = new VertexNode(s, vertices);
			return;
		}
		VertexNode temp = vertices;
		while(temp.nextV != null && temp.nextV.name.compareTo(s) > 0){
			temp = temp.nextV;
		}
		temp.nextV = new VertexNode(s,temp.nextV);
	}**/
	
	//create vertex
	//update vertex linked list
	public void addVertex(String s){
		VertexNode vertex = new VertexNode(s, vertices);
		vertices = vertex;
		numVertices++;
	}
	
	//takes a string: The name of the Vertex Node
	//looks at vertices linked list 
	//returns vertexNode with the corresponding string name
	private VertexNode findNodeNamed(String n1){
		VertexNode temp = vertices;
		while(temp != null){
			if(temp.name.equals(n1)){
				return temp;
			}
			temp = temp.nextV;
		}
		return null;
		//would use FileFormatException if the edge list contains a 
		//vertex that is not in the vertex list
	}

	//given string of names of vertex nodes to be connected
	//findNodes
	//create edge node linked list based off of charitable node
	//increase inDegree of charity node
	public void addEdge(String n1, String n2){
		VertexNode node1 = findNodeNamed(n1);
		VertexNode node2 = findNodeNamed(n2);
		node1.edges = new EdgeNode(node1, node2, node1.edges);
		node2.inDegree++;
	}
	
	//Go through the vertex nodes till "v" 
	//then go through "v"s edges
	//for each edge of "v" decrement the edge's vertex's inDegree by one
	private void updateInDegree(VertexNode v){
		EdgeNode tempE = v.edges;
		while(tempE != null){
			tempE.vertex2.inDegree--;
			tempE = tempE.nextE;
		}
	}
	
	private LinkedList<VertexNode> inDegreeOfZero = new LinkedList<VertexNode>();
	private void findNextVertex(){
		VertexNode temp = vertices;
		while(temp != null){
			if(temp.inDegree == 0 && !inDegreeOfZero.contains(temp)){
				inDegreeOfZero.add(temp);
				updateInDegree(temp);
			}
			temp = temp.nextV;
		}
	}
	
	public String topoSort(){
		String topo = "";
		for(int i = 0; i < numVertices; i++){
			findNextVertex();	
		}
		if (inDegreeOfZero.size() != numVertices){
			return "This graph is cyclical";
		}
		for(int i = 0; i < numVertices; i++){
			topo = topo + inDegreeOfZero.remove().name;
		}
		return topo;
	}
	
	
	public static void main(String args[]){
		try{
			H2 graph = new H2();
			BufferedReader fin = new BufferedReader(new FileReader(args[0]));
			String verts = fin.readLine();
			String[] vertexArray = verts.split("\\s+");
			for(int i = 0; i < vertexArray.length;i++) {
				graph.addVertex(vertexArray[i]);
			}
			String line;
			while((line = fin.readLine()) != null){
				graph.addEdge(line.charAt(0) + "", line.charAt(2) + "");
			}
			System.out.println(graph.topoSort());
		}catch(IOException e){
			System.out.print("IO exception");
		}
	}
}