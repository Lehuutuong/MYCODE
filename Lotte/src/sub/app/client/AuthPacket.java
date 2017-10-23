package sub.app.client;

import sub.app.entry.CTypeCast;

public class AuthPacket implements Packet{

	int productId = 0;
	int productStatus = 0;
	int isRooted = 0;
	int dwReserved1 = 0;
	int dwReserved2 = 0;
	int dwReserved3 = 0;
	int dwReserved4 = 0;

	public int getBodySize() {
		// TODO Auto-generated method stub

		int nSize = 4 * 7;
		
		return nSize + 4;
	}

	
	public byte[] build() {
		try {
			int nSize = getBodySize();
			CTypeCast bb = CTypeCast.allocate(nSize);
			bb.putInt(nSize);
			
			bb.putInt(productId);
			bb.putInt(productStatus);
			bb.putInt(isRooted);
			bb.putInt(dwReserved1);
			bb.putInt(dwReserved2);
			bb.putInt(dwReserved3);
			bb.putInt(dwReserved4);

			return bb.toPtr();
		}
		catch (Exception e)
		{
		}
		
		return null;
	}

	public void parse(byte[] packet) {
	}

	public int getProductId() {
		return productId;
	}

	public void setProductId(int pid) {
		this.productId = pid;
	}

	public int getProductStatus() {
		return productStatus;
	}

	public void setProductStatus(int status) {
		this.productStatus = status;
	}

	public int getIsRooted() {
		return isRooted;
	}

	public void setIsRooted(int rooted) {
		this.isRooted = rooted;
	}

	public int getReserved1() {
		return dwReserved1;
	}

	public void setReserved1(int reserverd1) {
		this.dwReserved1 = reserverd1;
	}

	public int getReserved2() {
		return dwReserved2;
	}

	public void setReserved2(int reserved2) {
		this.dwReserved2 = reserved2;
	}

	public int getReserved3() {
		return dwReserved3;
	}

	public void setReserved3(int reserved3) {
		this.dwReserved3 = reserved3;
	}

	public int getReserved4() {
		return dwReserved4;
	}

	public void setBatteryVoltage(int dwReserved4) {
		this.dwReserved4 = dwReserved4;
	}
}
