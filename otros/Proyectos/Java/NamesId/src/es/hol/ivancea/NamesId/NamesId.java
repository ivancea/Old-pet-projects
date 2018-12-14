package es.hol.ivancea.NamesId;
 
import java.util.ArrayList;
import java.util.List;

import org.bukkit.Material;
import org.bukkit.TreeType;
import org.bukkit.block.BlockFace;
import org.bukkit.block.CreatureSpawner;
import org.bukkit.command.Command;
import org.bukkit.command.CommandSender;
import org.bukkit.entity.EntityType;
import org.bukkit.entity.ExperienceOrb;
import org.bukkit.entity.LivingEntity;
import org.bukkit.entity.Player;
import org.bukkit.entity.Projectile;
import org.bukkit.entity.Skeleton;
import org.bukkit.entity.Zombie;
import org.bukkit.event.EventHandler;
import org.bukkit.event.Listener;
import org.bukkit.event.block.Action;
import org.bukkit.event.block.BlockBreakEvent;
import org.bukkit.event.entity.EntityDamageByEntityEvent;
import org.bukkit.event.entity.EntityDamageEvent;
import org.bukkit.event.entity.EntityDamageEvent.DamageCause;
import org.bukkit.event.entity.EntityDeathEvent;
import org.bukkit.event.player.PlayerInteractEvent;
import org.bukkit.inventory.ItemStack;
import org.bukkit.inventory.meta.BookMeta;
import org.bukkit.inventory.meta.ItemMeta;
import org.bukkit.inventory.meta.SkullMeta;
import org.bukkit.material.SpawnEgg;
import org.bukkit.plugin.java.JavaPlugin;
import org.bukkit.potion.PotionEffect;
import org.bukkit.potion.PotionEffectType;
import org.bukkit.projectiles.ProjectileSource;
 
public final class NamesId extends JavaPlugin implements Listener {
	
