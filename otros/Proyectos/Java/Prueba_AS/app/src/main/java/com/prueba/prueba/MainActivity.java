package com.prueba.prueba;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.pm.ActivityInfo;
import android.graphics.Bitmap;
import android.graphics.Bitmap.CompressFormat;
import android.graphics.Bitmap.Config;
import android.graphics.Color;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.os.Environment;
import android.os.Vibrator;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnLayoutChangeListener;
import android.view.View.OnTouchListener;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.Toast;

import java.io.File;
import java.io.FileOutputStream;

public class MainActivity extends Activity implements SensorEventListener  {
	Bitmap bmp_immutable, bmp;
	boolean bmp_is_made=false;
	ImageView iv;
	int width, height;
	boolean painting=false;
	int size=1, color=Color.WHITE;
	float prevX=0, prevY=0;
	Vibrator vibr;
	SensorManager sensor;
	Context context = this;
	float mAccel;
	float mAccelCurrent;  
	float mAccelLast;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
		vibr = (Vibrator) getSystemService(VIBRATOR_SERVICE);
		sensor = (SensorManager) getSystemService(SENSOR_SERVICE);
	    sensor.registerListener(this, sensor.getDefaultSensor(Sensor.TYPE_ACCELEROMETER), SensorManager.SENSOR_DELAY_NORMAL);
	    mAccel = 0.00f;
	    mAccelCurrent = SensorManager.GRAVITY_EARTH;
	    mAccelLast = SensorManager.GRAVITY_EARTH;
		iv = (ImageView)findViewById(R.id.imageView1);
		iv.addOnLayoutChangeListener(new OnLayoutChangeListener(){
			
			public void onLayoutChange(View v, int a, int b, int c, int d, int e, int f, int g, int h){
				width = iv.getWidth();
				height = iv.getHeight();
				if(!bmp_is_made){
			        int[] colors = new int[width*height];
			        for(int i=0; i<width*height; i++)
			        	colors[i]=Color.BLACK;
					bmp_immutable = Bitmap.createBitmap(colors,width,height,Config.ARGB_8888);
					bmp = bmp_immutable.copy(bmp_immutable.getConfig(),true);
					bmp_is_made=true;
				}
				iv.setImageBitmap(bmp);
				iv.setOnTouchListener(new OnTouchListener(){
					@Override
					public boolean onTouch(View v, MotionEvent e){
						switch(e.getAction()){
						case MotionEvent.ACTION_DOWN:
							prevX = e.getX();
							prevY = e.getY();
							painting=true;
							if(e.getX()<bmp.getWidth() && e.getY()<bmp.getHeight() && e.getX()>=0 && e.getY()>=0)
								bmp.setPixel((int)e.getX(), (int)e.getY(), color);
							iv.setImageBitmap(bmp);
							iv.invalidate();
							break;
						case MotionEvent.ACTION_UP:
							painting=false;
							size = 1;
							color= Color.WHITE;
							break;
						case MotionEvent.ACTION_MOVE:
							drawLine(bmp,prevX,prevY,e.getX(), e.getY(), color,size);
							prevX=e.getX();
							prevY=e.getY();
							iv.setImageBitmap(bmp);
							iv.invalidate();
							break;
						}
						return false;
					}
					
				});
			}
		});
		
	}
	
	void drawLine(Bitmap bmp, float p1x, float p1y, float p2x, float p2y, int color, int grosor){
        float dX,dY,Pasos;
        float xInc,yInc,i,x,y;

        dX = p1x - p2x;
        dY = p1y - p2y;

        if (Math.abs(dX) > Math.abs(dY)){
            Pasos = Math.abs(dX);
        }else
            Pasos = Math.abs(dY);

        xInc = dX/Pasos;
        yInc = dY/Pasos;

        x = p1x;
        y = p1y;

        /** PIXEL INICIAL **/
        if (x>=0 && x<bmp.getWidth() && y>=0 && y<bmp.getHeight())
        bmp.setPixel((int)x,(int)y,color);

        for (i=1; i<=Pasos; i++){
            x -= xInc;
            y -= yInc;

            /** GROSOR **/
            if(grosor > 1){
            for (int j=(int)Math.floor(x-grosor/2); j<=x+grosor/2; j++)
                for (int z=(int)Math.floor(y-grosor/2); z<=y+grosor/2; z++)
                    if (j>=0 && j<bmp.getWidth() && z>=0 && z<bmp.getHeight())
                        bmp.setPixel(j,z,color);
            }else 
                if (x>=0 && x<bmp.getWidth() && y>=0 && y<bmp.getHeight())
                	bmp.setPixel((int)x,(int)y,color);
        }
    }

	@Override
	public void onSensorChanged(SensorEvent event) {
	      float x = event.values[0];
	      float y = event.values[1];
	      float z = event.values[2];
	      mAccelLast = mAccelCurrent;
	      mAccelCurrent = (float) Math.sqrt((double) (x*x + y*y + z*z));
	      float delta = mAccelCurrent - mAccelLast;
	      mAccel = mAccel * 0.9f + delta;
	      if(mAccel>2.5){
	    	  int c = Color.rgb((int)(Math.random()*256),(int)(Math.random()*256),(int)(Math.random()*256));
	    	  for(int i=0; i<bmp.getWidth(); i++)
	    		  for(int j=0; j<bmp.getHeight(); j++)
	    			  if(bmp.getPixel(i,j)==Color.WHITE)
	    				  bmp.setPixel(i,j,c);
	    	  iv.setImageBitmap(bmp);
	    	  iv.invalidate();
	      }
	}

	@Override
	public void onAccuracyChanged(Sensor sensor, int accuracy) {
		
		
	}
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu){
		getMenuInflater().inflate(R.menu.actionbar, menu);
		return true;
	}
	
	@Override
	public boolean onOptionsItemSelected(MenuItem item){
		switch(item.getItemId()){
		case R.id.menu_new:
			eraseBMP();
	  	  	break;
	  	  	
		case R.id.menu_save:
			saveBMP();
			break;
			
		}
		return true;
	}

	public void eraseBMP(){
		vibr.vibrate(50);
  	  	for(int i=0; i<bmp.getWidth(); i++)
  	  		for(int j=0; j<bmp.getHeight(); j++)
  	  			bmp.setPixel(i,j,Color.BLACK);
	}

	public void saveBMP(){
			
			AlertDialog.Builder alert = new AlertDialog.Builder(this);

			alert.setTitle("File");
			alert.setMessage(R.string.get_file_name);

			// Set an EditText view to get user input 
			final EditText input = new EditText(this);
			alert.setView(input);

			alert.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
			public void onClick(DialogInterface dialog, int whichButton) {
				String name_new_bmp = input.getText().toString();
				if(name_new_bmp.length()<1) return;
				try {
					if(!name_new_bmp.substring(name_new_bmp.length()).equalsIgnoreCase(".png"))
						name_new_bmp += ".png";
		            File myFile = new File(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES)+"/"+name_new_bmp);
		            myFile.createNewFile();
		            FileOutputStream fOut = new FileOutputStream(myFile);
		            bmp.compress(CompressFormat.PNG, 0, fOut);
		            fOut.flush();
		            fOut.close();
		            Toast.makeText(context,"Picture '"+name_new_bmp+"' created at "+Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES), Toast.LENGTH_LONG).show();
		        } 
		        catch (Exception e) 
		        {
		            Toast.makeText(context, e.getMessage(),Toast.LENGTH_SHORT).show();
		        }
			}
			});

			alert.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
			  public void onClick(DialogInterface dialog, int whichButton) {
			  }
			});
			
			alert.show();
	}
}


