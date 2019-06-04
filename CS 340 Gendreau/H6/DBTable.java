import java.util.*;
import java.io.*;
public class DBTable {
	RandomAccessFile rows; 
	BTree btree;
	long free;
	int numOtherFields;
	int[] otherFieldLengths;
	int bsize;
	private class Row{
		private int keyField;
		private char otherFields[][];
		
		private Row(int keyField, char[][] otherFields) throws IOException{
			this.keyField = keyField;
			this.otherFields = otherFields;
		}

		private Row(long addr) throws IOException{
			rows.seek(addr);
			this.keyField = rows.readInt();
			this.otherFields = new char[numOtherFields][];
			for(int i = 0; i < numOtherFields; i++){
				this.otherFields[i] = new char[otherFieldLengths[i]];
			}
			for(int i = 0; i < numOtherFields; i++){
				for(int j = 0; j < otherFields[i].length; j++){
					otherFields[i][j] = rows.readChar();
				}
			}
		}
			
		//determines the address of the row and writes it
		private void writeRow(long addr) throws IOException{
			rows.seek(addr);
			rows.writeInt(keyField);
			//the key itself
			for(int i = 0; i < otherFields.length; i++){
				for(int j = 0; j < otherFields[i].length; j++){
					rows.writeChar(otherFields[i][j]);
				}
			}
		}
	}
	
	public DBTable(String filename, int fL[], int bsize) throws IOException{
		File r = new File(filename);
		if(r.exists()){
			r.delete();
		}
		rows = new RandomAccessFile(new File(filename), "rw");
		rows.seek(0);
		numOtherFields = fL.length;
		otherFieldLengths = new int[fL.length];
		for(int i = 0; i < numOtherFields; i++){
			otherFieldLengths[i] = fL[i];
		}
		rows.writeInt(numOtherFields);
		for(int i = 0; i < numOtherFields; i++){
			rows.writeInt(fL[i]);
			//write the field lengths
		}
		free = 0;
		rows.writeLong(free);
		this.bsize = bsize;
		btree = new BTree(filename + "_btree", bsize);
	}
	
	public DBTable(String filename) throws IOException{
		File r = new File(filename);
		rows = new RandomAccessFile(r, "rw");
		rows.seek(0);
		numOtherFields = rows.readInt();
		otherFieldLengths = new int[numOtherFields];
		for(int i = 0; i < numOtherFields; i++){
			otherFieldLengths[i] = rows.readInt();
		}
		free = rows.readLong();
		btree = new BTree(filename + "_btree");
		bsize = btree.getBlockSize();
	}
	
	private long getFree() throws IOException{
		long addr = 0;
		if(free == 0){
			addr = rows.length();
		}else{
			addr = free;
			rows.seek(free);
			free = rows.readLong();
		}
		return addr;
	}

	//the length of each row in fields matches the expected length
	public boolean insert(int key, char[][] fields) throws IOException{
		System.out.println("NEW ROW WITH KEY: " + key);
		for(int k = 0; k < numOtherFields; k++){
			System.out.println("At fields[" + k + "]: (field length should be " + fields[k].length + " long):  ");
			for(int l = 0; l < fields[k].length; l++){
				System.out.print("At fields[" + k + "][" + l + "]: ");
				System.out.print(fields[k][l] + " ");
			}
		}
		System.out.println();
		System.out.println();
		if(btree.root == 0 || btree.search(key) == 0 ){
			System.out.println("NEW ROW WITH KEY: " + key);
			System.out.println();
			System.out.println();
			Row data = new Row(key, fields);
			for(int k = 0; k < numOtherFields; k++){
				System.out.println("At otherFields[" + k + "]: (field length should be " + data.otherFields[k].length + " long) ");
				for(int l = 0; l < data.otherFields[k].length; l++){
					System.out.print("At otherFields[" + k + "][" + l + "]: ");
					System.out.print(data.otherFields[k][l] + " ");
				}
			}
			long addr = getFree();
			data.writeRow(addr);
			Row datas = new Row(addr);
			System.out.println("THIS");
			System.out.println("THIS IS BEING DELETED IN THE PRINT---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
			for(int k = 0; k < numOtherFields; k++){
				System.out.println("At otherFields[" + k + "]: (field length should be " + datas.otherFields[k].length + " long) ");
				for(int l = 0; l < datas.otherFields[k].length; l++){
					System.out.print("At otherFields[" + k + "][" + l + "]: ");
					System.out.print(datas.otherFields[k][l] + " ");
				}
			}
			return btree.insert(key, addr);
		}else{
			return false;
		}
		//use the B+tree to determine existence of key to be inserted
		//if exists return true;
		//if row is added, the key is also added to the B+tree
	}
	
	public boolean remove(int key){
		//use the B+tree to determine existence of key to be removed
		//if row is removed-----the key in the B+tree must be removed
		return false;
	}
	
	public LinkedList<String> search(int key) throws IOException{
		LinkedList<String> list = new LinkedList<String>();
		if(btree.search(key) == 0){
			return null;
		}else{
			long addr = btree.search(key);
			Row tempRow = new Row(addr);
			rows.seek(addr);
			for(int i = 0; i < numOtherFields; i++){
				String str = "";
				for(int j = 0; j < otherFieldLengths[i]; j++){
					char c = tempRow.otherFields[i][j];
					str = str + c;	
				}
				str.trim();
				System.out.print("Data from other fields:  ");
				for(int k = 0; k < numOtherFields; k++){
					for(int l = 0; l < tempRow.otherFields[k].length; l++){
						System.out.print(tempRow.otherFields[k][l] + " ");
					}
				}
				//remove trailing whitespace
				list.push(str);
			}
			for(int i = 0; i < list.size(); i++){
				System.out.print(list.get(i) + ", ");
			}
			System.out.println();
			return list;
		}
	}
	
	//PRE: low <= high

	public LinkedList<LinkedList<String>> search(int low, int high) throws IOException{
		LinkedList<LinkedList<String>> string = new LinkedList<LinkedList<String>>();
		LinkedList<Long> addrs = btree.search(low, high);
		for(int i = 0; i < addrs.size(); i++){
			rows.seek(addrs.get(i));
			int key = rows.readInt();
			//check
			LinkedList<String> fields = new LinkedList<String>();
			Row row = new Row(addrs.get(i));
			for(int k = 0; k < numOtherFields; k++){
				String str = "";
				for(int j = 0; j < otherFieldLengths[k]; j++){
					char c = row.otherFields[k][j];
					str = str + c;
				}
				str = key + str;
				fields.add(str);
			}
			string.add(fields);
		}
		return string;
	}
	
	public void print() throws IOException{
		LinkedList<LinkedList<String>> string = search(Integer.MIN_VALUE, Integer.MAX_VALUE);
		for(int i= 0; i < string.size(); i++){
			for(int j = 0; j < string.get(i).size(); j++){
				System.out.println(string.get(i).get(j));
			//ascending order
			//traverse the leaves with neighbor calls
			}
		}
		
	}
	
	public void close() throws IOException{
		btree.close();
		rows.close();
	}
}