	public void onEnable() {
        getServer().getPluginManager().registerEvents(this, this);
    }
	public boolean onCommand(CommandSender sender, Command cmd, String label, String[] args) {
		if(sender instanceof Player){
			if(cmd.getName().equalsIgnoreCase("getblockinfo")){
				Player p = (Player)sender;
				p.sendMessage("Ese bloque es \"" + p.getTargetBlock(null,200).getType().toString() + "\", tiene la id " + p.getTargetBlock(null,200).getType().getId() + " y tiene la información: " + p.getTargetBlock(null,200).getType().getData());
				return true;
			}else if(cmd.getName().equalsIgnoreCase("enchant")){
				Player p = (Player)sender;
				if(p.getLevel()<60){
					p.sendMessage("Necesitas 60 niveles para encantar.");
					return true;
				}
				if(isItem(p.getItemInHand(),MultiItem.PICKAXE)){
					if(p.getItemInHand().getItemMeta().getLore()!=null)
						if(p.getItemInHand().getItemMeta().getLore().contains("Explosivo")){
							sender.sendMessage("Ese pico ya está encantado con \"Explosivo\"");
							return true;
						}
					ItemMeta im = p.getItemInHand().getItemMeta();
					List<String> list = new ArrayList<String>();
					list.add("Explosivo");
					im.setLore(list);
					p.getItemInHand().setItemMeta(im);
					p.setLevel(p.getLevel()-60);
				}else if(isItem(p.getItemInHand(),MultiItem.HOE)){
					if(p.getItemInHand().getItemMeta().getLore()!=null)
						if(p.getItemInHand().getItemMeta().getLore().contains("Comealmas")){
							sender.sendMessage("Esa azada ya está encantada con \"Comealmas\"");
							return true;
						}
					ItemMeta im = p.getItemInHand().getItemMeta();
					List<String> list = new ArrayList<String>();
					list.add("Comealmas");
					im.setLore(list);
					p.getItemInHand().setItemMeta(im);
					p.setLevel(p.getLevel()-60);
				}else if(isItem(p.getItemInHand(),MultiItem.CHESTPLATE)){
					if(p.getItemInHand().getItemMeta().getLore()!=null)
						if(p.getItemInHand().getItemMeta().getLore().contains("Reflejo")){
							sender.sendMessage("Esa armadura ya está encantada con \"Reflejo\"");
							return true;
						}
					ItemMeta im = p.getItemInHand().getItemMeta();
					List<String> list = new ArrayList<String>();
					list.add("Reflejo");
					im.setLore(list);
					p.getItemInHand().setItemMeta(im);
					p.setLevel(p.getLevel()-60);
				}else if(p.getItemInHand().getType()==Material.BOOK_AND_QUILL){
					ItemStack is = new ItemStack(Material.WRITTEN_BOOK);
					BookMeta bm = (BookMeta)is.getItemMeta();
					bm.setAuthor("Dios");
					bm.setTitle("Biblia");
					bm.setDisplayName("Biblia");
					List<String> list = new ArrayList<String>();
					list.add("Libro sagrado");
					list.add("200");
					bm.setLore(list);
					bm.addPage("En el principio, creó Dios el cielo y la tierra.\nLa tierra, empero, estaba informe y vacía, y las tinieblas cubrían la superficie del abismo, y el Espíritu de Dios se movía sobre las aguas.");
					bm.addPage("Dijo, pues, Dios: Sea hecha la luz. Y la luz quedó hecha. Y vió Dios que la luz era buena, y dividió la luz de las tinieblas. A la luz la llamó día, y a las tinieblas noche; y así de la tarde aquella y de la mañana siguiente resultó el primer día.");
					bm.addPage("Dijo asimismo Dios: Haya un firmamento o una grande extensión en medio de las aguas, que separe unas aguas de otras. E hizo Dios el firmamento, y separó las aguas que estaban bajo el firmamento, de aquellas que estaban sobre él. Y quedó hecho así.");
					bm.addPage("Y al firmamento llamole Dios cielo. Con lo que de tarde y de mañana se cumplió el segundo día. Dijo también Dios: Re nanse en  n lu  r l s aguas que  st´n d b jo  el c elo,   a arezca l  ar d  y lo  ec . Y así    h zo.  §8(El resto no es legible)");
					is.setItemMeta(bm);
					p.setItemInHand(is);
					p.setLevel(p.getLevel()-60);
					p.getWorld().strikeLightningEffect(p.getLocation());
				}else
					p.sendMessage("Necesitas un objeto encantable en la mano.");
				return true;
			}
		}

		if(cmd.getName().equalsIgnoreCase("getnamebyid")){
			if(args.length!=1)
				return false;
			int n=-1;
			try{
				n = Integer.parseInt(args[0]);
			}catch(NumberFormatException e){return false;}
			if(Material.getMaterial(n)==null){
				sender.sendMessage("Id incorrecta.");
				return true;
			}
			sender.sendMessage("La id " + n + " corresponde al material \"" + Material.getMaterial(n).toString() + "\".");
			return true;
		}else if(cmd.getName().equalsIgnoreCase("getidbyname")){
			if(args.length!=1)
				return false;
			if(Material.getMaterial(args[0])==null){
				sender.sendMessage("Nombre del material inválido.");
				return true;
			}
			sender.sendMessage("El material \"" + Material.getMaterial(args[0]).toString() + "\" corresponde a la id " + Material.getMaterial(args[0]).getId());
			return true;
		}
		
		return false;
	}
	
	@EventHandler
	public void paraPicoExplosivo(PlayerInteractEvent e){
		if(isItem(e.getPlayer().getItemInHand(),MultiItem.PICKAXE))
			if(e.getPlayer().getItemInHand().getItemMeta().getLore().contains("Explosivo") && e.getAction().equals(Action.LEFT_CLICK_BLOCK)){
				e.getPlayer().getWorld().createExplosion(e.getClickedBlock().getRelative(e.getBlockFace()).getLocation(), 3);
				e.getPlayer().getItemInHand().setDurability((short)(e.getPlayer().getItemInHand().getDurability()-1));
			}
	}
	
