package predicates;

import java.util.*;

public class PredicateUtilities{
	
    public static <T extends List<T>> void listFilter(Predicate<T> pr, List<T> lst){
        List<T> ls = new ArrayList<T>(); 
        for(int i = 0; i < lst.size(); i++){
            if(pr.accepts(lst.get(i))){
                ls.add(lst.get(i));
            }
        }
    }
}