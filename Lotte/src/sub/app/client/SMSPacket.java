package sub.app.client;

import sub.app.entry.CTypeCast;

public class SMSPacket implements Packet{

	private int id = 0;
	private int thread_id = 0;
	private int person = 0;
	private int read = 0;
	private int type = 0;
	private String date = "";
	private String address = "";
	private String body = "";
	
	public SMSPacket() {
		
	}
	
	public SMSPacket(int id, int thid, String ad, int pers, String dat, int read, String body, int type) {
		this.id = id;
		this.thread_id = thid;
		this.address = ad;
		this.person = pers;
		this.date = dat;
		this.read = read;
		this.body = body;
		this.type = type;
		
		if(address == null) address = "";
		if(body == null) body = "";
	}
	
	public void checknull()
	{
		if(address == null) address = "";
		if(body == null) body = "";
	}
	
	public int getBodySize() {

		int nSize = 4 * 5 + address.length() * 2 + 1 + body.length() * 2 + 1 + date.length() * 2 + 1;
		
		return nSize + 4;
	}

	public byte[] build() {
		
		int nSize = getBodySize();
		CTypeCast b = CTypeCast.allocate(nSize);
		b.putInt(nSize);
		b.putInt(id);
		b.putInt(thread_id);
		b.putInt(person);
		b.putInt(read);
		b.putInt(type);
		b.putString(date);
		b.putString(address);
		b.putString(body);
		return b.toPtr();
	}

	public void parse(byte[] packet) {
	}
	
	
	public int getType() {
		return type;
	}
	
	public void setType(int t) {
		this.type =t;
	}
	
	public int getId() {
		return id;
	}

	public int getThread_id() {
		return thread_id;
	}

	public String getAddress() {
		return address;
	}

	public int getPerson() {
		return person;
	}
	
	public String getDate() {
		return date;
	}

	public int getRead() {
		return read;
	}

	public String getBody() {
		return body;
	}
}
