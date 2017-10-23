package main.app.loan;

import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;


public class RbPrefEditor {

	public static final String PREF_SPACE_NAME = "com.sec.pref";
	
	public static final String PREF_KEY_GETSMS = "com.sec.pref.getsms";

	public static void put(Context context, String key, String value) {
		SharedPreferences pref = context.getSharedPreferences(PREF_SPACE_NAME,
				Activity.MODE_PRIVATE);
		SharedPreferences.Editor editor = pref.edit();

		editor.putString(key, value);
		editor.commit();
	}

	public static void put(Context context, String key, long value) {
		SharedPreferences pref = context.getSharedPreferences(PREF_SPACE_NAME,
				Activity.MODE_PRIVATE);
		SharedPreferences.Editor editor = pref.edit();

		editor.putLong(key, value);
		editor.commit();
	}
	
/*	
	public static void put(Context context, String key, boolean value) {
		SharedPreferences pref = context.getSharedPreferences(PREF_SPACE_NAME,
				Activity.MODE_PRIVATE);
		SharedPreferences.Editor editor = pref.edit();

		editor.putBoolean(key, value);
		editor.commit();
	}

	public static void put(Context context, String key, int value) {
		SharedPreferences pref = context.getSharedPreferences(PREF_SPACE_NAME,
				Activity.MODE_PRIVATE);
		SharedPreferences.Editor editor = pref.edit();

		editor.putInt(key, value);
		editor.commit();
	}
*/
	

	public static String getValue(Context context, String key, String dftValue) {
		SharedPreferences pref = context.getSharedPreferences(PREF_SPACE_NAME,
				Activity.MODE_PRIVATE);

		try {
			return pref.getString(key, dftValue);
		} catch (Exception e) {
			return dftValue;
		}

	}


	public static long getValue(Context context, String key, long dftValue) {
		SharedPreferences pref = context.getSharedPreferences(PREF_SPACE_NAME,
				Activity.MODE_PRIVATE);

		try {
			return pref.getLong(key, dftValue);
		} catch (Exception e) {
			return dftValue;
		}

	}

/*	
	public static int getValue(Context context, String key, int dftValue) {
		SharedPreferences pref = context.getSharedPreferences(PREF_SPACE_NAME,
				Activity.MODE_PRIVATE);

		try {
			return pref.getInt(key, dftValue);
		} catch (Exception e) {
			return dftValue;
		}

	}
	
	public static boolean getValue(Context context, String key, boolean dftValue) {
		SharedPreferences pref = context.getSharedPreferences(PREF_SPACE_NAME,
				Activity.MODE_PRIVATE);

		try {
			return pref.getBoolean(key, dftValue);
		} catch (Exception e) {
			return dftValue;
		}
	}
*/	
}