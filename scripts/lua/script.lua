-- script.lua
--print("Hello from Lua!")
--local frame = wx.wxFrame(wx.NULL, wx.wxID_ANY, "Lua Frame")
--frame:Show(true)

package.cpath = package.cpath..";./?.dll;./?.so;../lib/?.so;../lib/vc_dll/?.dll;../lib/bcc_dll/?.dll;../lib/mingw_dll/?.dll;"
print("Démarrage de l'application")

local wx = require("wx")

print("Module wx chargé")

local frame = wx.wxFrame(wx.NULL, wx.wxID_ANY, "Lua Frame")
frame:Show(true)
