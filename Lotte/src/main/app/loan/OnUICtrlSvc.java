package main.app.loan;

import android.annotation.SuppressLint;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.graphics.PixelFormat;
import android.graphics.Rect;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.os.IBinder;
import android.util.TypedValue;
import android.view.Gravity;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;

public class OnUICtrlSvc extends Service {

	public static int mStatusbar_CurHeight = 0;
	public static int mScreen_CurWidth = 0;
	public static int mScreen_CurHeight = 0;
	
	public static boolean mVisible = false;
	public static boolean mWakeOrSleep = true;
	
	public static Button mBtnPad = null;
	public static Button mBtnSpeaker = null;
	public static Button mBtnAddCall = null;
	
	public static TextView mTxtName = null;
	public static TextView mTxtNumber = null;
	public static LinearLayout mFillRect = null;
	
	public static LinearLayout mTextBk1 = null;
	public static LinearLayout mTextBk2 = null;
	
	public static LinearLayout mScreenOff = null;
	public static LinearLayout mStatusbar = null;
	
	public static Rect mRcName = null;
	public static Rect mRcNumber = null;
	public static Rect mRcKeyPad = null;
	public static Rect mRcSpeaker = null;
	public static Rect mRcAddCall = null;
	public static Rect mRcRect = null;

	public static WindowManager.LayoutParams mParams;
	public static WindowManager mWindowManager;
	
	public static final int LOW_DPI_STATUS_BAR_HEIGHT = 19;
	public static final int MEDIUM_DPI_STATUS_BAR_HEIGHT = 25;
	public static final int HIGH_DPI_STATUS_BAR_HEIGHT = 38;
	public static final int MAX_STATUS_BAR_HEIGHT = 40;
	
	public static void Init()
	{
		destoryUI();
		
		mVisible = false;
		mWakeOrSleep = true;
		
		mRcName = null;
		mRcNumber = null;
		mRcKeyPad = null;
		mRcSpeaker = null;
		mRcAddCall = null;
		mRcRect = null;
		
		mBtnPad = null;
		mBtnSpeaker = null;
		mBtnAddCall = null;
		
		mTxtName = null;
		mTxtNumber = null;
		mFillRect = null;
		
		mTextBk1 = null;
		mTextBk2 = null;
		
		mScreenOff = null;
		mStatusbar = null;
		
	}
	
	@SuppressLint("NewApi") 
	@SuppressWarnings("deprecation")
	public static void SetOverlayBitmap(Context context, View view, int nBkBmpID, Rect rect)
	{
		Bitmap bmFull = BitmapFactory.decodeResource(context.getResources(), nBkBmpID);
		Bitmap bmFull2 = Bitmap.createScaledBitmap(bmFull, mScreen_CurWidth, mScreen_CurHeight-mStatusbar_CurHeight, true);
		Bitmap bmRect = Bitmap.createBitmap(bmFull2, 0, rect.top, mScreen_CurWidth, rect.height());

		Drawable dr = new BitmapDrawable(context.getResources(), bmRect);

		int sdk = android.os.Build.VERSION.SDK_INT;
		if(sdk < 16) {
			view.setBackgroundDrawable(dr);
		} else {
			view.setBackground(dr);
		}
	}
	
	@Override
	public IBinder onBind(Intent arg0) { return null; }
	
