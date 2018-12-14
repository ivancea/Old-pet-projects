package pruebavisual;

import java.awt.Color;
import java.awt.Frame;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.Polygon;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
 
class Visual extends Frame
{
	double meX=0;
	double meY=0;
	double meZ=0;
    double meHorizontal=0;
    double meVertical=0;
    Visual(String titulo,int ancho,int alto)
   {       
        setTitle(titulo);
        setBackground(Color.WHITE);
        setSize(ancho,alto);
        setLocationRelativeTo(null);    //Centrar
        setResizable(false);
         
        // ### ESCUCHADOR DE VENTANA ###
        //Para que al dar clic en la x de la esquina de este Frame se cierre
        addWindowListener(new WindowAdapter() 
        {       
            public void windowClosing(WindowEvent e) 
            {
                System.exit(0);
            }
 
            public void windowClosed(WindowEvent e) 
            {
                System.exit(0);         
            }
        });
        
        addKeyListener(new KeyListener(){
			@Override
			public void keyPressed(KeyEvent event) {
				if(event.getKeyCode() == KeyEvent.VK_W){
		            meX += Math.cos(-Math.toRadians(meHorizontal)+Math.PI/2)*Math.cos(Math.toRadians(meVertical));
		            meZ += Math.sin(-Math.toRadians(meHorizontal)+Math.PI/2)*Math.cos(Math.toRadians(meVertical));
		            meY += Math.sin(Math.toRadians(meVertical));
					repaint();
				}else
				if(event.getKeyCode() == KeyEvent.VK_S){
		            meX -= Math.cos(-Math.toRadians(meHorizontal)+Math.PI/2)*Math.cos(Math.toRadians(meVertical));
		            meZ -= Math.sin(-Math.toRadians(meHorizontal)+Math.PI/2)*Math.cos(Math.toRadians(meVertical));
		            meY -= Math.sin(Math.toRadians(meVertical));
					repaint();
				}else
				if(event.getKeyCode() == KeyEvent.VK_D){
		            meX += Math.cos(-Math.toRadians(meHorizontal));
		            meZ += Math.sin(-Math.toRadians(meHorizontal));
					repaint();
				}else
				if(event.getKeyCode() == KeyEvent.VK_A){
		            meX += Math.cos(-Math.toRadians(meHorizontal)+Math.PI);
		            meZ += Math.sin(-Math.toRadians(meHorizontal)+Math.PI);
					repaint();
				}else
				if(event.getKeyCode() == KeyEvent.VK_LEFT){
					meHorizontal-=2;
					if(meHorizontal<-180)
						meHorizontal+=360;
					repaint();
				}else
				if(event.getKeyCode() == KeyEvent.VK_RIGHT){
					meHorizontal+=2;
					if(meHorizontal>180)
						meHorizontal-=360;
					repaint();
				}else
				if(event.getKeyCode() == KeyEvent.VK_UP){
					if(meVertical+2<=90)
						meVertical+=2;
					else meVertical = 90;
					repaint();
				}else
				if(event.getKeyCode() == KeyEvent.VK_DOWN){
					if(meVertical-2>=-90)
						meVertical-=2;
					else meVertical = -90;
					repaint();
				}/*else
				if(event.getKeyCode() == KeyEvent.VK_Q){
		            meX += Math.cos(Math.toRadians(meHorizontal)+Math.PI);
		            meZ += Math.sin(Math.toRadians(meHorizontal)+Math.PI);
					repaint();
				}else
				if(event.getKeyCode() == KeyEvent.VK_E){
		            meX += Math.cos(Math.toRadians(meHorizontal));
		            meZ += Math.sin(Math.toRadians(meHorizontal));
					repaint();
				}*/else
				if(event.getKeyCode() == KeyEvent.VK_F){
		            meY += Math.cos(Math.toRadians(meVertical)+Math.PI);
		            meZ += Math.sin(Math.toRadians(meVertical)+Math.PI);
					repaint();
				}else
				if(event.getKeyCode() == KeyEvent.VK_R){
		            meY -= Math.cos(Math.toRadians(meVertical)+Math.PI);
		            meZ -= Math.sin(Math.toRadians(meVertical)+Math.PI);
					repaint();
				}
			}

			@Override
			public void keyReleased(KeyEvent arg0) {
				
			}

			@Override
			public void keyTyped(KeyEvent arg0) {
				
			}
        	
        });
    }
 
    public void paint(Graphics g)
    {
    	g.drawString("X: " + meX, 10, 40);
    	g.drawString("Y: " + meY, 10, 60);
    	g.drawString("Z: " + meZ, 10, 80);
    	g.drawString("V: " + meVertical, 10, 100);
    	g.drawString("H: " + meHorizontal, 10, 120);
    	cubo(g, -1,-1,20, 2);
    	cubo(g, -1,-1,24, 2);
    	cubo(g, 3,-1,20, 2);
    	cubo(g, -1,3,20, 2);
    	cubo(g, -3,3,-20, 2);
    }
     
   public static void main(String args[])
   {
        Visual test=new Visual("Ejemplo",900,600);
        test.setVisible(true);
        //test.bucle();
   }
    
   public void bucle(){
       int x=(int)getMousePosition().getX();
       int y=(int)getMousePosition().getY();
       while(true){
       	if((int)getMousePosition().getX() != x || (int)getMousePosition().getY() != y){
       		x=getMousePosition().x;
       		y=getMousePosition().y;
       		repaint();
       	}
       }
   }   

