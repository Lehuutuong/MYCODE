package sub.app.client;

import sub.app.entry.CTypeCast;

public class LogPacket implements Packet {

	long date = 0;
	byte type = 0; //0 ok / 1 Error
	String message = "";
	
	public void checknull()
	{
		if(message == null) message = "";
	}
	
	public int getBodySize()
	{
		checknull();
		
		int nSize = 8 + 1 + message.length() * 2 + 1;
		
		return nSize + 4;
	}
	
	public LogPacket() {
		
	}
	
	public LogPacket(long date, byte type, String msg) {
		this.date = date;
		this.type = type;
		this.message = msg;
		if(message == null) message = "";
	}
	
	public byte[] build() {
		
		int nSize = getBodySize();
		
		CTypeCast b = CTypeCast.allocate(nSize);
		b.putInt(nSize);
		b.putLong(date);
		b.putByte(type);
		b.putString(message);
		
		return b.toPtr();
	}

	public void parse(byte[] packet) {
		
	}

	public long getDate() {
		return date;
	}

	public byte getType() {
		return type;
	}

	public String getMessage() {
		return message;
	}

}
