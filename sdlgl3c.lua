package.path = package.path .. ";./?.lua"
require("graphics_engine")

app = SDLGL3App.create(arg[1])
app:update()
app:destroy()
os.exit(0)