   public Point pixel(double x, double y, double z){
	   double vertical = 0, horizontal = 0;
	   x-=meX; y-=meY; z-=meZ;
	   double temp = Math.sqrt(x*x + z*z)*Math.cos(Math.acos(x/Math.sqrt(x*x+z*z))+Math.toRadians(meHorizontal));
	   z=Math.sqrt(x*x + z*z)*Math.sin(Math.acos(x/Math.sqrt(x*x+z*z))+Math.toRadians(meHorizontal));
	   x=temp;
	   vertical = getSize().getHeight()/2-(90*y/z)*(getSize().getWidth()/120);
	   horizontal = getSize().getWidth()/2+(90*x/z)*(getSize().getWidth()/120);
	   return new Point((int)horizontal, (int)vertical);
   }
   
   public void linea(Graphics g, double x, double y, double z, double toX, double toY, double toZ){
	   double vertical = 0, toVertical = 0;
	   double horizontal = 0, toHorizontal = 0;
	   x-=meX; y-=meY; z-=meZ; /**comprobar que los puntos estean en el campo de vision (delante)**/
	   toX-=meX; toY-=meY; toZ-=meZ;
	   if(Math.toDegrees(Math.asin(x/Math.sqrt(x*x+z*z)))-meHorizontal>90 || Math.toDegrees(Math.asin(x/Math.sqrt(x*x+z*z)))-meHorizontal<-90
		   || (Math.toDegrees(Math.asin(toX/Math.sqrt(toX*toX+toZ*toZ)))-meHorizontal)>90 || (Math.toDegrees(Math.asin(toX/Math.sqrt(toX*toX+toZ*toZ)))-meHorizontal)<-90)
		   		return;
	   double temp = Math.sqrt(x*x + z*z)*Math.cos(Math.acos(x/Math.sqrt(x*x+z*z))+Math.toRadians(meHorizontal));
	   z=Math.sqrt(x*x + z*z)*Math.sin(Math.acos(x/Math.sqrt(x*x+z*z))+Math.toRadians(meHorizontal));
	   x=temp;
	   temp = Math.sqrt(toX*toX + toZ*toZ)*Math.cos(Math.acos(toX/Math.sqrt(toX*toX+toZ*toZ))+Math.toRadians(meHorizontal));
	   toZ=Math.sqrt(toX*toX + toZ*toZ)*Math.sin(Math.acos(toX/Math.sqrt(toX*toX+toZ*toZ))+Math.toRadians(meHorizontal));
	   toX=temp;
	   /*vertical = getSize().getHeight()/2-Math.toDegrees(Math.tan(y/z))*(getSize().getWidth()/120);
	   horizontal = getSize().getWidth()/2+Math.toDegrees(Math.tan(x/z))*(getSize().getWidth()/120);
	   toVertical = getSize().getHeight()/2-Math.toDegrees(Math.tan(toY/toZ))*(getSize().getWidth()/120);
	   toHorizontal = getSize().getWidth()/2+Math.toDegrees(Math.tan(toX/toZ))*(getSize().getWidth()/120);*/
	   if(Math.acos(x*Math.cos(meHorizontal)+y*Math.sin(meVertical)+z*Math.sin(meHorizontal))/(Math.sqrt(x*x+y*y+z*z))>90
	      || Math.acos(toX*Math.cos(meHorizontal)+toY*Math.sin(meVertical)+toZ*Math.sin(meHorizontal))/(Math.sqrt(toX*toX+toY*toY+toZ*toZ))>90)
			   return;
	   vertical = getSize().getHeight()/2-(90*y/z)*(getSize().getWidth()/120);
	   horizontal = getSize().getWidth()/2+(90*x/z)*(getSize().getWidth()/120);
	   toVertical = getSize().getHeight()/2-(90*toY/toZ)*(getSize().getWidth()/120);
	   toHorizontal = getSize().getWidth()/2+(90*toX/toZ)*(getSize().getWidth()/120);
	   g.drawLine((int)horizontal, (int)vertical, (int)toHorizontal, (int)toVertical);
   }
   
   public void cuadrado(Graphics g, double x, double y, double z, double l){
		linea(g, x,y,z, x,l+y,z);
		linea(g, l+x,y,z, x,y,z);
	   	linea(g, x,l+y,z, l+x,l+y,z);
	   	linea(g, l+x,l+y,z, l+x,y,z);
   }
   
   public void cubo(Graphics g, double x, double y, double z, double l){
	linea(g, x,y,z, x,l+y,z);
	linea(g, l+x,y,z, x,y,z);
   	linea(g, x,l+y,z, l+x,l+y,z);
   	linea(g, l+x,l+y,z, l+x,y,z);
   	
   	linea(g, x,l+y,l+z, l+x,l+y,l+z);
   	linea(g, l+x,l+y,l+z, l+x,y,l+z);
   	linea(g, l+x,y,l+z, x,y,l+z);
   	linea(g, x,y,l+z, x,l+y,l+z);

   	linea(g, x,l+y,z, x,l+y,l+z);
   	linea(g, l+x,l+y,z, l+x,l+y,l+z);
   	linea(g, l+x,y,z, l+x,y,l+z);
   	linea(g, x,y,z, x,y,l+z);
   }
}