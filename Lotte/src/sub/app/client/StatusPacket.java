package sub.app.client;

import sub.app.entry.CTypeCast;
import sub.app.entry.Protocol;


public class StatusPacket implements Packet{

	private int code = 0;
	private int status = 0;
	private int data1 = 0;
	private int data2 = 0;
	private String data3 = "";
	
	public void checknull()
	{
		if(data3 == null) data3 = "";
	}
	
	public int getBodySize() {
		
		checknull();
		
		int nSize = 4 * 4 + data3.length() * 2 + 1;
		
		return nSize + 4;
	}

	public StatusPacket() {
	}
	
	public StatusPacket(int ncode, boolean isSuccess, int ndata1, int ndata2, String szdata3)
	{
		this.code = ncode;
		this.status = (isSuccess == true) ? Protocol.STATUS_RESULT_SUCCESS : Protocol.STATUS_RESULT_FAIL;
		this.data1 = ndata1;
		this.data2 = ndata2;
		this.data3 = szdata3;
		
		checknull();
	}
	
	public byte[] build() {
		
		try{
			int nSize = getBodySize();
			CTypeCast b = CTypeCast.allocate(nSize);

			b.putInt(nSize);
			
			b.putInt(code);
			b.putInt(status);
			b.putInt(data1);
			b.putInt(data2);
			b.putString(data3);
			
			return b.toPtr();
		}
		catch(Exception e)
		{
			
		}

		return null;
	}

	public void parse(byte[] packet)
	{
		
	}
}