	@EventHandler
	public void paraPicoExplosivo(EntityDamageByEntityEvent e){
		if(e.getDamager() instanceof Player){
			Player p = (Player)e.getDamager();
			if(p.getItemInHand().getItemMeta().getLore()!=null)
				if(isItem(p.getItemInHand(),MultiItem.PICKAXE) && p.getItemInHand().getItemMeta().getLore().contains("Explosivo")){
					p.getWorld().createExplosion(e.getEntity().getLocation(), 3);
					p.getItemInHand().setDurability((short)(p.getItemInHand().getDurability()-1));
				}
		}
	}
	
	@EventHandler
	public void paraExplosionDePicoExplosivo(EntityDamageEvent e){
		if(e.getCause().equals(DamageCause.BLOCK_EXPLOSION) && e.getEntity() instanceof Player){
			ItemStack is = ((Player)e.getEntity()).getItemInHand();
			if(is.getItemMeta().getLore()!=null)
				if(isItem(is,MultiItem.PICKAXE) && is.getItemMeta().getLore().contains("Explosivo")){
					e.setDamage(e.getDamage()/10);
					is.setDurability((short)(is.getDurability()-1));
				}
		}
	}
	
	@EventHandler
	public void paraAzadaComealmas(EntityDamageByEntityEvent e){
		if(e.getDamager() instanceof Player){
			Player p = (Player)e.getDamager();
			if(p.getItemInHand().getItemMeta().getLore()!=null)
				if(isItem(p.getItemInHand(),MultiItem.HOE) && p.getItemInHand().getItemMeta().getLore().contains("Comealmas")){
					switch(e.getEntityType()){
					case VILLAGER:
						e.setCancelled(true);
						return;
					case GIANT:
					case ZOMBIE:
						if(((Zombie)e.getEntity()).isVillager()){
							e.getEntity().getWorld().strikeLightningEffect(e.getEntity().getLocation());
							((ExperienceOrb)e.getEntity().getWorld().spawn(e.getEntity().getLocation(), ExperienceOrb.class)).setExperience(50);
							e.getEntity().getWorld().spawnEntity(e.getEntity().getLocation(), EntityType.VILLAGER);
							((LivingEntity)e.getEntity()).setHealth(0.0);
							return;
						}
						e.setDamage(e.getDamage()*8);
					default:
						if(e.getEntity() instanceof LivingEntity){
							p.getItemInHand().setDurability((short)(p.getItemInHand().getDurability()-1));
							((LivingEntity)e.getEntity()).addPotionEffect(new PotionEffect(PotionEffectType.WITHER,200,1));
						}
					}
				}
		}
	}
	
	@EventHandler
	public void paraPetoReflejo(EntityDamageByEntityEvent e){
		if(!(e.getEntity() instanceof Player))
			return;
		if(e.getCause() != DamageCause.PROJECTILE || !(e.getDamager() instanceof Projectile))
			return;
		if(isItem(((Player)e.getEntity()).getEquipment().getChestplate(),MultiItem.CHESTPLATE)
		&& ((Player)e.getEntity()).getEquipment().getChestplate().getItemMeta().getLore()!=null){
			if(((Player)e.getEntity()).getEquipment().getChestplate().getItemMeta().getLore().contains("Reflejo")){
				Projectile m = (Projectile)e.getDamager();
				m.setShooter((ProjectileSource)e.getEntity());
				m.setVelocity(m.getVelocity().multiply(-1));
				m.setFireTicks(100);
				e.setCancelled(true);
			}
		}
	}
	
	@EventHandler
	public void paraLibroSagradoCurar(EntityDamageByEntityEvent e){
		if(!(e.getDamager() instanceof Player))
			return;
		Player p = (Player)e.getDamager();
		if(p.getItemInHand()==null)
			return;
		if(p.getItemInHand().getItemMeta().getLore()==null)
			return;
		if(!p.getItemInHand().getItemMeta().getLore().contains("Libro sagrado"))
			return;
		if(e.getEntity() instanceof Player){
			p = (Player)e.getEntity();
			p.setHealth((p.getMaxHealth()/10+p.getHealth()>p.getMaxHealth())?p.getMaxHealth():p.getMaxHealth()/10+p.getHealth());
			usosBiblia(p.getItemInHand(),1);
		}
	}
	
