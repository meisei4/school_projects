package predicates;

public class GreaterThan<T> implements Predicate<Comparable<T>>{
    
	private T c;

    public GreaterThan(T c){
        this.c = c;
    }

    public boolean accepts(Comparable<T> co){
        return co.compareTo(c) > 0 ? true : false;
    } 
}
