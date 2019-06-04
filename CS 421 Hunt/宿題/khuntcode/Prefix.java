
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Stack;

public class Prefix {
    
	public static Object parse( String[] s ) {
		if( s.length == 1 ) return s[0];
    
		Stack<List<? super Object>> stack = new Stack<>();
		for( String token : s ) {
			if( token.equals("(") ) {
				List<Object> list = new ArrayList<>();
				stack.push( list );
			} else if( token.equals(")") ) {
				List<Object> top = stack.pop();
				if( stack.isEmpty() ) {
					return top;
				} else {
					stack.peek().add( top );
				}
			} else {
				stack.peek().add( token );
			}
		}
		
		throw new RuntimeException("Error with input expression");
	}
	
	public static boolean isNumber( String s) {
		try {
			new Integer(s);
			return true;
		} catch(Exception e) {
			return false;
		}
	}
	
	private static boolean isEnvironment(Object x) {
		return (x instanceof List) && ( ((List)x).get(0) instanceof List );
	}
	
	public static Map<String, Integer> environment( List<Map<String,Integer>> env, List<Object> envExpression ) {
		Map<String,Integer> result = new HashMap<String, Integer>();
		for( Object def : envExpression ) {
			List<? super Object> definition = (List<Object>)def;
			result.put( (String)definition.get(0), evaluate( env, definition.get(1) ) );			
		}
		return result;
	}
	
	public static Integer lookup( List<Map<String, Integer>> env, String variable ){
		for( int i = env.size()-1; i >= 0 ; i-- ) {
			Map<String, Integer> e = env.get(i);
			if( e.get( variable ) != null ) return e.get(variable);
		}
		return null;
	}
	
	// the env is a list of maps.  Each map is from a single env-expression.
	// The inner-most env is at the beginning of the list
	public static Integer evaluate( List<Map<String,Integer>> env, Object x ) {
		if( x instanceof String ) {
			if( x.equals("undefined") ) return null;
			else if( isNumber( (String)x )) return new Integer( (String) x);
			else return lookup( env, (String)x );
		} else { 
			// x must be an instanceof list in this branch 
			if( isEnvironment( x ) ) {
				List<Map<String, Integer>> newEnvironment = new ArrayList<>( env );
				newEnvironment.add( environment( env, (List<Object>) ((List)x).get(0) ) );
				return evaluate( newEnvironment, ((List)x).get(1) );
			} else {
				// x must be an operator-type expression 
				return apply(env, (List<Object>)x );
			}
		}
	}
	
	private static Integer apply(List<Map<String,Integer>> env, List<? super Object> x) {
		String operator = (String)x.get(0);
		Integer op1 = evaluate( env, x.get(1) );
		Integer op2 = evaluate( env, x.get(2) );
		if( op1 == null || op2 == null ) return null;
		if( op2.equals(0) && operator.equals("/") ) return null;
		
		switch( operator ) {
		case "+" : return op1 + op2;
		case "-" : return op1 - op2;
		case "*" : return op1 * op2;
		case "/" : return op1 / op2;
		case "%" : return op1 % op2;	
		case "^" : return (int)Math.floor( Math.pow(op1,  op2) );
		}
		
		throw new RuntimeException( "syntax error in binary op" + x );
	}

	public static void main( String[] args ) {
		// this driver can be easily modified to read each line of a file and print the results of evaluating those lines. 
		// This code is just for quick testing.
		String[] exps = {
				"undefined",
				"( + ( - 10 2 ) 3 )",
				"( + 4 ( ( ( e 5 ) ( v 10 ) ( j 7 ) ( z ( / 2 1 ) ) )  ( ( ( r 6 ) )  ( * 9 7 ) )  )  )", // 67				
				"( ( ( a 3 ) ) ( * ( ( ( a 4 ) ) a ) a ) )",
				"( ( ( a ( * 3 1 ) ) ) ( * ( ( ( a 4 ) ) a ) a ) )",
				"( ( ( a ( * 3 1 ) ) ) ( * ( ( ( a 4 ) ( b a ) ) b ) a ) )",
				"( % 12 5 )", 
				"( ^ 12 5 )", 
				"( + a 3 )",
				"( ( ( a 3 ) ( b 10 ) ) ( * a ( + b 1 ) ) )",
				"( ( ( a 5 ) ) ( * a ( ( ( a 3 ) ( b 10 ) ) ( * a ( + b 1 ) ) ) ) )"
		};
		
		for( String exp : exps ) {
			exp = exp.replaceAll("[ ]+", " "); // replace all sequences of spaces with a single space
			Integer result = evaluate( new ArrayList<>(), parse( exp.split(" ") ) );
			System.out.println( exp + " ==> " + (result == null ? "undefined" : result ) );
		}
	}
}
