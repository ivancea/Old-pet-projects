package ivancea.hol.es;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.util.Scanner;

public class Receiver {
	public static void start(Scanner scn){
		String ip = Util.getIP(scn);
		int port = Util.getPort(scn);
		File file = Util.selectFolder("Folder to save");
		if(file != null){
			try{
				Socket client = new Socket(ip, port);
				client.getOutputStream().write(0);
				startReceiver(client, file);
			}catch(IOException e){}
		}else{
			System.out.println("Abort");
		}
		
		System.out.println("Finished");
	}
	
	private static void startReceiver(Socket socket, File file){
		System.out.println("Receiving...");
		byte buffer[] = new byte[FileTransfer.BufferSize];
		int bytesReceived = 0, bytesTotal = 0;
		final long initialTime = System.currentTimeMillis();
		String path = "";
		try{	
			File newFile;
			InputStream in = socket.getInputStream();
			while(true){
				byte b = (byte) in.read();
				if(b == 0){
					break;
				}else if(b == 1){
					path = readPath(in);
					System.out.println(file.getAbsolutePath() +"/"+path+" "+(file.getAbsolutePath() +"/"+path).length());
					newFile = new File(file, path);
					if(newFile.exists()){
						newFile.delete();
					}
					byte bLong[] = new byte[8];
					in.read(bLong);
					ByteBuffer bf = ByteBuffer.wrap(bLong);
					long length = bf.getLong();
					FileOutputStream out = new FileOutputStream(newFile);
					while(length > 0){
						int n = in.read(buffer, 0, (int)(length>buffer.length?buffer.length:length));
						out.write(buffer, 0, n);
						bytesReceived += n;
						if(bytesReceived >= 10485760){
							long nowTime = System.currentTimeMillis(), timeSinceStart = (nowTime - initialTime)/1000;
							bytesTotal += bytesReceived;
							System.out.print((bytesTotal/1048576) + "MB");
							if(timeSinceStart>0)
								System.out.println(" at " + ((bytesTotal/1024)/timeSinceStart) + "KB/s");
							bytesReceived = 0;
						}
						length -= n;
					}
					out.close();
				}else if(b == 2){
					path = readPath(in);
					newFile = new File(file, path);
					if(newFile.exists()){
						newFile.delete();
					}
					newFile.mkdir();
				}
			}	
		}catch(IOException e){
			System.out.println("ERROR WITH: "+file.getAbsolutePath()+"/" + path);
			e.printStackTrace();
		}
		System.out.println("Received " + (bytesTotal/1048576) + "MB");
	}
	
	private static String readPath(InputStream in){
		byte b[] = new byte[4];
		try{
			in.read(b);
			ByteBuffer bf = ByteBuffer.wrap(b);	
			int pathLength = bf.getInt();
			b = new byte[pathLength];
			in.read(b);
		}catch(IOException e){
			e.printStackTrace();
		}
		return new String(b);
	}
	
}