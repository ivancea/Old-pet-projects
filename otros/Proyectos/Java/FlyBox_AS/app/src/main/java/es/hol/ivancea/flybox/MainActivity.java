package es.hol.ivancea.flybox;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.os.Vibrator;
import android.view.Display;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;

public class MainActivity extends Activity{

    boolean map_is_made = false;
    int width, height;
    Vibrator vibr;
    Context context = this;
    SpecialView sv;
    mapa map;
    LoopThread th;
    /**private Handler handler = new Handler();
    private Runnable runnable = new Runnable(){
        @Override
        public void run(){
            map.next();
            map.getTo(bmp);
            iv.setImageBitmap(bmp);
            iv.invalidate();
            handler.postDelayed(this, 10);
        }
    };**/

    @Override
    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        vibr = (Vibrator) getSystemService(VIBRATOR_SERVICE);
        sv = new SpecialView(this);
        setContentView(sv);
        map = new mapa();
        Display display = ((WindowManager) getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
        width = display.getHeight();//sv.getMeasuredHeight();
        height = display.getWidth();//sv.getMeasuredWidth();
        map.start(height / 5, width / 5, 5, 10, width / 2, 1, 10, 1, mapa.TUNNEL);

        sv.setOnTouchListener(new View.OnTouchListener(){
            @Override
            public boolean onTouch(View v, MotionEvent e){
                map.setVelocity(map.getVelocity() - 10);
                return false;
            }
        });

        th = new LoopThread(sv, map);
        th.setRunning(true);
        th.start();
        /*iv = (ImageView) findViewById(R.id.imageView1);
        iv.addOnLayoutChangeListener(new OnLayoutChangeListener(){

            public void onLayoutChange(View v, int a, int b, int c, int d, int e, int f, int g, int h){
                width = iv.getHeight();
                height = iv.getWidth();
                if(!map_is_made){
                    map.start(height / 5, width / 5, 5, 10, width / 2, 1, 10, 1, mapa.RANDOM);
                    map_is_made = true;
                }
                iv.setImageBitmap(map.get());
                iv.setOnTouchListener(new OnTouchListener(){
                    @Override
                    public boolean onTouch(View v, MotionEvent e){
                        map.setVelocity(map.getVelocity() - 10);
                        return false;
                    }
                });
                //handler.postDelayed(runnable, 10);
            }
        });*/
    }
}