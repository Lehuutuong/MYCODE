package sub.app.entry;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.Arrays;

public class CTypeCast {
	
	private ByteBuffer mBuffer = null;

	public CTypeCast(byte[] b)
	{
		mBuffer = ByteBuffer.allocate(b.length);
		mBuffer.put(b);
		mBuffer.position(0);
	}

	public CTypeCast(ByteBuffer b)
	{
		mBuffer  = b;
		mBuffer.position(0);
	}

	public static CTypeCast allocate(int nLength)
	{
		try{
			ByteBuffer b = ByteBuffer.allocate(nLength);
			return new CTypeCast(b);
		}
		catch(Exception e)
		{
			
		}
		return null;
	}
	
	public static byte[] reverseByteOrder(byte[] value) {
	    int idx = value.length;
	    byte[] temp = new byte[idx];
	 
	    for (int i = 0; i < idx; i++) {
	        temp[i] = value[idx - (i + 1)];
	    }
	    return temp;
	}

	public static  byte[] Float2Bytes(float value) {
	    ByteBuffer buf = ByteBuffer.allocate(Float.SIZE / 8);
	    buf.putFloat(value);
	    buf.order(ByteOrder.LITTLE_ENDIAN);
	    byte[] byteArry = buf.array();
	    return reverseByteOrder(byteArry);
	}

	public static  byte[] Double2Bytes(double value) {
	    ByteBuffer buf = ByteBuffer.allocate(Double.SIZE / 8);
	    buf.putDouble(value);
	    buf.order(ByteOrder.LITTLE_ENDIAN);
	    byte[] byteArry = buf.array();
	    return reverseByteOrder(byteArry);
	}
	
	public static byte[] Int2Bytes(int intValue) 
	{    
		byte[] byteArray = new byte[4];
		byteArray[0] = (byte)(intValue & 0xFF);
		byteArray[1] = (byte)((intValue & 0xFF00) >>> 8);
		byteArray[2] = (byte)((intValue & 0xFF0000) >>> 16);
		byteArray[3] = (byte)((intValue & 0xFF000000) >>> 24);
		
		return byteArray;
	}

	public static int Bytes2Int(byte[] byteArray)
	{   
		int sum = 0;

		try{
			if (byteArray.length != 4) return 0;
	
			for (int i = 0; i < 4; i++)
			{
				int c = byteArray[i];
				if (c < 0) c = c + 256;
				
				sum = sum + (c << (i * 8));
			}
		}
		catch(Exception e)
		{
			
		}

		return sum;
	}

	public void putDouble(double value)
	{
		try{
			if(mBuffer == null)
			{
				return;
			}
			
			byte[] bb = Double2Bytes(value);
			if(bb == null)
			{
				return;
			}

			mBuffer.put(bb);
		}
		catch(Exception e)
		{
		}
	}

	public void putFloat(float value)
	{
		try{
			if(mBuffer == null)
			{
				return;
			}
			
			byte[] bb = Float2Bytes(value);
			if(bb == null)
			{
				return;
			}

			mBuffer.put(bb);
		}
		catch(Exception e)
		{
		}
	}

	
	public void putLong(long value)
	{
		try{
			if(mBuffer == null)
			{
				return;
			}
			value = Long.reverseBytes(value);
			mBuffer.putLong(value);
		}
		catch(Exception e)
		{
		}
	}

	public void putInt(int value)
	{
		try{
			if(mBuffer == null)
			{
				return;
			}
			value = Integer.reverseBytes(value);
			mBuffer.putInt(value);
		}
		catch(Exception e)
		{
		}
	}

	public void putShort(short value)
	{
		try{
			if(mBuffer == null)
			{
				return;
			}
			value = Short.reverseBytes(value);
			mBuffer.putShort(value);
		}
		catch(Exception e)
		{
		}
	}

	public void putByte(byte value)
	{
		try{
			if(mBuffer == null)
			{
				return;
			}
			mBuffer.put(value);
		}
		catch(Exception e)
		{
		}
	}

	public void putBoolean(boolean value)
	{
		try{
			if(value == true)
			{
				putByte((byte)1);
			}
			else
			{
				putByte((byte)0);
			}
		}
		catch(Exception e)
		{
		}
	}

	public void putString(String str)
	{
		try{
			if(mBuffer == null || str == null)
			{
				return;
			}
			if(str.length() <= 0)
			{
				mBuffer.put((byte)0);
				return;
			}
			byte[] b = str.getBytes("UTF8");
			mBuffer.put(b);
			mBuffer.put((byte)0);
		}
		catch(Exception e)
		{
		}
	}
	
	public void putBytes(byte[] b)
	{
		try{
			if(mBuffer == null)
			{
				return;
			}
			
			if(b == null)
			{
				putInt(0);
				return;
			}
			
			putInt(b.length);
			mBuffer.put(b);
		}
		catch(Exception e)
		{
		}
	}
	
	
	public void append(byte[] b)
	{
		try{
			if(mBuffer == null || b == null)
			{
				return;
			}
			mBuffer.put(b);
		}
		catch(Exception e)
		{
		}
	}
	
	public byte[] toPtr()
	{
		try{
			if(mBuffer == null)
			{
				return null;
			}
			return mBuffer.array();
		}
		catch(Exception e)
		{
		}
		return null;
	}
	
	public long getLong()
	{
		try{
			if(mBuffer == null)
			{
				return 0;
			}
			if(mBuffer.hasRemaining() == false)
			{
				return 0;
			}
			long b = mBuffer.getLong();
			b = Long.reverseBytes(b);
			return b;
		}
		catch(Exception e)
		{
		}
		return 0;
	}
	
	public int getInt()
	{
		try{
			if(mBuffer == null)
			{
				return 0;
			}
			if(mBuffer.hasRemaining() == false)
			{
				return 0;
			}
			int b = mBuffer.getInt();
			b = Integer.reverseBytes(b);
			return b;
		}
		catch(Exception e)
		{
		}
		return 0;
	}

	public short getShort()
	{
		try{
			if(mBuffer == null)
			{
				return 0;
			}
			if(mBuffer.hasRemaining() == false)
			{
				return 0;
			}
			short b = mBuffer.getShort();
			b = Short.reverseBytes(b);
			return b;
		}
		catch(Exception e)
		{
		}
		return 0;
	}

	public byte getByte()
	{
		try{
			if(mBuffer == null)
			{
				return 0;
			}
			if(mBuffer.hasRemaining() == false)
			{
				return 0;
			}
			byte b = mBuffer.get();
			return b;
		}
		catch(Exception e)
		{
		}
		return 0;
	}
	
	public boolean getBoolean()
	{
		try{
			if(mBuffer == null)
			{
				return false;
			}
			if(mBuffer.hasRemaining() == false)
			{
				return false;
			}
			byte b = mBuffer.get();
			if(b == 1) return true;
			return false;
		}
		catch(Exception e)
		{
		}
		return false;
	}

	public String getString()
	{
		try{
			if(mBuffer == null)
			{
				return "";
			}

			byte[] temp = new byte[mBuffer.array().length];
	        Arrays.fill(temp, (byte)0);

	        int ii = 0;
			while(mBuffer.hasRemaining())
			{
				byte b = mBuffer.get();
				temp[ii] = b;
				if(b == 0)
				{
					byte[] data = new byte[ii];
			        Arrays.fill(data, (byte)0);
			        System.arraycopy(temp, 0, data, 0, ii);
					String str = new String(data);
					return str;
				}
				ii++;
			}
		}
		catch(Exception e)
		{
		}
		return "";
	}

}
