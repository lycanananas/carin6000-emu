-- Text-only probe for the CARiN driver, loaded with -autoboot_script (see tools/carin_run.sh).
-- Configuration through environment variables (all optional):
--   PROBE_AT=12.5            emulated second at which registers, stack and memory are dumped
--   PROBE_MEM=9F8800:64,...  hex address:length pairs dumped at PROBE_AT
--   PROBE_WTAP=9F8800-9F88FF,...   log CPU writes to these ranges (pc, address, data)
--   PROBE_RTAP=...           same for reads
--   PROBE_EXIT=1             quit right after the PROBE_AT dump
-- Output: trace/probe.log (capped, duplicate pc/address pairs are counted, not repeated).

local machine = manager.machine
local cpu = machine.devices[":maincpu"]
local space = cpu.spaces["program"]

local LOG_PATH = "trace/probe.log"
local MAX_LINES = 400
local TAP_LINES_PER_PC = 6
local tap_counts = {}

local out = io.open(LOG_PATH, "w")
local lines = 0

local function log(text)
	if lines >= MAX_LINES then
		return
	end
	lines = lines + 1
	out:write(text, "\n")
	if lines == MAX_LINES then
		out:write("# probe.log line cap reached\n")
	end
	out:flush()
end

local function reg(name)
	local ok, value = pcall(function() return cpu.state[name].value end)
	return ok and value or 0
end

local function now()
	return machine.time:as_double()
end

