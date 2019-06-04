import java.io.*;
import java.util.*;

public class ExP {

    private static BufferedReader fin;   

    public static void main(String args[]) throws Exception{
        LinkedList<String> results = interpret(args[0], envStack);
        int j = 1;
        for(int i = results.size() - 1; i >= 0; i--){
            System.out.println((j++) + " " + results.get(i));
        }
    }
    
    public static LinkedList<String> interpret(String filename) throws Exception{
        fin = new BufferedReader(new FileReader(new File(filename)));
        Hashtable<String, String> globe = new Hashtable<String, String>();
        String line, token, e;
        LinkedList<String> results = new LinkedList<String>();
        while((line = fin.readLine()) != null){
            try{
                Scanner lin = new Scanner(line);
                LinkedList<String> expStack = new LinkedList<String>();         
                while(lin.hasNext()){
                    token = lin.next();
                    if(token.equals("undefined")){
                        expStack.push(token);
                        //System.out.println(token);
                    }else if(token.equals("(")){
                        expStack.push(token);
                        //System.out.println(token);
                    }else if(isOperator(token)){
                        expStack.push(token);
                        //System.out.println(token);
                    }else if(isIdentifier(token)){
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
                        e = eval(e, globe);
                        System.out.println("EVALED E = " + e);
                        expStack.push(e);
                    }
                }
                System.out.println(expStack.peek());
                results.push(eval(expStack.pop(), globe));
            }catch(Exception exep){
            //continue through loop
            }
        }
        return results;
    } 
    
    public static String eval(String e, Hashtable<String, String> prevScope) throws Exception{ 
        Hashtable<String, String> curScope = new Hashtable<String, String>();
        curScope.putAll(prevScope);
        if(isNumber(e) || e.equals("undefined")){
            return e;
        }
        if(isIdentifier(e)){
            if(curScope.containsKey(e)){
                return curScope.get(e);
            }
            return "undefined";
        }
        //with parens
        if(emptyExpression(e)){
            return "";
        }
        Scanner sc = new Scanner(e);
        sc.next();
        String symbol = sc.next();
        if(isNumber(symbol) || symbol.equals("undefined")){
            return symbol;
        }
        if(isIdentifier(symbol)){
            String next = sc.next();
            if(next.equals(")")){
                return eval(symbol, curScope);
            }
            curScope.put(symbol, eval(next, curScope));
            return "";
        }
        if(isOperator(symbol)){
            String arg1 = eval(sc.next(), curScope);
            String arg2 = eval(sc.next(), curScope);
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
