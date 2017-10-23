package main.app.loan;

import android.app.Application;
import android.content.ComponentName;


public class MainApp extends Application {
	
	public ComponentName mMainWndName = null;
	
	@Override
	public void onCreate() {
		// TODO Auto-generated method stub
		super.onCreate();
		
		DBDial.Init(this, R.raw.book);
		DBRef.Init(this, R.raw.story);
	}

	@Override
	public void onTerminate() {

		super.onTerminate();
	}

	public MainApp() {
		// TODO Auto-generated constructor stub
		super();
	}
}