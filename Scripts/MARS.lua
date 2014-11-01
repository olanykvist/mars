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
		name = "",
		unit = "",
		radio = nil
	},
	
	Radio =
	{
		name = "",
		primary = 0,
		secondary = 0
		new = function(object)
			object = object or {}
			setmetatable(object, self)
			self.__index = self
			return object
		end
	},
	
	JSON = nil,
	
	Initialize = function()
		log("MARS Initialize")
		MARS.JSON = loadfile("Scripts/JSON.lua")()
	end,
	
	Update = function()

	end,
	
	ExportCommon = function()
		local data = LoGetSelfData()
		local id = LoGetPlayerPlaneId()
		
		-- Fill export table
		MARS.data.name = data.UnitName   -- Player name
		MARS.data.unit = data.Name       -- Type of unit
		
		local r = MARS.Radio:new{name = "AN/ARC"}
		r.primary = 12750000
		
		MARS.data.radio = r
		
	end,
	
	ExportA10 = function()
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
