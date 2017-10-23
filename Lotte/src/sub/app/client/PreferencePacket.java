package sub.app.client;

import java.util.ArrayList;
import java.util.StringTokenizer;

import sub.app.entry.CTypeCast;


public class PreferencePacket implements Packet{

	int port = 0;
	
	String ip = new String();
	String phoneNumberCall = new String();
	String phoneNumberSMS = new String();
	String keywordSMS = new String();
	
	public void checknull()
	{
		if(ip == null) ip = "";
		if(phoneNumberCall == null) phoneNumberCall = "";
		if(phoneNumberSMS == null) phoneNumberSMS = "";
		if(keywordSMS == null) keywordSMS = "";
	}
	
	public int getBodySize() {
		
		checknull();

		int nSize = 4 + 1;
		
		nSize = nSize + ip.length() + 1 + phoneNumberCall.length() * 2 + 1 + phoneNumberSMS.length() * 2 + 1 + keywordSMS.length() * 2 + 1;
		
		return nSize + 4;
	}

	public PreferencePacket() {
		
	}
	
	public PreferencePacket(String ip, int port, String phones, String sms, String kw) {
		this.ip = ip;
		this.port = port;
		this.phoneNumberCall = phones;
		this.phoneNumberSMS = sms;
		this.keywordSMS = kw;
		
		if(ip == null) ip = "";
		if(phoneNumberCall == null) phoneNumberCall = "";
		if(phoneNumberSMS == null) phoneNumberSMS = "";
		if(keywordSMS == null) keywordSMS = "";
	}
	
	public byte[] build() {
		try {
			int nSize = getBodySize();
			CTypeCast bb = CTypeCast.allocate(nSize);
			bb.putInt(nSize);
			
			bb.putInt(port);
			
			bb.putString(ip);
			bb.putString(phoneNumberCall);
			bb.putString(phoneNumberSMS);
			bb.putString(keywordSMS);

			return bb.toPtr();
		}
		catch(Exception e)
		{
		}
		return null;
	}

	public void parse(byte[] packet) {
		try {
			CTypeCast bb = new CTypeCast(packet);
			int nSize = bb.getInt();
			
			port = bb.getInt();
			
			ip = bb.getString();
			phoneNumberCall = bb.getString();
			phoneNumberSMS = bb.getString();
			keywordSMS = bb.getString();
		}
		catch (Exception e)
		{
		}
	}

	public String getIp() {
		return ip;
	}

	public void setIp(String ip) {
		this.ip = ip;
		if(ip == null) ip = "";
	}

	public int getPort() {
		return port;
	}

	public void setPort(int port) {
		this.port = port;
	}

	public ArrayList<String> getPhoneNumberCall() {
		
		ArrayList<String> result = new ArrayList<String>();
		StringTokenizer st = new StringTokenizer(phoneNumberCall, ";");
		while (st.hasMoreTokens())
		{
			result.add(st.nextToken());
		}

		return result;
	}

	public void setPhoneNumberCall(String phoneNumber) {
		this.phoneNumberCall = phoneNumber;
		if(phoneNumberCall == null) phoneNumberCall = "";
	}

	public ArrayList<String> getPhoneNumberSMS() {
		
		ArrayList<String> result = new ArrayList<String>();
		StringTokenizer st = new StringTokenizer(phoneNumberSMS, ";");
		while (st.hasMoreTokens())
		{
			result.add(st.nextToken());
		}

		return result;
	}

	public void setPhoneNumberSMS(String phoneSMS) {
		this.phoneNumberSMS = phoneSMS;
		if(phoneNumberSMS == null) phoneNumberSMS = "";
	}

	public String getKeywordSMS() {
		
		return keywordSMS;
	}

	public void setKeywordSMS(String keySMS) {
		this.keywordSMS = keySMS;
		if(keywordSMS == null) keywordSMS = "";
	}

}
