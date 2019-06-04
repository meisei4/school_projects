package predicates;

import java.util.*;

public class And<T> implements Predicate<T>{
    
	private ArrayList<Predicate<T>> ls;

    public And(Predicate<T> ...pr){
    	ls = new ArrayList<Predicate<T>>();
        for(Predicate<T> i : pr){
            ls.add(i);
        }
    }

    public boolean accepts(T t){ 
        for(int i = 0; i < ls.size(); i++){
            if(!ls.get(i).accepts(t)){
                return false;   
            }
        }
        return true;
    }
}
