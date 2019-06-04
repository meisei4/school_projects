package predicates;

public class StartsWith implements Predicate<String>{
    
    private String str;

    public StartsWith(String str){
        this.str = str;
    }

    public boolean accepts(String stri){
        return stri.startsWith(str);
    }
}
