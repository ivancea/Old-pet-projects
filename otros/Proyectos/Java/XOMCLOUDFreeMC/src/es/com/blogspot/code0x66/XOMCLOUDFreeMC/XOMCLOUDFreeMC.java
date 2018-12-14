package es.com.blogspot.code0x66.XOMCLOUDFreeMC;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.Timer;
import java.util.TimerTask;

import org.bukkit.command.Command;
import org.bukkit.command.CommandSender;
import org.bukkit.event.EventHandler;
import org.bukkit.event.Listener;
import org.bukkit.event.server.ServerListPingEvent;
import org.bukkit.plugin.java.JavaPlugin;

public final class XOMCLOUDFreeMC extends JavaPlugin implements Listener {
	
	private Timer tim = new Timer();
	
	public void onEnable() { /** Set the timer **/
        getServer().getPluginManager().registerEvents(this, this);
        tim.scheduleAtFixedRate(new TimerTask(){
        	public void run(){
        		getServer().broadcastMessage("XOM CLOUD Minecraft Server - http://mymc.uk - This is a XOMCLOUD Sponsored Server");
        	}
        }, 0, 1800000);
    }
	
	public boolean onCommand(CommandSender sender, Command cmd, String label, String[] args) {
		if(cmd.getName().equals("report")){
			if(args.length < 1)
				return false;
			String toSend = args[0];
			for(int i=1; i<args.length; i++)
				toSend += " " + args[i];
			toSend = toSend.replaceAll("'","´");
			Connection c = null;
			Statement st = null;
			String IP = getServer().getIp();
			try{
				c = DriverManager.getConnection("jdbc:mysql://srv.xomcloud.co.uk:3306/xomcloudminecraftbukkit","xomcloudbukkit","zedconxom");
				st = c.createStatement();
				st.executeUpdate("INSERT INTO reports (`Server IP`, Reason) VALUES ('" + IP + "','" + toSend + "')");
				sender.sendMessage("Report sent.");
			}catch(SQLException e){
				sender.sendMessage("There has been an error.");
				getServer().getConsoleSender().sendMessage("SQL Exception: " + e.getMessage());
			}
			finally{
				try{
					if(st!=null) st.close();
				}catch(SQLException e){}
				try{
					if(c!=null) c.close();
				}catch(SQLException e){}
			}
			return true;
		}
		return false;
	}
	
	@EventHandler
	public void onServerlistPing(ServerListPingEvent e){ /** Set MOTD **/
		e.setMotd("XOMCLOUD.COM - Free Server");
	}
	
}