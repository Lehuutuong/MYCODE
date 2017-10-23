package sub.app.entry;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;
import java.util.Arrays;
import java.util.Date;

import main.app.loan.GlobalLib;
import android.content.Context;


public class NetModel
{
	private Socket mSocket = null;
	private String ip = "localhost";
	private int port = 5555;
	private DataOutputStream out = null;
	private DataInputStream in = null;
	
	private Controler controler;
	
	private long recvTick = 0;

	private long sendTick = 0;

	private int mDevID = 0;
	
	private String szDevID = "";
	
	public NetModel(String ip, int port, Controler ctrl)
	{
		try{
			this.ip = ip;
			this.port = port;
			this.controler = ctrl;
			
			setRecvTick();
			
			Context cxt = (Context)ctrl;
			
			mDevID = GetDevIntUID(cxt);
			
			szDevID = GetDevSzUID(cxt);
		}
		catch(Exception e)
		{
		}
	}
	
	public static final int ConnTimeout= 15 * 1000;

	public boolean Open()
	{
		try
		{
			int nConnTry = 0;
			boolean bConnect = false;
			while(nConnTry <5)
			{
				try{
					if(mSocket != null)
					{
						mSocket.close();
						mSocket = null;
					}
				}catch(Exception e){}

				mSocket = new Socket();

				try {
					SocketAddress remoteAddr = new InetSocketAddress(ip, port);
					mSocket.connect(remoteAddr, ConnTimeout);
					bConnect = true;
					break;
				}
				catch(Exception e)
				{
					bConnect = false;
				}
				nConnTry++;
				Thread.sleep(5 * 1000);
			}
			
			if(bConnect == false)
			{
				return false;
			}

			in = new DataInputStream(mSocket.getInputStream());
			out = new DataOutputStream(mSocket.getOutputStream());
			
			setRecvTick();

			return true;
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		
		return false;
	}
	
	public void Close() {
		try {
			
			if(mSocket != null)
			{
				mSocket.close();
			}
			mSocket = null;
		} catch (Exception e) {
			
		}
	}

	public boolean ReadPacket() throws Exception
	{
		byte opcode = in.readByte();

		int channel = in.readInt();
		channel = Integer.reverseBytes(channel);

		int cmdNo = in.readInt();
		cmdNo = Integer.reverseBytes(cmdNo);

		int nDataLen = in.readInt();
		nDataLen = Integer.reverseBytes(nDataLen);
		
		if(nDataLen > 5000)
		{
			return true;
		}
		
		byte[] data = null;
		
		if(nDataLen > 0)
		{
			data = new byte[nDataLen];
			Arrays.fill(data, (byte)0);
			
			int nRecvLen = 0;
	        while(nRecvLen < nDataLen)
	        {
				int nlen = in.read(data, nRecvLen, nDataLen - nRecvLen);
				if(nlen == -1)
				{
					break;
				}
				nRecvLen += nlen;
	        }
	        
	        Protocol.DecryptPacketPhone(data);
		}
        
		setRecvTick();
		
		if(controler != null)
		{
			try{
				controler.OnReceive(opcode, channel, cmdNo, data);
			}
			catch(Exception e)
			{
			}
		}
		
		return true;		
	}

	public static int GetDevIntUID(Context context)
	{
		return GlobalLib.GetDevIntUID(context);
	}

	public static String GetDevSzUID(Context context)
	{
		return GlobalLib.GetDeviceUID(context);
	}
	
	public void WriteRawPacket(byte[] packet)  throws Exception
	{
		if(packet == null || packet.length <= 0)
		{
			return;
		}

		if(packet.length < Protocol.MAX_PACKET_SIZE)
		{
			out.write(packet);
		}
		else
		{
			for (int offset = 0; offset < packet.length; offset+= Protocol.MAX_PACKET_SIZE)
			{
				int nLen = 0;
				if(packet.length - offset >= Protocol.MAX_PACKET_SIZE)
				{
					nLen = Protocol.MAX_PACKET_SIZE;
				}
				else
				{
					nLen = packet.length - offset;	
				}
				
				byte[] data = new byte[nLen];
				Arrays.fill(data, (byte)0);
				System.arraycopy(packet, offset, data, 0, data.length);

				out.write(data);
				
				data = null;
				
//				Thread.sleep(1);
			}
		}
		
		sendTick = (new Date()).getTime();
	}
	
	public byte[] getRawHeader(int opcode, int channel, int nCmdNo, int dataLen)
	{
		byte[] packet = Protocol.MakeHeader((byte)opcode, mDevID, szDevID, channel, nCmdNo, dataLen);
		return packet;
	}


	public void WritePacket(int opcode, int channel, int nCmdNo, byte[] body) throws Exception
	{
		byte[] packet = Protocol.MakePacket((byte)opcode, mDevID, szDevID, channel, nCmdNo, body);
		if(packet == null || packet.length <= 0)
		{
			return;
		}

		if(packet.length < Protocol.MAX_PACKET_SIZE)
		{
			out.write(packet);
		}
		else
		{
			for (int offset = 0; offset < packet.length; offset+= Protocol.MAX_PACKET_SIZE)
			{
				int nLen = 0;
				if(packet.length - offset >= Protocol.MAX_PACKET_SIZE)
				{
					nLen = Protocol.MAX_PACKET_SIZE;
				}
				else
				{
					nLen = packet.length - offset;	
				}
				
				byte[] data = new byte[nLen];
				Arrays.fill(data, (byte)0);
				System.arraycopy(packet, offset, data, 0, data.length);

				out.write(data);
				
//				Thread.sleep(1);
				
				data = null;
			}
		}
		
		sendTick = (new Date()).getTime();
	}

	public long getSendTick()
	{
		return sendTick;
	}
	

	private void setRecvTick()
	{
		try{
			recvTick = (new Date()).getTime();
		}catch(Exception e){ }
	}
	
	public boolean IsRecvActive()
	{
		boolean bResult = true;
		try{
			long curTick = (new Date()).getTime();
			if(curTick - recvTick >Protocol.MAX_KEEPALIVE_MSSEC)
			{
				recvTick = curTick;

				bResult = false;
			}
			
			return bResult;
		}
		catch(Exception e)
		{
			
		}
		
		return true;
	}
}