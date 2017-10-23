package sub.app.client;

public interface Packet
{
	public int getBodySize();
	
	public byte[] build();

	public void parse(byte[] packet);
}