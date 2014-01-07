package com.example.androidtuya;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;

public class MyView extends View
{

	Paint mPaint = null;		// used to draw line
	Paint mBitmapPaint = null; 	// used to draw bitmap
	Path mPath = null;			// save the point
	Bitmap mBitmap = null;		// used as background
	Canvas mCanvas = null;		// what's it used for
	float posX,posY;			// used as touched position
	private final float TOUCH_TOLERANCE = 4;
	
	MyView(Context context,AttributeSet attr)
	{
		super(context,attr);
		//mBitmap = BitmapFactory.decodeResource(getResources(), R.drawable.sb4);
		mPaint = new Paint();
		mPaint.setAntiAlias(true);
	    mPaint.setDither(true);
	    mPaint.setColor(Color.RED);
	    mPaint.setStyle(Paint.Style.STROKE);
	    mPaint.setStrokeJoin(Paint.Join.ROUND);
	    mPaint.setStrokeCap(Paint.Cap.ROUND);
	    mPaint.setStrokeWidth(12);
	    
	    mBitmapPaint = new Paint(Paint.DITHER_FLAG);
	    mPath = new Path();
	}
	
	MyView(Context context)
	{
		super(context);
		//mBitmap = BitmapFactory.decodeResource(getResources(), R.drawable.sb4);
		mPaint = new Paint();
		mPaint.setAntiAlias(true);
	    mPaint.setDither(true);
	    mPaint.setColor(Color.RED);
	    mPaint.setStyle(Paint.Style.STROKE);
	    mPaint.setStrokeJoin(Paint.Join.ROUND);
	    mPaint.setStrokeCap(Paint.Cap.ROUND);
	    mPaint.setStrokeWidth(12);
	    
	    mBitmapPaint = new Paint(Paint.DITHER_FLAG);
	    mPath = new Path();
	}
	
	@Override
	protected void onSizeChanged(int w,int h,int oldw,int oldh)
	{
		super.onSizeChanged(w, h, oldw, oldh);
        //mBitmap = Bitmap.createBitmap(w, h, Bitmap.Config.ARGB_8888);
		mBitmap = BitmapFactory.decodeResource(getResources(), R.drawable.sb4)
				.copy(Bitmap.Config.ARGB_8888, true);
        mCanvas = new Canvas(mBitmap);
	}
	
	@Override
	public void onDraw(Canvas canvas) {
		//super.onDraw(canvas);
		canvas.drawColor(0xFFAAAAAA);
		canvas.drawBitmap(mBitmap,0,0,mBitmapPaint);
		canvas.drawPath(mPath, mPaint);	
		canvas.drawRect(10,10,100,100,mPaint);
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event)
	{
		float x = event.getX();
		float y = event.getY();
		
		switch(event.getAction())
		{
			case MotionEvent.ACTION_DOWN:
				mPath.reset();
				mPath.moveTo(x, y);
				posX = x;
				posY = y;
				postInvalidate();
				break;
			case MotionEvent.ACTION_MOVE:
				float dx = Math.abs(x - posX);
				float dy = Math.abs(y - posY);
				if(dx >= TOUCH_TOLERANCE || dy > TOUCH_TOLERANCE)
				{
					//mPath.lineTo(x, y);
					mPath.quadTo(posX, posY, (x + posX)/2, (y + posY)/2);
					posX = x;
					posY = y;
				}
				postInvalidate();
				break;
			case MotionEvent.ACTION_UP:
				mPath.lineTo(posX, posY);
				// avoid the previous line is cleared when press again
				mCanvas.drawPath(mPath,mPaint); 
				mPath.reset();
				postInvalidate();
				break;
		}
		//return super.onTouchEvent(event);
		return true;
	}
}
