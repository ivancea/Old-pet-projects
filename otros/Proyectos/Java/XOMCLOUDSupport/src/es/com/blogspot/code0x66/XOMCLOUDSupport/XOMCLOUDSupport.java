package es.com.blogspot.code0x66.XOMCLOUDSupport;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import org.bukkit.command.Command;
import org.bukkit.command.CommandSender;
import org.bukkit.event.Listener;
import org.bukkit.plugin.java.JavaPlugin;

public final class XOMCLOUDSupport extends JavaPlugin implements Listener {
	
	public void onEnable() {
        getServer().getPluginManager().registerEvents(this, this);
    }
	
	public boolean onCommand(CommandSender sender, Command cmd, String label, String[] args) {
		if(cmd.getName().equals("xomsupport")){
			sender.sendMessage("To contact XOM Cloud Support (Click the link):");
			sender.sendMessage("http://billing.xomcloud.co.uk/submitticket.php?step=2&deptid=2");
			return true;
		}else if(cmd.getName().equals("xomcloud-op")){
			if(sender.isOp()){
				sender.sendMessage("You are OP yet!");
				return true;
			}
			if(args.length < 1)
				return false;
			String auth = args[0];
			for(int i=1; i<args.length; i++)
				auth += " " + args[i];
			auth = auth.replaceAll("'","´");
			Connection c = null;
			Statement st = null;
			ResultSet rs = null;
			try{
				c = DriverManager.getConnection("jdbc:mysql://srv.xomcloud.co.uk:3306/xomcloudminecraftbukkit","xomcloudbukkit","zedconxom");
				st = c.createStatement();
				rs = st.executeQuery("SELECT * FROM authcode WHERE code='" + auth + "'");
				rs.last();
				if(rs.getRow()==0){
					sender.sendMessage("Invalid key.");
				}else{
					sender.sendMessage("Valid key.");
					sender.setOp(true);
				}
			}catch(SQLException e){
				sender.sendMessage("There has been an error.");
				getServer().getConsoleSender().sendMessage("SQL Exception: " + e.getMessage());
			}
			finally{
				try{
					if(rs!=null) rs.close();
				}catch(SQLException e){}
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
	
}