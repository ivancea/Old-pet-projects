package es.com.blogspot.code0x66.DummyChallenge;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.Collections;
import java.util.Comparator;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;
import java.util.logging.Level;

import org.bukkit.ChatColor;
import org.bukkit.Location;
import org.bukkit.World;
import org.bukkit.block.BlockFace;
import org.bukkit.command.Command;
import org.bukkit.command.CommandSender;
import org.bukkit.entity.Arrow;
import org.bukkit.entity.Entity;
import org.bukkit.entity.EntityType;
import org.bukkit.entity.Player;
import org.bukkit.entity.Zombie;
import org.bukkit.event.EventHandler;
import org.bukkit.event.Listener;
import org.bukkit.event.entity.EntityDamageByEntityEvent;
import org.bukkit.event.entity.EntityDamageEvent;
import org.bukkit.event.entity.EntityDamageEvent.DamageCause;
import org.bukkit.metadata.FixedMetadataValue;
import org.bukkit.metadata.MetadataValue;
import org.bukkit.plugin.Plugin;
import org.bukkit.plugin.java.JavaPlugin;

public class DummyChallenge extends JavaPlugin implements Listener {
	
	private HashMap<String,Integer> rec;
	
	@Override
	public void onEnable() {
        getServer().getPluginManager().registerEvents(this, this);
        File f = getDataFolder();
        if(!f.exists())
        	f.mkdirs();
        if(!loadRecords() || rec==null){
        	rec = new HashMap<String,Integer>();
        	if(rec==null){
            	getServer().getLogger().log(Level.WARNING, "MPF records corrupted.");
            }else getServer().getLogger().log(Level.WARNING, "No MPF records saved/found or data corrupted.");
        }
    }
	@Override
	public void onDisable(){
		saveRecords();
	}
	@Override
	public boolean onCommand(CommandSender sender, Command cmd, String label, String[] args) {
		if(sender instanceof Player){
			Player p = (Player) sender;
			if(cmd.getName().equals("placempf")){
				if(!p.isOp()){
					p.sendMessage("You must be OP for use this command.");
					return true;
				}
				putmpf(p.getWorld(),p.getTargetBlock(null,  20).getRelative(BlockFace.UP).getLocation());
				return true;
			}
		}
		if(cmd.getName().equals("mpfscore")){
			if(args.length==0 && sender instanceof Player){
				Integer t = rec.get(((Player)sender).getName());
				if(t==null)
					t = new Integer(0);
				sender.sendMessage("Your record is: " + t.toString());
				return true;
			}else if(args.length==1){
				Integer t = rec.get(args[0]);
				if(t==null)
					t = new Integer(0);
				sender.sendMessage("The record of " + args[0] + " is: " + t.toString());
				return true;
			}
		}else if(cmd.getName().equals("mpftop")){
			HashMap hm = sortByValues(rec);
			Set set2 = hm.entrySet();
		    Iterator iterator = set2.iterator();
		    int a = 1;
		    sender.sendMessage(ChatColor.GREEN + "TOP MPF records:");
		    String str = "                      ";
		    while(iterator.hasNext() && a<=10) {
		         Map.Entry me = (Map.Entry)iterator.next();
		         sender.sendMessage(a + ")" + ChatColor.GOLD + me.getKey() + ChatColor.WHITE + str.substring(0,str.length()-me.getKey().toString().length()-me.getValue().toString().length()) + ChatColor.RED + me.getValue().toString() + ChatColor.WHITE + " points.");
		         ++a;
	        }
			return true;
		}else if(cmd.getName().equals("mpfrestart")){
			if(!sender.isOp()){
				sender.sendMessage("You must be OP for use this command.");
				return true;
			}
			for(World w:getServer().getWorlds())
				for(Entity e:w.getEntities())
					if(e.hasMetadata("mpf_challenge"))
						e.remove();
			rec.clear();
			getServer().broadcastMessage("MPF records restarted.");
			return true;
		}
		return false;
	}
	
	@EventHandler
	public void onEntityDamageMpf(EntityDamageByEntityEvent e){
		if(e.getDamager().hasMetadata("mpf_challenge")){
			e.setCancelled(true);
			return;
		}
		if(!e.getEntity().hasMetadata("mpf_challenge"))
			return;
		if(e.getDamager() instanceof Player){
			String name = ((Player)e.getDamager()).getName();
			HashMap<String,Integer> t = (HashMap<String,Integer>)getMetadata(e.getEntity(), "mpf_challenge");
			if(t.containsKey(name))
				t.put(name, new Integer((int)(t.get(name)+e.getDamage()*2)));
			else
				t.put(name, new Integer((int)(e.getDamage()*2)));
			e.getEntity().removeMetadata("mpf_challenge", this);
			e.getEntity().setMetadata("mpf_challenge", new FixedMetadataValue(this, t));
		}else if(e.getDamager() instanceof Arrow){
			Arrow a = (Arrow)e.getDamager();
			if(a.getShooter() instanceof Player){
				String name = ((Player)e.getDamager()).getName();
				HashMap<String,Integer> t = (HashMap<String,Integer>)getMetadata(e.getEntity(), "mpf_challenge");
				if(t.containsKey(name))
					t.put(name, new Integer((int)(t.get(name)+e.getDamage()*2)));
				else
					t.put(name, new Integer((int)(e.getDamage()*2)));
				e.getEntity().removeMetadata("mpf_challenge", this);
				e.getEntity().setMetadata("mpf_challenge", new FixedMetadataValue(this, t));
			}
		}
		e.setDamage(0.0);
	}
	
