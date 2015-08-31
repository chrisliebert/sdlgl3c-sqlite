if arg[1] == nil then
  print("You must supply a database")
  os.exit(1)
end

package.path = package.path .. ";./?.lua"
require("graphics_engine")

local app = SDLGLApp:create(arg[1])

while app:getRunLevel() > 0 do
  app:update()
end

app:destroy()
collectgarbage()
os.exit(0)
