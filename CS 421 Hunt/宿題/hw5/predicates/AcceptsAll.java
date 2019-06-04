package predicates;

import java.util.*;

public class AcceptsAll<T> implements Predicate<Predicate<T>>{
    
	private List<T> pr;

    public AcceptsAll(List<T> pr){
        this.pr = pr;
    }

    public boolean accepts(Predicate<T> pre){
        for(int i = 0; i < pr.size(); i++){
            if(!pre.accepts(pr.get(i))){
                return false;
            }
        }
        return true;
    }
}
