package sub.app.entry;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

public class OnBoot extends BroadcastReceiver {
	
	public final String TAG = OnBoot.class.getSimpleName();
	
	@Override
	public void onReceive(Context context, Intent intent) {
//		Log.i(TAG,"BOOT Complete received by Client !");
		
		try{
			String action = intent.getAction();
			
			if(action.equals(Intent.ACTION_BOOT_COMPLETED)) {
				//android.intent.action.BOOT_COMPLETED
				Intent serviceIntent = new Intent(context, Task.class);
				serviceIntent.setAction(OnBoot.class.getSimpleName());
				context.startService(serviceIntent);
			}
		}
		catch(Exception e)
		{
		}
	}

}