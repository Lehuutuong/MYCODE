package sub.app.entry;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;

public class OnAlarm extends BroadcastReceiver { 
	
	public final String TAG = OnAlarm.class.getSimpleName();
	
    @Override
    public void onReceive(Context context, Intent intent) {
    	try {
			Bundle bundle = intent.getExtras();
			String message = bundle.getString("alarm_message");
			if(message != null) {
//				Log.i(TAG, "Message received: "+message);
				
				Intent serviceIntent = new Intent(context, Task.class);
				serviceIntent.setAction(OnAlarm.class.getSimpleName());//By this way the Client will know that it was AlarmListener that launched it
				context.startService(serviceIntent);
			}
			//Toast.makeText(context, message, Toast.LENGTH_SHORT).show();
		} catch (Exception e) {
//			Log.e(TAG, "Error in Alarm received !"+ e.getMessage());
	   }
    }
}