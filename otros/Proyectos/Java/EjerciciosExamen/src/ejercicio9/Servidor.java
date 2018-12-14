package ejercicio9;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Scanner;

public class Servidor {
	public static void main(String[] args){//como lo entiendo es el servidor escribe mientras lea algo no?
		Scanner et = new Scanner(System.in);
		String res= "";
		try{
			Socket util;
			ServerSocket servidor = new ServerSocket(Cliente.PORT);
			while(true){
				System.out.println("\"ESPERANDO CLIENTE\"");
				util = servidor.accept();
				System.out.println(util.getInetAddress());
				DataInputStream dis = new DataInputStream(util.getInputStream());
				DataOutputStream dos = new DataOutputStream(util.getOutputStream());
				do{
					res = dis.readUTF();
					System.out.println("CLIENTE DICE: "+ res);
					if(res.equalsIgnoreCase("ADIOS")){
						util.close();
						System.out.println("\"CONEXIÓN CON CLIENTE FINALIZADA\"");
					}else{
						res = et.nextLine();
						System.out.println("SERVIDOR DICE: "+ res);
						dos.writeUTF(res);
					}
				}while(!res.equalsIgnoreCase("ADIOS"));
			}
		}catch(IOException e){
			e.printStackTrace();
		}
		et.close();
	}
}

 /* 
  */