	@EventHandler
	public void paraLibroSagradoCosechas(BlockBreakEvent e){
		Player p = e.getPlayer();
		if(p.getItemInHand() == null)
			return;
		if(p.getItemInHand().getItemMeta().getLore()==null)
			return;
		if(!p.getItemInHand().getItemMeta().getLore().contains("Libro sagrado"))
			return;
		if(e.getBlock().getType()==Material.CARROT || e.getBlock().getType()==Material.POTATO || e.getBlock().getType()==Material.CROPS){
			if(e.getBlock().getData()!=7){
				e.getBlock().setData((byte)7);
				usosBiblia(p.getItemInHand(),1);
				e.setCancelled(true);
			}
		}else if(e.getBlock().getType()==Material.SAPLING){
			e.getBlock().setType(Material.AIR);                                                                     /** ERROR**/
			p.getWorld().generateTree(e.getBlock().getLocation(), TreeType.values()[e.getBlock().getData()]);
			usosBiblia(p.getItemInHand(),2);
		}else if(e.getBlock().getType()==Material.LEAVES || e.getBlock().getType()==Material.LEAVES_2){
			boolean appleDropped=false;
			for(ItemStack it:e.getBlock().getDrops()){
				p.getWorld().dropItemNaturally(e.getBlock().getLocation(), it);
				if(it.getType()==Material.APPLE)
					appleDropped=true;
			}
			if(!appleDropped) p.getWorld().dropItemNaturally(e.getBlock().getLocation(),new ItemStack(Material.APPLE));
			usosBiblia(p.getItemInHand(),1);
		}
	}
	/* convertir agua en hielo o caminar sobre el agua
	 */
	
	private void usosBiblia(ItemStack is, int nUsos){
		ItemMeta im = is.getItemMeta();
		List<String> list = im.getLore();
		if(list.size()<=1) return;
		int usos = Integer.parseInt(list.get(1));
		usos-=nUsos;
		if(usos<=0){
			list.set(0, "Libro quemado");
			list.remove(1);
		}else{
			list.set(1, String.valueOf(usos));
		}
		im.setLore(list);
		is.setItemMeta(im);
	}
	
	@EventHandler
	public void paraSoltarCabezas(EntityDeathEvent e){
		if(e.getEntity().getKiller() instanceof Player){
			Player p = (Player) e.getEntity().getKiller();
			if(p.getItemInHand().getType().equals(Material.SHEARS)){
				switch(e.getEntityType()){
				case CREEPER:
					e.getDrops().add(new ItemStack(Material.SKULL_ITEM,1,(byte)4));
					break;
				case SKELETON:
					Skeleton s = (Skeleton)e.getEntity();
					e.getDrops().add(new ItemStack(Material.SKULL_ITEM,1,(byte)s.getSkeletonType().getId()));
					break;
				case ZOMBIE:
					e.getDrops().add(new ItemStack(Material.SKULL_ITEM,1,(byte)2));
					break;
				case WITHER:
					e.getDrops().add(new ItemStack(Material.SKULL_ITEM,3,(byte)1));
					break;
				case PIG_ZOMBIE:
					e.getDrops().add(getHead("MHF_PigZombie"));
					break;
				case CHICKEN:
					e.getDrops().add(getHead("MHF_Chicken"));
					break;
				case IRON_GOLEM:
					e.getDrops().add(getHead("MHF_Golem"));
					break;
				case ENDERMAN:
					e.getDrops().add(getHead("MHF_Enderman"));
					break;
				case COW:
					e.getDrops().add(getHead("MHF_Cow"));
					break;
				case SLIME:
					e.getDrops().add(getHead("MHF_Slime"));
					break;
				case SHEEP:
					e.getDrops().add(getHead("MHF_Sheep"));
					break;
				case SPIDER:
					e.getDrops().add(getHead("MHF_Spider"));
					break;
				case CAVE_SPIDER:
					e.getDrops().add(getHead("MHF_CaveSpider"));
					break;
				case PIG:
					e.getDrops().add(getHead("MHF_Pig"));
					break;
				case MAGMA_CUBE:
					e.getDrops().add(getHead("MHF_LavaSlime"));
					break;
				case MUSHROOM_COW:
					e.getDrops().add(getHead("MHF_MushroomCow"));
					break;
				case VILLAGER:
					e.getDrops().add(getHead("MHF_Villager"));
					break;
				default:
				}
			}
		}
	}
	
