package es.hol.ivancea.flybox;

import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.graphics.Color;

import java.util.Vector;

class mapa{
	
    public static final int FULL=-1;
    public static final int NONE=0;
  	public static final int RANDOM=1;
    public static final int HOLES=2;
    public static final int TUNNEL=3;
    public static final int ASTEROIDS=4;
	
    private Vector<String> _map = new Vector<String>();
    private int _tamX, _tamY, _pixelSize, _moved, _blocks, _type;
    private double _x, _y;
    private double _velocity, _maxVelocity;
    private double _grav;
    private int _player, _walls, _background;/** Color **/
    private int _generate, _generate2;

    private String voidString(){
        String t = new String("1");
        for(int i=0; i<_tamY-2; i++)
            t+='0';
        t+='1';
        return t;
    }
    
    private String fullString(){
        String t = new String();
        for(int i=0; i<_tamY; i++)
            t+='0';
        return t;
    }

    private String makeTunnel(int index, int size){
        StringBuilder t = new StringBuilder(voidString());
        for(int i=1; i<index; i++)
        	t.setCharAt(i,'1');
        for(int i=index+size; i<_tamY-1; i++)
            t.setCharAt(i,'1');
        return t.toString();
    }

    private String generateRandom(){
        StringBuilder t = new StringBuilder(voidString());
        for(int c = _blocks; c>0 && _tamY-2-_blocks+c>0; c--){
            int r = (int)(Math.random()*(_tamY-2-_blocks+c));
            for(int i=1+r; i<_tamY-1; i++)
                if(t.toString().toCharArray()[i]=='0'){
                    t.setCharAt(i,'1');
                    break;
                }
        }
        return t.toString();
    }

    private String generateHoles(){
        if(_generate>=10 || _generate<0){
            _generate=0;
            return makeTunnel((int)(1+Math.random()*(_tamY-2-_blocks)),_blocks);
        }else ++_generate;
        return voidString();
    }

    private String generateTunnel(){
        if(_generate==0){
            _generate = (int)(Math.random()*(_tamY-_blocks-2) + 1);
            return makeTunnel(_generate,_blocks);
        }
        if(_generate<=1)
            _generate+=Math.random()*2;
        else if(_generate>=_tamY-1-_blocks)
            _generate+=(int)(Math.random()*2)-1;
        else
            _generate+=(int)(Math.random()*3)-1;
        return makeTunnel(_generate,_blocks);
    }

    private String generateAsteroids(){
        StringBuilder t = new StringBuilder(voidString());
        if(_generate==0){
            _generate2 = (int)(Math.random()*(_tamY-_blocks-2));
        }
        if(_generate<_blocks){
            for(int i=0;i<_blocks;i++)
                if(_generate2+i+1<t.length())
                    t.setCharAt(_generate2 + i + 1,'1');
            _generate++;
        }else if(_generate>=_blocks*2-1)
            _generate=0;
        else ++_generate;
        return t.toString();
    }


    /*mapa(int tamX, int tamY, int pixelSize, double x, double y, double grav, double maxVelocity, int blocks, int type){
        _tamX=(tamX);_tamY(tamY);_pixelSize(pixelSize); _type(type);
         _x(x);_y(y);_velocity(0);_grav(grav);_moved(0); _blocks(blocks);
         _maxVelocity(maxVelocity);_player(Color(50,125,255));
         _walls(Color(0,0,0));_background(Color(255,255,255));
        String t = voidString();
        for(int i=0; i<_tamX+1; i++)
            _map.push_back(t);

        _generate = 0;
        _generate2 = 0;
    }*/

    void start(int tamX, int tamY, int pixelSize, double x, double y, double grav, double maxVelocity, int blocks, int type){
        _tamX=tamX;
        _tamY=tamY;
        _pixelSize=pixelSize;
        _type=type;
        _x=x;
        _y=y;
        _velocity=0;
        _grav=grav;
        _moved=0;
        _blocks=blocks;
        _maxVelocity=maxVelocity;
        _player=Color.rgb(50,125,255);
        _walls=Color.rgb(0,0,0);
        _background=Color.rgb(255,255,255);
        String t = voidString();
        _map.clear();
        for(int i=0; i<_tamX+1; i++)
            _map.add(t);
        _generate=0;
        _generate2=0;
    }

    int getSizeX(){return _tamX;}
    int getSizeY(){return _tamY;}
    int getPixelSize(){return _pixelSize;}
    double getX(){return _x;}
    double getY(){return _y;}
    double getVelocity(){return _velocity;}
    double getGravity(){return _grav;}
    int getBlocks(){return _blocks;}
    int getType(){return _type;}

