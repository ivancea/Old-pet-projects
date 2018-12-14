package chess;

import java.awt.Color;
import java.awt.Font;
import java.awt.Frame;
import java.awt.Graphics;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.util.ArrayList;


public class Chess extends Frame{
	private static final long serialVersionUID = 1L;

	public ArrayList<Piece> pieces;

	int squareSize = 50;
	
	int selectionX,
		selectionY;
	
	private ArrayList<Piece> chargePieces(){
		ArrayList<Piece> temp = new ArrayList<Piece>();
		for(int x=1; x<=8; x++){
			temp.add(new Piece (PieceType.Pawn, PieceColor.Black, x,2));
			temp.add(new Piece (PieceType.Pawn, PieceColor.White, x,7));
			if(x==1||x==8){
				temp.add(new Piece(PieceType.Rook, PieceColor.Black, x,1));
				temp.add(new Piece(PieceType.Rook, PieceColor.White, x,8));
			}else if(x==2||x==7){
				temp.add(new Piece(PieceType.Knight, PieceColor.Black, x,1));
				temp.add(new Piece(PieceType.Knight, PieceColor.White, x,8));
			}else if(x==3||x==6){
				temp.add(new Piece(PieceType.Bishop, PieceColor.Black, x,1));
				temp.add(new Piece(PieceType.Bishop, PieceColor.White, x,8));
			}else if(x==4){
				temp.add(new Piece(PieceType.Queen, PieceColor.Black, x,1));
				temp.add(new Piece(PieceType.Queen, PieceColor.White, x,8));
			}else{
				temp.add(new Piece(PieceType.King, PieceColor.Black, x,1));
				temp.add(new Piece(PieceType.King, PieceColor.White, x,8));
			}
			temp.add(new Piece(PieceType.Queen, PieceColor.Black, 4,5));
		}
		return temp;
	}
	
	Chess(){
		selectionX = selectionY = -1;
		pieces = new ArrayList<Piece>();
		pieces = chargePieces();
		setTitle("Chess 1.0");
        setBackground(Color.WHITE);
        setSize(500,500);
        setLocationRelativeTo(null);    //Centrar
        setResizable(false);
        this.addMouseListener(new MouseListener(){

			public void mouseClicked(MouseEvent e) {
				int x = e.getX()/squareSize,
					y = e.getY()/squareSize;
				// selectionX, selectionY = -1
				if(x>8||x<1||y>8||y<1)
					return;
				if(selectionX==x && selectionY==y){
					selectionX = selectionY = -1;
					return;
				}
				System.out.println('a');
				if(selectionX > 8 || selectionX < 1 || selectionY > 8 || selectionY < 1){
					System.out.println('b');
					for(Piece p:pieces){
						if(p.getX()==x && p.getY()==y){
							selectionX = x;
							selectionY = y;
							break;
						}
					}
				}else{
					System.out.println('c');
					int index = -1;
					for(int i=0;i<pieces.size();i++){
						if(pieces.get(i).getX()==selectionX && pieces.get(i).getY()==selectionY){
							index = i;
							break;
						}
					}
					if(index==-1){
						selectionX = selectionY = -1;
						return;
					}
					if(pieces.get(index).canMove(pieces, x , y)){
						System.out.println('d');
						pieces.get(index).move(pieces, x, y);
						selectionX = selectionY = -1;
					}
				}
			}

			public void mousePressed(MouseEvent e) {}
			public void mouseReleased(MouseEvent e) {}
			public void mouseEntered(MouseEvent e) {}
			public void mouseExited(MouseEvent e) {}
        	
        });
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
	}
	
	public boolean canMove(int index, int x, int y){
		return pieces.get(index).canMove(pieces, x,y);
	}

	public void paint(Graphics g){
		for(int i=0; i<8; i++){
			for(int j=0; j<8; j++){
				if((i+j)%2==0)
					g.setColor(Color.WHITE);
				else
					g.setColor(Color.BLACK);
				g.fillRect(squareSize+i*squareSize, squareSize+j*squareSize, squareSize, squareSize);
			}
		}
		g.setColor(Color.BLACK);
		g.drawRect(squareSize, squareSize, this.getSize().width-squareSize*2, this.getSize().height-squareSize*2);
		
		String temp = "KQPRBN";
		for(Piece p:pieces){
			if(p.getX()==selectionX && p.getY()==selectionY)
				g.setColor(Color.DARK_GRAY);
			else
				g.setColor(Color.GRAY);
			g.fillOval(p.getX()*squareSize + 5, p.getY()*squareSize + 5, squareSize-10, squareSize-10);
			if(p.getColor()==PieceColor.Black){			
				g.setColor(Color.BLACK);
			}else{
				g.setColor(Color.WHITE);
			}
			g.setFont(new Font("Arial", Font.BOLD, 25));
			g.drawBytes(temp.getBytes(), p.getType().ordinal(), 1, 16 + squareSize*p.getX(), 35 + squareSize*p.getY());
		}
	}
	
	public static void main(String[] args){
		Chess ch = new Chess();
		ch.setVisible(true);
	}

}

/*
1:



*/