-- MARS Export Script

MARS =
{
    options =
    {
		host = "localhost",
        port = 2000
    },
	
	originals =
	{
		LuaExportStart = LuaExportStart,
		LuaExportAfterNextFrame = LuaExportAfterNextFrame,
		LuaExportStop = LuaExportStop
	},
	
	data =
	{
		id = 0,
		player = "",
		unit = "",
		pos = {x = 0, y = 0, z = 0},
		radio =
		{
			["1"] = {name = "", primary = 0, secondary = 0, modulation = 0},
			["2"] = {name = "", primary = 0, secondary = 0, modulation = 0},
			["3"] = {name = "", primary = 0, secondary = 0, modulation = 0},
			selected = 0,
		}
	},
	
	JSON = nil,
	
	Initialize = function()
		log("MARS Initialize")
		MARS.JSON = loadfile("Scripts/JSON.lua")()
	end,
	
	Update = function()
		MARS.ExportCommon()
	end,
	
	ExportCommon = function()
		local id = LoGetPlayerPlaneId()
		local player = LoGetPilotName()
		local self = LoGetSelfData()
		
		MARS.data.id = id
		MARS.data.player = player
		MARS.data.unit = self.Name
		MARS.data.pos = {x = self.Position.x, y = self.Position.y, z = self.Position.z}
		
	end,
	
	ExportA10 = function()
		local vhf_am = GetDevice();
	end,
	
	ExportP51 = function()
	end,
	
	ExportMI8 = function()
	end,
	
	ExportKA50 = function()
	end,
	
	ExportUH1 = function()
	end,
	
	ExportF86 = function()
	end,
	
	ExportFW190 = function()
	end,
	
	ExportMIG21 = function()
	end,
	
	Quit = function()
		log("MARS Quit")
		local data = MARS.JSON:encode(MARS.data)
		log(data)
	end,
	
	NearEqual = function(a, b, epsilon)
		return math.abs(a - b) < epsilon
	end
}

-- CALLBACK: Called by DCS when mission unpauses before start
LuaExportStart = function()
	log("MARS LuaExportStart")

	MARS.Initialize()
	
    -- Call original function if it exists
    if MARS.originals.LuaExportStart then
        MARS.originals.LuaExportStart()
    end
end

-- CALLBACK: Called by DCS after each frame
LuaExportAfterNextFrame = function()
	MARS.Update()
	
    -- Call original function if it exists
    if MARS.originals.LuaExportAfterNextFrame then
        MARS.originals.LuaExportAfterNextFrame()
    end
end

-- CALLBACK: Called by DCS when exiting a mission
LuaExportStop = function()
	log("MARS LuaExportStop")

	MARS.Quit()
	
    -- Call original function if it exists
    if MARS.originals.LuaExportStop then
        MARS.originals.LuaExportStop()
    end
end
