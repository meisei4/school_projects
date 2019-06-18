import java.io.*;
import java.util.*;

public class ExP {

    private static BufferedReader fin;   
    private static ArrayList<Hashtable<String, String>> scopes;
    private static ArrayList<String> identifiers;
    private static boolean wasId = false;

    public static void main(String args[]) throws Exception{
        LinkedList<String> results = interpret(args[0]);
        int j = 1;
        for(int i = results.size() - 1; i >= 0; i--){
            System.out.println((j++) + " " + results.get(i));
        }
    }
    
    public static LinkedList<String> interpret(String filename) throws Exception{
        fin = new BufferedReader(new FileReader(new File(filename)));
        scopes = new ArrayList<Hashtable<String, String>>();
        scopes.add(new Hashtable<String, String>());
        String line, token, e;
        LinkedList<String> results = new LinkedList<String>();
        while((line = fin.readLine()) != null){
            int[] depth = new int[1];
            depth[0] = 0;
            identifiers = new ArrayList<String>();
            scopes.add(new Hashtable<String, String>());
            //try{
                Scanner lin = new Scanner(line);
                LinkedList<String> expStack = new LinkedList<String>();   
                 
                while(lin.hasNext()){
                    token = lin.next();
                    if(token.equals("undefined")){
                        expStack.push(token);
                        //System.out.println(token);
                    }else if(token.equals("(")){
                        depth[0]++;
                        Hashtable<String, String> nest = clone(scopes.get(depth[0] - 1));
                        scopes.add(nest);
                        System.out.println("HASH = ");
                        printHash(nest);
                        expStack.push(token);
                        //System.out.println(token);
                    }else if(isOperator(token)){
                        expStack.push(token);
                        //System.out.println(token);
                    }else if(isIdentifier(token)){
                        if(!identifiers.contains(token)){
                            identifiers.add(token);
                        }
                        expStack.push(token);
                        //System.out.println(token);
                    }else if(isNumber(token)){
                        expStack.push(token);
                        //System.out.println(token);
                    }else if(token.equals(")")){
                        e = token;
                        while(!expStack.isEmpty() && !expStack.peek().equals("(")){
                            e = expStack.pop() + " " + e;
                        }
                        if(expStack.isEmpty()){
                            throw new Exception("invalid E-expression");
                        }
                        e = expStack.pop() + " " + e;
                        System.out.println("UNEVALED E = " + e);
                        printHash(scopes.get(depth[0]));
                        System.out.println("DEPTH = " + depth[0]);
                        e = eval(e, depth);
                        depth[0]--;
                        System.out.println("EVALED E = " + e);

                        System.out.println();
                        expStack.push(e);
                        printStack(expStack);
                    }
                }
                System.out.println(expStack.peek());
                results.push(eval(expStack.pop(), depth));
            //}catch(Exception exep){
                //continue
            //}
        }
        return results;
    } 
    
    public static void printHash(Hashtable<String, String> hash){
        for(int i = 0; i < hash.size(); i++){
            System.out.println(identifiers.get(i) + " --> " + hash.get(identifiers.get(i)));
        }
    }
    public static Hashtable<String, String> clone(Hashtable<String, String> mom){
        Hashtable<String, String> dolly = new Hashtable<String, String>();
        for(int i = 0; i < mom.size(); i++){
            dolly.put(identifiers.get(i), mom.get(identifiers.get(i)));
        }
        return dolly;
    }

    public static void printStack(LinkedList<String> expstack){
        LinkedList<String> ha = new LinkedList<String>();
        System.out.println();
        for(int i = ha.size() - 1; i >= 0; i--){
            System.out.print(ha.get(i) + " ");
        }        
        System.out.println();
    }

    public static String eval(String e, int[] depth) throws Exception{
        if(isNumber(e) || e.equals("undefined")){
            return e;
        }
        if(isIdentifier(e)){
            if(scopes.get(depth[0]).containsKey(e)){
                return scopes.get(depth[0] - 2).get(e);
            }
            return "undefined";
        }
        if(emptyExpression(e)){
            return "";
        }
        Scanner sc = new Scanner(e);
        sc.next();
        String symbol = sc.next();
        if(isIdentifier(symbol)){
            scopes.get(depth[0] - 2).put(symbol, eval(sc.next(), depth));
            if(!wasId){
                depth[0]--;
            }

            wasId = true;
            return "";
        }
        if(isNumber(symbol) || symbol.equals("undefined")){
            return symbol;
        }
        if(isOperator(symbol)){
            depth[0]--;
            String arg1 = eval(sc.next(), depth);
            String arg2 = eval(sc.next(), depth);
            wasId = false;
            return applyOperation(arg1, arg2, symbol);
        }
        throw new Exception("invalid E-expression");
    }
    
    public static boolean emptyExpression(String e){
        Scanner s = new Scanner(e);
        return s.next().equals("(") && s.next().equals(")");
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
