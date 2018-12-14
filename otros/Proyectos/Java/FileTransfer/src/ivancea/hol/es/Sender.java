package ivancea.hol.es;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.util.Scanner;

public class Sender {
	public static void start(Scanner scn){
		int port = Util.getPort(scn);
		File file = Util.selectFolder("Folder to send");
		if(file!=null){
			ServerSocket ss = null;
			try {
				ss = new ServerSocket();
				ss.bind(new InetSocketAddress(port));
				System.out.println("Sending \"" + file.getName() + "\".");
				System.out.println("Waiting for a client...");
				startSend(ss.accept(), file);
			} catch (IOException e) {
				System.out.println("Couldn't create and bind server: " + e.getMessage());
			}finally{
				if(ss!=null && !ss.isClosed()){
					try {
						ss.close();
					} catch (IOException e) {}
				}
			}
		}else{
			System.out.println("Aborting...");
		}
	}
	
	private static void startSend(Socket sock, File parent){
		System.out.println("Sending to cliente: " + sock.getInetAddress().getHostAddress());
		OutputStream out = null;
		try {
			out = sock.getOutputStream();
		} catch (IOException e) {
			System.out.println("Error on socket");
			return;
		}

		byte b[] = new byte[1];
		b[0]=2;
		try {
			out.write(b);
			b = parent.getName().getBytes();
			out.write(ByteBuffer.allocate(4).putInt(b.length).array());
			out.write(b);
		} catch (IOException e) {
			System.out.println(e.getMessage());
		}
		
		recursiveSend(out, parent, "");
		
		try {
			b[0]=0;
			out.write(b);
		} catch (IOException e) {
			System.out.println("Error with output stream...");
		}

		try {
			while(sock.getInputStream().read()==-1){
				Thread.sleep(1);
			}
		} catch (IOException e) {
		} catch (InterruptedException e){}
		
		System.out.println("Finished");
	}
	
	private static void recursiveSend(OutputStream out, File parent, String path){
		File f = new File(parent, path);
		String paths[] = f.list();
		byte[] buffer = new byte[FileTransfer.BufferSize];
		for(int i=0; i<paths.length; i++){
			File child = new File(f, paths[i]);
			InputStream in = null;
			byte b[] = new byte[]{1};
			if(child.isDirectory())
				b[0] = 2;
			else{
				try {
					in = new FileInputStream(child);
				} catch (FileNotFoundException e) {
					System.out.println(e.getMessage());
				}
			}
			try {
				out.write(b);
				b = (parent.getName()+"/"+path+"/"+paths[i]).getBytes();
				out.write(ByteBuffer.allocate(4).putInt(b.length).array());
				out.write(b);
				System.out.println(parent.getName()+"/"+path+"/"+paths[i]);
			} catch (IOException e) {
				System.out.println(e.getMessage());
				return;
			}
			if(!child.isDirectory()){
				long len = child.length();
				try {
					out.write(ByteBuffer.allocate(8).putLong(len).array());
				} catch (IOException e) {
				}
				while(len>0){
					int n = 0;
					try {
						n = in.read(buffer);
						out.write(buffer, 0,n);
					} catch (IOException e) {
					}
					len -= n;
				}
			}else recursiveSend(out, parent, path+"/"+paths[i]);
			
		}
	}
}

/*

{ Para cada archivo

	1 byte: (0|1|2) 0: fin, 1: archivo, 2: carpeta
	(4 bytes N) N caracteres: ruta y nombre
	(4 bytes N) N Bytes: contenido

}

*/