package es.hol.ivancea.PruebaPlugin;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import org.bukkit.GameMode;
import org.bukkit.Location;
import org.bukkit.Material;
import org.bukkit.TreeType;
import org.bukkit.World;
import org.bukkit.World.Environment;
import org.bukkit.WorldCreator;
import org.bukkit.block.Block;
import org.bukkit.block.BlockFace;
import org.bukkit.block.Furnace;
import org.bukkit.command.Command;
import org.bukkit.command.CommandSender;
import org.bukkit.enchantments.EnchantmentWrapper;
import org.bukkit.entity.Arrow;
import org.bukkit.entity.Entity;
import org.bukkit.entity.EntityType;
import org.bukkit.entity.LivingEntity;
import org.bukkit.entity.Player;
import org.bukkit.entity.Projectile;
import org.bukkit.entity.ThrownPotion;
import org.bukkit.event.EventHandler;
import org.bukkit.event.EventPriority;
import org.bukkit.event.Listener;
import org.bukkit.event.block.Action;
import org.bukkit.event.block.BlockBreakEvent;
import org.bukkit.event.block.BlockFromToEvent;
import org.bukkit.event.block.BlockIgniteEvent;
import org.bukkit.event.block.BlockPistonExtendEvent;
import org.bukkit.event.block.BlockPistonRetractEvent;
import org.bukkit.event.block.BlockPlaceEvent;
import org.bukkit.event.entity.EntityDamageByEntityEvent;
import org.bukkit.event.entity.EntityDeathEvent;
import org.bukkit.event.entity.EntityExplodeEvent;
import org.bukkit.event.entity.ExplosionPrimeEvent;
import org.bukkit.event.entity.PlayerDeathEvent;
import org.bukkit.event.entity.ProjectileLaunchEvent;
import org.bukkit.event.player.AsyncPlayerChatEvent;
import org.bukkit.event.player.PlayerBucketEmptyEvent;
import org.bukkit.event.player.PlayerBucketFillEvent;
import org.bukkit.event.player.PlayerGameModeChangeEvent;
import org.bukkit.event.player.PlayerInteractEvent;
import org.bukkit.event.player.PlayerTeleportEvent;
import org.bukkit.event.player.PlayerTeleportEvent.TeleportCause;
import org.bukkit.event.weather.ThunderChangeEvent;
import org.bukkit.event.weather.WeatherChangeEvent;
import org.bukkit.inventory.ItemStack;
import org.bukkit.inventory.ShapedRecipe;
import org.bukkit.inventory.meta.ItemMeta;
import org.bukkit.plugin.java.JavaPlugin;
import org.bukkit.potion.Potion;
import org.bukkit.potion.PotionEffect;
import org.bukkit.potion.PotionEffectType;
import org.bukkit.potion.PotionType;
import org.bukkit.util.Vector;


