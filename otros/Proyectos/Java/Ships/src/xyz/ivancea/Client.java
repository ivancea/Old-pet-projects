package xyz.ivancea;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Scanner;

public class Client {
	public static void start(){
		System.out.print("Server IP: ");
		String IP = (new Scanner(System.in)).nextLine();
		try{	
			Socket client = new Socket(IP, 12345);
			if(!client.isConnected()){
				System.out.println("Could not connect with the server. Try again in a few seconds.");
			}
			InputStream serverInput = client.getInputStream();
			OutputStream serverOutput = client.getOutputStream();
			byte player;
			while( (player = (byte) serverInput.read()) == -1 ){
				try{
					Thread.sleep(10);
				}catch(InterruptedException e){}
			}
			System.out.println("JUGADOR: "+ (int) player);
		}catch(UnknownHostException e){
			e.printStackTrace();
		}catch(IOException e) {
			System.out.println("Connection fail. Closing");
			e.printStackTrace();
		}
		
	}
}


/*

#Conexion cliente
(Cuando haya 2 clientes)
Server -> Jugador 1/2
(Colocar barcos)
Cliente -> posicion barcos
(Cuando ambos enviaron posiciones)

(HASTA QUE SE PIERDA)

	Cliente envía posicion
	Server -> Agua/tocado/tocado y hundido
	Server -> Avisa a otro cliente

(...)


*/