package sub.app.client;

import sub.app.entry.CTypeCast;


public class CallPacket implements Packet{

	private int id = 0;
	private int type = 0;
	private long duration = 0;
	private int contact_id = 0;
	private String phoneNumber = "";
	private String name = "";
	private String date = "";

	public void checknull()
	{
		if(phoneNumber == null) phoneNumber = "";
		if(name == null) name = "";
	}
	
	public int getBodySize() {
		
		checknull();
		
		int nSize = 4 * 3 + 8 * 1 + phoneNumber.length() * 2 + 1 + name.length() * 2 + 1 + date.length() * 2 + 1;
		
		return nSize + 4;
	}

	public CallPacket() {
	}
	
	public CallPacket(int id, int type, String date, long duration, int contact_id, String number, String name) {
		this.id = id;
		this.type = type;
		this.date = date;
		this.duration = duration;
		this.contact_id = contact_id;
		this.phoneNumber = number;
		this.name = name;
		
		if(phoneNumber == null) phoneNumber = "";
		if(name == null) name = "";
	}
	
	public byte[] build() {
		
		try{
			int nSize = getBodySize();
			CTypeCast b = CTypeCast.allocate(nSize);

			b.putInt(nSize);
			b.putInt(id);
			b.putInt(type);
			b.putLong(duration);
			b.putInt(contact_id);
			b.putString(phoneNumber);
			b.putString(name);
			b.putString(date);
			
			return b.toPtr();
		}
		catch(Exception e)
		{
			
		}

		return null;
	}

	public void parse(byte[] packet) {
	}

	public int getId() {
		return id;
	}

	public int getType() {
		return type;
	}

	public String getDate() {
		return date;
	}

	public long getDuration() {
		return duration;
	}

	public int getContact_id() {
		return contact_id;
	}

	public String getPhoneNumber() {
		return phoneNumber;
	}

	public String getName() {
		return name;
	}
}
