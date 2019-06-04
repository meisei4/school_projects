import java.io.*;
import java.util.*;

public class Hw1 {

    private static Scanner fin;
    private static LinkedList<Hashtable<String, String>> scopes;
    private static ArrayList<String> results;
    //right paren accumulator 
    private static int rightparens;
    //left paren accumulator
    private static int leftparens;
    //read token for each non-space character
    private static String token;

    public static void main(String args[]) throws Exception{
        fin = new Scanner(new FileReader(new File(args[0])));
        fin.useDelimiter(" ");
        scopes = new LinkedList<Hashtable<String, String>>();
        results = new ArrayList<String>();
        rightparens = 0;
        leftparens = 0;
        while(fin.hasNext()){
            String result = evalExpr();
            results.add(result);
        }
        for(int i = 0; i < results.size(); i++){
            System.out.println(results.get(i));
        }
    }
   
    public static void evalEnv() throws Exception{
        Hashtable<String, String> env = new Hashtable<String, String>();
        String key = fin.next();
        //DEAL WITH EMPTY ENV
        if(key.equals(")")){
            rightparens++;
            key = fin.next();
        }
        while(rightparens != (leftparens-1)){
            String val = evalExpr();
            env.put(key, val);
        }
        scopes.push(env);
    }

    public static String evalExpr() throws Exception{
        token = fin.next();
        if(token.equals("(")){
            leftparens++;
            token = fin.next();
            if(token.equals("(")){
                leftparens++;
                evalEnv();
                scopes.pop();
                return evalExpr();
            }
        }
        if(isOperator(token)){
            String arg1 = evalExpr();
            String arg2 = evalExpr();
            return applyOperation(arg1, arg2, token);
        }
        if(isIdentifier(token)){
            if(!scopes.isEmpty() && scopes.peek().get(token) != null){
                return scopes.peek().get(token);
            }
            return "undefined";
        }
        if(isNumber(token) || token.equals("undefined")){
            return token;
        }
        if(token.equals(")")){
            rightparens++;
            return evalExpr();
        }
        //shouldn't reach
        throw new Exception("SHOULD NOT REACH HERE");
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
            default: return "";
        }
    }
}
