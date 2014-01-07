package com.example.androidtuya;

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

public class MainActivity extends Activity {

	final int SELECT_IMAGE = 1;
	private MyView TouchView = null;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		TouchView = new MyView(MainActivity.this);
		//setContentView(R.layout.activity_main);
		setContentView(TouchView);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
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
    						Cursor cursor = cr.query(imgUri, null, null, null, null);
    						if(cursor != null)
    						{
    							int nID = cursor.getColumnIndex(MediaStore.Images.Media.DATA);
    							if(cursor.moveToFirst())
    							{
    								String imgPath = cursor.getString(1);
    								ImageView imageView = new ImageView(MainActivity.this);  
    								imageView.setImageBitmap(BitmapFactory.decodeFile(imgPath));
    							}
    						}
    					}
//  					    BitmapFactory.Options bmpOption = new BitmapFactory.Options();
//    					bmpOption.inJustDecodeBounds = true;
//    					Bitmap bmp =BitmapFactory.decodeStream(cr.openInputStream(imgUri),
//    							null,bmpOption );
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

}
