package com.cal3d.android;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Vector;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;


import android.app.Activity;
import android.app.AlertDialog;
import android.content.pm.ApplicationInfo;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.util.DisplayMetrics;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;
import android.view.ViewGroup.LayoutParams;
import android.widget.TextView;
import android.widget.Toast;

public class Cal3DActivity extends Activity
{
	public final Handler mHandler = new Handler();
    private static final int APPSTATUS_UNINITED         = -1;
    private static final int APPSTATUS_INIT_APP         = 0;
    private static final int APPSTATUS_INIT_APP_AR      = 2;
    private static final int APPSTATUS_INITED           = 4;
        
    private static final String NATIVE_LIB_CAL3D = "Cal3DNative"; 
    private static final String NATIVE_LIB_PROGRAM = "AndroidWrapper";

    private ARGLView mGlView;
    
    private ARRenderer mRenderer;
    private int mScreenWidth = 0;
    private int mScreenHeight = 0;
    
    private int mAppStatus = APPSTATUS_UNINITED;
    
    /** Static initializer block to load native libraries on start-up. */
    static
    {
        loadLibrary(NATIVE_LIB_CAL3D);
        loadLibrary(NATIVE_LIB_PROGRAM);
    }
 

    public native int getOpenGlEsVersionNative();
    
    protected native void onNativeResume();
    protected native void onNativePause();
    protected native void onNativeMultiTouch(float x, float y, int eventID,int pointerID, int pointerCount);
    
    protected native void initNativeProgram(int width, int height);
    protected native void deinitNativeProgram();
    protected native void initNative();
    protected native void deinitNative();
    
    protected native void SetReadPath(String path);
    protected TextView mTextView;
    
    /** Called when the activity first starts or the user navigates back to an activity. */
    protected void onCreate(Bundle savedInstanceState)
    {
        DebugLog.LOGD("Activity::onCreate");

        super.onCreate(savedInstanceState);
       // setContentView(R.layout.main);
       // mTextView = (TextView)findViewById(R.id.textbox);
    	initNative();

        ApplicationInfo appInfo = getApplication().getApplicationInfo();
		SetReadPath(appInfo.dataDir + "/files/");
		
		mLoaderRunnable.start();
		(new LoadResourceTask()).execute();
		//mLoaderRunnable.stop();
		
        //copyResourceToExternal();

        // Update the application status to start initializing application
        //updateApplicationStatus(APPSTATUS_INIT_APP);
    }  

   /** Called when the activity will start interacting with the user.*/
    protected void onResume()
    {
        DebugLog.LOGD("ARActivity::onResume");
        super.onResume();
        onNativeResume();
        
        // Resume the GL view:
        if (mGlView != null)
        {
            mGlView.setVisibility(View.VISIBLE);
            mGlView.onResume();
        }       
    }
    
    
    /** Called when the system is about to start resuming a previous activity.*/
    protected void onPause()
    {
        DebugLog.LOGD("Activity::onPause");
        super.onPause();
        
        if (mGlView != null)
        {
            mGlView.setVisibility(View.INVISIBLE);
            mGlView.onPause();
        }
        onNativePause();
    }
    
    /** The final call you receive before your activity is destroyed.*/
    protected void onDestroy()
    {
        DebugLog.LOGD("Activity::onDestroy");
        super.onDestroy();
        
        deinitNativeProgram();
        deinitNative();

        System.gc();
        
        android.os.Process.killProcess(android.os.Process.myPid());
    }
    
    @Override
	public boolean onTouchEvent(final MotionEvent event)
	{
    	int action = event.getAction() & MotionEvent.ACTION_MASK;
    	
    	//int id = event.getAction() >> MotionEvent.ACTION_POINTER_ID_SHIFT;
    	//event.getPointerId((event.getAction() & MotionEvent.ACTION_POINTER_INDEX_MASK) >> MotionEvent.ACTION_POINTER_INDEX_SHIFT)
    	int id = (event.getAction() & MotionEvent.ACTION_POINTER_INDEX_MASK) >> MotionEvent.ACTION_POINTER_INDEX_SHIFT;
    	
    	
    	if(action != MotionEvent.ACTION_MOVE)
    		onNativeMultiTouch(event.getX(id), event.getY(id),event.getAction() & MotionEvent.ACTION_MASK, event.getPointerId(id),event.getPointerCount());
    	else
    	{
    		for(int i = 0; i < event.getPointerCount(); i++)
    			onNativeMultiTouch(event.getX(i),event.getY(i),MotionEvent.ACTION_MOVE,event.getPointerId(i),event.getPointerCount());
    	}
		return true;
	}

   // private native void startCamera();
    /** NOTE: this method is synchronized because of a potential concurrent
     * access by ImageTargets::onResume() and InitQCARTask::onPostExecute(). */
    private synchronized void updateApplicationStatus(int appStatus)
    {
        if (mAppStatus == appStatus)
            return;
     
        mAppStatus = appStatus;

        // Execute application state-specific actions
        switch (mAppStatus)
        {
            case APPSTATUS_INIT_APP:
                // Initialize application elements that do not rely on QCAR
                // initialization  
                initApplication();
                updateApplicationStatus(APPSTATUS_INIT_APP_AR);
                break;
          
            case APPSTATUS_INIT_APP_AR:
                // Initialize Augmented Reality-specific application elements
                // that may rely on the fact that the QCAR SDK has been
                // already initialized
                initApplicationAR();
                updateApplicationStatus(APPSTATUS_INITED);
                break;
                
            case APPSTATUS_INITED:
                System.gc();
               
                mRenderer.mIsActive = true;

                addContentView(mGlView, new LayoutParams(
                                LayoutParams.FILL_PARENT,
                                LayoutParams.FILL_PARENT));	
                             
                break;
            default:
                throw new RuntimeException("Invalid application state");
        }
    }
    

