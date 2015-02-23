-- MARS Export Script

package.path  = package.path..";.\\LuaSocket\\?.lua"
package.cpath = package.cpath..";.\\LuaSocket\\?.dll"

local socket = require("socket")

MARS = {}

MARS.options = 
{
	host = "localhost",
	port = 10112
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

MARS.data = {}

MARS.JSON = nil                       -- JSON library
MARS.connection = nil                 -- TCP connection
MARS.queue = {}                       -- Message queue
MARS.coroutines = {}                  -- Table of coroutines
MARS.coroutinesRegistered = false

MARS.Initialize = function()
	MARS.JSON = assert(loadfile("Scripts/JSON.lua"))()
	MARS.InitializeData()
end

MARS.InitializeData = function()
	MARS.data =
	{
		internal = false,
		name = "init",
		unit = "init",
		pos = {x = 0, y = 0, z = 0},
		selected = 0,
		radios =
		{
			{ id = 1, name = "init", primary = 0, secondary = 0, modulation = 0 },
			{ id = 2, name = "init", primary = 0, secondary = 0, modulation = 0 },
			{ id = 3, name = "init", primary = 0, secondary = 0, modulation = 0 }
		}
	}
end

MARS.EnsureConnection = function()
	if not MARS.connection then
		local err
		MARS.connection, err = socket.connect(MARS.options.host, MARS.options.port)
	
		if not MARS.connection then
			MARS.Log("Failed to connect: " .. err)
		else
			MARS.Log("Connected")
			MARS.connection:setoption("tcp-nodelay", true)
			MARS.InitializeData()
			MARS.SendStartCommand()
		end
	end
end

-- Coroutine function
MARS.Update = function(tCurrent)
	local tNext = tCurrent
	while true do
		MARS.ExportCommon()
		tNext = coroutine.yield()
	end
end

MARS.ExportPosition = function(tCurrent)
	local tNext = tCurrent
	while true do
		MARS.SendPosCommand(MARS.data.pos)
		tNext = coroutine.yield()
	end
end

MARS.ExportCommon = function()	
	local name = nil
	local export = nil     -- Unit specific export function
	local unit = nil
	local internal = true
	local data = LoGetSelfData()
	
	if data == nil then
		unit = "GC"
		name = "N/A"
		MARS.data.pos.x = 0
		MARS.data.pos.y = 0
		MARS.data.pos.z = 0
		internal = false
	else
		unit = data.Name
		name = data.UnitName
		MARS.data.pos.x = data.Position.x
		MARS.data.pos.y = data.Position.y
		MARS.data.pos.z = data.Position.z
	end
	
	if MARS.UnitHasInternalRadio(unit) == true then
		local panel = GetDevice(0)
		if panel == 0 then
			unit = "Parachute" -- Pilot ejected
			internal = false
		end
	end
	
	if MARS.data.name ~= name or MARS.data.unit ~= unit then -- Switched unit
		MARS.InitializeData()
		MARS.SendInfoCommand(name, unit)
		MARS.data.name = name
		MARS.data.unit = unit
	end
	
	---[[
	--internal = MARS.UnitHasInternalRadio(unit)
	if MARS.data.internal ~= internal then
		MARS.SendUseCommand(internal)
		MARS.data.internal = internal
	end
	--]]
	
	if unit == "A-10C" then
		export = MARS.ExportA10()
	elseif unit == "Ka-50" then
		export = MARS.ExportKA50()
	elseif unit == "MiG-21Bis" then
		export = MARS.ExportMIG21()
	end
	
	if export ~= nil then
		export()
	end
end

MARS.ExportA10 = function()
	local radio =
	{
		id = 1,
		name = "AN/ARC-186(V)",
		primary = MARS.Round(MARS.GetFrequency(55), 5000),
		secondary = 0,
		modulation = MARS.modulation.AM
	}
	
	if not MARS.FastCompare(MARS.data.radios[1], radio) then
		MARS.SendSetCommand(radio)
		MARS.data.radios[1] = MARS.FastCopy(radio)
	end
	
	radio =
	{
		id = 2,
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
		MARS.SendSetCommand(radio)
		MARS.data.radios[2] = MARS.FastCopy(radio)
	end
	
	radio =
	{
		id = 3,
		name = "AN/ARC-186(V)",
		primary = MARS.Round(MARS.GetFrequency(56), 5000),
		secondary = 0,
		modulation = MARS.modulation.FM
	}
	
	if not MARS.FastCompare(MARS.data.radios[3], radio) then
		MARS.SendSetCommand(radio)
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
		id = 1,
		name = "R-828",
		primary = MARS.Round(MARS.GetFrequency(49), 50000),
		secondary = 0,
		modulation = MARS.modulation.FM
	}
	
	if not MARS.FastCompare(MARS.data.radios[1], radio) then
		MARS.SendSetCommand(radio)
		MARS.data.radios[1] = MARS.FastCopy(radio)
	end

	radio =
	{
		id = 2,
		name = "R-800L1",
		primary = MARS.Round(MARS.GetFrequency(48), 5000),
		secondary = 0,
		modulation = MARS.modulation.AM
	}
	
	-- Get modulation mode
	local panel = GetDevice(0)
	local switch = panel:get_argument_value(417)
	if MARS.NearEqual(switch, 0.0, 0.03) then
		radio.modulation = MARS.modulation.FM
	else
		radio.modulation = MARS.modulation.AM
	end
	
	-- Emergency receiver mode?
	switch = panel:get_argument_value(241)
	if MARS.NearEqual(switch, 0.0, 0.03) and radio.primary == 121500000 then
		radio.primary = 0
		radio.secondary = 121500000
	else
		radio.secondary = 0
	end
	
	if not MARS.FastCompare(MARS.data.radios[2], radio) then
		MARS.SendSetCommand(radio)
		MARS.data.radios[2] = MARS.FastCopy(radio)
	end

	-- Get selected radio from SPU-9
	switch = panel:get_argument_value(428)
	local selected = 0
	
	if MARS.NearEqual(switch, 0.0, 0.03) then
		selected = 2
	elseif MARS.NearEqual(switch, 0.1, 0.03) then
		selected = 1
	else
		selected = 0
	end
	
	if MARS.data.selected ~= selected then
		MARS.SendSelectCommand(selected)
		MARS.data.selected = selected
	end
	
	-- Clear radio 3
	radio =	{ id = 3, name = "N/A", primary = 0, secondary = 0, modulation = MARS.modulation.AM }
	if not MARS.FastCompare(MARS.data.radios[3], radio) then
		MARS.SendSetCommand(radio)
		MARS.data.radios[3] = MARS.FastCopy(radio)
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
		id = 1,
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
		MARS.SendSetCommand(radio)
		MARS.data.radios[1] = MARS.FastCopy(radio)
	end
	
	-- Clear radio 2
	radio = { id = 2, name = "N/A", primary = 0, secondary = 0, modulation = 0 }
	if not MARS.FastCompare(MARS.data.radios[2], radio) then
		MARS.SendSetCommand(radio)
		MARS.data.radios[2] = MARS.FastCopy(radio)
	end
	
	-- Clear radio 3
	radio = { id = 3, name = "N/A", primary = 0, secondary = 0, modulation = 0 }
	if not MARS.FastCompare(MARS.data.radios[3], radio) then
		MARS.SendSetCommand(radio)
		MARS.data.radios[3] = MARS.FastCopy(radio)
	end
end

MARS.SendUseCommand = function(internal)
	local modes
	
	if internal == true then
		modes = "internal"
	else
		modes = "external"
	end
	
	local command =
	{
		command = "use",
		mode = modes
	}
	
	local json = MARS.JSON:encode(command)
	MARS.QueueMessage(json)
end

MARS.SendInfoCommand = function(name, unit)
	local command =
	{
		command = "info",
		name = name,
		unit = unit
	}
	
	local json = MARS.JSON:encode(command)
	MARS.QueueMessage(json)
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
	MARS.QueueMessage(json)
end

MARS.SendSelectCommand = function(id)
	local command =
	{
		command = "select",
		radio = id
	}
	
	local json = MARS.JSON:encode(command)
	MARS.QueueMessage(json)
end

MARS.SendStartCommand = function()
	local command =
	{
		command = "start",
	}
	
	local json = MARS.JSON:encode(command)
	MARS.Send(json)
end

MARS.SendStopCommand = function()
	local command =
	{
		command = "stop",
	}
	
	local json = MARS.JSON:encode(command)
	MARS.Send(json)
end

MARS.SendSetCommand = function(radio)
	local command =
	{
		command = "set",
		internal = true,
		name = radio.name,
		radio = radio.id,
		primary = radio.primary,
		secondary = radio.secondary,
		modulation = radio.modulation
	}
	
	local json = MARS.JSON:encode(command)
	MARS.QueueMessage(json)
end

MARS.QueueMessage = function(message)
	if message then
		table.insert(MARS.queue, message)
	end
end

-- Coroutine function
MARS.SendQueuedCommands = function(tCurrent)
	local tNext = tCurrent
	while true do
		if #MARS.queue > 0 then
			local item = table.remove(MARS.queue, 1)
			if item then
				MARS.Send(item)
			end
		end
		tNext = coroutine.yield()
	end
end

-- Coroutine function
MARS.KeepAlive = function(tCurrent)
	local tNext = tCurrent
	while true do
		MARS.QueueMessage("poke")
		tNext = coroutine.yield()
	end
end

MARS.EnsureCoroutines = function()
	if not MARS.coroutinesRegistered then
		MARS.Log("Registering coroutines")
		
		local currentTime = LoGetModelTime()
		MARS.coroutines[1] = coroutine.create(MARS.KeepAlive)
		LoCreateCoroutineActivity(1, currentTime + 2.0, 5.0)
		
		MARS.coroutines[2] = coroutine.create(MARS.SendQueuedCommands)
		LoCreateCoroutineActivity(2, currentTime + 1.0, 0.1)

		MARS.coroutines[3] = coroutine.create(MARS.Update)
		LoCreateCoroutineActivity(3, currentTime + 1.5, 0.2)
		
		MARS.coroutines[4] = coroutine.create(MARS.ExportPosition)
		LoCreateCoroutineActivity(4, currentTime + 2.0, 8.0)
		
		MARS.coroutinesRegistered = true
	end
end

MARS.Send = function(message)
	if MARS.connection and message then
		local count, err = MARS.connection:send(message)
		if not count then
			MARS.Log("Failed to send; " .. err)
			MARS.connection:close()
			MARS.connection = nil
		else
			MARS.Log("Sent; " .. message)
		end
	end
end

MARS.Disconnect = function()
	if MARS.connection then
		MARS.connection:close()
	end
	MARS.connection = nil
	MARS.Log("Connection closed")
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

MARS.UnitHasInternalRadio = function(unit)
	if unit == "A-10C" then
		return true
	elseif unit == "Ka-50" then
		return true
	elseif unit == "MiG-21Bis" then
		return true
	else
		return false
	end
end

MARS.Log = function(message)
	if message then
		log("MARS: " .. message)
	end
end

-- CALLBACK: Called by DCS when mission unpauses before start
LuaExportStart = function()
	MARS.Initialize()
	--MARS.SendStartCommand()

	-- Call original function if it exists
	if MARS.originals.LuaExportStart then
		MARS.originals.LuaExportStart()
	end
end

-- CALLBACK: Called by DCS after each frame
LuaExportAfterNextFrame = function()

	-- Call original function if it exists
	if MARS.originals.LuaExportAfterNextFrame then
		MARS.originals.LuaExportAfterNextFrame()
	end
end

-- CALLBACK: Called by DCS every "t" time
LuaExportActivityNextEvent = function(tCurrent)
	local tNext = tCurrent + 5.0
	MARS.EnsureConnection()
	MARS.EnsureCoroutines()

	-- Call original function if it exists
	if MARS.originals.LuaExportActivityNextEvent then
		return MARS.originals.LuaExportActivityNextEvent(tCurrent)
	end
	
	return tNext
end

-- CALLBACK: Called by DCS when exiting a mission
LuaExportStop = function()
	MARS.SendStopCommand()
	MARS.Disconnect()

	-- Call original function if it exists
	if MARS.originals.LuaExportStop then
		MARS.originals.LuaExportStop()
	end
end

-- CALLBACK: Called by DCS to resume every registered coroutine
CoroutineResume = function(index, tCurrent)
	coroutine.resume(MARS.coroutines[index], tCurrent)
	return coroutine.status(MARS.coroutines[index]) ~= "dead"
end