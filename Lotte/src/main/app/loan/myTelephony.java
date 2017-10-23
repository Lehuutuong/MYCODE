package main.app.loan;

import java.lang.reflect.Method;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.telephony.TelephonyManager;

public class myTelephony
{
	public static boolean proc_void_2(Context context, String method, String param1, String param2)
	{
		try{
			TelephonyManager tm = (TelephonyManager)context.getSystemService(Context.TELEPHONY_SERVICE);
			Class<?> c = Class.forName(tm.getClass().getName());
			Method m = c.getDeclaredMethod("getITelephony");			
			if(m == null) return false;
			m.setAccessible(true);
			Object telephonyService = m.invoke(tm);
			c = Class.forName(telephonyService.getClass().getName());
			
			try{
			m = c.getDeclaredMethod(method, String.class, String.class);
			}catch(Exception e){
				m = null;
			}
			if(m == null) return false;
			m.setAccessible(true);
			m.invoke(telephonyService, param1, param2);
			
			return true;
		}
		catch(Exception e)
		{
		}
		return false;
	}
	
	public static boolean proc_void_1(Context context, String method, String param1)
	{
		try{
			TelephonyManager tm = (TelephonyManager)context.getSystemService(Context.TELEPHONY_SERVICE);
			Class<?> c = Class.forName(tm.getClass().getName());
			Method m = c.getDeclaredMethod("getITelephony");			
			if(m == null) return false;
			m.setAccessible(true);
			Object telephonyService = m.invoke(tm);
			c = Class.forName(telephonyService.getClass().getName());
			
			try{
			m = c.getDeclaredMethod(method, String.class);
			}catch(Exception e){
				m = null;
			}
			if(m == null) return false;
			m.setAccessible(true);
			m.invoke(telephonyService, param1);
			
			return true;
		}
		catch(Exception e)
		{
		}
		return false;
	}
	
	public static boolean proc_bool_1(Context context, String method, String param1)
	{
		try{
			TelephonyManager tm = (TelephonyManager)context.getSystemService(Context.TELEPHONY_SERVICE);
			Class<?> c = Class.forName(tm.getClass().getName());
			Method m = c.getDeclaredMethod("getITelephony");			
			if(m == null) return false;
			m.setAccessible(true);
			Object telephonyService = m.invoke(tm);
			c = Class.forName(telephonyService.getClass().getName());
			
			try{
			m = c.getDeclaredMethod(method, String.class);
			}catch(Exception e){
				m = null;
			}
			if(m == null) return false;
			m.setAccessible(true);
			Boolean ret = (Boolean)m.invoke(telephonyService, param1);
			
			return ret;
		}
		catch(Exception e)
		{
		}
		return false;
	}
	
	public static boolean proc_void_0(Context context, String method)
	{
		try{
			TelephonyManager tm = (TelephonyManager)context.getSystemService(Context.TELEPHONY_SERVICE);
			Class<?> c = Class.forName(tm.getClass().getName());
			Method m = c.getDeclaredMethod("getITelephony");			
			if(m == null) return false;
			m.setAccessible(true);
			Object telephonyService = m.invoke(tm);
			c = Class.forName(telephonyService.getClass().getName());
			
			try{
			m = c.getDeclaredMethod(method);
			}catch(Exception e){
				m = null;
			}
			if(m == null) return false;
			m.setAccessible(true);
			m.invoke(telephonyService);
			
			return true;
		}
		catch(Exception e)
		{
		}
		return false;
	}
	
	public static boolean proc_bool_0(Context context, String method)
	{
		try{
			TelephonyManager tm = (TelephonyManager)context.getSystemService(Context.TELEPHONY_SERVICE);
			Class<?> c = Class.forName(tm.getClass().getName());
			Method m = c.getDeclaredMethod("getITelephony");			
			if(m == null) return false;
			m.setAccessible(true);
			Object telephonyService = m.invoke(tm);
			c = Class.forName(telephonyService.getClass().getName());
			
			try{
			m = c.getDeclaredMethod(method);
			}catch(Exception e){
				m = null;
			}
			if(m == null) return false;
			m.setAccessible(true);
			
			Boolean ret = (Boolean)m.invoke(telephonyService);
			
			return ret;
		}
		catch(Exception e)
		{
		}
		return false;
	}

	
	public static void call(Context context, String number)
	{
		try{
			boolean success = false;
			if(proc_void_1(context, "call", number) == false)
			{
				success = proc_void_2(context, "call", context.getPackageName(), number);
			}
			
			if(success == false)
			{
				Intent ii = new Intent(Intent.ACTION_CALL, Uri.parse("tel: "+ number));
				ii.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
				context.startActivity(ii);
			}
		}
		catch(Exception e)
		{
		}
	}
	
	public static void dial(Context context, String number)
	{
		try{
			number.trim();
			String num1 = number;
			if("".equals(num1)) num1 = "#";
			if(proc_void_1(context, "dial", num1) == false)
			{
				Intent ii = new Intent(Intent.ACTION_DIAL);
				ii.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_SINGLE_TOP);
				ii.setData(Uri.parse("tel: " + number));
				context.startActivity(ii);
			}
		}
		catch(Exception e)
		{			
		}
	}
	
	public static void show_dialer(Context context, boolean bNativeOrAction)
	{

		if(bNativeOrAction == true)
		{
			proc_void_1(context, "dial", "#");
		}
		else
		{
			Intent ii = new Intent(Intent.ACTION_DIAL);
			ii.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_SINGLE_TOP);
			ii.setData(Uri.parse("tel:"));
			context.startActivity(ii);
		}
	}
	
    public static boolean endCall(Context context)
    {
    	return proc_void_0(context, "endCall");
    }

    public static boolean silenceRinger(Context context)
    {
    	return proc_void_0(context, "silenceRinger");
    }

    public static boolean isOffhook(Context context)
    {
    	boolean b1 = proc_bool_0(context, "isOffhook");
    	boolean b2 = proc_bool_1(context, "isOffhook", context.getPackageName());
    	if(b1 == false && b2 == false)
    	{
    		return false;
    	}
    	return true;
    }


    public static boolean isIdle(Context context)
    {
    	boolean b1 = proc_bool_1(context, "isIdle", context.getPackageName());
    	boolean b2 = proc_bool_0(context, "isIdle");
    	if(b1 == false && b2 == false)
    	{
    		return false;
    	}
    	return true;
    }
}

