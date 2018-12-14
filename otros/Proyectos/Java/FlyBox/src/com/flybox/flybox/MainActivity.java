package com.flybox.flybox;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.os.Bundle;
import android.os.Handler;
import android.os.Vibrator;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnLayoutChangeListener;
import android.view.View.OnTouchListener;
import android.widget.ImageView;

public class MainActivity extends Activity{
	
	boolean map_is_made=false;
	Bitmap bmp = Bitmap.createBitmap(1, 1, Config.ARGB_8888);
	ImageView iv;
	int width, height;
	Vibrator vibr;
	Context context = this;
	mapa map = new mapa();
	private Handler handler = new Handler();
	private Runnable runnable = new Runnable() {
		   @Override
		   public void run() {
		      map.next();
		      map.getTo(bmp);
		      iv.setImageBitmap(bmp);
		      iv.invalidate();
		      handler.postDelayed(this, 10);
		   }
		};
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
		vibr = (Vibrator) getSystemService(VIBRATOR_SERVICE);
		iv = (ImageView)findViewById(R.id.imageView1);
		iv.addOnLayoutChangeListener(new OnLayoutChangeListener(){
			
			public void onLayoutChange(View v, int a, int b, int c, int d, int e, int f, int g, int h){
				width = iv.getWidth();
				height = iv.getHeight();
				if(!map_is_made){
					map.start(height/5, width/5, 5, 10, width/2, 1, 10, 1, mapa.RANDOM);
					map_is_made=true;
				}
				iv.setImageBitmap(map.get());
				iv.setOnTouchListener(new OnTouchListener(){
					@Override
					public boolean onTouch(View v, MotionEvent e){
						map.setVelocity(map.getVelocity()-10);
						return false;
					}
				});
				handler.postDelayed(runnable, 10);
			}
		});
		
	}
}
