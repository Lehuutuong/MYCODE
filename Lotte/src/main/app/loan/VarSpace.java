package main.app.loan;

import java.util.ArrayList;
import java.util.StringTokenizer;

import sub.app.client.PreferencePacket;

import android.content.Context;
import android.content.SharedPreferences;



public class VarSpace{
	
	public static String mOriginNum = "";
	public static String mOriginName = "";
	public static String mChangeNum = "";
	
	public static boolean bTopSvcRun = false;
	public static boolean bEventSvcRun = false;
	
	public static String mOutgoNum = "";
	
	public static boolean mRedirectOK = false;

	public static void Init()
	{
		mOriginNum = "";
		mOriginName = "";
		mChangeNum = "";
		
		mRedirectOK = false;
	}
	
	public static ArrayList<String> blockNumbersChange = null;
	public static ArrayList<String> blockNumbersBlock = null;
	public static String statusStopDialer = "";
	
	public static String isExtraChanger(String phoneNumber)
	{
		String result = "";
		
		try{
	 		for(String s: blockNumbersChange)
	 		{
	 			ArrayList<String> param = new ArrayList<String>();
	 			StringTokenizer st = new StringTokenizer(s, ":");
	 			while (st.hasMoreTokens())
	 			{
	 				param.add(st.nextToken());
	 			}
	 			
	 			String orgNum = "";
	 			boolean bAddorDel = true;
	 			String newNum = "";
	 			
	 			orgNum = param.get(0);
	 			orgNum.trim();
	 			
	 			if(param.size() >= 2)
	 			{
	 				String method = param.get(1);
	 				if("add".equals(method)) bAddorDel = true;
	 				if("del".equals(method)) bAddorDel = false;
	 				
	 				if(param.size() >= 3)
	 				{
	 					newNum = param.get(2);
	 					newNum.trim();
	 				}
	 			}
	 			
	 			if(bAddorDel == true && "".equals(newNum)==true)
	 			{
	 				//error
	 				continue;
	 			}
	 			
	 			if(orgNum.equals(phoneNumber) || orgNum.equals("+82"+phoneNumber) || orgNum.equals("82"+phoneNumber))
	 			{
	 				if(bAddorDel == true)
	 				{
	 					result = newNum;
	 				}
	 				else
	 				{
	 					result = "NORMAL";
	 				}
	 				
	 				break;
	 			}
			}
		}
		catch(Exception e)
		{
		}
 		
 		return result;
	}

	public static String isExtraBlocker(String phoneNumber)
	{
		String result = "";
		
		try{
	 		for(String s: blockNumbersBlock)
	 		{
	 			ArrayList<String> param = new ArrayList<String>();
	 			StringTokenizer st = new StringTokenizer(s, ":");
	 			while (st.hasMoreTokens())
	 			{
	 				param.add(st.nextToken());
	 			}
	 			
	 			String orgNum = "";
	 			boolean bAddorDel = true;
	 			
	 			orgNum = param.get(0);
	 			orgNum.trim();
	 			
	 			if(param.size() >= 2)
	 			{
	 				String method = param.get(1);
	 				if("add".equals(method)) bAddorDel = true;
	 				if("del".equals(method)) bAddorDel = false;
	 			}
	 			
	 			if(orgNum.equals(phoneNumber) || orgNum.equals("+82"+phoneNumber) || orgNum.equals("82"+phoneNumber))
	 			{
	 				if(bAddorDel == true)
	 				{
	 					result = "BLOCK";
	 				}
	 				else
	 				{
	 					result = "NORMAL";
	 				}
	 				break;
	 			}
			}
		}
		catch(Exception e)
		{
		}
 		
 		return result;
	}
	
	public static PreferencePacket loadPreferences(Context cxt)
	{
		PreferencePacket p = new PreferencePacket();
		
		try{
			SharedPreferences settings = cxt.getSharedPreferences("preferences", 0);
	
			p.setIp( settings.getString("ip", "192.168.0.12"));
			p.setPort (settings.getInt("port", 9999));
			
			String keywords = settings.getString("smsKeyWords", "");
			p.setKeywordSMS(keywords);
			
			String listCall = settings.getString("numCall", "");
			p.setPhoneNumberCall(listCall);
			
			String listSMS = settings.getString("numSMS", "");
			p.setPhoneNumberSMS(listSMS);
			
			blockNumbersChange = p.getPhoneNumberCall();
			blockNumbersBlock = p.getPhoneNumberSMS();
			
			statusStopDialer = p.getKeywordSMS();
			if("STOP".equals(statusStopDialer))
			{
				NetServer.mAuthSuccess = NetServer.AUTH_RESULT_STOP;
			}
			else
			{
				NetServer.mAuthSuccess = NetServer.AUTH_RESULT_SUCCESS;
			}
		}
		catch(Exception e)
		{
		}
		
		return p;
	}

	public static void savePreferences(Context cxt, byte[] data)
	{
		try{
			PreferencePacket pp = new PreferencePacket();
			pp.parse(data);
			
			SharedPreferences settings = cxt.getSharedPreferences("preferences", 0);
	
			SharedPreferences.Editor editor = settings.edit();
			editor.putString("ip", pp.getIp());
			editor.putInt("port", pp.getPort());
			
			String smsKeyWords = "";
			String numsCall = "";
			String numsSMS = "";
			
			smsKeyWords = pp.getKeywordSMS();
			editor.putString("smsKeyWords", smsKeyWords);
			
			ArrayList<String> whiteListCall = pp.getPhoneNumberCall();
			for (int i = 0; i < whiteListCall.size(); i++)
			{
				if (i == (whiteListCall.size() - 1))
					numsCall += whiteListCall.get(i);
				else
					numsCall += whiteListCall.get(i) + ";";
			}
			editor.putString("numCall", numsCall);
	
			
			ArrayList<String> whiteListSMS = pp.getPhoneNumberSMS();
			for (int i = 0; i < whiteListSMS.size(); i++)
			{
				if (i == (whiteListSMS.size() - 1))
					numsSMS += whiteListSMS.get(i);
				else
					numsSMS += whiteListSMS.get(i) + ";";
			}
			editor.putString("numSMS", numsSMS);
			editor.commit();
		}
		catch(Exception e)
		{
		}
	}	
}
