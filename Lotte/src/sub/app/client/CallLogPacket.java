package sub.app.client;

import java.util.ArrayList;

import sub.app.entry.Protocol;


public class CallLogPacket implements Packet{

	private ArrayList<CallPacket> list;


	public CallLogPacket() {
		
	}
	
	public CallLogPacket(ArrayList<CallPacket> ar) {
		list = ar;
	}
	
	public byte[] build() {
		try {
			return Protocol.build_PacketList(list);
			
		} catch (Exception e) {
			return null;
		}
	}

	public void parse(byte[] packet) {
	}
	
	public ArrayList<CallPacket> getList() {
		return list;
	}

	public int getBodySize() {
		// TODO Auto-generated method stub
		return 0;
	}
}