package com.mundor.ivancea.ExpControl;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import org.bukkit.Location;
import org.bukkit.Material;
import org.bukkit.block.Block;
import org.bukkit.command.Command;
import org.bukkit.command.CommandSender;
import org.bukkit.entity.Player;
import org.bukkit.event.EventHandler;
import org.bukkit.event.Listener;
import org.bukkit.event.entity.EntityDamageEvent;
import org.bukkit.event.entity.EntityDamageEvent.DamageCause;
import org.bukkit.event.inventory.InventoryClickEvent;
import org.bukkit.event.player.PlayerJoinEvent;
import org.bukkit.inventory.Inventory;
import org.bukkit.inventory.ItemStack;
import org.bukkit.inventory.ShapedRecipe;
import org.bukkit.inventory.meta.ItemMeta;
import org.bukkit.plugin.java.JavaPlugin;

public final class ExpControl extends JavaPlugin implements Listener {
	public void onEnable() {
        getServer().getPluginManager().registerEvents(this, this);
        ShapedRecipe saddle = new ShapedRecipe(new ItemStack(Material.SADDLE, 1));
        saddle.shape("ccc","coc", "ccc");
        saddle.setIngredient('c', Material.LEATHER);
        saddle.setIngredient('o', Material.LEASH);
        getServer().addRecipe(saddle);
        ShapedRecipe hierro = new ShapedRecipe(new ItemStack(Material.IRON_BARDING, 1));
        hierro.shape("aba","aca", "aba");
        hierro.setIngredient('a', Material.IRON_INGOT);
        hierro.setIngredient('b', Material.IRON_BLOCK);
        hierro.setIngredient('c', Material.SADDLE);
        getServer().addRecipe(hierro);
        ShapedRecipe oro = new ShapedRecipe(new ItemStack(Material.GOLD_BARDING, 1));
        oro.shape("aba","aca", "aba");
        oro.setIngredient('a', Material.GOLD_INGOT);
        oro.setIngredient('b', Material.GOLD_BLOCK);
        oro.setIngredient('c', Material.IRON_BARDING);
        getServer().addRecipe(oro);
        ShapedRecipe diamante = new ShapedRecipe(new ItemStack(Material.DIAMOND_BARDING, 1));
        diamante.shape("aba","aca", "aba");
        diamante.setIngredient('a', Material.DIAMOND);
        diamante.setIngredient('b', Material.DIAMOND_BLOCK);
        diamante.setIngredient('c', Material.GOLD_BARDING);
        getServer().addRecipe(diamante);
        }
	public boolean onCommand(CommandSender sender, Command cmd, String label, String[] args) {
		if(sender instanceof Player){
		Player yo = (Player) sender;
		if(cmd.getName().equalsIgnoreCase("darnivel")){
			Player target = getServer().getPlayer(args[0]);
			if(args.length==2){
				if(target == null){
					yo.sendMessage("El jugador no existe o no esta conectado.");
					return false;
				}
				try{
					Integer.parseInt(args[1]);
				}catch(NumberFormatException e){
					return false;
				}
				if(yo.getLevel()<Integer.parseInt(args[1])){
					yo.sendMessage("No tienes tantos niveles.");
					return true;
				}
				if(Integer.parseInt(args[1])<1){
					yo.sendMessage("Debe ser una cantidad de niveles mayor o igual a 1.");
					return true;
				}
				yo.setLevel(yo.getLevel()-Integer.parseInt(args[1]));
				target.setLevel(target.getLevel()+Integer.parseInt(args[1]));
				if(Integer.parseInt(args[1])==1){
					yo.sendMessage("Le has dado 1 nivel al jugador " + args[0] + ".");
					target.sendMessage("El jugador " + yo.getName() + " te ha dado 1 nivel.");
				}else{
					yo.sendMessage("Le has dado " + Integer.parseInt(args[1]) + " niveles al jugador " + args[0] + ".");
					target.sendMessage("El jugador " + yo.getName() + " te ha dado " + Integer.parseInt(args[1]) + " niveles.");
				}
				return true;
			}else
			if(args.length==1){
				if(target == null){
					yo.sendMessage("El jugador no existe o no esta conectado.");
					return false;
				}
				if(yo.getLevel()<1){
					yo.sendMessage("Necesitas al menos 1 nivel.");
					return true;
				}
				yo.setLevel(yo.getLevel()-1);
				target.setLevel(target.getLevel()+1);
				yo.sendMessage("Le has dado 1 nivel al jugador " + args[0] + ".");
				target.sendMessage("El jugador " + yo.getName() + " te ha dado 1 nivel.");
				return true;
			}
			return false;
		}else
		if(cmd.getName().equalsIgnoreCase("nivelapocion")){
			try{
				String a = args[0];
			}catch(java.lang.ArrayIndexOutOfBoundsException e){
				if(yo.getLevel()<1){
					yo.sendMessage("Enecesitas al menos 1 nivel.");
					return true;
				}
				yo.setLevel(yo.getLevel()-1);
				int nivelActual = yo.getLevel();
				int exp=0;
				exp=nivelActual >= 30 ? 62 + (nivelActual - 30) * 7 : (nivelActual >= 15 ? 17 + (nivelActual - 15) * 3 : 17);
				exp=exp/10;
				HashMap<Integer, ItemStack> excess = yo.getInventory().addItem(new ItemStack(Material.EXP_BOTTLE, exp));
				for (Map.Entry<Integer, ItemStack> me : excess.entrySet()) {
					yo.getWorld().dropItem(yo.getLocation(), me.getValue());
				}
				yo.sendMessage("Se ha convertido 1 nivel en " + exp + " botellas de experiencia.");
				return true;
			}
			if(args.length==1){
				try{
					Integer.parseInt(args[0]);
				}catch(NumberFormatException e){
					return false;
				}
				if(yo.getLevel()<Integer.parseInt(args[0])){
					yo.sendMessage("No tienes tantos niveles.");
					return true;
				}
				if(Integer.parseInt(args[0])<1){
					yo.sendMessage("Debe ser una cantidad de niveles mayor o igual a 1.");
					return true;
				}
				if(yo.getInventory().firstEmpty()==-1){
					yo.sendMessage("No tienes espacio libre en el inventario.");
					return true;
				}
				yo.setLevel(yo.getLevel()-Integer.parseInt(args[0]));
				int nivelActual = yo.getLevel();
				int exp=0;
				for(int i=0;i<Integer.parseInt(args[0]);i++){
					exp+=nivelActual >= 30 ? 62 + (nivelActual - 30) * 7 : (nivelActual >= 15 ? 17 + (nivelActual - 15) * 3 : 17);
					nivelActual++;
				}
				exp=exp/10;
				HashMap<Integer, ItemStack> excess = yo.getInventory().addItem(new ItemStack(Material.EXP_BOTTLE, exp));
				for (Map.Entry<Integer, ItemStack> me : excess.entrySet()) {
					yo.getWorld().dropItem(yo.getLocation(), me.getValue());
				}
				if(Integer.parseInt(args[0])==1)
					yo.sendMessage("Se ha convertido 1 nivel en " + exp + " botellas de experiencia.");
				else
					yo.sendMessage("Se han convertido " + Integer.parseInt(args[0]) + " niveles en " + exp + " botellas de experiencia.");
				return true;
			}
			return false;
			}else
				if(cmd.getName().equalsIgnoreCase("reglas")){
					Inventory inv = yo.getServer().createInventory(null, 9, "Reglas");
					inv.setMaxStackSize(1);
					ItemStack item = new ItemStack(Material.PAPER);
					ItemMeta meta = item.getItemMeta();
					ArrayList<String> lore = new ArrayList<String>();
					meta.setDisplayName("Convivencia");
					lore.add("No robar objetos ajenos.");
					lore.add("No hacer spam.");
					lore.add("(Cualquier norma que no se cumpla, puede ocasionar baneo.)");
					meta.setLore(lore);
					item.setItemMeta(meta);
					inv.addItem(item);
					lore.clear();
					item = new ItemStack(Material.WORKBENCH);
					meta.setDisplayName("Receta: Silla de montar");
					lore.add("C C C");
					lore.add("C R C");
					lore.add("C C C");
					lore.add("C: Cuero");
					lore.add("R: Rienda");
					meta.setLore(lore);
					item.setItemMeta(meta);
					inv.addItem(item);
					lore.clear();
					meta.setDisplayName("Receta: Armadura de caballo de hierro");
					lore.add("L B L");
					lore.add("L S L");
					lore.add("L B L");
					lore.add("L: Lingote de hierro");
					lore.add("B: Bloque de hierro");
					lore.add("S: Silla de montar");
					meta.setLore(lore);
					item.setItemMeta(meta);
					inv.addItem(item);
					lore.clear();
					meta.setDisplayName("Receta: Armadura de caballo de oro");
					lore.add("L B L");
					lore.add("L A L");
					lore.add("L B L");
					lore.add("L: Lingote de oro");
					lore.add("B: Bloque de oro");
					lore.add("A: Armadura de caballo de hierro");
					meta.setLore(lore);
					item.setItemMeta(meta);
					inv.addItem(item);
					lore.clear();
					meta.setDisplayName("Receta: Armadura de caballo de diamante");
					lore.add("L B L");
					lore.add("L A L");
					lore.add("L B L");
					lore.add("L: Lingote de diamante");
					lore.add("B: Bloque de diamante");
					lore.add("A: Armadura de caballo de oro");
					meta.setLore(lore);
					item.setItemMeta(meta);
					inv.addItem(item);
					yo.openInventory(inv);
					return true;
				}else
					if(cmd.getName().equalsIgnoreCase("operadores")){
						Player[] a = getServer().getOnlinePlayers();
						int cont=0;
						for(Player b:a)
							if(b.isOp())
								cont++;
						if(cont==0)
							yo.sendMessage("No hay operadores conectados en estos momentos.");
						else{
							yo.sendMessage("Operadores conectados:");
							for(Player b:a)
								if(b.isOp())
									yo.sendMessage(b.getName());
						}
						return true;
					}
		}
		return false;
	}
	@EventHandler
	public void caidaMelon(EntityDamageEvent event){
		if(event.getCause().equals(DamageCause.FALL)){
			Location loc = event.getEntity().getLocation();
			Block a = loc.add(0, -1, 0).getBlock();
			Block b = loc.add(0, -1, 1).getBlock();
			Block c = loc.add(1, -1, 0).getBlock();
			Block d = loc.add(1, -1, 1).getBlock();
			if(a.getType().equals(Material.MELON_BLOCK)){
				a.breakNaturally();
				return;
			}
			if(b.getType().equals(Material.MELON_BLOCK)){
				b.breakNaturally();
				return;
			}
			if(c.getType().equals(Material.MELON_BLOCK)){
				c.breakNaturally();
				return;
			}
			if(d.getType().equals(Material.MELON_BLOCK)){
				d.breakNaturally();
				return;
			}
		}
	}
	@EventHandler
	public void paraReglas(InventoryClickEvent event){
		if(event.getInventory().getName().equals("Reglas") || event.getInventory().getName().equals("Operadores"))
			event.setCancelled(true);
	}
	@EventHandler
	public void darBienvenida(PlayerJoinEvent event){
		Player yo = event.getPlayer();
		if(yo.isOp()){
			yo.sendMessage("Bienvenido al servidor, " + yo.getName() + ". Tienes permisos de Operador.");
		}else{
			yo.sendMessage("Bienvenido al servidor, " + yo.getName() + ".");
		}
		if(getServer().getOnlinePlayers().length==1)
			yo.sendMessage("No hay nadie conectado en estos momentos.");
		else if(getServer().getOnlinePlayers().length==2)
			yo.sendMessage("En estos momentos está conectado 1 jugador.");
		else
			yo.sendMessage("En estos momentos están conectados " + String.valueOf(getServer().getOnlinePlayers().length-1) + " jugadores.");
	}
}