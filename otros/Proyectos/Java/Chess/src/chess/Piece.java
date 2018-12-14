package chess;

import java.util.ArrayList;

enum PieceType{
	King,
	Queen,
	Pawn,
	Rook,
	Bishop,
	Knight
}

enum PieceColor{
	Black,
	White
}

public class Piece {
	
	private PieceType _type;
	
	private PieceColor _color;
	
	private int _x,_y;
	
	Piece(PieceType type, PieceColor color, int x, int y){
		_type=type;
		_color=color;
		_x=x;
		_y=y;
	}
	
	public void move(ArrayList<Piece> pieces, int x, int y){
		for(int i=0; i<pieces.size(); i++)
			if(pieces.get(i).getX()==x && pieces.get(i).getY()==y){
				pieces.remove(i);
			}
		_x = x;
		_y = y;
	}
	
	public boolean canMove(ArrayList<Piece> pieces, int x,int y){
		if(x<1 || x>8 || y<1 || y>8 || (_x==x && _y==y))
			return false;
		for(Piece p:pieces){
			if(p.getX()==x && p.getY()==y && p.getColor()==_color)
				return false;
		}
		
		switch(_type){
		case King:
			return (x-_x<=1 && x-_x>=-1 && y-_y<=1 && y-_y>=-1);
		case Queen: // TODO
			if(x==_x || y==_y){
				for(Piece p:pieces){
					int pX = p.getX();
					int pY = p.getY();
					if(pX==x && pY==y)
						continue;
					if((pX==x && ((pX<x && pX>_x) || (pX<_x && pX>x)))
				    || (pY==y && ((pY<y && pY>_y) || (pY<_y && pY>y))))
						return false;
				}	
			}else if(Math.abs(x-_x)==Math.abs(y-_y)){
				for(Piece p:pieces){
					int pX = p.getX();
					int pY = p.getY();
					if(pX==_x || pY==_y)
						continue;
					if(pX==x || pY==y)
						continue;
					if(Math.abs(pX-_x)/(pX-_x)==Math.abs(x-_x)/(x-_x)
					&&(Math.abs(pY-_y)/(pY-_y)==Math.abs(y-_y)/(y-_y))){
						if((((x-_x)*(x-_x))+(y-_y)*(y-_y))>((pX-_x)*(pX-_x)+(pY-_y)*(pY-_y)))
							return false;
					}
				}
			}else return false;
			return true;
		case Knight:
			return ((Math.abs(x-_x)==1 && Math.abs(y-_y)==2)
				 || (Math.abs(x-_x)==2 && Math.abs(y-_y)==1));
		case Rook:
			if(x!=_x && y!=_y)
				return false;
			for(Piece p:pieces){
				int pX = p.getX();
				int pY = p.getY();
				if(pX==x && pY==y)
					continue;
				if((pX==x && ((pX<x && pX>_x) || (pX<_x && pX>x)))
			    || (pY==y && ((pY<y && pY>_y) || (pY<_y && pY>y))))
					return false;
			}
			return true;
		case Bishop:
			if(Math.abs(x-_x)!=Math.abs(y-_y))
				return false;
			for(Piece p:pieces){
				int pX = p.getX();
				int pY = p.getY();
				if(pX==_x || pY==_y)
					continue;
				if(pX==x || pY==y)
					continue;
				if(Math.abs(pX-_x)/(pX-_x)==Math.abs(x-_x)/(x-_x)
				&&(Math.abs(pY-_y)/(pY-_y)==Math.abs(y-_y)/(y-_y))){
					if((((x-_x)*(x-_x))+(y-_y)*(y-_y))>((pX-_x)*(pX-_x)+(pY-_y)*(pY-_y)))
						return false;
				}
			}
			return true;
		case Pawn:
			if(_color==PieceColor.White){
				if(x==_x){
					if(y-_y==2 || y-_y==1)
						break;
					return false;
				}else{
					for(Piece p:pieces){
						int pX=p.getX();
						int pY=p.getY();
						if(pX==x && pY==y)
							continue;
						if(y-_y!=1 || Math.abs(x-_x)!=1)
							return false;
						if(p.getColor()==PieceColor.White)
							return false;
					}
				}
			}else{
				if(x==_x){	
					if(y-_y==-2 || y-_y==-1)
						break;
					return false;
				}else{
					for(Piece p:pieces){
						int pX=p.getX();
						int pY=p.getY();
						if(pX==x && pY==y)
							continue;
						if(y-_y!=-1 || Math.abs(x-_x)!=1)
							return false;
						if(p.getColor()==PieceColor.Black)
							return false;
					}
				}
			}
		}
		return true;
	}
	
	public PieceType getType(){
		return _type;
	}
	
	public PieceColor getColor(){
		return _color;
	}
	
	public int getX(){
		return _x;
	}
	
	public int getY(){
		return _y;
	}
}
