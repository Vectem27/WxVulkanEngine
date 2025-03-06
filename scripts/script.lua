-- script.lua
--print("Hello from Lua!")
--local frame = wx.wxFrame(wx.NULL, wx.wxID_ANY, "Lua Frame")
--frame:Show(true)

function retourner_nombre()
    return 42  -- Le nombre retourné peut être modifié selon tes besoins
end

-- Exemple d'appel de la fonction
local nombre = retourner_nombre()
print(nombre)