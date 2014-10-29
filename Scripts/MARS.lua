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
		unit = ""
	},
	
	JSON = nil,
	
	Initialize = function()
		log("MARS Initialize")
		MARS.JSON = loadfile("Scripts/JSON.lua")()
	end,
	
	Update = function()
		local data = LoGetSelfData()
		local id = LoGetPlayerPlaneId()
		
		-- Fill export table
		MARS.data.name = data.UnitName   -- Player name
		MARS.data.unit = data.Name       -- Type of unit
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
