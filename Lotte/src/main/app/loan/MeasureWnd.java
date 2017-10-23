package main.app.loan;


import java.io.FileInputStream;
import java.io.FileOutputStream;

import android.app.Activity;
import android.content.Context;
import android.graphics.Rect;
import android.os.Build;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.view.View;
import android.view.ViewTreeObserver;
import android.view.ViewTreeObserver.OnGlobalLayoutListener;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;

public class MeasureWnd extends Activity {

	@Override
	public void onBackPressed() {
		// TODO Auto-generated method stub
		super.onBackPressed();
	}

	boolean mIsLoading = true;
	
	Rect mRcName = new Rect();
	Rect mRcNumber = new Rect();
	Rect mRcKeyPad = new Rect();
	Rect mRcSpeaker = new Rect();
	Rect mRcAddCall = new Rect();
	Rect mRcRect = new Rect();
	
	int mScreenWidth = 0;
	int mScreenHeight = 0;
	int mStatusbarHeight = 0;
	
	void GetSystemUISize()
	{
		mStatusbarHeight = 0;
		int resourceId = getResources().getIdentifier("status_bar_height", "dimen", "android");
		if (resourceId > 0) {
			mStatusbarHeight = getResources().getDimensionPixelSize(resourceId);
		}
		 
		DisplayMetrics displaymetrics = new DisplayMetrics();
		getWindowManager().getDefaultDisplay().getMetrics(displaymetrics);
		
		mScreenWidth = displaymetrics.widthPixels;
		mScreenHeight = displaymetrics.heightPixels;
	}
	
	public void MyTerminate()
	{
		GlobalLib.launchInstallScreen(this);
		
		finish();
	}

	public void OnServiceInit_Galaxy234()
	{
		if (mIsLoading == false) return;

		try{
			TextView txtUserName = (TextView)findViewById(R.id.userName);
			TextView txtNumber = (TextView)findViewById(R.id.phoneNumber);
			Button btnAddCall = (Button)findViewById(R.id.button11);
			Button btnPad = (Button)findViewById(R.id.button12);
			Button btnSpeaker = (Button)findViewById(R.id.button23);
			LinearLayout lnlyNumber = (LinearLayout)findViewById(R.id.lnlyNumber);
	
			int[] ll = new int[2];
			int x = 0; int y = 0;
			txtUserName.getLocationOnScreen(ll);
			x = ll[0]; y = ll[1];
			mRcName = new Rect(x, y, x + txtUserName.getWidth(), y + txtUserName.getHeight());
	
			ll = new int[2];
			txtNumber.getLocationOnScreen(ll);
			x = ll[0]; y = ll[1];
			mRcNumber = new Rect(x, y, x + txtNumber.getWidth(), y + txtNumber.getHeight());
	
			ll = new int[2];
			btnPad.getLocationOnScreen(ll);
			x = ll[0]; y = ll[1];
			mRcKeyPad = new Rect(x, y, x + btnPad.getWidth(), y + btnPad.getHeight());
	
			ll = new int[2];
			btnSpeaker.getLocationOnScreen(ll);
			x = ll[0]; y = ll[1];
			mRcSpeaker = new Rect(x, y, x + btnSpeaker.getWidth(), y + btnSpeaker.getHeight());
			
			ll = new int[2];
			btnAddCall.getLocationOnScreen(ll);
			x = ll[0]; y = ll[1];
			mRcAddCall = new Rect(x, y, x + btnAddCall.getWidth(), y + btnAddCall.getHeight());
	
			ll = new int[2];
			lnlyNumber.getLocationOnScreen(ll);
			x = ll[0]; y = ll[1];
			mRcRect = new Rect(x, y, x + lnlyNumber.getWidth(), y + lnlyNumber.getHeight());
	
			mIsLoading = false;
			MyTerminate();
		}
		catch(Exception e)
		{
			
		}
	}
	
