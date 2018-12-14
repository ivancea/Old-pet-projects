package xyz.ivancea;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;

public class Server {
	public static void start(){
		Socket last = null;
		
		try {
			ServerSocket server = new ServerSocket();
			server.bind(new InetSocketAddress(12345));
			System.out.println("Server connected");
			while(true){
				Socket client = server.accept();
				System.out.println("Client connected from: " + client.getInetAddress().getHostAddress());
				if(last==null){
					last = client;
				}else{
					final Socket clientA, clientB;
					clientA = last;
					clientB = client;
					Thread th = new Thread(new Runnable(){
						public void run() {
							play(clientA, clientB);
						}});
					th.setDaemon(true);
					th.start();
					last = null;
				}
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	private static void play(Socket clientA, Socket clientB){
		try {
		OutputStream outA = clientA.getOutputStream(),
					 outB = clientB.getOutputStream();
		InputStream  inA = clientA.getInputStream(),
				 	 inB = clientB.getInputStream();
			outA.write(1);
			outB.write(2);
			try {
				Thread.sleep(10000);
			} catch (InterruptedException e) {
			}
		} catch (IOException e1) {
			e1.printStackTrace();
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