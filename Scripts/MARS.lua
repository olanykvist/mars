-- MARS Export Script

package.path  = package.path..";.\\LuaSocket\\?.lua"
package.cpath = package.cpath..";.\\LuaSocket\\?.dll"

local socket = require("socket")

MARS = {}

MARS.options = 
{
	host = "localhost",
	port = 2000
}

MARS.originals =
{
	LuaExportStart = LuaExportStart,
	LuaExportAfterNextFrame = LuaExportAfterNextFrame,
	LuaExportActivityNextEvent = LuaExportActivityNextEvent,
	LuaExportStop = LuaExportStop
}

MARS.modulation =
{
	AM = 0,
	FM = 1
}

MARS.data =
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
}

MARS.JSON = nil
MARS.connection = nil
MARS.queue = {}

MARS.Initialize = function()
	MARS.JSON = assert(loadfile("Scripts/JSON.lua"))()
	MARS.Connect()
end

MARS.Connect = function()
	--local result, err = 
	connection = socket.try(socket.connect(MARS.options.host, MARS.options.port))
	connection:setoption("tcp-nodelay", true)
end

MARS.Update = function()
	MARS.ExportCommon()
end

MARS.ExportCommon = function()	
	local name = LoGetPilotName()
	local data = LoGetSelfData()
	local unit = nil
	
	if not name then
		name = "Hemliga arne"
	end
	
	if data then
		unit = data.Name
		MARS.data.pos.x = data.Position.x
		MARS.data.pos.y = data.Position.y
		MARS.data.pos.z = data.Position.z
	else
		unit = "GC"
		MARS.data.pos.x = 0
		MARS.data.pos.y = 0
		MARS.data.pos.z = 0
	end
	
	if MARS.data.name ~= name or MARS.data.unit ~= unit then
		MARS.SendInfoCommand(name, unit)
		MARS.data.name = name
		MARS.data.unit = unit
		MARS.SendSetCommand("init", 1, 0, 0, 0)
		MARS.SendSetCommand("init", 2, 0, 0, 0)
		MARS.SendSetCommand("init", 3, 0, 0, 0)
	end
	
	if unit == "MiG-21Bis" then
		MARS.ExportMIG21()
	elseif unit == "Ka-50" then
		MARS.ExportKA50()
	elseif unit == "A-10C" then
		MARS.ExportA10()
	end
	
end

MARS.ExportA10 = function()
	local radio =
	{
		name = "AN/ARC-186(V)",
		primary = MARS.Round(MARS.GetFrequency(55), 5000),
		secondary = 0,
		modulation = MARS.modulation.AM
	}
	
	if not MARS.FastCompare(MARS.data.radios[1], radio) then
		MARS.SendSetCommand(radio.name, 1, radio.primary, radio.secondary, radio.modulation)
		MARS.data.radios[1] = MARS.FastCopy(radio)
	end
	
	radio =
	{
		name = "AN/ARC-164",
		primary = MARS.Round(MARS.GetFrequency(54), 5000),
		secondary = 0,
		modulation = MARS.modulation.AM
	}
	
	local panel = GetDevice(0)
	local knob = panel:get_argument_value(168) -- Function selector knob
	if MARS.NearEqual(knob, 0.2, 0.03) and radio.primary > 0 then
		radio.secondary = 243000000
	end
	
	if not MARS.FastCompare(MARS.data.radios[2], radio) then
		MARS.SendSetCommand(radio.name, 2, radio.primary, radio.secondary, radio.modulation)
		MARS.data.radios[2] = MARS.FastCopy(radio)
	end
	
	radio =
	{
		name = "AN/ARC-186(V)",
		primary = MARS.Round(MARS.GetFrequency(56), 5000),
		secondary = 0,
		modulation = MARS.modulation.FM
	}
	
	if not MARS.FastCompare(MARS.data.radios[3], radio) then
		MARS.SendSetCommand(radio.name, 3, radio.primary, radio.secondary, radio.modulation)
		MARS.data.radios[3] = MARS.FastCopy(radio)
	end		
end

MARS.ExportP51 = function()
end

MARS.ExportMI8 = function()
end

MARS.ExportKA50 = function()
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
end

MARS.ExportUH1 = function()
end

MARS.ExportF86 = function()
end

MARS.ExportFW190 = function()
end
	
MARS.ExportMIG21 = function()
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
end

MARS.SendInfoCommand = function(name, unit)
	local command =
	{
		command = "info",
		name = name,
		unit = unit
	}
	
	local json = MARS.JSON:encode(command)
	table.insert(MARS.queue, json)
	--socket.try(connection:send(json .. "\0"))
end
	
MARS.SendPosCommand = function(pos)
	local command =
	{
		command = "pos",
		x = pos.x,
		y = pos.y,
		z = pos.z
	}
	
	local json = MARS.JSON:encode(command)
	table.insert(MARS.queue, json)
	--socket.try(connection:send(json .. "\0"))
end
	
MARS.SendSelectCommand = function(radio)
	local command =
	{
		command = "select",
		radio = radio
	}
	
	local json = MARS.JSON:encode(command)
	table.insert(MARS.queue, json)
	--socket.try(connection:send(json .. "\0"))
end

MARS.SendStartCommand = function()
	local command =
	{
		command = "start",
	}
	
	local json = MARS.JSON:encode(command)
	--table.insert(MARS.queue, json)
	socket.try(connection:send(json))
end

MARS.SendStopCommand = function()
	local command =
	{
		command = "stop",
	}
	
	local json = MARS.JSON:encode(command)
	--table.insert(MARS.queue, json)
	socket.try(connection:send(json))
end

MARS.SendSetCommand = function(name, radio, primary, secondary, modulation)
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
	table.insert(MARS.queue, json)
	--socket.try(connection:send(json .. "\0"))
end

MARS.SendQueuedCommands = function()
	if #MARS.queue > 0 then
		local item = table.remove(MARS.queue, 1)
		if item then
			socket.try(connection:send(item))
		end
	end
end

MARS.Quit = function()
	log("MARS Quit")
end

MARS.FastCompare = function(t1, t2)
	for k in pairs(t1) do
		if t1[k] ~= t2[k] then
			return false
		end
	end
	return true
end

MARS.FastCopy = function(t)
	local copy = {}
	for k, v in pairs(t) do
		copy[k] = v
	end
	return copy
end

MARS.Round = function(number, step)
	if number == 0 then
		return 0
	else
		return math.floor((number + step / 2) / step) * step
	end
end

MARS.GetFrequency = function(deviceId)
	local device = GetDevice(deviceId)
	
	if device then
		if device:is_on() then
			return device:get_frequency()
		end
	end
	
	return 0
end

MARS.NearEqual = function(a, b, epsilon)
	return math.abs(a - b) < epsilon
end


-- CALLBACK: Called by DCS when mission unpauses before start
LuaExportStart = function()
	log("MARS LuaExportStart")

	MARS.Initialize()
	MARS.SendStartCommand()
	
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

LuaExportActivityNextEvent = function(t)
	local tn = t + 0.2
	MARS.SendQueuedCommands()
	return tn
end

-- CALLBACK: Called by DCS when exiting a mission
LuaExportStop = function()
	log("MARS LuaExportStop")

	MARS.SendStopCommand()
	MARS.Quit()
	
    -- Call original function if it exists
    if MARS.originals.LuaExportStop then
        MARS.originals.LuaExportStop()
    end
end
