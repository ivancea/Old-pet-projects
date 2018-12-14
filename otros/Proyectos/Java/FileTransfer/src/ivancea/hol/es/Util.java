package ivancea.hol.es;

import java.io.File;
import java.util.InputMismatchException;
import java.util.Scanner;

import javax.swing.JFileChooser;

public class Util {
	
	public static int getPort(Scanner scn){
		System.out.print("Enter the port: ");
		boolean correct = true;
		int port = 0;
		do{
			try{
				port = scn.nextInt();
			}catch(InputMismatchException e){
				System.out.println("Incorrect port");
				correct = false;
			}
			if(port<0 || port>65535){
				System.out.println("Number out of range (0-65535)");
				correct = false;
			}
		}while(!correct);
		return port;
	}
	
	
	
	public static String getIP(Scanner scn){
		System.out.print("Enter your IP: ");
		String s = "";
		boolean b = true;
		do{
			s = scn.nextLine();
			b = testIP(s);
			if(!b){
				System.out.println("Invalid IP");
			}
		}while(!testIP(s));
		return s;
	}
	
	public static boolean testIP(String s){
		String test[] = s.split("\\.");
		int in = 0;
		if(test.length == 4){
			for(int i = 0; i < test.length; i++){
				try{
					in = Integer.parseInt(test[i]);
					if(in < 0 || in > 255){
						return false;
					}
				}catch(NumberFormatException e){
					return false;
				}
			}
		}else{
			return false;
		}
		return true;
	}
	
	public static File selectFolder(String title){
		JFileChooser jfc = new JFileChooser();
		jfc.setDialogTitle(title);
		jfc.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
		if(jfc.showSaveDialog(null) == JFileChooser.APPROVE_OPTION){
			return jfc.getSelectedFile();
		}
		return null;
	}
	
}
