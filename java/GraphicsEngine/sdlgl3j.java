class sdlgl3c
{
	public static void main(String[] args) {
		GraphicsEngineJNI ge = new GraphicsEngineJNI();
		
		long app = ge.createApp("test.db");
		
		while(ge.getRunLevel(app) > 0) {
			ge.updateApp(app);
		}
		
		ge.destroyApp(app);
		
	}
}