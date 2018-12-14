package es.hol.ivancea.flybox;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class SpecialView extends SurfaceView{
    Bitmap bmp = null;
    SurfaceHolder holder;
    Paint p;

    SpecialView(Context context){
        super(context);
        holder = getHolder();
        holder.addCallback(new SurfaceHolder.Callback(){

            @Override
            public void surfaceDestroyed(SurfaceHolder holder){
            }
            @Override
            public void surfaceCreated(SurfaceHolder holder){
                Canvas c = holder.lockCanvas(null);
                draw(c);
                holder.unlockCanvasAndPost(c);
            }

            @Override
            public void surfaceChanged(SurfaceHolder holder, int format, int width, int height){
            }
        });
        p = new Paint();
    }

    public void setBMP(Bitmap bitmap){
        bmp = bitmap;
    }

    @Override
    public void onDraw(Canvas canvas){
        if(bmp==null)
            return;
        canvas.drawColor(Color.WHITE);
        canvas.drawBitmap(bmp, 0, 0, p);
    }
}
