package predicates;
import java.lang.*;
import java.awt.*;

public class SimilarColor implements Predicate<Color>{
    
    private Color c;

    public SimilarColor(Color c){
        this.c = c;
    }

    public boolean accepts(Color co){
        return (Math.abs(c.getRed() - co.getRed())
              + Math.abs(c.getGreen() - co.getGreen())
              + Math.abs(c.getBlue() - co.getBlue())) <= 30;
    }
}