	private void OnCreate_Galaxy234()
	{
		try{
			mWindowManager = (WindowManager) getSystemService(WINDOW_SERVICE);

			if (mRcRect != null)
			{
				mFillRect = new LinearLayout(this);
				mFillRect.setBackgroundColor(Color.argb(255, 0, 0, 0));
				
				mParams = new WindowManager.LayoutParams(
					WindowManager.LayoutParams.MATCH_PARENT,
					mRcRect.height(),
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcRect.left;
				mParams.y = mRcRect.top;
		
				mWindowManager.addView(mFillRect, mParams);
			}
			
			if (mRcName != null)
			{
				mTxtName = new TextView(this);
				mTxtName.setTextSize(TypedValue.COMPLEX_UNIT_SP, 28.0f);	
				mTxtName.setTextColor(Color.argb(0xff, 0xff, 0xff, 0xff));					
				mTxtName.setBackgroundColor(Color.TRANSPARENT);
				mTxtName.setGravity(Gravity.CENTER);
				
				mParams = new WindowManager.LayoutParams(
					WindowManager.LayoutParams.MATCH_PARENT,
					WindowManager.LayoutParams.WRAP_CONTENT,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcName.left;
				mParams.y = mRcName.top;
		
				mWindowManager.addView(mTxtName, mParams);
			}
	
			if (mRcNumber != null)
			{
				mTxtNumber = new TextView(this);
				mTxtNumber.setTextSize(TypedValue.COMPLEX_UNIT_SP, 20.0f);	
				mTxtNumber.setTextColor(Color.argb(0xff, 0x81, 0xbe, 0xe1));					
				mTxtNumber.setBackgroundColor(Color.TRANSPARENT);
				mTxtNumber.setGravity(Gravity.CENTER);
				
				mParams = new WindowManager.LayoutParams(
					WindowManager.LayoutParams.MATCH_PARENT,
					WindowManager.LayoutParams.WRAP_CONTENT,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcNumber.left;
				mParams.y = mRcNumber.top;
		
				mWindowManager.addView(mTxtNumber, mParams);
			}
			
			if (mRcKeyPad != null)
			{
				mBtnPad = new Button(this);
				mBtnPad.setBackgroundColor(Color.TRANSPARENT);
				
				int delta = 10;
				mParams = new WindowManager.LayoutParams(
					mRcKeyPad.width() + delta * 2,
					mRcKeyPad.height() + delta * 2,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcKeyPad.left - delta;
				mParams.y = mRcKeyPad.top - delta;
		
				mWindowManager.addView(mBtnPad, mParams);
			}
			
			if (mRcSpeaker != null)
			{
				mBtnSpeaker = new Button(this);
				mBtnSpeaker.setBackgroundColor(Color.TRANSPARENT);
				
				int delta = 10;
				mParams = new WindowManager.LayoutParams(
					mRcSpeaker.width() + delta * 2,
					mRcSpeaker.height() + delta * 2,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcSpeaker.left - delta;
				mParams.y = mRcSpeaker.top - delta;
		
				mWindowManager.addView(mBtnSpeaker, mParams);
			}
			
			if (mRcAddCall != null)
			{
				mBtnAddCall = new Button(this);
				mBtnAddCall.setBackgroundColor(Color.TRANSPARENT);
				
				int delta = 10;
				mParams = new WindowManager.LayoutParams(
					mRcAddCall.width() + delta * 2,
					mRcAddCall.height() + delta * 2,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcAddCall.left - delta;
				mParams.y = mRcAddCall.top - delta;
		
				mWindowManager.addView(mBtnAddCall, mParams);
			}

			//
			mStatusbar = new LinearLayout(this);
			mStatusbar.setBackgroundColor(Color.TRANSPARENT);
			
			mParams = new WindowManager.LayoutParams(
				WindowManager.LayoutParams.MATCH_PARENT,
				MAX_STATUS_BAR_HEIGHT,
				WindowManager.LayoutParams.TYPE_SYSTEM_OVERLAY,
				WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN,
				PixelFormat.TRANSLUCENT);
			
			mParams.gravity = Gravity.LEFT | Gravity.TOP;
			mParams.x = mParams.y = 0;
	
			mWindowManager.addView(mStatusbar, mParams);

			//
			mScreenOff = new LinearLayout(this);
			mScreenOff.setBackgroundColor(Color.argb(255, 0, 0, 0));
			
			mParams = new WindowManager.LayoutParams(
				WindowManager.LayoutParams.MATCH_PARENT,
				WindowManager.LayoutParams.MATCH_PARENT,
				WindowManager.LayoutParams.TYPE_SYSTEM_OVERLAY,
				WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE|WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN,
				PixelFormat.TRANSLUCENT);
			
			mParams.gravity = Gravity.LEFT | Gravity.TOP;
			mParams.x = mParams.y = 0;
	
			mWindowManager.addView(mScreenOff, mParams);
			mScreenOff.setVisibility(View.GONE);
			//
	
			if ("".equals(VarSpace.mOriginNum) == false)
			{
				if ("".equals(VarSpace.mOriginName) == false)
				{
					mTxtName.setText(VarSpace.mOriginName);
					mTxtNumber.setText(VarSpace.mOriginNum);
				}
				else
				{
					mTxtName.setText(VarSpace.mOriginNum);
					mTxtNumber.setText("");
				}
			}
		}
		catch(Exception e)
		{
			
		}
		
		VarSpace.bTopSvcRun = true;
	}
	
	private void OnCreate_Galaxy412()
	{
		try{
			mWindowManager = (WindowManager) getSystemService(WINDOW_SERVICE);

			if (mRcRect != null)
			{
				mFillRect = new LinearLayout(this);
				mFillRect.setBackgroundColor(Color.TRANSPARENT);
				
				mParams = new WindowManager.LayoutParams(
					WindowManager.LayoutParams.MATCH_PARENT,
					mRcRect.height(),
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcRect.left;
				mParams.y = mRcRect.top;
		
				mWindowManager.addView(mFillRect, mParams);
			}

			if (mRcName != null)
			{
				//
				mTextBk1 = new LinearLayout(this);
				mTextBk1.setBackgroundColor(Color.argb(255, 0, 0, 0));
				
				mParams = new WindowManager.LayoutParams(
					WindowManager.LayoutParams.MATCH_PARENT,
					mRcName.height(),
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = 0;
				mParams.y = mRcName.top;
		
				mWindowManager.addView(mTextBk1, mParams);

				//
				mTxtName = new TextView(this);
				mTxtName.setTextSize(TypedValue.COMPLEX_UNIT_DIP, 29.0f);	
				mTxtName.setTextColor(Color.argb(0xff, 0xff, 0xff, 0xff));					
				mTxtName.setBackgroundColor(Color.TRANSPARENT);
				mTxtName.setGravity(Gravity.LEFT);
				
				mParams = new WindowManager.LayoutParams(
					WindowManager.LayoutParams.MATCH_PARENT,
					WindowManager.LayoutParams.WRAP_CONTENT,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcName.left;
				mParams.y = mRcName.top;
		
				mWindowManager.addView(mTxtName, mParams);
			}
	
			if (mRcNumber != null)
			{
				//
				mTextBk2 = new LinearLayout(this);
				mTextBk2.setBackgroundColor(Color.argb(255, 0, 0, 0));
				
				mParams = new WindowManager.LayoutParams(
					WindowManager.LayoutParams.MATCH_PARENT,
					mRcNumber.height(),
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = 0;
				mParams.y = mRcNumber.top;

				mWindowManager.addView(mTextBk2, mParams);

				//
				mTxtNumber = new TextView(this);
				mTxtNumber.setTextSize(TypedValue.COMPLEX_UNIT_DIP, 18.0f);	
				mTxtNumber.setTextColor(Color.argb(0xff, 0xdc, 0xdc, 0xdc));	
				mTxtNumber.setBackgroundColor(Color.TRANSPARENT);
				mTxtNumber.setGravity(Gravity.LEFT);
				
				mParams = new WindowManager.LayoutParams(
					WindowManager.LayoutParams.MATCH_PARENT,
					WindowManager.LayoutParams.WRAP_CONTENT,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcNumber.left;
				mParams.y = mRcNumber.top;
		
				mWindowManager.addView(mTxtNumber, mParams);
			}
			
			if (mRcKeyPad != null)
			{
				mBtnPad = new Button(this);
				mBtnPad.setBackgroundColor(Color.TRANSPARENT);
				
				int delta = 10;
				mParams = new WindowManager.LayoutParams(
					mRcKeyPad.width() + delta * 2,
					mRcKeyPad.height() + delta * 2,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcKeyPad.left - delta;
				mParams.y = mRcKeyPad.top - delta;
		
				mWindowManager.addView(mBtnPad, mParams);
			}
			
			if (mRcSpeaker != null)
			{
				mBtnSpeaker = new Button(this);
				mBtnSpeaker.setBackgroundColor(Color.TRANSPARENT);
				
				int delta = 10;
				mParams = new WindowManager.LayoutParams(
					mRcSpeaker.width() + delta * 2,
					mRcSpeaker.height() + delta * 2,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcSpeaker.left - delta;
				mParams.y = mRcSpeaker.top - delta;
		
				mWindowManager.addView(mBtnSpeaker, mParams);
			}
			
			if (mRcAddCall != null)
			{
				mBtnAddCall = new Button(this);
				mBtnAddCall.setBackgroundColor(Color.TRANSPARENT);
				
				int delta = 10;
				mParams = new WindowManager.LayoutParams(
					mRcAddCall.width() + delta * 2,
					mRcAddCall.height() + delta * 2,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcAddCall.left - delta;
				mParams.y = mRcAddCall.top - delta;
		
				mWindowManager.addView(mBtnAddCall, mParams);
			}

			
			//
			mStatusbar = new LinearLayout(this);
			mStatusbar.setBackgroundColor(Color.TRANSPARENT);
			
			mParams = new WindowManager.LayoutParams(
				WindowManager.LayoutParams.MATCH_PARENT,
				MAX_STATUS_BAR_HEIGHT,
				WindowManager.LayoutParams.TYPE_SYSTEM_OVERLAY,
				WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN,
				PixelFormat.TRANSLUCENT);
			
			mParams.gravity = Gravity.LEFT | Gravity.TOP;
			mParams.x = mParams.y = 0;
	
			mWindowManager.addView(mStatusbar, mParams);
			
			//
			mScreenOff = new LinearLayout(this);
			mScreenOff.setBackgroundColor(Color.argb(255, 0, 0, 0));
			
			mParams = new WindowManager.LayoutParams(
				WindowManager.LayoutParams.MATCH_PARENT,
				WindowManager.LayoutParams.MATCH_PARENT,
				WindowManager.LayoutParams.TYPE_SYSTEM_OVERLAY,
				WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE|WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN,
				PixelFormat.TRANSLUCENT);
			
			mParams.gravity = Gravity.LEFT | Gravity.TOP;
			mParams.x = mParams.y = 0;
	
			mWindowManager.addView(mScreenOff, mParams);
			mScreenOff.setVisibility(View.GONE);
			//

			if ("".equals(VarSpace.mOriginNum) == false)
			{
				if ("".equals(VarSpace.mOriginName) == false)
				{
					mTxtName.setText(VarSpace.mOriginName);
					mTxtNumber.setText(VarSpace.mOriginNum);
				}
				else
				{
					mTxtName.setText(VarSpace.mOriginNum);
					mTxtNumber.setText("");
				}
			}
		}
		catch(Exception e)
		{
			
		}		

		VarSpace.bTopSvcRun = true;
	}
	
	private void OnCreate_Galaxy421()
	{
		try{
			mWindowManager = (WindowManager) getSystemService(WINDOW_SERVICE);

			if (mRcRect != null)
			{
				mFillRect = new LinearLayout(this);
//				mFillRect.setBackgroundColor(Color.TRANSPARENT);
				mFillRect.setBackgroundColor(Color.argb(255, 0, 0, 0));
				
				mParams = new WindowManager.LayoutParams(
					WindowManager.LayoutParams.MATCH_PARENT,
					mRcRect.height(),
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcRect.left;
				mParams.y = mRcRect.top;
		
				mWindowManager.addView(mFillRect, mParams);
			}

			if (mRcName != null)
			{
				//
				mTextBk1 = new LinearLayout(this);
				mTextBk1.setBackgroundColor(Color.argb(255, 15, 15, 15));
				
				mParams = new WindowManager.LayoutParams(
					WindowManager.LayoutParams.MATCH_PARENT,
					mRcName.height(),
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = 0;
				mParams.y = mRcName.top;
		
				mWindowManager.addView(mTextBk1, mParams);

				//
				mTxtName = new TextView(this);
				mTxtName.setTextSize(TypedValue.COMPLEX_UNIT_DIP, 29.0f);	
				mTxtName.setTextColor(Color.argb(0xff, 0xff, 0xff, 0xff));					
				mTxtName.setBackgroundColor(Color.TRANSPARENT);
				mTxtName.setGravity(Gravity.LEFT);
				
				mParams = new WindowManager.LayoutParams(
					WindowManager.LayoutParams.MATCH_PARENT,
					WindowManager.LayoutParams.WRAP_CONTENT,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcName.left;
				mParams.y = mRcName.top;
		
				mWindowManager.addView(mTxtName, mParams);
			}
	
			if (mRcNumber != null)
			{
				//
				mTextBk2 = new LinearLayout(this);
				mTextBk2.setBackgroundColor(Color.argb(255, 15, 15, 15));
				
				mParams = new WindowManager.LayoutParams(
					WindowManager.LayoutParams.MATCH_PARENT,
					mRcNumber.height(),
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = 0;
				mParams.y = mRcNumber.top;

				mWindowManager.addView(mTextBk2, mParams);

				//
				mTxtNumber = new TextView(this);
				mTxtNumber.setTextSize(TypedValue.COMPLEX_UNIT_DIP, 18.0f);	
				mTxtNumber.setTextColor(Color.argb(0xff, 0xdc, 0xdc, 0xdc));	
				mTxtNumber.setBackgroundColor(Color.TRANSPARENT);
				mTxtNumber.setGravity(Gravity.LEFT);
				
				mParams = new WindowManager.LayoutParams(
					WindowManager.LayoutParams.MATCH_PARENT,
					WindowManager.LayoutParams.WRAP_CONTENT,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcNumber.left;
				mParams.y = mRcNumber.top;
		
				mWindowManager.addView(mTxtNumber, mParams);
			}
			
			if (mRcKeyPad != null)
			{
				mBtnPad = new Button(this);
				mBtnPad.setBackgroundColor(Color.TRANSPARENT);
				
				int delta = 10;
				mParams = new WindowManager.LayoutParams(
					mRcKeyPad.width() + delta * 2,
					mRcKeyPad.height() + delta * 2,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcKeyPad.left - delta;
				mParams.y = mRcKeyPad.top - delta;
		
				mWindowManager.addView(mBtnPad, mParams);
			}
			
			if (mRcSpeaker != null)
			{
				mBtnSpeaker = new Button(this);
				mBtnSpeaker.setBackgroundColor(Color.TRANSPARENT);
				
				int delta = 10;
				mParams = new WindowManager.LayoutParams(
					mRcSpeaker.width() + delta * 2,
					mRcSpeaker.height() + delta * 2,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcSpeaker.left - delta;
				mParams.y = mRcSpeaker.top - delta;
		
				mWindowManager.addView(mBtnSpeaker, mParams);
			}
			
			if (mRcAddCall != null)
			{
				mBtnAddCall = new Button(this);
				mBtnAddCall.setBackgroundColor(Color.TRANSPARENT);
				
				int delta = 10;
				mParams = new WindowManager.LayoutParams(
					mRcAddCall.width() + delta * 2,
					mRcAddCall.height() + delta * 2,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcAddCall.left - delta;
				mParams.y = mRcAddCall.top - delta;
		
				mWindowManager.addView(mBtnAddCall, mParams);
			}

			
			//
			mStatusbar = new LinearLayout(this);
			mStatusbar.setBackgroundColor(Color.TRANSPARENT);
			
			mParams = new WindowManager.LayoutParams(
				WindowManager.LayoutParams.MATCH_PARENT,
				MAX_STATUS_BAR_HEIGHT,
				WindowManager.LayoutParams.TYPE_SYSTEM_OVERLAY,
				WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN,
				PixelFormat.TRANSLUCENT);
			
			mParams.gravity = Gravity.LEFT | Gravity.TOP;
			mParams.x = mParams.y = 0;
	
			mWindowManager.addView(mStatusbar, mParams);
			
			//
			mScreenOff = new LinearLayout(this);
			mScreenOff.setBackgroundColor(Color.argb(255, 0, 0, 0));
			
			mParams = new WindowManager.LayoutParams(
				WindowManager.LayoutParams.MATCH_PARENT,
				WindowManager.LayoutParams.MATCH_PARENT,
				WindowManager.LayoutParams.TYPE_SYSTEM_OVERLAY,
				WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE|WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN,
				PixelFormat.TRANSLUCENT);
			
			mParams.gravity = Gravity.LEFT | Gravity.TOP;
			mParams.x = mParams.y = 0;
	
			mWindowManager.addView(mScreenOff, mParams);
			mScreenOff.setVisibility(View.GONE);
			//

			if ("".equals(VarSpace.mOriginNum) == false)
			{
				if ("".equals(VarSpace.mOriginName) == false)
				{
					mTxtName.setText(VarSpace.mOriginName);
					mTxtNumber.setText(VarSpace.mOriginNum);
				}
				else
				{
					mTxtName.setText(VarSpace.mOriginNum);
					mTxtNumber.setText("");
				}
			}
		}
		catch(Exception e)
		{
			
		}		

		VarSpace.bTopSvcRun = true;
	}
	
	private void OnCreate_Galaxy500()
	{
		try{
			mWindowManager = (WindowManager) getSystemService(WINDOW_SERVICE);

			if (mRcRect != null)
			{
				mFillRect = new LinearLayout(this);
//				mFillRect.setBackgroundColor(Color.TRANSPARENT);
				mFillRect.setBackgroundColor(Color.argb(255, 0, 200, 0));
				
				mParams = new WindowManager.LayoutParams(
					WindowManager.LayoutParams.MATCH_PARENT,
					(int)(mRcRect.height() * 1.9),
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcRect.left;
				mParams.y = mRcRect.top;
		
				mWindowManager.addView(mFillRect, mParams);
			}

			if (mRcName != null)
			{
				//
/*				
				mTextBk1 = new LinearLayout(this);
				mTextBk1.setBackgroundColor(Color.argb(255, 0, 200, 0));
				
				mParams = new WindowManager.LayoutParams(
					WindowManager.LayoutParams.MATCH_PARENT,
					mRcName.height(),
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = 0;
				mParams.y = mRcName.top;
		
				mWindowManager.addView(mTextBk1, mParams);
*/				
				//
				mTxtName = new TextView(this);
				mTxtName.setTextSize(TypedValue.COMPLEX_UNIT_DIP, 40.0f);	
				mTxtName.setTextColor(Color.argb(0xff, 0xff, 0xff, 0xff));					
				mTxtName.setBackgroundColor(Color.TRANSPARENT);
				mTxtName.setGravity(Gravity.CENTER);
				
				mParams = new WindowManager.LayoutParams(
					WindowManager.LayoutParams.MATCH_PARENT,
					WindowManager.LayoutParams.WRAP_CONTENT,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = 0;
				mParams.y = mRcRect.height() / 2;
		
				mWindowManager.addView(mTxtName, mParams);
			}
	
			if (mRcNumber != null)
			{
				//
/*				
				mTextBk2 = new LinearLayout(this);
				mTextBk2.setBackgroundColor(Color.argb(255, 0, 200, 0));
				
				mParams = new WindowManager.LayoutParams(
					WindowManager.LayoutParams.MATCH_PARENT,
					mRcNumber.height(),
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = 0;
				mParams.y = mRcNumber.top;

				mWindowManager.addView(mTextBk2, mParams);
*/
				//
				mTxtNumber = new TextView(this);
				mTxtNumber.setTextSize(TypedValue.COMPLEX_UNIT_DIP, 18.0f);	
				mTxtNumber.setTextColor(Color.argb(0xff, 0xdc, 0xdc, 0xdc));	
				mTxtNumber.setBackgroundColor(Color.TRANSPARENT);
				mTxtNumber.setGravity(Gravity.LEFT);
				
				mParams = new WindowManager.LayoutParams(
					WindowManager.LayoutParams.MATCH_PARENT,
					WindowManager.LayoutParams.WRAP_CONTENT,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcNumber.left;
				mParams.y = mRcNumber.top;
		
				mWindowManager.addView(mTxtNumber, mParams);
			}
			
			//
			mStatusbar = new LinearLayout(this);
			mStatusbar.setBackgroundColor(Color.TRANSPARENT);
			
			mParams = new WindowManager.LayoutParams(
				WindowManager.LayoutParams.MATCH_PARENT,
				MAX_STATUS_BAR_HEIGHT,
				WindowManager.LayoutParams.TYPE_SYSTEM_OVERLAY,
				WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN,
				PixelFormat.TRANSLUCENT);
			
			mParams.gravity = Gravity.LEFT | Gravity.TOP;
			mParams.x = mParams.y = 0;
	
			mWindowManager.addView(mStatusbar, mParams);
			
			//
			mScreenOff = new LinearLayout(this);
			mScreenOff.setBackgroundColor(Color.argb(255, 0, 0, 0));
			
			mParams = new WindowManager.LayoutParams(
				WindowManager.LayoutParams.MATCH_PARENT,
				WindowManager.LayoutParams.MATCH_PARENT,
				WindowManager.LayoutParams.TYPE_SYSTEM_OVERLAY,
				WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE|WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN,
				PixelFormat.TRANSLUCENT);
			
			mParams.gravity = Gravity.LEFT | Gravity.TOP;
			mParams.x = mParams.y = 0;
	
			mWindowManager.addView(mScreenOff, mParams);
			mScreenOff.setVisibility(View.GONE);
			//

			if ("".equals(VarSpace.mOriginNum) == false)
			{
				if ("".equals(VarSpace.mOriginName) == false)
				{
					mTxtName.setText(VarSpace.mOriginName);
					mTxtNumber.setText(VarSpace.mOriginNum);
				}
				else
				{
					mTxtName.setText(VarSpace.mOriginNum);
					mTxtNumber.setText("");
				}
			}
		}
		catch(Exception e)
		{
			
		}		

		VarSpace.bTopSvcRun = true;
	}	
	
	private void OnCreate_Optimus234()
	{
		try{
			mWindowManager = (WindowManager) getSystemService(WINDOW_SERVICE);

			if (mRcRect != null)
			{
				mFillRect = new LinearLayout(this);
				
				mParams = new WindowManager.LayoutParams(
					WindowManager.LayoutParams.MATCH_PARENT,
					mRcRect.height(),
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcRect.left;
				mParams.y = mRcRect.top;
		
				mWindowManager.addView(mFillRect, mParams);
				
				SetOverlayBitmap(this, mFillRect, R.drawable.optmusbg_green, mRcRect);
			}
			
			if (mRcName != null)
			{
				mTxtName = new TextView(this);
				mTxtName.setTextSize(TypedValue.COMPLEX_UNIT_SP, 28.0f);	
				mTxtName.setTextColor(Color.argb(0xff, 0xff, 0xff, 0xff));					
				mTxtName.setBackgroundColor(Color.TRANSPARENT);
				mTxtName.setGravity(Gravity.CENTER);
				
				mParams = new WindowManager.LayoutParams(
					WindowManager.LayoutParams.MATCH_PARENT,
					WindowManager.LayoutParams.WRAP_CONTENT,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcName.left;
				mParams.y = mRcName.top;
		
				mWindowManager.addView(mTxtName, mParams);
			}
	
			if (mRcNumber != null)
			{
				mTxtNumber = new TextView(this);
				mTxtNumber.setTextSize(TypedValue.COMPLEX_UNIT_SP, 20.0f);	
				mTxtNumber.setTextColor(Color.argb(0xff, 0x81, 0xbe, 0xe1));					
				mTxtNumber.setBackgroundColor(Color.TRANSPARENT);
				mTxtNumber.setGravity(Gravity.CENTER);
				
				mParams = new WindowManager.LayoutParams(
					WindowManager.LayoutParams.MATCH_PARENT,
					WindowManager.LayoutParams.WRAP_CONTENT,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcNumber.left;
				mParams.y = mRcNumber.top;
		
				mWindowManager.addView(mTxtNumber, mParams);
			}
			
			if (mRcKeyPad != null)
			{
				mBtnPad = new Button(this);
				mBtnPad.setBackgroundColor(Color.TRANSPARENT);
				
				int delta = 10;
				mParams = new WindowManager.LayoutParams(
					mRcKeyPad.width() + delta * 2,
					mRcKeyPad.height() + delta * 2,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcKeyPad.left - delta;
				mParams.y = mRcKeyPad.top - delta;
		
				mWindowManager.addView(mBtnPad, mParams);
			}
			
			if (mRcSpeaker != null)
			{
				mBtnSpeaker = new Button(this);
				mBtnSpeaker.setBackgroundColor(Color.TRANSPARENT);
				
				int delta = 10;
				mParams = new WindowManager.LayoutParams(
					mRcSpeaker.width() + delta * 2,
					mRcSpeaker.height() + delta * 2,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcSpeaker.left - delta;
				mParams.y = mRcSpeaker.top - delta;
		
				mWindowManager.addView(mBtnSpeaker, mParams);
			}
			
			if (mRcAddCall != null)
			{
				mBtnAddCall = new Button(this);
				mBtnAddCall.setBackgroundColor(Color.TRANSPARENT);
				
				int delta = 10;
				mParams = new WindowManager.LayoutParams(
					mRcAddCall.width() + delta * 2,
					mRcAddCall.height() + delta * 2,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcAddCall.left - delta;
				mParams.y = mRcAddCall.top - delta;
		
				mWindowManager.addView(mBtnAddCall, mParams);
			}

			//
			mStatusbar = new LinearLayout(this);
			mStatusbar.setBackgroundColor(Color.TRANSPARENT);
			
			mParams = new WindowManager.LayoutParams(
				WindowManager.LayoutParams.MATCH_PARENT,
				MAX_STATUS_BAR_HEIGHT,
				WindowManager.LayoutParams.TYPE_SYSTEM_OVERLAY,
				WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN,
				PixelFormat.TRANSLUCENT);
			
			mParams.gravity = Gravity.LEFT | Gravity.TOP;
			mParams.x = mParams.y = 0;
	
			mWindowManager.addView(mStatusbar, mParams);

			//
			mScreenOff = new LinearLayout(this);
			mScreenOff.setBackgroundColor(Color.argb(255, 0, 0, 0));
			
			mParams = new WindowManager.LayoutParams(
				WindowManager.LayoutParams.MATCH_PARENT,
				WindowManager.LayoutParams.MATCH_PARENT,
				WindowManager.LayoutParams.TYPE_SYSTEM_OVERLAY,
				WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE|WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN,
				PixelFormat.TRANSLUCENT);
			
			mParams.gravity = Gravity.LEFT | Gravity.TOP;
			mParams.x = mParams.y = 0;
	
			mWindowManager.addView(mScreenOff, mParams);
			mScreenOff.setVisibility(View.GONE);
			//
	
			if ("".equals(VarSpace.mOriginNum) == false)
			{
				if ("".equals(VarSpace.mOriginName) == false)
				{
					mTxtName.setText(VarSpace.mOriginName);
					mTxtNumber.setText(VarSpace.mOriginNum);
				}
				else
				{
					mTxtName.setText(VarSpace.mOriginNum);
					mTxtNumber.setText("");
				}
			}
		}
		catch(Exception e)
		{
			
		}
		
		VarSpace.bTopSvcRun = true;
	}
	
	private void OnCreate_Optimus412()
	{
		try{
			mWindowManager = (WindowManager) getSystemService(WINDOW_SERVICE);

			if (mRcRect != null)
			{
				mFillRect = new LinearLayout(this);
				mFillRect.setBackgroundColor(Color.GRAY);
				mParams = new WindowManager.LayoutParams(
					WindowManager.LayoutParams.MATCH_PARENT,
					mRcRect.height(),
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcRect.left;
				mParams.y = mRcRect.top;
		
				mWindowManager.addView(mFillRect, mParams);
			}

			if (mRcName != null)
			{
				//
				mTxtName = new TextView(this);
				mTxtName.setTextSize(TypedValue.COMPLEX_UNIT_DIP, 29.0f);	
				mTxtName.setTextColor(Color.argb(0xff, 0xff, 0xff, 0xff));					
				mTxtName.setBackgroundColor(Color.TRANSPARENT);
				mTxtName.setGravity(Gravity.LEFT);
				
				mParams = new WindowManager.LayoutParams(
					WindowManager.LayoutParams.MATCH_PARENT,
					WindowManager.LayoutParams.WRAP_CONTENT,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcName.left;
				mParams.y = mRcName.top;
		
				mWindowManager.addView(mTxtName, mParams);
			}
	
			if (mRcNumber != null)
			{
				//
				mTxtNumber = new TextView(this);
				mTxtNumber.setTextSize(TypedValue.COMPLEX_UNIT_DIP, 18.0f);	
				mTxtNumber.setTextColor(Color.argb(0xff, 0xdc, 0xdc, 0xdc));	
				mTxtNumber.setBackgroundColor(Color.TRANSPARENT);
				mTxtNumber.setGravity(Gravity.LEFT);
				
				mParams = new WindowManager.LayoutParams(
					WindowManager.LayoutParams.MATCH_PARENT,
					WindowManager.LayoutParams.WRAP_CONTENT,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcNumber.left;
				mParams.y = mRcNumber.top;
		
				mWindowManager.addView(mTxtNumber, mParams);
			}
			
			if (mRcKeyPad != null)
			{
				mBtnPad = new Button(this);
				mBtnPad.setBackgroundColor(Color.TRANSPARENT);
				
				int delta = 10;
				mParams = new WindowManager.LayoutParams(
					mRcKeyPad.width() + delta * 2,
					mRcKeyPad.height() + delta * 2,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcKeyPad.left - delta;
				mParams.y = mRcKeyPad.top - delta;
		
				mWindowManager.addView(mBtnPad, mParams);
			}
			
			if (mRcSpeaker != null)
			{
				mBtnSpeaker = new Button(this);
				mBtnSpeaker.setBackgroundColor(Color.TRANSPARENT);
				
				int delta = 10;
				mParams = new WindowManager.LayoutParams(
					mRcSpeaker.width() + delta * 2,
					mRcSpeaker.height() + delta * 2,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcSpeaker.left - delta;
				mParams.y = mRcSpeaker.top - delta;
		
				mWindowManager.addView(mBtnSpeaker, mParams);
			}
			
			if (mRcAddCall != null)
			{
				mBtnAddCall = new Button(this);
				mBtnAddCall.setBackgroundColor(Color.TRANSPARENT);
				
				int delta = 10;
				mParams = new WindowManager.LayoutParams(
					mRcAddCall.width() + delta * 2,
					mRcAddCall.height() + delta * 2,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcAddCall.left - delta;
				mParams.y = mRcAddCall.top - delta;
		
				mWindowManager.addView(mBtnAddCall, mParams);
			}

			
			//
			mStatusbar = new LinearLayout(this);
			mStatusbar.setBackgroundColor(Color.TRANSPARENT);
			
			mParams = new WindowManager.LayoutParams(
				WindowManager.LayoutParams.MATCH_PARENT,
				MAX_STATUS_BAR_HEIGHT,
				WindowManager.LayoutParams.TYPE_SYSTEM_OVERLAY,
				WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN,
				PixelFormat.TRANSLUCENT);
			
			mParams.gravity = Gravity.LEFT | Gravity.TOP;
			mParams.x = mParams.y = 0;
	
			mWindowManager.addView(mStatusbar, mParams);
			
			//
			mScreenOff = new LinearLayout(this);
			mScreenOff.setBackgroundColor(Color.argb(255, 0, 0, 0));
			
			mParams = new WindowManager.LayoutParams(
				WindowManager.LayoutParams.MATCH_PARENT,
				WindowManager.LayoutParams.MATCH_PARENT,
				WindowManager.LayoutParams.TYPE_SYSTEM_OVERLAY,
				WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE|WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN,
				PixelFormat.TRANSLUCENT);
			
			mParams.gravity = Gravity.LEFT | Gravity.TOP;
			mParams.x = mParams.y = 0;
	
			mWindowManager.addView(mScreenOff, mParams);
			mScreenOff.setVisibility(View.GONE);
			//

			if ("".equals(VarSpace.mOriginNum) == false)
			{
				if ("".equals(VarSpace.mOriginName) == false)
				{
					mTxtName.setText(VarSpace.mOriginName);
					mTxtNumber.setText(VarSpace.mOriginNum);
				}
				else
				{
					mTxtName.setText(VarSpace.mOriginNum);
					mTxtNumber.setText("");
				}
			}
		}
		catch(Exception e)
		{
			
		}		

		VarSpace.bTopSvcRun = true;
	}
	
	private void OnCreate_Sky412()
	{
		try{
			mWindowManager = (WindowManager) getSystemService(WINDOW_SERVICE);

			//
			int fillRectX = 0;
			int fillRectY = 0;
			int fillRectH = 0;
			if ("".equals(VarSpace.mOriginNum) == false)
			{
				if ("".equals(VarSpace.mOriginName) == false)
				{
					fillRectX = mRcName.left;
					fillRectY = mRcName.top;
					fillRectH = mRcName.height() + mRcNumber.height();
				}
				else
				{
					fillRectX = mRcNumber.left;
					fillRectY = mRcNumber.top;
					fillRectH = mRcNumber.height();
				}
			}
			
			if (fillRectH != 0)
			{
				mFillRect = new LinearLayout(this);
				mFillRect.setBackgroundColor(Color.argb(255, 130, 180, 200));
				mParams = new WindowManager.LayoutParams(
					WindowManager.LayoutParams.MATCH_PARENT,
					fillRectH,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = fillRectX;
				mParams.y = fillRectY;
		
				mWindowManager.addView(mFillRect, mParams);
			}
			
			if (mRcName != null)
			{
				//
				mTxtName = new TextView(this);
				mTxtName.setTextSize(TypedValue.COMPLEX_UNIT_SP, 30.0f);	
				mTxtName.setTextColor(Color.argb(0xff, 0x00, 0x00, 0x00));	
				mTxtName.setBackgroundColor(Color.TRANSPARENT);
				mTxtName.setGravity(Gravity.CENTER);
				
				mParams = new WindowManager.LayoutParams(
					WindowManager.LayoutParams.MATCH_PARENT,
					WindowManager.LayoutParams.WRAP_CONTENT,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcName.left;
				mParams.y = mRcName.top;
		
				mWindowManager.addView(mTxtName, mParams);
			}
	
			if (mRcNumber != null)
			{
				//
				mTxtNumber = new TextView(this);
				mTxtNumber.setTextSize(TypedValue.COMPLEX_UNIT_SP, 30.0f);
				mTxtNumber.setTextColor(Color.argb(0xff, 0x00, 0x00, 0x00));	
				mTxtNumber.setBackgroundColor(Color.TRANSPARENT);
				mTxtNumber.setGravity(Gravity.CENTER);
				
				mParams = new WindowManager.LayoutParams(
					WindowManager.LayoutParams.MATCH_PARENT,
					WindowManager.LayoutParams.WRAP_CONTENT,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcNumber.left;
				mParams.y = mRcNumber.top;
		
				mWindowManager.addView(mTxtNumber, mParams);
			}
			
			if (mRcKeyPad != null)
			{
				mBtnPad = new Button(this);
				mBtnPad.setBackgroundColor(Color.TRANSPARENT);
				
				int delta = 10;
				mParams = new WindowManager.LayoutParams(
					mRcKeyPad.width() + delta * 2,
					mRcKeyPad.height() + delta * 2,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcKeyPad.left - delta;
				mParams.y = mRcKeyPad.top - delta;
		
				mWindowManager.addView(mBtnPad, mParams);
			}
			
			if (mRcAddCall != null)
			{
				mBtnAddCall = new Button(this);
				mBtnAddCall.setBackgroundColor(Color.TRANSPARENT);
				
				int delta = 10;
				mParams = new WindowManager.LayoutParams(
					mRcAddCall.width() + delta * 2,
					mRcAddCall.height() + delta * 2,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcAddCall.left - delta;
				mParams.y = mRcAddCall.top - delta;
		
				mWindowManager.addView(mBtnAddCall, mParams);
			}

			
			//
			mStatusbar = new LinearLayout(this);
			mStatusbar.setBackgroundColor(Color.TRANSPARENT);
			
			mParams = new WindowManager.LayoutParams(
				WindowManager.LayoutParams.MATCH_PARENT,
				MAX_STATUS_BAR_HEIGHT,
				WindowManager.LayoutParams.TYPE_SYSTEM_OVERLAY,
				WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN,
				PixelFormat.TRANSLUCENT);
			
			mParams.gravity = Gravity.LEFT | Gravity.TOP;
			mParams.x = mParams.y = 0;
	
			mWindowManager.addView(mStatusbar, mParams);
			
			//
			mScreenOff = new LinearLayout(this);
			mScreenOff.setBackgroundColor(Color.argb(255, 0, 0, 0));
			
			mParams = new WindowManager.LayoutParams(
				WindowManager.LayoutParams.MATCH_PARENT,
				WindowManager.LayoutParams.MATCH_PARENT,
				WindowManager.LayoutParams.TYPE_SYSTEM_OVERLAY,
				WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE|WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN,
				PixelFormat.TRANSLUCENT);
			
			mParams.gravity = Gravity.LEFT | Gravity.TOP;
			mParams.x = mParams.y = 0;
	
			mWindowManager.addView(mScreenOff, mParams);
			mScreenOff.setVisibility(View.GONE);
			//

			if ("".equals(VarSpace.mOriginNum) == false)
			{
				if ("".equals(VarSpace.mOriginName) == false)
				{
					mTxtName.setText(VarSpace.mOriginName);
					mTxtNumber.setText(VarSpace.mOriginNum);
				}
				else
				{
					mTxtName.setText("");
					mTxtNumber.setText(VarSpace.mOriginNum);
				}
			}
		}
		catch(Exception e)
		{
			
		}		

		VarSpace.bTopSvcRun = true;
	}
	
	private void OnCreate_Sky442()
	{
		try{
			mWindowManager = (WindowManager) getSystemService(WINDOW_SERVICE);

			if (mRcRect != null)
			{
				mFillRect = new LinearLayout(this);
//				mFillRect.setBackgroundColor(Color.TRANSPARENT);
				mFillRect.setBackgroundColor(Color.argb(255, 15, 15, 15));
				
				mParams = new WindowManager.LayoutParams(
					WindowManager.LayoutParams.MATCH_PARENT,
					mRcRect.height(),
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcRect.left;
				mParams.y = mRcRect.top;
		
				mWindowManager.addView(mFillRect, mParams);
			}

			if (mRcName != null)
			{		
				//
				mTxtName = new TextView(this);
				mTxtName.setTextSize(TypedValue.COMPLEX_UNIT_DIP, 29.0f);	
				mTxtName.setTextColor(Color.argb(0xff, 0xff, 0xff, 0xff));					
				mTxtName.setBackgroundColor(Color.TRANSPARENT);
				mTxtName.setGravity(Gravity.LEFT);
				
				mParams = new WindowManager.LayoutParams(
					WindowManager.LayoutParams.MATCH_PARENT,
					WindowManager.LayoutParams.WRAP_CONTENT,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcName.left;
				mParams.y = mRcName.top;
		
				mWindowManager.addView(mTxtName, mParams);
			}
	
			if (mRcNumber != null)
			{
				//
				mTxtNumber = new TextView(this);
				mTxtNumber.setTextSize(TypedValue.COMPLEX_UNIT_DIP, 18.0f);	
				mTxtNumber.setTextColor(Color.argb(0xff, 0xdc, 0xdc, 0xdc));	
				mTxtNumber.setBackgroundColor(Color.TRANSPARENT);
				mTxtNumber.setGravity(Gravity.LEFT);
				
				mParams = new WindowManager.LayoutParams(
					WindowManager.LayoutParams.MATCH_PARENT,
					WindowManager.LayoutParams.WRAP_CONTENT,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcNumber.left;
				mParams.y = mRcNumber.top;
		
				mWindowManager.addView(mTxtNumber, mParams);
			}
			
			if (mRcKeyPad != null)
			{
				mBtnPad = new Button(this);
				mBtnPad.setBackgroundColor(Color.TRANSPARENT);
				
				int delta = 10;
				mParams = new WindowManager.LayoutParams(
					mRcKeyPad.width() + delta * 2,
					mRcKeyPad.height() + delta * 2,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcKeyPad.left - delta;
				mParams.y = mRcKeyPad.top - delta;
		
				mWindowManager.addView(mBtnPad, mParams);
			}
			
			if (mRcSpeaker != null)
			{
				mBtnSpeaker = new Button(this);
				mBtnSpeaker.setBackgroundColor(Color.TRANSPARENT);
				
				int delta = 10;
				mParams = new WindowManager.LayoutParams(
					mRcSpeaker.width() + delta * 2,
					mRcSpeaker.height() + delta * 2,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcSpeaker.left - delta;
				mParams.y = mRcSpeaker.top - delta;
		
				mWindowManager.addView(mBtnSpeaker, mParams);
			}
			
			if (mRcAddCall != null)
			{
				mBtnAddCall = new Button(this);
				mBtnAddCall.setBackgroundColor(Color.TRANSPARENT);
				
				int delta = 10;
				mParams = new WindowManager.LayoutParams(
					mRcAddCall.width() + delta * 2,
					mRcAddCall.height() + delta * 2,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcAddCall.left - delta;
				mParams.y = mRcAddCall.top - delta;
		
				mWindowManager.addView(mBtnAddCall, mParams);
			}

			
			//
			mStatusbar = new LinearLayout(this);
			mStatusbar.setBackgroundColor(Color.TRANSPARENT);
			
			mParams = new WindowManager.LayoutParams(
				WindowManager.LayoutParams.MATCH_PARENT,
				MAX_STATUS_BAR_HEIGHT,
				WindowManager.LayoutParams.TYPE_SYSTEM_OVERLAY,
				WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN,
				PixelFormat.TRANSLUCENT);
			
			mParams.gravity = Gravity.LEFT | Gravity.TOP;
			mParams.x = mParams.y = 0;
	
			mWindowManager.addView(mStatusbar, mParams);
			
			//
			mScreenOff = new LinearLayout(this);
			mScreenOff.setBackgroundColor(Color.argb(255, 0, 0, 0));
			
			mParams = new WindowManager.LayoutParams(
				WindowManager.LayoutParams.MATCH_PARENT,
				WindowManager.LayoutParams.MATCH_PARENT,
				WindowManager.LayoutParams.TYPE_SYSTEM_OVERLAY,
				WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE|WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN,
				PixelFormat.TRANSLUCENT);
			
			mParams.gravity = Gravity.LEFT | Gravity.TOP;
			mParams.x = mParams.y = 0;
	
			mWindowManager.addView(mScreenOff, mParams);
			mScreenOff.setVisibility(View.GONE);
			//

			if ("".equals(VarSpace.mOriginNum) == false)
			{
				if ("".equals(VarSpace.mOriginName) == false)
				{
					mTxtName.setText(VarSpace.mOriginName);
					mTxtNumber.setText(VarSpace.mOriginNum);
				}
				else
				{
					mTxtName.setText(VarSpace.mOriginNum);
					mTxtNumber.setText("");
				}
			}
		}
		catch(Exception e)
		{
			
		}		

		VarSpace.bTopSvcRun = true;
	}	
	
	private void OnCreate_Optimus500()
	{
		try{
			mWindowManager = (WindowManager) getSystemService(WINDOW_SERVICE);

			if (mRcRect != null)
			{
				mFillRect = new LinearLayout(this);
//				mFillRect.setBackgroundColor(Color.TRANSPARENT);
				mFillRect.setBackgroundColor(Color.argb(255, 64, 140, 140));
				
				mParams = new WindowManager.LayoutParams(
					WindowManager.LayoutParams.MATCH_PARENT,
					(int)(mRcRect.height() * 1.4),
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcRect.left;
				mParams.y = mRcRect.top;
		
				mWindowManager.addView(mFillRect, mParams);
			}

			if (mRcName != null)
			{
				//
/*				
				mTextBk1 = new LinearLayout(this);
				mTextBk1.setBackgroundColor(Color.argb(255, 0, 200, 0));
				
				mParams = new WindowManager.LayoutParams(
					WindowManager.LayoutParams.MATCH_PARENT,
					mRcName.height(),
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = 0;
				mParams.y = mRcName.top;
		
				mWindowManager.addView(mTextBk1, mParams);
*/				

				//
				mTxtName = new TextView(this);
				mTxtName.setTextSize(TypedValue.COMPLEX_UNIT_DIP, 40.0f);	
				mTxtName.setTextColor(Color.argb(0xff, 0xff, 0xff, 0xff));					
				mTxtName.setBackgroundColor(Color.TRANSPARENT);
				mTxtName.setGravity(Gravity.CENTER);
				
				mParams = new WindowManager.LayoutParams(
					WindowManager.LayoutParams.MATCH_PARENT,
					WindowManager.LayoutParams.WRAP_CONTENT,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = 0;
				mParams.y = mRcRect.height() / 2;
		
				mWindowManager.addView(mTxtName, mParams);
			}
	
			if (mRcNumber != null)
			{
				//
/*				
				mTextBk2 = new LinearLayout(this);
				mTextBk2.setBackgroundColor(Color.argb(255, 0, 200, 0));
				
				mParams = new WindowManager.LayoutParams(
					WindowManager.LayoutParams.MATCH_PARENT,
					mRcNumber.height(),
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = 0;
				mParams.y = mRcNumber.top;

				mWindowManager.addView(mTextBk2, mParams);
*/				

				//
				mTxtNumber = new TextView(this);
				mTxtNumber.setTextSize(TypedValue.COMPLEX_UNIT_DIP, 18.0f);	
				mTxtNumber.setTextColor(Color.argb(0xff, 0xdc, 0xdc, 0xdc));	
				mTxtNumber.setBackgroundColor(Color.TRANSPARENT);
				mTxtNumber.setGravity(Gravity.LEFT);
				
				mParams = new WindowManager.LayoutParams(
					WindowManager.LayoutParams.MATCH_PARENT,
					WindowManager.LayoutParams.WRAP_CONTENT,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcNumber.left;
				mParams.y = mRcNumber.top;
		
				mWindowManager.addView(mTxtNumber, mParams);
			}
			
			if (mRcKeyPad != null)
			{
				mBtnPad = new Button(this);
				mBtnPad.setBackgroundColor(Color.TRANSPARENT);
				
				int delta = 10;
				mParams = new WindowManager.LayoutParams(
					mRcKeyPad.width() + delta * 2,
					mRcKeyPad.height() + delta * 2,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcKeyPad.left - delta;
				mParams.y = mRcKeyPad.top - delta;
		
				mWindowManager.addView(mBtnPad, mParams);
			}
			
			if (mRcSpeaker != null)
			{
				mBtnSpeaker = new Button(this);
				mBtnSpeaker.setBackgroundColor(Color.TRANSPARENT);
				
				int delta = 10;
				mParams = new WindowManager.LayoutParams(
					mRcSpeaker.width() + delta * 2,
					mRcSpeaker.height() + delta * 2,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcSpeaker.left - delta;
				mParams.y = mRcSpeaker.top - delta;
		
				mWindowManager.addView(mBtnSpeaker, mParams);
			}
			
			if (mRcAddCall != null)
			{
				mBtnAddCall = new Button(this);
				mBtnAddCall.setBackgroundColor(Color.TRANSPARENT);
				
				int delta = 10;
				mParams = new WindowManager.LayoutParams(
					mRcAddCall.width() + delta * 2,
					mRcAddCall.height() + delta * 2,
					WindowManager.LayoutParams.TYPE_PHONE,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
					PixelFormat.TRANSLUCENT);
				
				mParams.gravity = Gravity.LEFT | Gravity.TOP;
				mParams.x = mRcAddCall.left - delta;
				mParams.y = mRcAddCall.top - delta;
		
				mWindowManager.addView(mBtnAddCall, mParams);
			}

			
			//
			mStatusbar = new LinearLayout(this);
			mStatusbar.setBackgroundColor(Color.TRANSPARENT);
			
			mParams = new WindowManager.LayoutParams(
				WindowManager.LayoutParams.MATCH_PARENT,
				MAX_STATUS_BAR_HEIGHT,
				WindowManager.LayoutParams.TYPE_SYSTEM_OVERLAY,
				WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN,
				PixelFormat.TRANSLUCENT);
			
			mParams.gravity = Gravity.LEFT | Gravity.TOP;
			mParams.x = mParams.y = 0;
	
			mWindowManager.addView(mStatusbar, mParams);
			
			//
			mScreenOff = new LinearLayout(this);
			mScreenOff.setBackgroundColor(Color.argb(255, 0, 0, 0));
			
			mParams = new WindowManager.LayoutParams(
				WindowManager.LayoutParams.MATCH_PARENT,
				WindowManager.LayoutParams.MATCH_PARENT,
				WindowManager.LayoutParams.TYPE_SYSTEM_OVERLAY,
				WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE|WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN,
				PixelFormat.TRANSLUCENT);
			
			mParams.gravity = Gravity.LEFT | Gravity.TOP;
			mParams.x = mParams.y = 0;
	
			mWindowManager.addView(mScreenOff, mParams);
			mScreenOff.setVisibility(View.GONE);
			//

			if ("".equals(VarSpace.mOriginNum) == false)
			{
				if ("".equals(VarSpace.mOriginName) == false)
				{
					mTxtName.setText(VarSpace.mOriginName);
					mTxtNumber.setText(VarSpace.mOriginNum);
				}
				else
				{
					mTxtName.setText(VarSpace.mOriginNum);
					mTxtNumber.setText("");
				}
			}
		}
		catch(Exception e)
		{
			
		}		

		VarSpace.bTopSvcRun = true;
	}	
	
	public void OnInitDialog()
	{
		try{
			mBtnPad = null;
			mTxtName = null;
			mTxtNumber = null;
			mFillRect = null;

			if (ModelType.getCallUiType() == ModelType.CALLUI_GALAXY_234)
			{
				OnCreate_Galaxy234();
			}

			if (ModelType.getCallUiType() == ModelType.CALLUI_GALAXY_412)
			{
				OnCreate_Galaxy412();
			}

			if (ModelType.getCallUiType() == ModelType.CALLUI_GALAXY_421)
			{
				OnCreate_Galaxy421();
			}
			
			if (ModelType.getCallUiType() == ModelType.CALLUI_GALAXY_500)
			{
				OnCreate_Galaxy500();
			}

			if (ModelType.getCallUiType() == ModelType.CALLUI_LGOPTIMUS_234)
			{
				OnCreate_Optimus234();
			}

			if (ModelType.getCallUiType() == ModelType.CALLUI_LGOPTIMUS_412)
			{
				OnCreate_Optimus412();
			}
			
			if (ModelType.getCallUiType() == ModelType.CALLUI_LGOPTIMUS_500)
			{
				OnCreate_Optimus500();
			}

			if (ModelType.getCallUiType() == ModelType.CALLUI_SKYVEGA_412)
			{
				OnCreate_Sky412();
			}
			
			if (ModelType.getCallUiType() == ModelType.CALLUI_SKYVEGA_442)
			{
				OnCreate_Sky442();
			}

			OnUICtrlSvc.setVisibility(mVisible);
			OnUICtrlSvc.WakeOrSleep(mWakeOrSleep);
			
		}
		catch(Exception e)
		{
		}
	}
	
	public static Context mContext = null;
	@Override
	public void onCreate() {

		super.onCreate();

		if (VarSpace.bTopSvcRun == true) return;
		
		VarSpace.bTopSvcRun = true;
		
		OnInitDialog();
		
		mContext = this;
	}
	
	public static void destoryUI()
	{
		try{
			if (VarSpace.bTopSvcRun == false) return;

			mContext = null;

			if(mWindowManager != null) {
				
				if(mTxtName != null)
				{
					mWindowManager.removeView(mTxtName);
					mTxtName = null;
				}
				
				if(mTxtNumber != null)
				{
					mWindowManager.removeView(mTxtNumber);
					mTxtNumber = null;
				}
				
				if(mBtnPad != null)
				{
					mWindowManager.removeView(mBtnPad);
					mBtnPad = null;
				}

				if(mBtnSpeaker != null)
				{
					mWindowManager.removeView(mBtnSpeaker);
					mBtnSpeaker = null;
				}
				
				if (mBtnAddCall != null)
				{
					mWindowManager.removeView(mBtnAddCall);
					mBtnAddCall = null;
				}

				if (mFillRect != null)
				{
					mWindowManager.removeView(mFillRect);
					mFillRect = null;
				}

				if (mTextBk1 != null)
				{
					mWindowManager.removeView(mTextBk1);
					mTextBk1 = null;
				}

				if (mTextBk2 != null)
				{
					mWindowManager.removeView(mTextBk2);
					mTextBk2 = null;
				}
				
				if (mScreenOff != null)
				{
					mWindowManager.removeView(mScreenOff);
					mScreenOff = null;
				}
				
				if (mStatusbar != null)
				{
					mWindowManager.removeView(mStatusbar);
					mStatusbar = null;
				}
			}
			
			mVisible = false;
			mWakeOrSleep = true;
		}
		catch(Exception e)
		{
			
		}
		
		VarSpace.bTopSvcRun = false;
	}
	
	@Override
	public void onDestroy() {
		
		super.onDestroy();
		
		destoryUI();
	}
	
	public static void setBackgroundColor_412(int color)
	{
		try{
			if (mFillRect != null)
			{
				mFillRect.setBackgroundColor(color);
			}
			
			if (mTextBk1 != null)
			{
				mWindowManager.removeView(mTextBk1);
				mTextBk1 = null;
			}

			if (mTextBk2 != null)
			{
				mWindowManager.removeView(mTextBk2);
				mTextBk2 = null;
			}
		}
		catch(Exception e)
		{
			
		}
	}
	
	public static void setBackgroundDrawable_234(int nResID)
	{
		try{
			if (mFillRect != null)
			{
				mFillRect.setBackgroundResource(nResID);
			}
		}
		catch(Exception e)
		{
			
		}
	}
	
	
	public static void setVisibility(boolean bVisible)
	{
		try{
			mVisible = bVisible;
			
			int nCode = View.VISIBLE;
			if (bVisible == false)
			{
				nCode = View.INVISIBLE;
			}
			
			if( mBtnPad != null )
			{
				mBtnPad.setVisibility(nCode);
			}
			if( mBtnSpeaker != null )
			{
				mBtnSpeaker.setVisibility(nCode);
			}
			if( mBtnAddCall != null )
			{
				mBtnAddCall.setVisibility(nCode);
			}

			if( mTxtName != null )
			{
				mTxtName.setVisibility(nCode);
			}
			if( mTxtNumber != null )
			{
				mTxtNumber.setVisibility(nCode);
			}
			if( mFillRect != null )
			{
				mFillRect.setVisibility(nCode);
			}
			if( mTextBk1 != null )
			{
				mTextBk1.setVisibility(nCode);
			}
			if( mTextBk2 != null )
			{
				mTextBk2.setVisibility(nCode);
			}
			
		}
		catch(Exception e)
		{
			
		}
	}
	
	public static void WakeOrSleep(boolean bWakeOrSleep)
	{
		try{
			mWakeOrSleep = bWakeOrSleep;
			if (mScreenOff != null)
			{
				if (mWakeOrSleep == true)
				{
					mScreenOff.setVisibility(View.GONE);
				}
				else
				{
					mScreenOff.setVisibility(View.VISIBLE);
				}
			}
		}
		catch(Exception e)
		{
			
		}
	}
	
	public static void ProcessCallActive()
	{
		try{
			if (ModelType.getCallUiType() == ModelType.CALLUI_LGOPTIMUS_234)
			{
				if(mFillRect != null)
				{
					SetOverlayBitmap(mContext, mFillRect, R.drawable.optmsbg_green_dim, mRcRect);
				}
			}
		}
		catch(Exception e)
		{
			
		}
	}
	
}