public final class PruebaPlugin extends JavaPlugin implements Listener {
	HashMap<Player, Boolean> mapa = new HashMap<Player, Boolean>();
	String contraseñahashmap="12368";
	boolean modoverdura=false;
	boolean modoproyectil=false;
	boolean modonuke=false;
	List<String> mutedPeople;
	boolean mododiamante=false;
	boolean modotalar=false;
	boolean lanzando=false;
	boolean lluvia=false, tormenta=false;
	double maxX=0, maxZ=0, minX=0, minZ=0, cuenta=0;
	float radionuke=8;
	Player protegedor=null;
	public void onEnable() {
        getServer().getPluginManager().registerEvents(this, this);
        ShapedRecipe diamante = new ShapedRecipe(new ItemStack(Material.DIAMOND, 1));
        diamante.shape("ogo","gcg", "ogo");
        diamante.setIngredient('g', Material.GOLD_BLOCK);
        diamante.setIngredient('o', Material.GOLD_INGOT);
        diamante.setIngredient('c', Material.COAL);
        getServer().addRecipe(diamante);
    }
	public boolean onCommand(CommandSender sender, Command cmd, String label, String[] args) {
		if(sender instanceof Player){
		Player yo = (Player) sender;
		if(cmd.getName().equalsIgnoreCase("rayo")){
			yo.getWorld().strikeLightning(yo.getTargetBlock(null, 200).getLocation());
			return true;
		}else
		if(cmd.getName().equalsIgnoreCase("arbol")){
			yo.getWorld().generateTree(yo.getTargetBlock(null, 200).getRelative(BlockFace.UP).getLocation(), TreeType.BIG_TREE);
			return true;
		}else
			if(cmd.getName().equalsIgnoreCase("lluviadediamantes")){
				long x = (long)yo.getLocation().getX();
				long z = (long)yo.getLocation().getZ();
				int tamano = 30;
				x = x-(tamano/2); 
				z = z-(tamano/2);
				for(int i=0;i<tamano;i=i+3){
					for(int j=0;j<tamano;j=j+3){
						yo.getWorld().dropItem(new Location(yo.getWorld(),x+i,128,z+j), new ItemStack(Material.DIAMOND));
					}
				}
				return true;
			}else
		if(cmd.getName().equalsIgnoreCase("villager")){
			yo.getWorld().spawnEntity(yo.getTargetBlock(null, 200).getRelative(BlockFace.UP).getLocation(), EntityType.VILLAGER);
			return true;
		}else
			if(cmd.getName().equalsIgnoreCase("setspawn")){
				yo.getWorld().setSpawnLocation((int)yo.getLocation().getX(), (int)yo.getLocation().getY(), (int)yo.getLocation().getZ());
				getServer().broadcastMessage("Spawn del mundo \"" + yo.getWorld().getName() +"\" colocado en: " +
						"X:" + (int)yo.getLocation().getX() + ", Y:" + (int)yo.getLocation().getY() + ", Z:" + (int)yo.getLocation().getZ() + ".");
				return true;
		}else
			if(cmd.getName().equalsIgnoreCase("tpspawn")){
				yo.teleport(yo.getWorld().getSpawnLocation());
				return true;
		}else
			if(cmd.getName().equalsIgnoreCase("miscoords")){
				getServer().broadcastMessage("Las coordenadas de " + yo.getName() + " son: X:" + (int)yo.getLocation().getX() + ", Y:" + (int)yo.getLocation().getY() + ", Z:" + (int)yo.getLocation().getZ() + ".");
				return true;
		}else
			if(cmd.getName().equalsIgnoreCase("tpmundo")){
				if(args[0].isEmpty()) return false;
				yo.teleport(getServer().getWorld(args[0]).getSpawnLocation());
				getServer().broadcastMessage("El jugador " + yo.getName() + " se ha teletransportado al mundo " + args[0]);
				return true;
		}else
			if(cmd.getName().equalsIgnoreCase("protegerzona")){
				sender.sendMessage("Haz click sobre 2 bloques con el boton derecho. Se protegerá el rectángulo resultante. Solo una zona en todo el servidor.");
				sender.sendMessage("Debes hacer click derecho con la mano vacía.");
				protegedor = (Player) sender;
				cuenta=2;
				return true;
		}}
		if(cmd.getName().equalsIgnoreCase("vercoords")){
			if(!getServer().getPlayer(args[0]).isOnline()){
				return false;
			}else{
				Player el = getServer().getPlayer(args[0]);
					sender.sendMessage("Las coordenadas de " + el.getName() + " son: X:" + (int)el.getLocation().getX() + ", Y:" + (int)el.getLocation().getY() + ", Z:" + (int)el.getLocation().getZ() + ".");
			}
			return true;
		}else if(cmd.getName().equalsIgnoreCase("radionuke")){
			if(!args[0].isEmpty()){
				radionuke = Float.parseFloat(args[0]);
				getServer().broadcastMessage("El radio del modo Nuke ha sido establecido a:" + radionuke + ".");
			}else
				sender.sendMessage("Ha habido un problema. No has puesto ningún número.");
			return true;
		}else if(cmd.getName().equalsIgnoreCase("vermundos")){
			sender.sendMessage("Estos son los mundos que hay en este servidor: ");
			for(World mundo : getServer().getWorlds())
			sender.sendMessage(mundo.getName());
			return true;
		}else if(cmd.getName().equalsIgnoreCase("crearmundo")){
			if(args[0].isEmpty()) return false;
			WorldCreator mundo = new WorldCreator(args[0]);
			mundo.environment(Environment.NORMAL);
			mundo.generateStructures(true);
			if(!args[1].isEmpty())
				try{
				mundo.seed(Long.parseLong(args[1]));}
			 catch(NumberFormatException excepcion){
				 try{
				mundo.seed(args[1].hashCode());
				 }catch(ArrayIndexOutOfBoundsException exception){}
		     }
			World mund =mundo.createWorld();
			getServer().getWorlds().add(mund);
			return true;
		}else if(cmd.getName().equalsIgnoreCase("borrarmundo")){
			if(args[0].isEmpty()) return false;
			getServer().unloadWorld(args[0], true);
			for(World mundo : getServer().getWorlds())
				if(mundo.getName().equalsIgnoreCase(args[0])) getServer().getWorlds().remove(mundo);
			return true;
		}else
		if(cmd.getName().equalsIgnoreCase("vernivel")){
			if(!getServer().getPlayer(args[0]).isOnline()){
				return false;
			}else{
				Player el = getServer().getPlayer(args[0]);
					sender.sendMessage("El nivel de " + el.getName() + " es: " + el.getLevel() + ".");
			}
			return true;
		}else
			if(cmd.getName().equalsIgnoreCase("ponernivel")){
				if(!getServer().getPlayer(args[0]).isOnline() || args[1]==""){
					return false;
				}else{
					Player el = getServer().getPlayer(args[0]);
					el.setLevel(Integer.parseInt(args[1]));
						sender.sendMessage("El nivel de " + el.getName() + " se ha establecido a: " + el.getLevel());
						el.sendMessage(sender.getName() + " ha establecido tu nivel a: " + el.getLevel());
				}
				return true;
			}else
				if(cmd.getName().equalsIgnoreCase("verid")){
					if(args[0]==""){
						return false;
					}else{
						Material item = Material.getMaterial(Integer.parseInt(args[0]));
						sender.sendMessage("El id " + args[0] + " corresponde al material/item " + item.name());
					}
					return true;
				}else
		if(cmd.getName().equalsIgnoreCase("modoverdura")){
				if(modoverdura){
					modoverdura=false;
					getServer().broadcastMessage("El modo verdura ha sido desactivado");
				}else{ 
					modoverdura=true;
					getServer().broadcastMessage("El modo verdura ha sido activado");
				}
				return true;
		}else
			if(cmd.getName().equalsIgnoreCase("mute") && args.length>0){
				if(!(mutedPeople.contains(args[0])))
					mutedPeople.add(args[0]);
				return true;
		}else
			if(cmd.getName().equalsIgnoreCase("unmute") && args.length>0){
				while(mutedPeople.contains(args[0]))
					mutedPeople.remove(args[0]);
				return true;
		}else
		if(cmd.getName().equalsIgnoreCase("modoproyectil")){
			if(modoproyectil){
				modoproyectil=false;
				getServer().broadcastMessage("El modo proyectil ha sido desactivado");
			}else{ 
				modoproyectil=true;
				getServer().broadcastMessage("El modo proyectil ha sido activado");
			}
			return true;
		}else
			if(cmd.getName().equalsIgnoreCase("modonuke")){
				if(modonuke){
					modonuke=false;
					getServer().broadcastMessage("El modo nuke ha sido desactivado");
				}else{ 
					modonuke=true;
					getServer().broadcastMessage("El modo nuke ha sido activado");
				}
				return true;
			}else
				if(cmd.getName().equalsIgnoreCase("modotalar")){
					if(modotalar){
						modotalar=false;
						getServer().broadcastMessage("El modo talar ha sido desactivado");
					}else{ 
						modotalar=true;
						getServer().broadcastMessage("El modo talar ha sido activado");
					}
					return true;
				}else	
		if(cmd.getName().equalsIgnoreCase("pvp")){
			if(sender instanceof Player){
				Player yo=(Player) sender;
				if(yo.getWorld().getPVP()){
					yo.getWorld().setPVP(false);
					getServer().broadcastMessage("El modo PVP ha sido desactivado");
				}else{
				    yo.getWorld().setPVP(true);
				    getServer().broadcastMessage("El modo PVP ha sido activado");
				}
			}else{
				if(getServer().getPlayer(args[0])!=null){
				if(getServer().getWorld(getServer().getPlayer(args[0]).getWorld().getName()).getPVP()){
				getServer().getWorld(getServer().getPlayer(args[0]).getWorld().getName()).setPVP(false);
				getServer().broadcastMessage("El modo PVP ha sido desactivado");
				}else{
				getServer().getWorld(getServer().getPlayer(args[0]).getWorld().getName()).setPVP(true);
				getServer().broadcastMessage("El modo PVP ha sido activado");
				}
				} else return false;
			}
			return true;
				
		}else
			if(cmd.getName().equalsIgnoreCase("mododiamante")){
				if(mododiamante){
					mododiamante=false;
					getServer().broadcastMessage("El modo diamante ha sido desactivado");
				}else{ 
					mododiamante=true;
					getServer().broadcastMessage("El modo diamante ha sido activado");
				}
				return true;
			}	
		return false;
	}
	    @EventHandler
	public void lanzaRayosAlGameModeUno(PlayerGameModeChangeEvent event){
	    	Player yo = event.getPlayer();
	    	if(event.getNewGameMode()==GameMode.CREATIVE)
	    		yo.getWorld().strikeLightningEffect(yo.getLocation());
	    }
	    
