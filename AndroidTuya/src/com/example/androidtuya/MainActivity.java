package com.example.androidtuya;

import net.margaritov.preference.colorpicker.*;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore;
import android.app.Activity;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.ImageView;
import android.app.Dialog;
import android.app.AlertDialog;
import android.content.ContentResolver;
import android.content.DialogInterface;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;

public class MainActivity extends Activity 
	implements ColorPickerDialog.OnColorChangedListener
{

	final int SELECT_IMAGE = 1;
	private MyView touchView = null;
	private int mBaseColor;
	private boolean mAlphaSliderEnabled = false;
	private boolean mHexValueEnabled = false;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		touchView = (MyView)findViewById(R.id.myView);
		
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
	
	@Override
	public void onColorChanged(int color)
	{
		mBaseColor = color;
	}
	public boolean onOptionsItemSelected(MenuItem Item)
	{
		int item_id = Item.getItemId();
		switch(item_id)
		{
		case R.id.choose:
			String[] itemsTo = {"Ñ¡ÔñÍ¼Æ¬","Ñ¡ÔñÑÕÉ«"};
			AlertDialog.Builder builder = new AlertDialog.Builder(MainActivity.this);
			builder.setTitle("Ñ¡Ôñ±³¾°");
			builder.setItems(itemsTo, new DialogInterface.OnClickListener() {
				
				@Override
				public void onClick(DialogInterface arg0, int arg1) {
					// TODO Auto-generated method stub
					if(arg1 == 0)
					{
						pickupLocalImage(SELECT_IMAGE);
					}
					if(arg1 == 1)
					{
						showDialog(null);
					}
				}
			});
			builder.create().show();
		case R.id.pen:
		case R.id.save:
		case R.id.clear:
		}
		return true;
	}

	protected void pickupLocalImage(int return_num) 
	{
		try {
			Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
			intent.addCategory(Intent.CATEGORY_OPENABLE);
			intent.setType("image/*");
			startActivityForResult(intent,return_num);
			} catch (Exception e) 
			{
				e.printStackTrace();
			}
	}
	
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
    	if(resultCode == RESULT_OK) {
    		switch(requestCode) {
    			case SELECT_IMAGE:
    				try
    				{
    					Uri imgUri = data.getData();
    					if(imgUri != null)
    					{
    						ContentResolver cr = this.getContentResolver();
    						String[] columnStr = new String[]{MediaStore.Images.Media.DATA};
    						Cursor cursor = cr.query(imgUri, columnStr, null, null, null);
    						if(cursor != null)
    						{
    							int nID = cursor.getColumnIndex(MediaStore.Images.Media.DATA);
    							if(cursor.moveToFirst())
    							{
    								String imgPath = cursor.getString(nID);
    								Bitmap bitmap = BitmapFactory.decodeFile(imgPath);
    								touchView.setBitmap(bitmap);
    							}
    						}
    					}
    				}catch(Exception e)
    				{
    					e.printStackTrace();
    				}
			  
    				break;
    			default:
    				break;
    		};
    	}
		super.onActivityResult(requestCode, resultCode, data);
	}
	protected void showDialog(Bundle state) {
		//ColorPickerDialog mDialog = new ColorPickerDialog(getContext(), mValue);
		mBaseColor = Color.BLACK;
		ColorPickerDialog dialog = new ColorPickerDialog(MainActivity.this,mBaseColor);
		dialog.setOnColorChangedListener(MainActivity.this);
		if (mAlphaSliderEnabled) {
			dialog.setAlphaSliderVisible(true);
		}
		if (mHexValueEnabled) {
			dialog.setHexValueEnabled(true);
		}
		if (state != null) {
			dialog.onRestoreInstanceState(state);
		}
		dialog.show();
	}
}
