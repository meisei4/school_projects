import java.io.*;
import java.util.*;

public class ExP {

    private static Scanner fin;
    private static LinkedList<Hashtable<String, String>> scopes;

    public static void main(String args[]) throws Exception{
        fin = new Scanner(new FileReader(new File(args[0])));
        //fin.useDelimiter(" ");
        Hashtable<String, String> env = new Hashtable<String, String>();
        String result = interp(env, false);
        System.out.println(result);
    }
 
    private static boolean isEnv;
    private static String symbol;
    private static boolean flag;

    public static String evalEnv(String env){
        symbol = fin.next();
        
    }
    public static String eval(String exp) throws Exception{ 
        if(!flag){
            symbol = fin.next();
            flag = true;
        }
        if(symbol.equals("(")){
            if(!expStack.isEmpty() && expStack.peek().equals("(")){
                Hashtable<String, String> scope = new Hashtable<String, String>();
                while(isEnv){
                    scope.put(fin.next(), eval(fin.next()));
                    scopes.push(scope);
                    eval(fin.next());
                }
            }
            return eval(fin.next());
        }
        if(isOperator(symbol)){
            String arg1 = eval(envStackRep, true);
            String arg2 = eval(envStackRep, true);
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
        if(symbol.equals(")") && (symbol = fin.next()).equals(")")){
            flag = false;
            isEnv = false;
            return eval(symbol);
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