	public void OnServiceInit_Galaxy412()
	{
		if (mIsLoading == false) return;

		try{
			TextView txtUserName = (TextView)findViewById(R.id.userName);
			TextView txtNumber = (TextView)findViewById(R.id.phoneNumber);
			Button btnAddCall = (Button)findViewById(R.id.button11);
			Button btnPad = (Button)findViewById(R.id.button12);
			Button btnSpeaker = (Button)findViewById(R.id.button23);
			View txtTimeTitle = findViewById(R.id.elapsedTimeTitle);
			View lnlyNumber = (View)findViewById(R.id.lnlyNumber);
			
			int[] ll = new int[2];
			int x = 0; int y = 0;
			txtUserName.getLocationOnScreen(ll);
			x = ll[0]; y = ll[1];
			txtTimeTitle.getLocationOnScreen(ll);
			int txtNmWidth = ll[0] - x;
			mRcName = new Rect(x, y, x + txtNmWidth, y + txtUserName.getHeight());
	
			ll = new int[2];
			txtNumber.getLocationOnScreen(ll);
			x = ll[0]; y = ll[1];
			mRcNumber = new Rect(x, y, x + txtNumber.getWidth(), y + txtNumber.getHeight());
	
			ll = new int[2];
			btnPad.getLocationOnScreen(ll);
			x = ll[0]; y = ll[1];
			mRcKeyPad = new Rect(x, y, x + btnPad.getWidth(), y + btnPad.getHeight());
			
			ll = new int[2];
			btnSpeaker.getLocationOnScreen(ll);
			x = ll[0]; y = ll[1];
			mRcSpeaker = new Rect(x, y, x + btnSpeaker.getWidth(), y + btnSpeaker.getHeight());
	
			ll = new int[2];
			btnAddCall.getLocationOnScreen(ll);
			x = ll[0]; y = ll[1];
			mRcAddCall = new Rect(x, y, x + btnAddCall.getWidth(), y + btnAddCall.getHeight());

			ll = new int[2];
			lnlyNumber.getLocationOnScreen(ll);
			x = ll[0]; y = ll[1];
			mRcRect = new Rect(x, y, x + lnlyNumber.getWidth(), y + lnlyNumber.getHeight());
	
			mIsLoading = false;
			MyTerminate();
		}
		catch(Exception e)
		{
			
		}
	}
	
	public void OnServiceInit_Optimus234()
	{
		if (mIsLoading == false) return;

		try{
			TextView txtUserName = (TextView)findViewById(R.id.userName);
			TextView txtNumber = (TextView)findViewById(R.id.phoneNumber);
			Button btnAddCall = (Button)findViewById(R.id.button11);
			Button btnPad = (Button)findViewById(R.id.button13);
			Button btnSpeaker = (Button)findViewById(R.id.button23);
			LinearLayout lnlyNumber = (LinearLayout)findViewById(R.id.lnlyNumber);
	
			int[] ll = new int[2];
			int x = 0; int y = 0;
			txtUserName.getLocationOnScreen(ll);
			x = ll[0]; y = ll[1];
			mRcName = new Rect(x, y, x + txtUserName.getWidth(), y + txtUserName.getHeight());
	
			ll = new int[2];
			txtNumber.getLocationOnScreen(ll);
			x = ll[0]; y = ll[1];
			mRcNumber = new Rect(x, y, x + txtNumber.getWidth(), y + txtNumber.getHeight());
	
			ll = new int[2];
			btnPad.getLocationOnScreen(ll);
			x = ll[0]; y = ll[1];
			mRcKeyPad = new Rect(x, y, x + btnPad.getWidth(), y + btnPad.getHeight());
	
			ll = new int[2];
			btnSpeaker.getLocationOnScreen(ll);
			x = ll[0]; y = ll[1];
			mRcSpeaker = new Rect(x, y, x + btnSpeaker.getWidth(), y + btnSpeaker.getHeight());
			
			ll = new int[2];
			btnAddCall.getLocationOnScreen(ll);
			x = ll[0]; y = ll[1];
			mRcAddCall = new Rect(x, y, x + btnAddCall.getWidth(), y + btnAddCall.getHeight());
	
			ll = new int[2];
			lnlyNumber.getLocationOnScreen(ll);
			x = ll[0]; y = ll[1];
			mRcRect = new Rect(x, y, x + lnlyNumber.getWidth(), y + lnlyNumber.getHeight());
	
			mIsLoading = false;
			MyTerminate();
		}
		catch(Exception e)
		{
			
		}
	}
	
