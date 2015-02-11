-- MARS Export Script

package.path  = package.path..";.\\LuaSocket\\?.lua"
package.cpath = package.cpath..";.\\LuaSocket\\?.dll"

local socket = require("socket")

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
	
	modulation =
	{
		AM = 0,
		FM = 1
	},
	
	data =
	{
		name = "init",
		unit = "init",
		pos = {x = 0, y = 0, z = 0},
		selected = 0,
		radios =
		{
			{ name = "init", primary = 0, secondary = 0, modulation = 0	},
			{ name = "init", primary = 0, secondary = 0, modulation = 0	},
			{ name = "init", primary = 0, secondary = 0, modulation = 0	}
		}
	},
	
	JSON = nil,
	
	Initialize = function()
		MARS.JSON = assert(loadfile("Scripts/JSON.lua"))()
		MARS.Connect()
	end,
	
	Connect = function()
		connection = socket.try(socket.connect(MARS.options.host, MARS.options.port))
		connection:setoption("tcp-nodelay", true)
	end,
	
	Update = function()
		MARS.ExportCommon()
	end,
	
	ExportCommon = function()
	--[[
		local id = LoGetPlayerPlaneId()
		local player = LoGetPilotName()
		local self = LoGetSelfData()
		
		MARS.data.id = id
		MARS.data.player = player
		MARS.data.unit = self.Name
		MARS.data.pos = {x = self.Position.x, y = self.Position.y, z = self.Position.z}
	--]]
		
		local name = LoGetPilotName()
		local data = LoGetSelfData()
		local unit = nil
		
		if not name then
			name = "Hemliga arne"
		end
		
		if data then
			unit = data.Name
		else
			unit = "GC"
		end
		
		if MARS.data.name ~= name or MARS.data.unit ~= unit then
			MARS.SendInfoCommand(name, unit)
			MARS.data.name = name
			MARS.data.unit = unit
		end
		
		if unit == "MiG-21Bis" then
			MARS.ExportMIG21()
		elseif unit == "Ka-50" then
			MARS.ExportKA50()
		end
		
	end,
	
	ExportA10 = function()
	end,
	
	ExportP51 = function()
	end,
	
	ExportMI8 = function()
	end,
	
	ExportKA50 = function()
		local radio =
		{
			name = "R-828",
			primary = MARS.Round(MARS.GetFrequency(49), 50000),
			secondary = 0,
			modulation = MARS.modulation.FM
		}
		
		if not MARS.FastCompare(MARS.data.radios[1], radio) then
			MARS.SendSetCommand(radio.name, 1, radio.primary, radio.secondary, radio.modulation)
			MARS.data.radios[1] = MARS.FastCopy(radio)
		end

		radio =
		{
			name = "R-800L1",
			primary = MARS.Round(MARS.GetFrequency(48), 5000),
			secondary = 0,
			modulation = MARS.modulation.AM
		}
		
		if not MARS.FastCompare(MARS.data.radios[2], radio) then
			MARS.SendSetCommand(radio.name, 2, radio.primary, radio.secondary, radio.modulation)
			MARS.data.radios[2] = MARS.FastCopy(radio)
		end
	
		-- Get selected radio from SPU-9
		local panel = GetDevice(0)
		local knob = panel:get_argument_value(428)
		local selected = 0
		
		if MARS.NearEqual(knob, 0.0, 0.03) then
			selected = 2
		elseif MARS.NearEqual(knob, 0.1, 0.03) then
			selected = 1
		else
			selected = 0
		end
		
		if MARS.data.selected ~= selected then
			MARS.SendSelectCommand(selected)
			MARS.data.selected = selected
		end
	end,
	
	ExportUH1 = function()
	end,
	
	ExportF86 = function()
	end,
	
	ExportFW190 = function()
	end,
	
	ExportMIG21 = function()
		local radio =
		{
			name = "R-828",
			primary = MARS.Round(MARS.GetFrequency(22), 5000),
			secondary = 0,
			modulation = MARS.modulation.AM
		}
		
		local selected = 1

		if MARS.data.selected ~= selected then
			MARS.SendSelectCommand(selected)
			MARS.data.selected = selected
		end
		
		if not MARS.FastCompare(MARS.data.radios[1], radio) then
			MARS.SendSetCommand(radio.name, 1, radio.primary, radio.secondary, radio.modulation)
			MARS.data.radios[1] = MARS.FastCopy(radio)
		end
	end,
	
	SendInfoCommand = function(name, unit)
		local command =
		{
			command = "info",
			name = name,
			unit = unit
		}
		
		local json = MARS.JSON:encode(command)
		socket.try(connection:send(json))
	end,
	
	SendSelectCommand = function(radio)
		local command =
		{
			command = "select",
			radio = radio
		}
		
		local json = MARS.JSON:encode(command)
		socket.try(connection:send(json))
	end,
	
	SendSetCommand = function(name, radio, primary, secondary, modulation)
		local command =
		{
			command = "set",
			internal = true,
			name = name,
			radio = radio,
			primary = primary,
			secondary = secondary,
			modulation = modulation
		}
		
		local json = MARS.JSON:encode(command)
		socket.try(connection:send(json))
	end,
	
	Quit = function()
		log("MARS Quit")
	end,
	
	FastCompare = function(t1, t2)
		for k in pairs(t1) do
			if t1[k] ~= t2[k] then
				return false
			end
		end
		return true
	end,
	
	FastCopy = function(t1)
		local t2 = {}
		for k, v in pairs(t1) do
			t2[k] = v
		end
		return t2
	end,
	
	Round = function(number, step)
		if number == 0 then
			return 0
		else
			return math.floor((number + step / 2) / step) * step
		end
	end,
	
	GetFrequency = function(deviceId)
		local device = GetDevice(deviceId)
		
		if device then
			if device:is_on() then
				return device:get_frequency()
			end
		end
		
		return 0
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
