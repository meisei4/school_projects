package predicates;
import java.util.*;

public class Subset<T extends List<?>> implements Predicate<T>{
    
	private T ls;

    public Subset(T ls){
        this.ls = ls;
    }

    public boolean accepts(T lst){
        for(int i = 0; i < lst.size(); i++){
            if(!ls.contains(lst.get(i))){
                return false;
            }
        }
        return true;
    }
}