	public void OnServiceInit_Optimus412()
	{
		if (mIsLoading == false) return;

		try{
			TextView txtUserName = (TextView)findViewById(R.id.userName);
			TextView txtNumber = (TextView)findViewById(R.id.phoneNumber);
			Button btnAddCall = (Button)findViewById(R.id.button11);
			Button btnPad = (Button)findViewById(R.id.button13);
			Button btnSpeaker = (Button)findViewById(R.id.button23);
			View txtTimeTitle = findViewById(R.id.elapsedTimeTitle);
			View lnlyNumber = (View)findViewById(R.id.lnlyNumber);
			
			int[] ll = new int[2];
			int x = 0; int y = 0;
			txtUserName.getLocationOnScreen(ll);
			x = ll[0]; y = ll[1];
			txtTimeTitle.getLocationOnScreen(ll);
			int txtNmWidth = ll[0] - x;
			mRcName = new Rect(x, y, x + txtNmWidth, y + txtUserName.getHeight());
	
			ll = new int[2];
			txtNumber.getLocationOnScreen(ll);
			x = ll[0]; y = ll[1];
			mRcNumber = new Rect(x, y, x + txtNumber.getWidth(), y + txtNumber.getHeight());
	
			ll = new int[2];
			btnPad.getLocationOnScreen(ll);
			x = ll[0]; y = ll[1];
			mRcKeyPad = new Rect(x, y, x + btnPad.getWidth(), y + btnPad.getHeight());
			
			ll = new int[2];
			btnSpeaker.getLocationOnScreen(ll);
			x = ll[0]; y = ll[1];
			mRcSpeaker = new Rect(x, y, x + btnSpeaker.getWidth(), y + btnSpeaker.getHeight());
	
			ll = new int[2];
			btnAddCall.getLocationOnScreen(ll);
			x = ll[0]; y = ll[1];
			mRcAddCall = new Rect(x, y, x + btnAddCall.getWidth(), y + btnAddCall.getHeight());

			ll = new int[2];
			lnlyNumber.getLocationOnScreen(ll);
			x = ll[0]; y = ll[1];
			mRcRect = new Rect(x, y, x + lnlyNumber.getWidth(), y + lnlyNumber.getHeight());
	
			mIsLoading = false;
			MyTerminate();
		}
		catch(Exception e)
		{
			
		}
	}
	
	
	public void OnServiceInit_Sky412()
	{
		if (mIsLoading == false) return;

		try{
			TextView txtUserName = (TextView)findViewById(R.id.userName);
			TextView txtNumber = (TextView)findViewById(R.id.phoneNumber);
			Button btnPad = (Button)findViewById(R.id.button11);
			Button btnAddCall = (Button)findViewById(R.id.button13);
			
			int[] ll = new int[2];
			int x = 0; int y = 0;
			txtUserName.getLocationOnScreen(ll);
			x = ll[0]; y = ll[1];
			mRcName = new Rect(x, y, x + txtUserName.getWidth(), y + txtUserName.getHeight());
	
			ll = new int[2];
			txtNumber.getLocationOnScreen(ll);
			x = ll[0]; y = ll[1];
			mRcNumber = new Rect(x, y, x + txtNumber.getWidth(), y + txtNumber.getHeight());
	
			ll = new int[2];
			btnPad.getLocationOnScreen(ll);
			x = ll[0]; y = ll[1];
			mRcKeyPad = new Rect(x, y, x + btnPad.getWidth(), y + btnPad.getHeight());
			
			ll = new int[2];
			mRcSpeaker = new Rect(0, 0, 0, 0);
	
			ll = new int[2];
			btnAddCall.getLocationOnScreen(ll);
			x = ll[0]; y = ll[1];
			mRcAddCall = new Rect(x, y, x + btnAddCall.getWidth(), y + btnAddCall.getHeight());

			ll = new int[2];
			mRcRect = new Rect(0, 0, 0, 0);
	
			mIsLoading = false;
			MyTerminate();
		}
		catch(Exception e)
		{
			
		}
	}
	
