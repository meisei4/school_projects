public class H4<T extends Comparable<? super T>> {

	T keys[]; // the values used to order the heap
	Object data[]; 
	int maxChildren;
	int size;
	//s is size of the tree
	public H4(int s, int m) {
		keys = (T[]) new Comparable[s];
		data = new Object[s];
		maxChildren = m;
		size = 0;
	}
	
	public T getMinKey() {
		return keys[0];
	}
	
	//PRE: !empty()
	public Object getMinData() {
		return data[0];
	}
//  ////////////////////////////////////TEST/////////////////////////////////////////////
//	public static void main(String[] args){
//		H4 test = new H4(30, 3);
//		for(int j = 1; j < 20; j++){
//			test.insert(j, new Integer(j));
//		}
//		for(int k = 0; k < 7; k++){
//			System.out.println(test.getMinKey());
//			test.removeMin();
//		}
//
//		for(int i = 0; i < test.getSize(); i++){
//			System.out.println("(" +  test.keys[i] + ", " + test.data[i] + ")  ");
//		}
//		
//	}
/////////////////////////////////////////////////////////////////////////////////////////	
	private int smallestChild(int parent){
		int smallest = parent*maxChildren + 1;
		int j = 0;
		for(int i = smallest; j < maxChildren && i+1 < size; i++){
			if(keys[smallest].compareTo(keys[i + 1]) > 0){
				smallest = i + 1;
			}
			j++;
			
		}
		return smallest;
	}
		
		
	//PRE: !empty()
	public void removeMin() {
		int parent;
		int child;
		T endK = keys[size-1];
		Object endO = data[size-1];
		size--;
		parent = 0;
		child = smallestChild(parent);
		keys[parent] = endK;
		data[parent] = endO;
		while(keys[parent].compareTo(keys[child]) >= 0){
			T tempK = keys[parent];
			Object tempO  = data[parent];
			keys[parent] = keys[child];
			data[parent] = data[child];
			keys[child] = tempK;
			data[child] = tempO;
			parent = child;
			if(parent*maxChildren + 1 > size){
				break;
			}
			child = smallestChild(parent);	
		}
	}

	//PRE: !full();
	public void insert(T k, Object d) {
		int child;
		int parent;
		size++;
		child = size - 1;
		//adjust for 0 indexing
		parent = (child-1)/maxChildren;
		if(size == 1){
			keys[child] = k;
			data[child] = d;
			return;
		}

		try{
			while (keys[parent].compareTo(k) > 0) { 
				keys[child] = keys[parent];
				data[child] = data[parent];
				child = parent;
				parent = (child-1)/maxChildren;
			}
			keys[child] = k;
			data[child] = d;
		}catch(ArrayIndexOutOfBoundsException e){
			keys[child] = k;
			data[child] = d;
		}
	}

	public boolean empty() {
		return size == 0;
	}

	public boolean full() {
		return size == keys.length;
	}

	public int getSize(){
		return size;
	}

}