package sub.app.func;


import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

import org.apache.commons.net.ftp.FTP;
import org.apache.commons.net.ftp.FTPClient;

import sub.app.entry.NetModel;

import android.content.Context;



public class FtpManager {
	
	private	FTPClient ftp = null;
	
	public FtpManager()
	{
		ftp = new FTPClient();
	}
	
	private String mServer = "";
	private int mPort = 21;
	private String mId = "";
	private String mPass = "";
	private String mEncoding = ""; //UTF-8 or EUC-KR
	
	private String szDeviceNum = "";
	
	public void setAccount(String server, String id, String pass, Context cxt)
	{
		try{
			mServer = server;
			mPort = 21;
			mId = id;
			mPass = pass;
			mEncoding = "UTF-8";
			szDeviceNum = NetModel.GetDevSzUID(cxt);
		}
		catch(Exception e)
		{
		}
	}
	
	public boolean FtpLogin()
	{
		boolean loginResult = false;
		try {
			ftp.setControlEncoding(mEncoding);
			ftp.connect(mServer, mPort);
			loginResult = ftp.login(mId, mPass);
			ftp.enterLocalPassiveMode();
			ftp.setFileType(FTP.BINARY_FILE_TYPE);
//			ftp.makeDirectory(mDefaultWorkDirectory);
//			ftp.changeWorkingDirectory(mDefaultWorkDirectory);
		}
		catch(Exception e)
		{
//			Log.d("FTP_LOGIN_ERR", e.toString());
		}

		if (!loginResult)
		{
//			Log.d("FTP_LOGIN_ERR", "로그인 실패");
			return false;
		}
		 
//		Log.d("FPT_LOGIN_OK", "로그인 성공");
		 
		return true;
	}
	
	public void FtpLogout()
	{
		if(ftp == null) return;
		
		try{
			try {
				ftp.logout();
			}
			catch(IOException e)
			{
				// TODO Auto-generated catch block
//				Log.d("FTP", "ftp 접속 스트림 닫고 로그아웃중 오류 발생");
//				e.printStackTrace();
			}
			
			if (ftp.isConnected())
			{
				try {
					ftp.disconnect();
				}
				catch(IOException e)
				{
//				   Log.d("FTP", "ftp 접속 종료중 문제 발생");
				}
			}
		}
		catch(Exception e)
		{
		}
	}
	
	public boolean FtpIsConnected()
	{
		boolean connected = false;
		try{
			connected =  ftp.isConnected();
		}catch(Exception e)
		{
			connected = false;
		}
		return connected;
	}

	public boolean FtpUploadFile(File file) {

		try{
			if(FtpIsConnected() == false)
			{
				return false;
			}
			
			ftp.cwd("/");
		    ftp.mkd(szDeviceNum);
		    ftp.cwd(szDeviceNum);
		     
			FileInputStream fis = null;

			try {
			   fis = new FileInputStream(file);

//             ftp.rest(file.getName());  // if ftp file exist, continue to write
//             ftp.appendFile(file.getName(), ifile); // if not, write new file

			   boolean uploadResult = ftp.storeFile(file.getName(), fis);
			   return uploadResult;
			}
			catch (Exception e)
			{
			}
			
			if (fis != null) {
			   try {fis.close();}catch(IOException e){}
			}
		}
		catch(Exception e)
		{
		}
		return false;
	}

}
