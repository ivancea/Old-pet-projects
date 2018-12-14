package ivancea.hol.es;

import java.util.Scanner;

public class FileTransfer {
	
	public final static int BufferSize = 16384;
	
	public static void main(String[] args) {
		Scanner scn = new Scanner(System.in);
		System.out.print("Do you want to send (S) or receive (R) a folder? ");
		boolean correct = true;
		do{
			String str = scn.nextLine();
			if(str.equalsIgnoreCase("S")){
				Sender.start(scn);
			}else if(str.equalsIgnoreCase("R")){
				Receiver.start(scn);
			}else{
				System.out.println("Just 'S' or 'R'");
				correct = false;
			}
		}while(!correct);
	}
}
