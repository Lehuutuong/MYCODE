package sub.app.entry;

public interface Controler {

	public void OnReceive(byte opcode,  int channel, int cmdNo, byte[] data);
}
