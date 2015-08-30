public class GraphicsEngineJNI {
	static {
		System.loadLibrary("libGraphicsEngineJNI");
		//System.load("C:/proj//sdlgl3c-sqlite-test/libGraphicsEngineJNI.dll");
	}
	
	public native void infoMsg(String msg);
	
	public native long createApp(String dbFile);
	public native void updateApp(long appPtr);
	public native void destroyApp(long appPtr);
	public native int getRunLevel(long appPtr);
}