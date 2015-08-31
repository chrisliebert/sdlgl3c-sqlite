//package com.GraphicsEngine;

public class GraphicsEngineJNI {
	static {	
		System.load(getSharedLibraryPath());	
	}

	public static String getSharedLibraryPath()
	{
		if(System.getProperty("os.name").startsWith("Windows")) {
			return System.getProperty("user.dir") + "\\libGraphicsEngineJNI.dll";
		} else {
			return System.getProperty("user.dir") + "/libGraphicsEngineJNI.so";
		}
	}
	
	public native void infoMsg(String msg);

	public native long createApp(String dbFile);
	public native void updateApp(long appPtr);
	public native void destroyApp(long appPtr);
	public native int getRunLevel(long appPtr);
}