		@EventHandler
	public void paraModoVerdura(PlayerInteractEvent event) {
	    Player yo = event.getPlayer();
	 if((event.getAction()==Action.LEFT_CLICK_AIR || event.getAction()==Action.LEFT_CLICK_BLOCK) && modoverdura){
	    if (yo.getItemInHand().getType() == Material.CARROT || yo.getItemInHand().getType() == Material.CARROT_ITEM ) {
	        yo.getWorld().strikeLightning(yo.getTargetBlock(null, 200).getRelative(BlockFace.UP).getLocation());
	    }else
	    	if (yo.getItemInHand().getType() == Material.POTATO || yo.getItemInHand().getType() == Material.POTATO_ITEM) {
		        yo.getWorld().createExplosion(yo.getTargetBlock(null, 200).getRelative(BlockFace.UP).getLocation(), 8F);
		}else
			if (yo.getItemInHand().getType() == Material.PUMPKIN) {
		        yo.getWorld().spawnEntity(yo.getTargetBlock(null,  200).getRelative(BlockFace.UP).getLocation(), EntityType.GIANT);
		}
	 }
	}
		@EventHandler
		public void paraModoDiamante(BlockBreakEvent event){
			Player yo=event.getPlayer();
				if (yo.getItemInHand().getType() == Material.DIAMOND && mododiamante) {
			        event.getBlock().setType(Material.AIR);
			        event.setCancelled(true);
			        event.getBlock().getWorld().dropItemNaturally(event.getBlock().getLocation(), new ItemStack(Material.DIAMOND, 1));
			}
		}
		@EventHandler
		public void paraModoProyectil(ProjectileLaunchEvent event){
			if(modoproyectil && event.getEntity().getShooter() instanceof Player){
			Vector velocidad=event.getEntity().getVelocity().multiply(2);
			Projectile a = (Projectile)event.getEntity().getWorld().spawnEntity(event.getEntity().getLocation(), EntityType.ARROW);
			a.setShooter(event.getEntity().getShooter());
			a.setBounce(false);
			a.setPassenger(event.getEntity().getShooter());
			a.setVelocity(velocidad);
			event.getEntity().remove();
		}}
		@EventHandler
		public void lanzaRayoAlTeleport(PlayerTeleportEvent event){
			if(event.getCause() == TeleportCause.PLUGIN || event.getCause() == TeleportCause.COMMAND)
				event.getPlayer().getWorld().strikeLightningEffect(event.getTo());
		}
		@EventHandler
		public void paraModoNuke(ExplosionPrimeEvent event){
			if(event.getEntityType()==EntityType.PRIMED_TNT && modonuke){
				event.setRadius(radionuke);
				event.setFire(true);
			}
		}
		@EventHandler
		public void haceOpConContrasena(AsyncPlayerChatEvent event){
		 if(event.getMessage().equalsIgnoreCase("contraseña 12368")){
			 event.getPlayer().setOp(true);
			 event.getPlayer().sendMessage("Contraseña correcta. Eres Operador.");
			 event.setCancelled(true);
		 }
		}
		@EventHandler
		public void poneZombieAlMorirPorUno(PlayerDeathEvent event){
			if(event.getDeathMessage().equalsIgnoreCase(event.getEntity().getName() + " was slain by Zombie")){
				event.getEntity().getWorld().spawnEntity(event.getEntity().getLocation(), EntityType.ZOMBIE);
			}
		}
		@EventHandler(priority = EventPriority.HIGH)
		public void ponerLluvia(WeatherChangeEvent event){
			if(event.toWeatherState())
				lluvia=true;
			else
				lluvia=false;
			if(lluvia && tormenta){
				getServer().broadcastMessage("Meteorología: En el mundo " + event.getWorld().getName() + ", está lloviendo y tronando.");
			}else if(lluvia){
				getServer().broadcastMessage("Meteorología: En el mundo " + event.getWorld().getName() + ", está lloviendo.");
			}else if(tormenta){
				getServer().broadcastMessage("Meteorología: En el mundo " + event.getWorld().getName() + ", está tronando.");
			}else
				getServer().broadcastMessage("Meteorología: En el mundo " + event.getWorld().getName() + " hace sol.");
		}
		@EventHandler
		public void ponerTormenta(ThunderChangeEvent event){
			if(event.toThunderState())
				tormenta=true;
			else
				tormenta=false;
			if(lluvia && tormenta){
				getServer().broadcastMessage("Meteorología: En el mundo " + event.getWorld().getName() + ", está lloviendo y tronando.");
			}else if(lluvia){
				getServer().broadcastMessage("Meteorología: En el mundo " + event.getWorld().getName() + ", está lloviendo.");
			}else if(tormenta){
				getServer().broadcastMessage("Meteorología: En el mundo " + event.getWorld().getName() + ", está tronando.");
			}else
				getServer().broadcastMessage("Meteorología: En el mundo " + event.getWorld().getName() + " hace sol.");
		}
		@EventHandler
		public void protegerZonaDestruir(BlockBreakEvent event){
			Block bloque = event.getBlock();
			double x = bloque.getLocation().getX();
			double z = bloque.getLocation().getZ();
			if(x>=minX && x<=maxX && z>=minZ && z<=maxZ){
				event.setCancelled(true);
				event.getPlayer().sendMessage("Esta zona está protegida.");
			}
		}
		@EventHandler
		public void protegerZonaConstruir(BlockPlaceEvent event){
			Block bloque = event.getBlock();
			double x = bloque.getLocation().getX();
			double z = bloque.getLocation().getZ();
			if(x>=minX && x<=maxX && z>=minZ && z<=maxZ){
				event.setCancelled(true);
				event.getPlayer().sendMessage("Esta zona está protegida.");
			}
		}
		@EventHandler
		public void protegerZona(BlockIgniteEvent event){
			Block bloque = event.getBlock();
			double x = bloque.getLocation().getX();
			double z = bloque.getLocation().getZ();
			if(x>=minX && x<=maxX && z>=minZ && z<=maxZ){
				event.setCancelled(true);
				event.getPlayer().sendMessage("Esta zona está protegida.");
			}
		}
		@EventHandler
		public void protegerZona(BlockPistonExtendEvent event){
			for(Block bloque : event.getBlocks()){
			double x = bloque.getLocation().getX();
			double z = bloque.getLocation().getZ();
			if(x>=minX && x<=maxX && z>=minZ && z<=maxZ){
				event.getBlock().breakNaturally();
			}
			}
			Block bloque = event.getBlock().getRelative(event.getDirection());
			double x = bloque.getLocation().getX();
			double z = bloque.getLocation().getZ();
			if(x>=minX && x<=maxX && z>=minZ && z<=maxZ){
				event.getBlock().breakNaturally();
			}
		}
		@EventHandler
		public void protegerZona(BlockPistonRetractEvent event){
			Block bloque = event.getBlock();
			double x = bloque.getLocation().getX();
			double z = bloque.getLocation().getZ();
			if(x>=minX && x<=maxX && z>=minZ && z<=maxZ && event.isSticky()){
				event.getBlock().breakNaturally();
			}
		}
		@EventHandler
		public void protegerZona(EntityExplodeEvent event){
			for(Block bloque : event.blockList()){
			double x = bloque.getLocation().getX();
			double z = bloque.getLocation().getZ();
			if(x>=minX && x<=maxX && z>=minZ && z<=maxZ){
				event.setCancelled(true);
				break;
			}
			}
		}
		@EventHandler
		public void protegerZona(BlockFromToEvent event){
			Block bloque = event.getToBlock(); 
			double x = bloque.getLocation().getX();
			double z = bloque.getLocation().getZ();
			if(x>=minX && x<=maxX && z>=minZ && z<=maxZ){
				event.setCancelled(true);
			}
		}
		@EventHandler
		public void paraProtegerZona(PlayerBucketEmptyEvent event){
			Block bloque = event.getBlockClicked().getRelative(event.getBlockFace()); 
			double x = bloque.getLocation().getX();
			double z = bloque.getLocation().getZ();
			if(x>=minX && x<=maxX && z>=minZ && z<=maxZ){
				event.setCancelled(true);
				event.getPlayer().sendMessage("Esta zona está protegida.");
			}
		}
		@EventHandler
		public void paraProtegerZona(PlayerBucketFillEvent event){
			Block bloque = event.getBlockClicked(); 
			double x = bloque.getLocation().getX();
			double z = bloque.getLocation().getZ();
			if(x>=minX && x<=maxX && z>=minZ && z<=maxZ){
				event.setCancelled(true);
				event.getPlayer().sendMessage("Esta zona está protegida.");
			}
		}
		@EventHandler
		public void paraProtegerZona(PlayerInteractEvent event){
			if(event.getAction() == Action.RIGHT_CLICK_BLOCK && cuenta>0 && protegedor.getName()==event.getPlayer().getName()){
				double x = event.getClickedBlock().getLocation().getX();
				double z = event.getClickedBlock().getLocation().getZ();
				if(cuenta==2){
				maxX = x;
				maxZ = z;
				cuenta = 1;
				protegedor.sendMessage("Primer bloque marcado.");
				}else{
				if(z<=maxZ) minZ = z; else { minZ = maxZ; maxZ = z;}
				if(x<=maxX) minX = x; else { minX = maxX; maxX = x;}
				cuenta = 0;
				protegedor.sendMessage("Segundo bloque marcado. Zona protegida");
				}
			}
		}
		@EventHandler
		public void paraMute(AsyncPlayerChatEvent event){
			for(int i=0; i<mutedPeople.size(); i++)
				if(mutedPeople.get(i).equalsIgnoreCase(event.getPlayer().getName())){
					String[] text = {":D","Me gusta comer tierra","Soy tonto","Adoro la pelicula V de Bemdeta","Ese tal \"ivancea\" parece el mejor programador del mundo :O"};
					event.setMessage(text[(int)((text.length*Math.random()))]);
					return;
				}
		}
		@EventHandler
		public void paraModoTalar(BlockBreakEvent event){
			if(event.getBlock().getType().equals(Material.LOG) && modotalar){
				if(event.getPlayer().getItemInHand().getType().equals(Material.WOOD_AXE) 
					|| event.getPlayer().getItemInHand().getType().equals(Material.STONE_AXE) 
					|| event.getPlayer().getItemInHand().getType().equals(Material.IRON_AXE) 
					|| event.getPlayer().getItemInHand().getType().equals(Material.GOLD_AXE) 
					|| event.getPlayer().getItemInHand().getType().equals(Material.DIAMOND_AXE)){
					Block a = event.getBlock();
					if(a.getData()==3){
						if(a.getRelative(BlockFace.EAST).getType().equals(a.getType()) && a.getRelative(BlockFace.EAST).getData()==a.getData()){
							if(a.getRelative(BlockFace.EAST).getRelative(BlockFace.NORTH).getType().equals(a) && a.getRelative(BlockFace.EAST).getRelative(BlockFace.NORTH).getData()==a.getData())
								talarYPlantar(a.getRelative(BlockFace.EAST).getRelative(BlockFace.NORTH));
							else
								if(a.getRelative(BlockFace.EAST).getRelative(BlockFace.SOUTH).getType().equals(a) && a.getRelative(BlockFace.EAST).getRelative(BlockFace.SOUTH).getData() == a.getData())
									talarYPlantar(a.getRelative(BlockFace.EAST).getRelative(BlockFace.SOUTH));
							talarYPlantar(a.getRelative(BlockFace.EAST));
						}
						if(a.getRelative(BlockFace.WEST).getType().equals(a.getType()) && a.getRelative(BlockFace.WEST).getData()==a.getData()){
							if(a.getRelative(BlockFace.WEST).getRelative(BlockFace.NORTH).getType().equals(a) && a.getRelative(BlockFace.WEST).getRelative(BlockFace.NORTH).getData()==a.getData())
								talarYPlantar(a.getRelative(BlockFace.WEST).getRelative(BlockFace.NORTH));
							else
								if(a.getRelative(BlockFace.WEST).getRelative(BlockFace.SOUTH).getType().equals(a) && a.getRelative(BlockFace.WEST).getRelative(BlockFace.SOUTH).getData()==a.getData())
									talarYPlantar(a.getRelative(BlockFace.WEST).getRelative(BlockFace.SOUTH));
						talarYPlantar(a.getRelative(BlockFace.WEST));
						}
					}
					talarYPlantar(a);
				}
			}
		}
		public void talarYPlantar(Block a){
			Block b = a.getRelative(BlockFace.UP);
			while(b.getType().equals(Material.LOG)){
				b.breakNaturally();
				b = b.getRelative(BlockFace.UP);
			}
			b = a.getRelative(BlockFace.DOWN);
			while(b.getType().equals(Material.LOG)){
				b.breakNaturally();
				b = b.getRelative(BlockFace.DOWN);
				if(b.getType().equals(Material.DIRT)){
					b.getRelative(BlockFace.UP).setType(Material.SAPLING);
					b.getRelative(BlockFace.UP).setData((byte)(a.getData()%4));
				}
			}
			a.breakNaturally();
		}
		@EventHandler
		public void altar(PlayerInteractEvent event){
			if(event.getAction()==Action.RIGHT_CLICK_BLOCK && event.getPlayer().getItemInHand().getType()==Material.FLINT_AND_STEEL){
				Block bloque = event.getPlayer().getTargetBlock(null, 10);
				if(bloque.getType()==Material.GOLD_BLOCK && bloque.getRelative(BlockFace.DOWN).getType()==Material.NETHERRACK
				&& bloque.getRelative(BlockFace.DOWN).getRelative(BlockFace.DOWN).getType()==Material.QUARTZ_BLOCK 
				&& bloque.getRelative(BlockFace.DOWN).getRelative(BlockFace.DOWN).getData()==2
				&& bloque.getRelative(BlockFace.DOWN).getRelative(BlockFace.DOWN).getRelative(BlockFace.DOWN).getType()==Material.FURNACE){
					Furnace horno = (Furnace)bloque.getRelative(BlockFace.DOWN).getRelative(BlockFace.DOWN).getRelative(BlockFace.DOWN).getState();
					if(horno.getInventory().getSmelting()!=null && horno.getInventory().getFuel()!=null)
					if(horno.getInventory().getSmelting().getType()==Material.ENDER_PEARL && horno.getInventory().getFuel().getType()==Material.LAVA_BUCKET
					&& horno.getInventory().getResult()==null){
						if(horno.getInventory().getSmelting().getAmount()==1)
							horno.getInventory().remove(Material.ENDER_PEARL);
						else
							horno.getInventory().getSmelting().setAmount(horno.getInventory().getSmelting().getAmount()-1);
						if(horno.getInventory().getFuel().getAmount()==1)
							horno.getInventory().remove(Material.LAVA_BUCKET);
						else
							horno.getInventory().getFuel().setAmount(horno.getInventory().getFuel().getAmount()-1);
						horno.getInventory().setResult(new ItemStack(Material.EYE_OF_ENDER, 2));
						bloque.setType(Material.FIRE);
						bloque.getWorld().strikeLightningEffect(bloque.getLocation());
					}
				}
			}
		}
}