    /** Initialize application GUI elements that are not related to AR. */
    private void initApplication()
    {
        // Query display dimensions
        DisplayMetrics metrics = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(metrics);
        mScreenWidth = metrics.widthPixels;
        mScreenHeight = metrics.heightPixels;

        // As long as this window is visible to the user, keep the device's
        // screen turned on and bright.
        getWindow().setFlags(
            WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON,
            WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
             
    }

    /** Initializes AR application components. */
    private void initApplicationAR()
    {        
        // Do application initialization in native code (e.g. registering
        // callbacks, etc.)
    	initNativeProgram(mScreenWidth,mScreenHeight);
        // Create OpenGL ES view:
        int depthSize = 16;
        int stencilSize = 0;
        boolean translucent = true;
        
        mGlView = new ARGLView(this);
        mGlView.init(false, translucent, depthSize, stencilSize);
        
        mRenderer = new ARRenderer();
        mGlView.setRenderer(mRenderer);
    }
   
    public void copyResourceToExternal()
    {
		// Load our library
		ApplicationInfo appInfo = getApplication().getApplicationInfo();

		// Set various OS specific strings used by PVRShell
		String ReadPath = appInfo.dataDir + "/files/";

		//SetWritePath(appInfo.dataDir + "/");
		//SetReadPath(ReadPath);

		/*
		     Copy our /assets/ contents to /data/data/com.powervr.AppName/files folder
		     so apps can have external files that the native code can get access
		     to and read.
		*/
		try
		{
			int size;
			byte[] buffer = new byte[2048];
			FileInputStream fIn = new FileInputStream(appInfo.sourceDir);
			ZipInputStream zIn = new ZipInputStream(new BufferedInputStream(fIn));
			ZipEntry entry;
			
			//mLoaderRunnable.start();
			while((entry = zIn.getNextEntry()) != null)
			{
				String name = entry.getName();

				if(name.startsWith("assets/"))
				{
					name = name.replaceFirst("assets/", ReadPath);

					DebugLog.LOGI("Creating " + name);
					{
						File file = new File(name);
						
						// Create any directories and files
						if(file.exists())
						{
							DebugLog.LOGI("File already exist, operation skipped");
							continue;
						}
						mLoaderRunnable.setFilename(name);
						this.runOnUiThread(mLoaderRunnable);
						file.getParentFile().mkdirs();
						file.createNewFile();

						FileOutputStream fileos = new FileOutputStream(file);

						// Write a copy of our file out to the data directory
						BufferedOutputStream bOut = new BufferedOutputStream(fileos, buffer.length);

						while((size = zIn.read(buffer, 0, buffer.length)) != -1)
							bOut.write(buffer, 0, size);

						bOut.flush();
						bOut.close();
					}
				}
			}
			//mLoaderRunnable.stop();
			zIn.close();
			fIn.close();
		}
		catch (IOException e)
		{
			e.printStackTrace();
		}
    }
    
    /** A helper for loading native libraries stored in "libs/armeabi*". */
    public static boolean loadLibrary(String nLibName)
    {
        try
        {
            System.loadLibrary(nLibName);
            DebugLog.LOGI("Native library lib" + nLibName + ".so loaded");
            return true;
        }
        catch (UnsatisfiedLinkError ulee)
        {
            DebugLog.LOGE("The library lib" + nLibName +
                            ".so could not be loaded");
        }
        catch (SecurityException se)
        {
            DebugLog.LOGE("The library lib" + nLibName +
                            ".so was not allowed to be loaded");
        }
        
        return false;
    }  
    
    private class LoadResourceTask extends AsyncTask<Void, Integer, Boolean>
    {
        protected Boolean doInBackground(Void... params)
        {
        	copyResourceToExternal();
            return true;
        }
        
        protected void onProgressUpdate(Integer... values)
        {
        }
        
        protected void onPostExecute(Boolean result)
        { 
        	mLoaderRunnable.stop();
            updateApplicationStatus(APPSTATUS_INIT_APP);
        }
    }

    
    LoaderRunnable mLoaderRunnable = new LoaderRunnable(this);
    
    public class LoaderRunnable implements Runnable {
        String mFileName;
        AlertDialog mAlertDialog;
        Activity mAC;
        LoaderRunnable(Activity ac)
        {
        	mAC = ac;
        }
        public void setFilename(String filename)
        {
        	mFileName = filename;
        }
        public void start()
        {
        	mAlertDialog = new AlertDialog.Builder(mAC).create();
        	mAlertDialog.setTitle("Resource Loading");
        	mAlertDialog.setMessage("Please wait");
        	mAlertDialog.show();
        }
        public void run(){
        	mAlertDialog.setMessage("Creating file - " + mFileName);
        }
        public void stop()
        {
        	mAlertDialog.hide();
        }
    }
}
