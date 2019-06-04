package predicates;

public class Negation<T> implements Predicate<T>{
    
	private Predicate<T> pr;

    public Negation(Predicate<T> pr){
        this.pr = pr;
    }

    public boolean accepts(T pre){
        return !pr.accepts(pre);
    }
}