local function parse_ranges(text)
	local ranges = {}
	for item in string.gmatch(text or "", "[^,]+") do
		local lo, hi = string.match(item, "^(%x+)%-(%x+)$")
		if lo then
			ranges[#ranges + 1] = { tonumber(lo, 16) & ~3, tonumber(hi, 16) | 3 }
		end
	end
	return ranges
end

local function install_taps(kind, ranges)
	for index, range in ipairs(ranges) do
		local name = string.format("probe_%s_%d", kind, index)
		local function callback(offset, data, mask)
			local pc = reg("PC")
			local key = string.format("%s pc=%06X", kind, pc)
			local count = (tap_counts[key] or 0) + 1
			tap_counts[key] = count
			if count <= TAP_LINES_PER_PC then
				local regs = ""
				if os.getenv("PROBE_REGS") == "1" then
					regs = string.format(" a0=%08X a1=%08X a2=%08X a3=%08X d0=%08X d1=%08X", reg("A0"), reg("A1"), reg("A2"), reg("A3"), reg("D0"), reg("D1"))
				end
				log(string.format("t=%.3f %s addr=%06X data=%08X mask=%08X pc=%06X%s", now(), kind, offset, data, mask, pc, regs))
				if os.getenv("PROBE_STACK") == "1" and count == 1 then
					local sp = reg("SP")
					local words = {}
					for i = 0, 23 do
						words[#words + 1] = string.format("%08X", space:read_u32(sp + i * 4))
					end
					log(string.format("  A6=%08X A7=%08X stack %s", reg("A6"), sp, table.concat(words, " ")))
				end
			end
			return data
		end
		if kind == "W" then
			space:install_write_tap(range[1], range[2], name, callback)
		else
			space:install_read_tap(range[1], range[2], name, callback)
		end
	end
end

install_taps("W", parse_ranges(os.getenv("PROBE_WTAP")))
install_taps("R", parse_ranges(os.getenv("PROBE_RTAP")))

local function dump_tap_counts()
	local keys = {}
	for key in pairs(tap_counts) do
		keys[#keys + 1] = key
	end
	table.sort(keys, function(a, b) return tap_counts[a] > tap_counts[b] end)
	for i = 1, math.min(#keys, 15) do
		log(string.format("TAPCOUNT %s n=%d", keys[i], tap_counts[keys[i]]))
	end
end

local function dump_breakpoints()
	if not os.getenv("PROBE_BP") then
		return
	end
	local debugger = manager.machine.debugger
	if not debugger then
		log("DEBUGGER unavailable")
		return
	end
	local console = debugger.consolelog
	log(string.format("DEBUGGER console=%d", #console))
	local written = 0
	for i = math.max(1, #console - 79), #console do
		local text = console[i]
		if text and string.find(text, "^BP ") and written < 40 then
			log(text)
			written = written + 1
		end
	end
end

local function dump_state()
	dump_tap_counts()
	dump_breakpoints()

	if os.getenv("PROBE_MODS") then
		-- PROBE_MODS=<substring>: list OS-9 modules in RAM (sync 4AFC) whose name contains it, "" = all.
		local filter = os.getenv("PROBE_MODS")
		for address = 0x800000, 0xA00000 - 0x30, 2 do
			if space:read_u16(address) == 0x4afc then
				local size = space:read_u32(address + 4)
				local name_offset = space:read_u32(address + 12)
				if size > 0x30 and size < 0x100000 and name_offset > 0 and name_offset < size then
					local chars = {}
					for i = 0, 31 do
						local c = space:read_u8(address + name_offset + i)
						chars[#chars + 1] = string.char(c & 0x7f)
						if c == 0 or c >= 0x80 then break end
					end
					local name = table.concat(chars)
					if filter == "" or string.find(name, filter, 1, true) then
						log(string.format("MOD %06X size=%X type=%02X name=%s", address, size, space:read_u8(address + 0x12), name))
					end
				end
			end
		end
	end
	local parts = {}
	for _, name in ipairs({ "PC", "SR", "SP" }) do
		parts[#parts + 1] = string.format("%s=%08X", name, reg(name))
	end
	for i = 0, 7 do
		parts[#parts + 1] = string.format("D%d=%08X", i, reg("D" .. i))
	end
	for i = 0, 6 do
		parts[#parts + 1] = string.format("A%d=%08X", i, reg("A" .. i))
	end
	log(string.format("t=%.3f REGS %s", now(), table.concat(parts, " ")))

	local sp = reg("SP")
	local words = {}
	for i = 0, 15 do
		words[#words + 1] = string.format("%08X", space:read_u32(sp + i * 4))
	end
	log("STACK " .. table.concat(words, " "))

	for item in string.gmatch(os.getenv("PROBE_MEM") or "", "[^,]+") do
		local address, length = string.match(item, "^(%x+):(%d+)$")
		if address then
			address = tonumber(address, 16)
			length = tonumber(length)
			for row = 0, length - 1, 16 do
				local bytes = {}
				for i = row, math.min(row + 15, length - 1) do
					bytes[#bytes + 1] = string.format("%02X", space:read_u8(address + i))
				end
				log(string.format("MEM %06X %s", address + row, table.concat(bytes, " ")))
			end
		end
	end
end

emu.add_machine_stop_notifier(function()
	local debugger = manager.machine.debugger
	if debugger and os.getenv("PROBE_BP") then
		local console = debugger.consolelog
		log(string.format("DEBUGGER console=%d errors=%d", #console, #debugger.errorlog))
		local written = 0
		for i = math.max(1, #console - 39), #console do
			local text = console[i]
			if text and written < 40 then
				log("CONSOLE " .. text)
				written = written + 1
			end
		end
		local errors = debugger.errorlog
		for i = math.max(1, #errors - 19), #errors do
			local text = errors[i]
			if text and (string.find(text, "breakpoint", 1, true) or string.find(text, "temp", 1, true)) then
				log("DEBUGGER " .. text)
			end
		end
	end
	out:close()
end)

-- PROBE_BP="821C18:d0,a0;82860A" -> breakpoint per address, logs pc and listed debugger expressions, max PROBE_BP_MAX hits each.
do
	local spec = os.getenv("PROBE_BP")
	local limit = tonumber(os.getenv("PROBE_BP_MAX")) or 20
	local index = 0
	for item in string.gmatch(spec or "", "[^;]+") do
		local address, exprs = string.match(item, "^(%x+):?(.*)$")
		if address and index < 10 then
			local fmt, args = "BP pc=%X", "pc"
			for expr in string.gmatch(exprs, "[^,]+") do
				fmt = fmt .. " " .. expr .. "=%X"
				args = args .. "," .. expr
			end
			local action = string.format('printf "%s\\n",%s; go', fmt, args)
			manager.machine.debugger:command(string.format("temp%d=0", index))
			local condition = string.format("(++temp%d <= %d)", index, limit)
			local id = cpu.debug:bpset(tonumber(address, 16), condition, action)
			log(string.format("BPSET id=%s addr=%s condition=%s", tostring(id), address, condition))
			index = index + 1
		end
	end
end

local probe_at = tonumber(os.getenv("PROBE_AT"))
local done = false

emu.register_frame_done(function()
	if probe_at and not done and now() >= probe_at then
		done = true
		dump_state()
		if os.getenv("PROBE_EXIT") == "1" then
			machine:exit()
		end
	end
end)
