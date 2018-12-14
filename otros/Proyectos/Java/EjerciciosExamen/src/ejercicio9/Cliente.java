package ejercicio9;

import java.io.IOException;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.net.Socket;
import java.util.Scanner;

public class Cliente {
	static int PORT = 12015;
	static String HOST = "localhost";
	public static void main(String[] args){
		Scanner et = new Scanner(System.in);
		String s = "";
		try{
			Socket cliente = new Socket(HOST, PORT);
			DataInputStream dis = new DataInputStream(cliente.getInputStream());
			DataOutputStream dos = new DataOutputStream(cliente.getOutputStream());
			System.out.println("\"CHAT INICIADO\"");
			do{
				s = et.nextLine();
				dos.writeUTF(s);
				System.out.println("CLIENTE: "+s);
				if(!s.equalsIgnoreCase("ADIOS")){
					s = dis.readUTF();
					System.out.println("SERVIDOR: " + s);
				}else{
					System.out.println("CONEXIÓN CERRADA");
				}
			}while(!s.equalsIgnoreCase("ADIOS"));
			cliente.close();
		}catch(IOException e){
			e.printStackTrace();
		}
		et.close();
	}
}
/* 
 */