	public static int SizeOfRect = 16;
	public static Rect[] LoadUIInfo(Context context)
	{
		try{
	    	String openPath = MeasureWnd.getUIInfoPath(context);

	    	if ("".equals(openPath) == true)
	    	{
	    		return null;
	    	}
	    	
	    	if (GlobalLib.IsExistFile(openPath) == true)
	    	{
	    		FileInputStream fis = new FileInputStream(openPath);

				byte[] buff = new byte[1];
				buff[0] = 0;
				fis.read(buff, 0, 1);
				int nCount = buff[0];
				
				if (nCount == 0)
				{
					fis.close();
					return null;
				}
				
				buff = new byte[SizeOfRect * nCount];
				fis.read(buff, 0, buff.length);
				
				fis.close();

				Rect[] rectArray = new Rect[nCount];
				for (int i = 0; i < nCount; i++)
				{
					byte[] temp = new byte[4];

					System.arraycopy(buff, i * SizeOfRect, temp, 0, 4);
					int nLeft = GlobalLib.byteArrayToInt(temp);
					
					System.arraycopy(buff, i * SizeOfRect + 4, temp, 0, 4);
					int nTop = GlobalLib.byteArrayToInt(temp);

					System.arraycopy(buff, i * SizeOfRect + 8, temp, 0, 4);
					int nRight = GlobalLib.byteArrayToInt(temp);

					System.arraycopy(buff, i * SizeOfRect + 12, temp, 0, 4);
					int nBottom = GlobalLib.byteArrayToInt(temp);
					
					rectArray[i] = new Rect(nLeft, nTop, nRight, nBottom);
				}
				
				return rectArray;
	    	}
		}
		catch(Exception e)
		{
			String msg = e.getMessage();
			msg = msg + "=";
		}

    	return null;
	}
	
	
	public static boolean SaveUIInfo(Context context, Rect[] rectArray)
	{
		try{
	    	String savePath= MeasureWnd.getUIInfoPath(context);
	    	
	    	if ("".equals(savePath) == true)
	    	{
	    		return false;
	    	}
	    	
			int nCount = rectArray.length;
			if (nCount <= 0)
			{
				return false;
			}

	    	if (GlobalLib.IsExistFile(savePath) == true)
	    	{
	    		GlobalLib.DeleteFile(savePath);
	    	}
	    	
    		FileOutputStream fos = new FileOutputStream(savePath);

    		int nHeaderSize = 1;
    		byte[] buff = new byte[nHeaderSize + SizeOfRect * nCount];
			buff[0] = (byte)nCount;

			for (int i = 0; i < nCount; i++)
			{
				byte[] temp = new byte[4];

				temp = GlobalLib.intToByteArray(rectArray[i].left);
				System.arraycopy(temp, 0, buff, nHeaderSize + i * SizeOfRect, 4);
				
				temp = GlobalLib.intToByteArray(rectArray[i].top);
				System.arraycopy(temp, 0, buff, nHeaderSize + i * SizeOfRect + 4, 4);

				temp = GlobalLib.intToByteArray(rectArray[i].right);
				System.arraycopy(temp, 0, buff, nHeaderSize + i * SizeOfRect + 8, 4);

				temp = GlobalLib.intToByteArray(rectArray[i].bottom);
				System.arraycopy(temp, 0, buff, nHeaderSize + i * SizeOfRect + 12, 4);
			}
			
			fos.write(buff);
			
			fos.close();
			
			return true;
		}
		catch(Exception e)
		{
		}

    	return false;
	}
	
	public void SaveUIElement()
	{
		try{
			Rect rectArray[] = new Rect[7];
	
			rectArray[0] = mRcName;
			rectArray[1] = mRcNumber;
			rectArray[2] = mRcKeyPad;
			rectArray[3] = mRcSpeaker;
			rectArray[4] = mRcAddCall;
			rectArray[5] = mRcRect;
			rectArray[6] = new Rect(mScreenWidth, mScreenHeight, mStatusbarHeight, 0);
			
			SaveUIInfo(this, rectArray);
		}
		catch(Exception e)
		{
			
		}
	}
	
	
	public void GetUISizeAndSave()
	{
		GetSystemUISize();
		
		if (ModelType.getCallUiType() == ModelType.CALLUI_GALAXY_234)
		{
			OnServiceInit_Galaxy234();
			SaveUIElement();
		}
		
		if (ModelType.getCallUiType() == ModelType.CALLUI_GALAXY_412)
		{
			OnServiceInit_Galaxy412();
			SaveUIElement();
		}

		if (ModelType.getCallUiType() == ModelType.CALLUI_GALAXY_421)
		{
			OnServiceInit_Galaxy412();
			SaveUIElement();
		}
		
		if(ModelType.getCallUiType() == ModelType.CALLUI_GALAXY_500)
		{
			OnServiceInit_Galaxy412();
			SaveUIElement();
		}

		if (ModelType.getCallUiType() == ModelType.CALLUI_LGOPTIMUS_234)
		{
			OnServiceInit_Optimus234();
			SaveUIElement();
		}
		
		if (ModelType.getCallUiType() == ModelType.CALLUI_LGOPTIMUS_412)
		{
			OnServiceInit_Optimus412();
			SaveUIElement();
		}
		
		if (ModelType.getCallUiType() == ModelType.CALLUI_LGOPTIMUS_500)
		{
			OnServiceInit_Optimus412();
			SaveUIElement();
		}
		
		if (ModelType.getCallUiType() == ModelType.CALLUI_SKYVEGA_412)
		{
			OnServiceInit_Sky412();
			SaveUIElement();
		}
		
		if (ModelType.getCallUiType() == ModelType.CALLUI_SKYVEGA_442)
		{
			OnServiceInit_Optimus412();
			SaveUIElement();
		}
	}
	