	@EventHandler
	public void onDamageMpf(EntityDamageEvent e){
		if(e.getEntity().hasMetadata("mpf_challenge")){
			if(!e.getCause().equals(DamageCause.ENTITY_ATTACK))
				e.setCancelled(true);
			e.getEntity().setFireTicks(0);
			if(e.getEntity().getMetadata("time").get(0).asLong()<(new Date()).getTime()-40000){
				e.getEntity().remove();
				e.setCancelled(true);
			}
		}
	}
	
	private void putmpf(World w, Location l){
		final Entity mpf = w.spawnEntity(l, EntityType.ZOMBIE);
		mpf.setMetadata("mpf_challenge", new FixedMetadataValue(this, new HashMap<String,Integer>()));
		Zombie z = (Zombie)mpf;
		z.setCustomName("MPF");
		z.setCustomNameVisible(true);
		mpf.setMetadata("time", new FixedMetadataValue(this, new Date().getTime()));
		final Plugin pl = this;
		getServer().getScheduler().scheduleSyncDelayedTask(this, new Runnable(){
			public void run(){
				if(!mpf.isValid())
					return;
				if(!mpf.hasMetadata("mpf_challenge"))
					return;
				HashMap<String,Integer> h = (HashMap<String,Integer>)getMetadata(mpf, "mpf_challenge");
				if(h==null){
					getServer().getScheduler().scheduleSyncDelayedTask(pl,  this, 1);
					return;
				}
				for(Entry<String, Integer> ent:h.entrySet()){
				    String key = ent.getKey();
				    Integer val = ent.getValue();
				    Player p = getServer().getPlayer(key);
				    p.sendMessage("Your mpf-record now: " + val.toString());
				    if(rec.containsKey(key)){
				    	if(rec.get(key)<val)
				    		rec.put(key, val);
				    }else rec.put(key, val);
				}
				mpf.removeMetadata("mpf_challenge", pl);
				mpf.remove();
			}
		}, 200);
	}
	private Object getMetadata(Entity ent, String key){
	List<MetadataValue> values = ent.getMetadata(key);  
		for(MetadataValue value : values){
			if(value.getOwningPlugin().getDescription().getName().equals(this.getDescription().getName())){
				return value.value();
		    }
		}
		return null;
	}
	private boolean loadRecords(){
		File f = null;
		FileInputStream is = null;
		ObjectInputStream obj = null;
		f = new File(this.getDataFolder()+File.separator+"record.dat");
		if(!f.exists())
			return false;
		try{
			is = new FileInputStream(f);
			obj = new ObjectInputStream(is);
		}catch(FileNotFoundException e){
			return false;
		}catch(IOException e){
			try{
				if(is!=null)
					is.close();
			}catch(IOException io){}
			return false;
		}
		try {
			Object temp = obj.readObject();
			if(temp!=null && (temp instanceof HashMap))
				rec = (HashMap<String,Integer>)temp;
			else return false;
		}catch(ClassNotFoundException e){
			return false;
		}catch(ClassCastException e){
			return false;
		}catch(IOException e){
			return false;
		}finally{
			try {
				obj.close();
				is.close();
			}catch(IOException e){}
		}
		return true;
	}
	private void saveRecords(){
		if(!getDataFolder().exists())
			getDataFolder().mkdirs();
		File f = new File(getDataFolder()+File.separator+"record.dat");
		if(f.exists())
			f.delete();
		try {
			f.createNewFile();
		} catch (IOException e) {
			return;
		}
		ObjectOutputStream obj = null;
		FileOutputStream out = null;
		try {
			out = new FileOutputStream(f);
		} catch (FileNotFoundException e) {
			return;
		}
		try {
			obj = new ObjectOutputStream(out);
		} catch (IOException e) {
			try {
				out.close();
			} catch (IOException e1) {}
			return;
		}
		try {
			obj.writeObject(rec);
			obj.flush();
		} catch (IOException e) {
		}finally{
			try {
				out.close();
			} catch (IOException e1) {}
		}
	}
	private static HashMap sortByValues(HashMap map) { 
	       List list = new LinkedList(map.entrySet());
	       // Defined Custom Comparator here
	       Collections.sort(list, new Comparator() {
	            public int compare(Object o1, Object o2) {
	               return -((Comparable) ((Map.Entry) (o1)).getValue())
	                  .compareTo(((Map.Entry) (o2)).getValue());
	            }
	       });

	       HashMap sortedHashMap = new LinkedHashMap();
	       for (Iterator it = list.iterator(); it.hasNext();) {
	              Map.Entry entry = (Map.Entry) it.next();
	              sortedHashMap.put(entry.getKey(), entry.getValue());
	       } 
	       return sortedHashMap;
	  }
}
