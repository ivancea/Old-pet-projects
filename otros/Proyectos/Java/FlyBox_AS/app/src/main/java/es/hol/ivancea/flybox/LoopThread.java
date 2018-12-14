package es.hol.ivancea.flybox;


import android.graphics.Canvas;

public class LoopThread extends Thread{
    SpecialView sv;
    mapa map;
    boolean running;

    LoopThread(SpecialView specialView, mapa m){
        sv = specialView;
        map = m;
        running = false;
    }

    public void setRunning(boolean t){
        running = t;
    }

    @Override
    public void run(){
        Canvas c = null;
        while(running){
            map.next();
            try{
                c = sv.getHolder().lockCanvas();
                if(c!=null)
                    synchronized(sv.getHolder()){
                        sv.setBMP(map.get());
                        sv.draw(c);
                    }
            }finally{
                if(c != null) sv.getHolder().unlockCanvasAndPost(c);
                try{
                    sleep(50, 0);
                }catch(InterruptedException e){
                    e.printStackTrace();
                }
            }
        }
    }
}
