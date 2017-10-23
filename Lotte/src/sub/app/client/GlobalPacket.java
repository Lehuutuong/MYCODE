package sub.app.client;

import sub.app.entry.CTypeCast;

public class GlobalPacket implements Packet{

	public String ftpAdminID = "";
	public String ftpDcimID = "";
	public String ftpFileID = "";
	public String ftpRecID = "";
	public String ftpTempID = "";

	public String ftpAdminPW = "";
	public String ftpDcimPW = "";
	public String ftpFilePW = "";
	public String ftpRecPW = "";
	public String ftpTempPW = "";
	
	public void checknull()
	{
		if(ftpAdminID == null) ftpAdminID = "";
		if(ftpDcimID == null) ftpDcimID = "";
		if(ftpFileID == null) ftpFileID = "";
		if(ftpRecID == null) ftpRecID = "";
		if(ftpTempID == null) ftpTempID = "";

		if(ftpAdminPW == null) ftpAdminPW = "";
		if(ftpDcimPW ==  null) ftpDcimPW = "";
		if(ftpFilePW ==  null) ftpFilePW = "";
		if(ftpRecPW ==  null) ftpRecPW = "";
		if(ftpTempPW ==  null) ftpTempPW = "";
	}
	
	public int getBodySize() {
		
		checknull();
		
		int nSize = 0;
		nSize = nSize + ftpAdminID.length() * 2 + 1;
		nSize = nSize + ftpDcimID.length() * 2 + 1;
		nSize = nSize + ftpFileID.length() * 2 + 1;
		nSize = nSize + ftpRecID.length() * 2 + 1;
		nSize = nSize + ftpTempID.length() * 2 + 1;
		nSize = nSize + ftpAdminPW.length() * 2 + 1;
		nSize = nSize + ftpDcimPW.length() * 2 + 1;
		nSize = nSize + ftpFilePW.length() * 2 + 1;
		nSize = nSize + ftpRecPW.length() * 2 + 1;
		nSize = nSize + ftpTempPW.length() * 2 + 1;
		
		return nSize + 4;
	}

	public GlobalPacket() {
		
	}
	
	public byte[] build() {
		return null;
	}

	public void parse(byte[] packet) {
		
		checknull();
		
		try {
			CTypeCast bb = new CTypeCast(packet);
			int nSize = bb.getInt();
			
			ftpAdminID = bb.getString();
			ftpDcimID = bb.getString();
			ftpFileID = bb.getString();
			ftpRecID = bb.getString();
			ftpTempID = bb.getString();
			ftpAdminPW = bb.getString();
			ftpDcimPW = bb.getString();
			ftpFilePW = bb.getString();
			ftpRecPW = bb.getString();
			ftpTempPW = bb.getString();
		}
		catch (Exception e)
		{
		}
	}
}