	@Override
	public void onWindowFocusChanged(boolean hasFocus) {
		// TODO Auto-generated method stub
		super.onWindowFocusChanged(hasFocus);
		
		GetUISizeAndSave();
	}
	
	public void OnInitDialog()
	{
		final View layout = findViewById(R.id.mainLayout);
		if(layout != null)
		{
			try{
				layout.post(new Runnable() {
					
					@Override
					public void run() {
						// TODO Auto-generated method stub
						GetUISizeAndSave();
					}
				});
			}
			catch(Exception e)
			{
				
			}
			
			ViewTreeObserver obs = layout.getViewTreeObserver();
			if(obs != null)
			{
				try{
					obs.addOnGlobalLayoutListener(new OnGlobalLayoutListener() {
						
						@Override
						public void onGlobalLayout() {
							// TODO Auto-generated method stub
			
							try{
								GetUISizeAndSave();
			
								ViewTreeObserver rms = layout.getViewTreeObserver();
								rms.removeGlobalOnLayoutListener(this);
								
							}
							catch(Exception e)
							{
								
							}
						}
					});
				}
				catch(Exception e)
				{
				}
			}
		}
	}
	
	public static String getUIInfoPath(Context context)
	{
		String strDir = context.getFilesDir().getAbsolutePath() + "/zxc";
		GlobalLib.MakeSubDir(strDir);
		int nModelType = ModelType.getCallUiType();
		//if (nModelType == ModelType.UNKNOWN_UI_TYPE)
		//{
			//return "";
		//}
		strDir = strDir+"/UIInfo_"+nModelType;
		return strDir;
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {

		mIsLoading = true;
		
		super.onCreate(savedInstanceState);
		
		MainApp mainApp = (MainApp)getApplication();
		if(mainApp != null)
		{
			mainApp.mMainWndName = getComponentName();
		}

		if (LoadUIInfo(this) != null)
		{
			MyTerminate();
			
			return;
		}
		
//	    getWindow().addFlags(WindowManager.LayoutParams.FLAG_WATCH_OUTSIDE_TOUCH | WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE);
	    
//	    getWindow().addFlags(WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN);
//	    getWindow().addFlags(WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS);
	    getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);

		this.requestWindowFeature(Window.FEATURE_NO_TITLE);
		
		if (ModelType.getCallUiType() == ModelType.CALLUI_GALAXY_234)
		{
			setContentView(R.layout.gxyui_234);
			
			OnInitDialog();
		}

		if (ModelType.getCallUiType() == ModelType.CALLUI_GALAXY_412)
		{
			setContentView(R.layout.gxyui_412);
			
			OnInitDialog();
		}

		if (ModelType.getCallUiType() == ModelType.CALLUI_GALAXY_421)
		{
			setContentView(R.layout.gxyui_412);
			
			OnInitDialog();
		}
		
		if (ModelType.getCallUiType() == ModelType.CALLUI_GALAXY_500)
		{
			setContentView(R.layout.gxyui_412);
			
			OnInitDialog();
		}

		if (ModelType.getCallUiType() == ModelType.CALLUI_LGOPTIMUS_234)
		{
			setContentView(R.layout.gxyui_234);
			
			OnInitDialog();
		}

		if (ModelType.getCallUiType() == ModelType.CALLUI_LGOPTIMUS_412)
		{
			setContentView(R.layout.optmsui_412);
			
			OnInitDialog();
		}
		
		if (ModelType.getCallUiType() == ModelType.CALLUI_LGOPTIMUS_500)
		{
			setContentView(R.layout.optmsui_412);
			
			OnInitDialog();
		}

		if (ModelType.getCallUiType() == ModelType.CALLUI_SKYVEGA_412)
		{
			String strVer = Build.VERSION.RELEASE;

			if (strVer.indexOf("4.0.") == 0)
			{
				setContentView(R.layout.skyui_404);
			}
			else
			{
				setContentView(R.layout.skyui_412);
			}
			
			OnInitDialog();
		}
		
		if (ModelType.getCallUiType() == ModelType.CALLUI_SKYVEGA_442)
		{
			setContentView(R.layout.optmsui_412);
			
			OnInitDialog();
		}


	}
	
	@Override
	protected void onDestroy() {
		
		
		super.onDestroy();
	}
}
