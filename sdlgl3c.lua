require("graphics_engine")

app = SDLGL3App.create(arg[1])
app:start()
app:destroy()
os.exit(0)
