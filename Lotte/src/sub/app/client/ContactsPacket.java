package sub.app.client;

import java.util.ArrayList;

import sub.app.entry.Protocol;


public class ContactsPacket implements Packet{

	ArrayList<Contact> list;

	public ContactsPacket() {
		
	}
	
	public ContactsPacket(ArrayList<Contact> ar) {
		list = ar;
	}
	
	public byte[] build() {
		try {
			byte[] bb = Protocol.build_PacketList(list);
			return bb;
		} catch (Exception e) {
		}
		return null;
	}

	public void parse(byte[] packet) {
	}
	
	public ArrayList<Contact> getList() {
		return list;
	}

	public int getBodySize() {
		// TODO Auto-generated method stub
		return 0;
	}
}