	@EventHandler
	public void paraPersonalizarMobSpawner(PlayerInteractEvent e){
		if(e.getAction().equals(Action.RIGHT_CLICK_BLOCK) && e.getItem() != null)
			if(e.getItem().getType().equals(Material.MONSTER_EGG)
			&& e.getClickedBlock().getType().equals(Material.MOB_SPAWNER)){
				((CreatureSpawner)e.getClickedBlock().getState()).setSpawnedType(((SpawnEgg)e.getItem().getData()).getSpawnedType());
				e.getClickedBlock().getState().update();
				e.setCancelled(true);
			}
	}
	
	private ItemStack getHead(String nick) {
		ItemStack item = new ItemStack(Material.SKULL_ITEM,1,(byte)3);
        SkullMeta meta = (SkullMeta) item.getItemMeta();
        meta.setOwner(nick);
        item.setItemMeta(meta);
        return item;
    }
	
	enum MultiItem{
		PICKAXE,
		SWORD,
		AXE,
		SPADE,
		HOE,
		BOOTS,
		LEGGINGS,
		HELMET,
		CHESTPLATE
	}
	
	private boolean isItem(ItemStack i, MultiItem t){
		return isItem(i.getType(), t);
	}
	
	private boolean isItem(Material m, MultiItem t){
		switch(t){
		case PICKAXE:
			if(m==Material.WOOD_PICKAXE || m==Material.STONE_PICKAXE || m==Material.IRON_PICKAXE || m==Material.GOLD_PICKAXE || m==Material.DIAMOND_PICKAXE)
				return true;
			return false;
		case SWORD:
			if(m==Material.WOOD_SWORD || m==Material.STONE_SWORD || m==Material.IRON_SWORD || m==Material.GOLD_SWORD || m==Material.DIAMOND_SWORD)
				return true;
			return false;
		case AXE:
			if(m==Material.WOOD_AXE || m==Material.STONE_AXE || m==Material.IRON_AXE || m==Material.GOLD_AXE || m==Material.DIAMOND_AXE)
				return true;
			return false;
		case SPADE:
			if(m==Material.WOOD_SPADE || m==Material.STONE_SPADE || m==Material.IRON_SPADE || m==Material.GOLD_SPADE || m==Material.DIAMOND_SPADE)
				return true;
			return false;
		case HOE:
			if(m==Material.WOOD_HOE || m==Material.STONE_HOE || m==Material.IRON_HOE || m==Material.GOLD_HOE || m==Material.DIAMOND_HOE)
				return true;
			return false;
		case BOOTS:
			if(m==Material.LEATHER_BOOTS || m==Material.CHAINMAIL_BOOTS || m==Material.IRON_BOOTS || m==Material.GOLD_BOOTS || m==Material.DIAMOND_BOOTS)
				return true;
			return false;
		case LEGGINGS:
			if(m==Material.LEATHER_LEGGINGS || m==Material.CHAINMAIL_LEGGINGS || m==Material.IRON_LEGGINGS || m==Material.GOLD_LEGGINGS || m==Material.DIAMOND_LEGGINGS)
				return true;
			return false;
		case HELMET:
			if(m==Material.LEATHER_HELMET || m==Material.CHAINMAIL_HELMET || m==Material.IRON_HELMET || m==Material.GOLD_HELMET || m==Material.DIAMOND_HELMET)
				return true;
			return false;
		case CHESTPLATE:
			if(m==Material.LEATHER_CHESTPLATE || m==Material.CHAINMAIL_CHESTPLATE || m==Material.IRON_CHESTPLATE || m==Material.GOLD_CHESTPLATE || m==Material.DIAMOND_CHESTPLATE)
				return true;
			return false;
		default:
			return false;
		}
	}
}