    void setX(double x){_x=x;}
    void setY(double y){_y=y;}
    void setVelocity(double velocity){_velocity=velocity;}
    void setGravity(double grav){_grav=grav;}
    void getBlocks(int blocks){_blocks=blocks;}
    void setWallsColor(int walls){_walls=walls;}
    void setWallsColor(int r,int g,int b){_walls = Color.rgb(r,g,b);}
    void setPlayerColor(int player){_player=player;}
    void setPlayerColor(int r,int g,int b){_player = Color.rgb(r,g,b);}
    void setBackgroundColor(int background){_background=background;}
    void setBackgroundColor(int r,int g,int b){_background = Color.rgb(r,g,b);}
    void setType(int type){_type=type;_generate=0;_generate2=0;}
    void setBlocks(int blocks){_blocks=blocks;_generate=0;_generate2=0;}

    Bitmap get(){
        Bitmap t;
        t=Bitmap.createBitmap(_tamX*_pixelSize,_tamY*_pixelSize,Config.ARGB_8888);
        for(int i=0; i<t.getWidth(); i++)
        	for(int j=0; j<t.getHeight(); j++)
        		t.setPixel(i, j, _background);
        for(int i=0; i<_map.size(); i++)
            for(int j=0; j<_tamY; j++)
                if(_map.get(i).toCharArray()[j]=='1')
                    for(int n=-_moved; n<_pixelSize-_moved;n++)
                        for(int m=0; m<_pixelSize;m++)
                            if(t.getWidth() > i*_pixelSize + n && t.getHeight() > j*_pixelSize + m && 0<=i*_pixelSize + n && 0<=j*_pixelSize + m)
                                t.setPixel(i*_pixelSize + n, j*_pixelSize + m, _walls);
        for(int i=0; i<_pixelSize; i++)
            for(int j=0; j<_pixelSize; j++)
                if(_x+i<t.getWidth() && _y+j<t.getHeight())
                    t.setPixel((int)_x+i,(int)_y+j,_player);
        return t;
    }

    void getTo(Bitmap t){
    	t = Bitmap.createBitmap(_tamX*_pixelSize,_tamY*_pixelSize,Config.ARGB_8888);
        for(int i=0; i<t.getWidth(); i++)
        	for(int j=0; j<t.getHeight(); j++)
        		t.setPixel(i, j, _background);
        for(int i=0; i<_map.size(); i++)
            for(int j=0; j<_tamY; j++)
                if(_map.get(i).toCharArray()[j]=='1')
                    for(int n=-_moved; n<_pixelSize-_moved;n++)
                        for(int m=0; m<_pixelSize;m++)
                            if(t.getWidth() > i*_pixelSize + n && t.getHeight() > j*_pixelSize + m && 0<=i*_pixelSize + n && 0<=j*_pixelSize + m)
                                t.setPixel(i*_pixelSize + n, j*_pixelSize + m, _walls);
        for(int i=0; i<_pixelSize; i++)
            for(int j=0; j<_pixelSize; j++)
                if(_x+i<t.getWidth() && _y+j<t.getHeight())
                    t.setPixel((int)_x+i,(int)_y+j,_player);
    }

    int next(){
        if(_moved >= _pixelSize){
            _map.remove(0);
            switch(_type){
            case FULL:
                    _map.add(fullString());
                break;
            case RANDOM:
                    _map.add(generateRandom());
                break;
            case HOLES:
                    _map.add(generateHoles());
                break;
            case TUNNEL:
                    _map.add(generateTunnel());
                break;
            case ASTEROIDS:
                    _map.add(generateAsteroids());
                break;
            default:
                    _map.add(voidString());
                break;
            }
            _moved -= _pixelSize;
        }else ++_moved;

        if(_maxVelocity>0)
            if(_velocity > _maxVelocity)
                _velocity = _maxVelocity;
            else if(_velocity<0 && -_velocity > _maxVelocity)
                _velocity = -_maxVelocity;
        _y += _velocity;
        _velocity += _grav;
        if(_y<0) _y = 0;
        else if(_y>=_tamY*_pixelSize) _y = (_tamY-1)*_pixelSize;
        for(int i=0; i<2; i++)
            for(int j=0; j<2; j++)
                if(_map.get((int)(_x/_pixelSize + i)).toCharArray()[(int)(_y/_pixelSize + j)]=='1')
                    return 1;
        return 0;
    }
};

/*int record(unsigned int now){
    ifstream f("FlyBoxRecord.icf",ios::binary);
    char *n;
    n = (char*)&now;
    if(f.fail() || f.eof()){
        f.close();
        ofstream fi("FlyBoxRecord.icf",ios::trunc|ios::binary);
        fi << n[0] << n[1] << n[2] << n[3];
        fi.close();
        return now;
    }
    char t[4];
    f >> t[0] >> t[1] >> t[2] >> t[3];
    int te = *((int*)&t);
    cout << te << endl;
    if(te<now){
        f.close();
        ofstream fi("FlyBoxRecord.icf",ios::trunc|ios::binary);
        fi << n[0] << n[1] << n[2] << n[3];
        fi.close();
        return now;
    }
    f.close();
    return te;
}*/
