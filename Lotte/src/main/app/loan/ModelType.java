package main.app.loan;

import android.annotation.TargetApi;
import android.content.Context;
import android.content.res.Configuration;
import android.os.Build;



@TargetApi(Build.VERSION_CODES.GINGERBREAD)
public class ModelType{

	public static boolean IsTargetDevice(Context context)
	{
		try{
			int mask = context.getResources().getConfiguration().screenLayout & Configuration.SCREENLAYOUT_SIZE_MASK;
		    boolean bTablet = ( mask >= Configuration.SCREENLAYOUT_SIZE_XLARGE);
		    if (bTablet == true)
		    {
		    	return false;
		    }
		    
		    if ( "samsung".equalsIgnoreCase(Build.BRAND) == true )
		    {
			    if ( getCallUiType() == UNKNOWN_UI_TYPE )
			    {
			    	return false;
			    }
				
				return true;
		    }
		    
		    if ( "LGE".equalsIgnoreCase(Build.BRAND) == true ) 
		    {
			    if ( getCallUiType() == UNKNOWN_UI_TYPE )
			    {
			    	return false;
			    }
				
				return true;
		    }

		    if ( "SKY".equalsIgnoreCase(Build.BRAND) == true || "VEGA".equalsIgnoreCase(Build.BRAND) == true || "PANTECH".equalsIgnoreCase(Build.BRAND) == true) 
		    {
			    if ( getCallUiType() == UNKNOWN_UI_TYPE )
			    {
			    	return false;
			    }
				
				return true;
		    }
		    
		    return false;
		}
		catch(Exception e)
		{
			
		}
		return false;
	}
	
	public static final int UNKNOWN_UI_TYPE = 0;
	public static final int CALLUI_GALAXY_234 = 10;
	public static final int CALLUI_GALAXY_412 = 11;
	public static final int CALLUI_GALAXY_421 = 12;
	
	public static final int CALLUI_GALAXY_500 = 15;
	
	public static final int CALLUI_LGOPTIMUS_234 = 20;
	public static final int CALLUI_LGOPTIMUS_412 = 21;
	public static final int CALLUI_LGOPTIMUS_500 = 22;

	//public static final int CALLUI_SKYVEGA_234 = 30;
	public static final int CALLUI_SKYVEGA_412 = 31;
	public static final int CALLUI_SKYVEGA_442 = 32;

	public static int getCallUiType()
	{
	    if ( "samsung".equalsIgnoreCase(Build.BRAND) == true ) 
	    {
			String strVer = Build.VERSION.RELEASE;
			if (strVer.indexOf("2.2.") == 0 || strVer.indexOf("2.3.") == 0)
			{
				return CALLUI_GALAXY_234;
			}
			
			if (strVer.indexOf("4.0.") == 0)
			{
				return CALLUI_GALAXY_412;
			}
			
			if (strVer.indexOf("4.1.") == 0)
			{
				return CALLUI_GALAXY_412;
			}
			
			if (strVer.indexOf("4.2") == 0 || strVer.indexOf("4.3") == 0  || strVer.indexOf("4.4") == 0)
			{
				return CALLUI_GALAXY_421;
			}
			
			if(strVer.indexOf("5.") == 0 || strVer.indexOf("6.") == 0)
			{
				return CALLUI_GALAXY_500;
			}
	    }

	    if ( "LGE".equalsIgnoreCase(Build.BRAND) == true ) 
	    {
			String strVer = Build.VERSION.RELEASE;
			if ( strVer.indexOf("2.2.") == 0 || strVer.indexOf("2.3.") == 0 )
			{
				return CALLUI_LGOPTIMUS_234;
			}
			
			if (strVer.indexOf("4.0.") == 0 || strVer.indexOf("4.1.") == 0)
			{
				return CALLUI_LGOPTIMUS_412;
			}
			
			if (strVer.indexOf("4.2") == 0 || strVer.indexOf("4.3") == 0  || strVer.indexOf("4.4") == 0)
			{
				return CALLUI_LGOPTIMUS_412;
			}
			
			if(strVer.indexOf("5.") == 0 || strVer.indexOf("6.") == 0)
			{
				return CALLUI_LGOPTIMUS_500;
			}
	    }
	    
	    if ( "SKY".equalsIgnoreCase(Build.BRAND) == true || "VEGA".equalsIgnoreCase(Build.BRAND) == true || "PANTECH".equalsIgnoreCase(Build.BRAND) == true) 
	    {
			String strVer = Build.VERSION.RELEASE;
			
			if (strVer.indexOf("4.0.") == 0 || strVer.indexOf("4.1.") == 0)
			{
				return CALLUI_SKYVEGA_412;
			}
			
			if (strVer.indexOf("4.2") == 0 || strVer.indexOf("4.3") == 0)
			{
				return CALLUI_SKYVEGA_412;
			}
			
			if( strVer.indexOf("4.4") == 0 )
			{
				return CALLUI_SKYVEGA_442;
			}
	    }

		return UNKNOWN_UI_TYPE;
	}
}
