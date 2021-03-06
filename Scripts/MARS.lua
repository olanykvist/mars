-- MARS Export Script, v1.0.1

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

MARS.unitsWithInternalRadio =
{
	["A-10C"] = true,
	["Ka-50"] = true,
	["MiG-21Bis"]= true,
	["P-51D"] = true,
	["TF-51D"] = true,
	["UH-1H"] = true,
	["F-86F Sabre"] = true,
	["FW-190D9"] = true,
	["Bf-109K-4"] = true,
	["Mi-8MT"] = true,
	["MiG-15bis"] = true,
	["L-39C"] = true
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
		internal = nil,
		name = "init",
		unit = "init",
		id = 0,
		pos = {x = 0, y = 0, z = 0},
		selected = -1,
		volume = {-1, -1, -1},
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
			MARS.connection:settimeout(0.001)
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
	local id = nil
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
		id = LoGetPlayerPlaneId()
		MARS.data.pos.x = data.Position.x
		MARS.data.pos.y = data.Position.y
		MARS.data.pos.z = data.Position.z
	end
	
	if MARS.UnitHasInternalRadio(unit) == true then
		local panel = GetDevice(0)
		if panel == 0 then
			unit = "Parachute" -- Pilot ejected
			internal = false
			if MARS.data.selected ~= 1 then
				MARS.SendSelectCommand(1)
				MARS.data.selected = 1
			end
		end
	end
	
	if MARS.data.name ~= name or MARS.data.unit ~= unit or MARS.data.id ~= id then -- Switched unit
		MARS.InitializeData()
		MARS.SendInfoCommand(name, unit, id)
		MARS.data.name = name
		MARS.data.unit = unit
		MARS.data.id = id
	end
	
	internal = MARS.UnitHasInternalRadio(unit)
	if MARS.data.internal ~= internal then
		MARS.SendUseCommand(internal)
		MARS.data.internal = internal
	end
	
	if unit == "A-10C" then
		export = MARS.ExportA10
	elseif unit == "Ka-50" then
		export = MARS.ExportKA50
	elseif unit == "MiG-21Bis" then
		export = MARS.ExportMIG21
	elseif unit == "P-51D" or unit == "TF-51D" then
		export = MARS.ExportP51
	elseif unit == "UH-1H" then
		export = MARS.ExportUH1
	elseif unit == "F-86F Sabre" then
		export = MARS.ExportF86
	elseif unit == "FW-190D9" then
		export = MARS.ExportFW190
	elseif unit == "Bf-109K-4" then
		export = MARS.ExportBF109
	elseif unit == "Mi-8MT" then
		export = MARS.ExportMI8
	elseif unit == "MiG-15bis" then
		export = MARS.ExportMIG15
	elseif unit == "L-39C" then
		export = MARS.ExportL39
	end
	
	if export ~= nil then
		export()
	end
end

MARS.ExportA10 = function()
	local panel = GetDevice(0)
	
	local radio =
	{
		id = 1,
		name = "AN/ARC-186(V)",
		primary = MARS.Round(MARS.GetFrequency(55), 5000),
		secondary = 0,
		modulation = MARS.modulation.AM
	}
	
	local selector = panel:get_argument_value(239) -- Intercom selector dial
	if MARS.NearEqual(selector, 0.0, 0.03) then
		radio.primary = 0 -- Intercom selected
	end
	
	MARS.CheckRadio(1, radio)
	
	local volume = panel:get_argument_value(133)
	MARS.CheckVolume(1, volume)
	
	radio =
	{
		id = 2,
		name = "AN/ARC-164",
		primary = MARS.Round(MARS.GetFrequency(54), 5000),
		secondary = 0,
		modulation = MARS.modulation.AM
	}
	
	local knob = panel:get_argument_value(168) -- Function selector knob
	if MARS.NearEqual(knob, 0.2, 0.03) and radio.primary > 0 then
		radio.secondary = 243000000
	end
	
	volume = panel:get_argument_value(171)
	MARS.CheckVolume(2, volume)
	
	MARS.CheckRadio(2, radio)
	
	radio =
	{
		id = 3,
		name = "AN/ARC-186(V)",
		primary = MARS.Round(MARS.GetFrequency(56), 5000),
		secondary = 0,
		modulation = MARS.modulation.FM
	}
	
	MARS.CheckRadio(3, radio)
	
	volume = panel:get_argument_value(147)
	MARS.CheckVolume(3, volume)
end

MARS.ExportP51 = function()
	local radio =
	{
		id = 1,
		name = "SCR522A",
		primary = MARS.Round(MARS.GetFrequency(24), 5000),
		secondary = 0,
		modulation = MARS.modulation.AM
	}
	
	local selected = 1

	if MARS.data.selected ~= selected then
		MARS.SendSelectCommand(selected)
		MARS.data.selected = selected
	end
	
	MARS.CheckRadio(1, radio)
	MARS.ClearRadio(2)
	MARS.ClearRadio(3)
end

MARS.ExportMI8 = function()
	local radio =
	{
		id = 1,
		name = "R-828",
		primary = MARS.Round(MARS.GetFrequency(39), 50000),
		secondary = 0,
		modulation = MARS.modulation.FM
	}
	
	MARS.CheckRadio(1, radio)
	
	radio =
	{
		id = 2,
		name = "R-863",
		primary = MARS.Round(MARS.GetFrequency(38), 5000),
		secondary = 0,
		modulation = MARS.modulation.AM
	}
	
	MARS.CheckRadio(2, radio)
	
	radio =
	{
		id = 3,
		name = "JADRO-1A",
		primary = MARS.Round(MARS.GetFrequency(37), 5000),
		secondary = 0,
		modulation = MARS.modulation.AM
	}
	
	MARS.CheckRadio(3, radio)
	
	local panel = GetDevice(0)
	local switch = panel:get_argument_value(550)
	local selected = 0
	
	if MARS.NearEqual(switch, 0.0, 0.03) then
		selected = 2
	elseif MARS.NearEqual(switch, 0.1, 0.03) then
		selected = 3
	elseif MARS.NearEqual(switch, 0.2, 0.03) then
		selected = 1
	else
		selected = 0
	end
	
	if MARS.data.selected ~= selected then
		MARS.SendSelectCommand(selected)
		MARS.data.selected = selected
	end
end

MARS.ExportKA50 = function()
	local panel = GetDevice(0)

	local radio =
	{
		id = 1,
		name = "R-828",
		primary = MARS.Round(MARS.GetFrequency(49), 50000),
		secondary = 0,
		modulation = MARS.modulation.FM
	}
	
	MARS.CheckRadio(1, radio)
	
	local volume = panel:get_argument_value(372)
	MARS.CheckVolume(1, volume)

	radio =
	{
		id = 2,
		name = "R-800L1",
		primary = MARS.Round(MARS.GetFrequency(48), 5000),
		secondary = 0,
		modulation = MARS.modulation.AM
	}
	
	-- Get modulation mode
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
	
	MARS.CheckRadio(2, radio)

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
	
	MARS.ClearRadio(3)
end

MARS.ExportUH1 = function()
	local panel = GetDevice(0)
	local volume = 0
	
	local radio =
	{
		id = 1,
		name = "AN/ARC-131",
		primary = MARS.Round(MARS.GetFrequency(23), 5000),
		secondary = 0,
		modulation = MARS.modulation.FM
	}
	
	MARS.CheckRadio(1, radio)
	
	-- Volume knob
	volume = -(MARS.ReRange(panel:get_argument_value(37), 0.3, 1.0, 0.001, 1.0) - 1.0)
	
	-- Receiver switch
	if panel:get_argument_value(23) < 0.5 then
		volume = 0
	end
	
	MARS.CheckVolume(1, volume)
	
	radio =
	{
		id = 2,
		name = "AN/ARC-51BX",
		primary = MARS.Round(MARS.GetFrequency(22), 5000),
		secondary = 0,
		modulation = MARS.modulation.AM
	}
	
	
	local knob = panel:get_argument_value(17) -- Function selector knob
	if MARS.NearEqual(knob, 0.2, 0.03) and radio.primary > 0 then
		radio.secondary = 243000000
	end
	
	volume = -(panel:get_argument_value(21) - 1.0)
	-- Receiver switch
	if panel:get_argument_value(24) < 0.5 then
		volume = 0
	end
	MARS.CheckVolume(2, volume)
	MARS.CheckRadio(2, radio)
	
	radio =
	{
		id = 3,
		name = "AN/ARC-134",
		primary = MARS.Round(MARS.GetFrequency(20), 5000),
		secondary = 0,
		modulation = MARS.modulation.AM
	}
	
	MARS.CheckRadio(3, radio)
	volume = MARS.ReRange(panel:get_argument_value(8), 0.0, 0.65, 0.001, 1.0)
		-- Receiver switch
	if panel:get_argument_value(25) < 0.5 then
		volume = 0
	end
	MARS.CheckVolume(3, volume)
	
	local switch = panel:get_argument_value(30)
	local selected = 0
	
	if MARS.NearEqual(switch, 0.2, 0.03) then
		selected = 1
	elseif MARS.NearEqual(switch, 0.3, 0.03) then
		selected = 2
	elseif MARS.NearEqual(switch, 0.4, 0.03) then
		selected = 3
	else
		selected = 0
	end
	
	if MARS.data.selected ~= selected then
		MARS.SendSelectCommand(selected)
		MARS.data.selected = selected
	end
end

MARS.ExportF86 = function()
	local radio =
	{
		id = 1,
		name = "AN/ARC-27",
		primary = MARS.Round(MARS.GetFrequency(26), 5000),
		secondary = 0,
		modulation = MARS.modulation.AM
	}
	
	local panel = GetDevice(0)
	local knob = panel:get_argument_value(805)
	if MARS.NearEqual(knob, 0.2, 0.03) and radio.primary > 0 then
		radio.secondary = 243000000
	end
	
	local volume = panel:get_argument_value(806) -- 0.1 - 0.9
	volume = MARS.ReRange(volume, 0.1, 0.9, 0.001, 1.0)
	MARS.CheckVolume(1, volume)
	
	local selected = 1

	if MARS.data.selected ~= selected then
		MARS.SendSelectCommand(selected)
		MARS.data.selected = selected
	end
	
	MARS.CheckRadio(1, radio)
	MARS.ClearRadio(2)
	MARS.ClearRadio(3)
end

MARS.ExportFW190 = function()
	local radio =
	{
		id = 1,
		name = "FuG 16ZY",
		primary = MARS.Round(MARS.GetFrequency(15), 5000),
		secondary = 0,
		modulation = MARS.modulation.AM
	}
	
	local selected = 1

	if MARS.data.selected ~= selected then
		MARS.SendSelectCommand(selected)
		MARS.data.selected = selected
	end
	
	MARS.CheckRadio(1, radio)
	MARS.ClearRadio(2)
	MARS.ClearRadio(3)
end

MARS.ExportBF109 = function()
	local radio =
	{
		id = 1,
		name = "FuG 16ZY",
		primary = MARS.Round(MARS.GetFrequency(14), 5000),
		secondary = 0,
		modulation = MARS.modulation.AM
	}
	
	local selected = 1

	if MARS.data.selected ~= selected then
		MARS.SendSelectCommand(selected)
		MARS.data.selected = selected
	end
	
	MARS.CheckRadio(1, radio)
	MARS.ClearRadio(2)
	MARS.ClearRadio(3)
end

MARS.ExportMIG21 = function()
	local panel = GetDevice(0)
	
	local radio =
	{
		id = 1,
		name = "R-828",
		primary = MARS.Round(MARS.GetFrequency(22), 5000),
		secondary = 0,
		modulation = MARS.modulation.AM
	}
	
	-- Check compass/radio switch
	if panel:get_argument_value(208) < 0.5 then
		radio.primary = 0
	end
	
	local volume = panel:get_argument_value(210)
	MARS.CheckVolume(1, volume)
	
	local selected = 1

	if MARS.data.selected ~= selected then
		MARS.SendSelectCommand(selected)
		MARS.data.selected = selected
	end

	MARS.CheckRadio(1, radio)
	MARS.ClearRadio(2)
	MARS.ClearRadio(3)
end

MARS.ExportMIG15 = function()
	local radio =
	{
		id = 1,
		name = "RSI-6K",
		primary = MARS.Round(MARS.GetFrequency(30), 5000),
		secondary = 0,
		modulation = MARS.modulation.AM
	}
	
	local selected = 1

	if MARS.data.selected ~= selected then
		MARS.SendSelectCommand(selected)
		MARS.data.selected = selected
	end
	
	MARS.CheckRadio(1, radio)
	MARS.ClearRadio(2)
	MARS.ClearRadio(3)
end

MARS.ExportL39 = function()
	local radio =
	{
		id = 1,
		name = "R-832M",
		primary = MARS.Round(MARS.GetFrequency(20), 5000),
		secondary = 0,
		modulation = MARS.modulation.AM
	}
	
	local selected = 1

	if MARS.data.selected ~= selected then
		MARS.SendSelectCommand(selected)
		MARS.data.selected = selected
	end
	
	MARS.CheckRadio(1, radio)
	MARS.ClearRadio(2)
	MARS.ClearRadio(3)
end

MARS.CheckRadio = function(id, radio)
	if not MARS.FastCompare(MARS.data.radios[id], radio) then
		MARS.SendSetCommand(radio)
		MARS.data.radios[id] = MARS.FastCopy(radio)
	end
end

MARS.CheckVolume = function(id, volume)
	if MARS.data.volume[id] ~= volume then
		MARS.SendVolCommand(id, volume)
		MARS.data.volume[id] = volume
	end
end

MARS.ClearRadio = function(id)
	local radio = { id = id, name = "N/A", primary = 0, secondary = 0, modulation = 0 }
	MARS.CheckRadio(id, radio)
	MARS.CheckVolume(id, 1.0)
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

MARS.SendInfoCommand = function(name, unit, id)
	local command =
	{
		command = "info",
		name = name,
		unit = unit,
		id = id
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

MARS.SendVolCommand = function(id, volume)
	local command =
	{
		command = "vol",
		radio = id,
		volume = volume
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

MARS.ReRange = function(value, oldmin, oldmax, newmin, newmax)
	return (((value - oldmin) * (newmax - newmin)) / (oldmax - oldmin)) + newmin
end

MARS.UnitHasInternalRadio = function(unit)
	return MARS.unitsWithInternalRadio[unit] == true
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