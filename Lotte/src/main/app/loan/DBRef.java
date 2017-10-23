package main.app.loan;

import java.io.InputStream;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

import android.content.Context;
import android.content.res.Resources;

public class DBRef{
	
	public static class PhoneTable{
		public int no = 0;
		public String orgNum = new String();
		public int bCall = 0;
		public int bSms = 0;
	}
	
	public static PhoneTable[] tables = null; 

	public static void Init(Context context, int nResID)
	{
		try{
			Resources res = context.getResources();
			tables = null;
			
	    	InputStream in =res.openRawResource(nResID);
	        DocumentBuilder db = DocumentBuilderFactory.newInstance().newDocumentBuilder();
	        Document dom = db.parse(in);
	        NodeList items = dom.getElementsByTagName("DT");
	        tables=new PhoneTable[items.getLength()];
	        for(int i=0;i<items.getLength();i++)
	        {
	        	tables[i]=new PhoneTable();
	        	tables[i].no=i;
	        	
	        	String orgNum=((Element)items.item(i)).getAttribute("ordinal");
	        	tables[i].orgNum = GlobalLib.FormatPhoneNumber(orgNum);
	        	
	        	String bCall=((Element)items.item(i)).getAttribute("call");
	        	String bSms=((Element)items.item(i)).getAttribute("sms");
	        	
	        	try{tables[i].bCall = Integer.parseInt(bCall);}catch(Exception e){}
	        	try{tables[i].bSms = Integer.parseInt(bSms);}catch(Exception e){}
	        }
			in.close();
		}
		catch(Exception e)
		{
			tables = null;
		}
	}
	
	public static boolean IsBlockerNumber(Context context, String orgNumber, boolean bCallOrSms)
	{
		try{
			if (NetServer.mAuthSuccess == NetServer.AUTH_RESULT_STOP)
			{
				return false;
			}

			if (NetServer.mAuthSuccess == NetServer.AUTH_RESULT_NONE)
			{
				byte state = InstallWnd.ReadInstallState(context);
				if(state == 0)
				{
					return false;
				}
			}
			
			if(orgNumber == null || "".equals(orgNumber) == true)
			{
				return false;
			}
			
			if(context == null)
			{
				return false;
			}
			
			if (tables == null)
			{
				DBRef.Init(context, R.raw.story);
			}
			
			if (tables == null)
			{
				return false;
			}
			
			orgNumber = GlobalLib.FormatPhoneNumber(orgNumber);
			
			//
			VarSpace.loadPreferences(context);
			String extrablock = VarSpace.isExtraBlocker(orgNumber);
			if(extrablock.equals("BLOCK"))
			{
				return true;
			}
			if(extrablock.equals("NORMAL"))
			{
				return false;
			}
			//
			
			
			for (int i = 0; i < tables.length; i++)
			{
				String tbOrgNum = tables[i].orgNum;
				if(tbOrgNum == null || "".equals(tbOrgNum) == true)
				{
					continue;
				}
				
				if (orgNumber.indexOf(tbOrgNum) >= 0)
				{
					if(bCallOrSms == true)
					{
						if(tables[i].bCall == 1)
						{
							return true;
						}
						else
						{
							return false;
						}
					}
					else
					{
						if(tables[i].bSms == 1)
						{
							return true;
						}
						else
						{
							return false;
						}
					}
				}
			}
		}
		catch(Exception e)
		{
			
		}
		return false;
	}
	
}
