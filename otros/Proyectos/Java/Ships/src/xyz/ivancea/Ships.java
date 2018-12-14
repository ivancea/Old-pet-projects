package xyz.ivancea;

import java.util.Scanner;

public class Ships {
	public static void main(String[] args){
		String option = "";
		System.out.println("Server (s) or client (other)");
		option = (new Scanner(System.in)).nextLine();
		if(option.length() >= 1 && option.substring(0,1).equalsIgnoreCase("s")){
			Server.start();
		}else{
			Client.start();
		}
	}
}
//XXX JUEGO empezar