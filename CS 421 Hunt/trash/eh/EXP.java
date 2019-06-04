import java.io.*;
import java.util.*;

public class hw1 {

    private static Scanner fin;
    private static LinkedList<Hashtable<String, String>> scopes;
    private static ArrayList<String> results;

    public static void main(String args[]) throws Exception{
        fin = new Scanner(new FileReader(new File(args[0])));
        fin.useDelimiter(" ");
        scopes = new LinkedList<Hashtable<String, String>>();
        inEnv = false;
        while(Scanner.hasNext()){
            String result = eval();
            results.add(result);
        }
    }
 
    private static boolean isEnv;

    public static String eval(String symbol) throws Exception{ 
        if(symbol.equals("(")){
            if((symbol = fin.next()).equals("(")){
                if((symbol = fin.next()).equals(")")){
                    return eval(fin.next());
                }
                Hashtable<String, String> scope = new Hashtable<String, String>();
                isEnv = true;
                scope.put(symbol, eval(fin.next()));
                scopes.push(scope);
                eval(fin.next());
                scopes.pop();
            }
            return eval(symbol);
        }
        if(isOperator(symbol)){
            isEnv = false;
            String arg1 = eval(fin.next());
            String arg2 = eval(fin.next());
            return applyOperation(arg1, arg2, symbol);
        }
        if(isIdentifier(symbol)){
            if(scopes.peek().containsKey(symbol)){
                return scopes.peek().get(symbol);
            }
            return "undefined";
        }
        if(isNumber(symbol) || symbol.equals("undefined")){
            return symbol;
        }
        if(symbol.equals(")")){
            return eval(fin.next());
        }
        throw new Exception("invalid expression");
    }

    public static boolean isOperator(String e){
        return e.equals("+") ||
               e.equals("-") ||
               e.equals("*") ||
               e.equals("/") ||
               e.equals("^") ||
               e.equals("%");
    }
    
    public static boolean isIdentifier(String e){
        return e.length() == 1 && Character.isLetter(new Character(e.charAt(0)));
    }
    
    public static boolean isNumber(String str){
        try{
            double d = Double.parseDouble(str);  
        }catch(NumberFormatException nfe){  
            return false;  
        }
        return true;  
    }

    public static String applyOperation(String arg1, String arg2, String op) throws Exception{
        if(arg1.equals("undefined") || arg2.equals("undefined")){
            return "undefined";
        }
        if((op.equals("/") || op.equals("%")) && arg2.equals("0")){
            return "undefined";
        }
        switch(op){
            case "+": return "" + (Integer.parseInt(arg1) + Integer.parseInt(arg2));
            case "-": return "" + (Integer.parseInt(arg1) - Integer.parseInt(arg2));
            case "*": return "" + (Integer.parseInt(arg1) * Integer.parseInt(arg2));
            case "/": return "" + (Integer.parseInt(arg1) / Integer.parseInt(arg2));
            case "^": return "" + (int) (Math.pow(Integer.parseInt(arg1), Integer.parseInt(arg2)));
            case "%": return "" + (Integer.parseInt(arg1) % Integer.parseInt(arg2));
            default: throw new Exception("invalid Operator");
        }
    }
}
