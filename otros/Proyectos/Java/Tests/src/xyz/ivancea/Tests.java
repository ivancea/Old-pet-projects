package xyz.ivancea;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.text.ParseException;
import java.util.Arrays;
import java.util.List;
import java.util.Map;
import java.util.Scanner;
import java.util.stream.Collectors;

class A {
	public A() {
		System.out.println("Default constructor");
	}
	
	public A(int n, String s) {
		for(int i=0; i<n; i++) {
			System.out.println(s);
		}
	}
	
	public A(int n, String s, boolean t) {
		for(int i=0; i<n; i++) {
			System.out.print(s);
			System.out.println(" -> " + t);
		}
	}
	
	public A(int n) {
		for(int i=0; i<n; i++) {
			System.out.println(".");
		}
	}
	
	public A(Integer n) {
		for(int i=0; i<n; i++) {
			System.out.println(".");
		}
	}
}

public class Tests {
	
	// Add "" to strings and check if null
	public static <T> T instantiate(Class<T> type, String input) {
		String[] inputArgs = input.split(",");
		
		for(int i=0; i<inputArgs.length; i++) {
			inputArgs[i] = inputArgs[i].trim();
		}
		
		
		

        boolean trimWords = true;
        List<String> words = Arrays.asList("As", "b", "adads");

        Map<String, Integer> t = words.stream()
            .collect(Collectors.<String, String, Integer>toMap(
                w -> w,
                w -> w.lenght()));
		
        
        
        
        
        
        
        
        
        
        
        
		
		try {
			for(Constructor<?> constructor : type.getConstructors()) {
				Class<?>[] parameterTypes = constructor.getParameterTypes();
				
				if(parameterTypes.length == inputArgs.length) {
					try {
						Object[] args = new Object[inputArgs.length];
						boolean ok = true;
						
						for(int i=0; i<parameterTypes.length; i++) {
							Class<?> parameterType = parameterTypes[i];
							
							if(parameterType.equals(byte.class) || parameterType.equals(Byte.class)) {
								args[i] = Byte.parseByte(inputArgs[i]);
							} else if(parameterType.equals(short.class) || parameterType.equals(Short.class)) {
								args[i] = Short.parseShort(inputArgs[i]);
							} else if(parameterType.equals(int.class) || parameterType.equals(Integer.class)) {
								args[i] = Integer.parseInt(inputArgs[i]);
							} else if(parameterType.equals(long.class) || parameterType.equals(Long.class)) {
								args[i] = Long.parseLong(inputArgs[i]);
							} else if(parameterType.equals(float.class) || parameterType.equals(Float.class)) {
								args[i] = Float.parseFloat(inputArgs[i]);
							} else if(parameterType.equals(double.class) || parameterType.equals(Double.class)) {
								args[i] = Double.parseDouble(inputArgs[i]);
							} else if(parameterType.equals(boolean.class) || parameterType.equals(Boolean.class)) {
								if(!inputArgs[i].equals("false") && !inputArgs[i].equals("true")) {
									throw new ParseException("", 0);
								}
								
								args[i] = Boolean.parseBoolean(inputArgs[i]);
							} else if(parameterType.equals(char.class) || parameterType.equals(Character.class)) {
								if(inputArgs[i].length() != 1) {
									throw new ParseException("", 0);
								}
								
								args[i] = inputArgs[i];
							} else if(parameterType.equals(String.class)) {
								args[i] = inputArgs[i];
							} else {
								ok = false;
								
								break;
							}
						}
						
						if(ok) {
							return (T) constructor.newInstance(args);
						}
					}catch(NumberFormatException | ParseException exc){}
				}
			}
		}catch(InstantiationException | IllegalAccessException | IllegalArgumentException | InvocationTargetException exc) {}
		
		return null;
	}

	public static void main(String[] args){
		Scanner scn = new Scanner(System.in);

		System.out.print("Parameters: ");
		String parameters = scn.nextLine();
		
		A a = instantiate(A.class, parameters);

		if(a == null) {
			System.out.println(a);
		}
	}
		
}
