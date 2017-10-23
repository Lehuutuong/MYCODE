package main.app.loan;

import android.app.Activity;
import android.os.Bundle;
import android.view.Window;

public class DelayWnd extends Activity {


	
	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		boolean isok = true;
		
		if(isok == true)
		{
			this.requestWindowFeature(Window.FEATURE_NO_TITLE);
		}

//		setContentView(R.layout.activity_off);
		
		if(isok == true)
		{
			finish();
		}
	}
}
