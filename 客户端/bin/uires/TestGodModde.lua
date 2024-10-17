local old
local fps
local god

TestMode = {}
local nullfunction = function() end

local function preprocessor(bool)
	if bool then
		if old and not fps and not god then
			AccelKey_Q = old.AccelKey_Q
			AccelKey_E = old.AccelKey_E
			AccelKey_R = old.AccelKey_R
			AccelKey_F = old.AccelKey_F
			AccelKey_Z = old.AccelKey_Z
			AccelKey_C = old.AccelKey_C
			old = nil
		end
	else
		if not old then
			old = {}
			old.AccelKey_Q = AccelKey_Q
			old.AccelKey_E = AccelKey_E
			old.AccelKey_R = AccelKey_R
			old.AccelKey_F = AccelKey_F
			old.AccelKey_Z = AccelKey_Z
			old.AccelKey_C = AccelKey_C
			
			AccelKey_Q = nullfunction
			AccelKey_E = nullfunction
			AccelKey_R = nullfunction
			AccelKey_F = nullfunction
			AccelKey_Z = nullfunction
			AccelKey_C = nullfunction
		end
	end
end

function TestMode.fpsclose()
	preprocessor(true)
	fps = nil
	fps_close()
end

function TestMode.fpsopen()
	preprocessor(false)
	fps = true
	fps_open()
end

local function processor(t, n)
	preprocessor(false)
	god = true
	god_begin(t, n)
end

local default_type = 261002
local default_num = 6

local function parser(extra)
	local _, _, t, n = string.find(extra, "^(%d+)%s*(%d*)")
	processor(tonumber(t) or default_type, tonumber(n) or default_num)
end

util.registerCommand("^//god%s+(.*)", parser)
util.registerCommand("^//god$", function()
	processor(default_type, default_num)
end)
util.registerCommand("^//fps%s+enter", function()
	preprocessor(false)
	fps = true
	fps_open()
end)
util.registerCommand("^//fps$", TestMode.fpsopen)
util.registerCommand("^//fps%s+(%-?%d+)", function(dist)
	preprocessor(false)
	fps = true
	fps_open(1, tonumber(dist))
end)
util.registerCommand("^//fps%s+speed%s+(%d+%.?%d*)", function(times)
	preprocessor(false)
	fps = true
	fps_open(tonumber(times))
end)
util.registerCommand("^//fps%s+(%-?%d+)%s+speed%s+(%d+%.?%d*)", function(dist, times)
	preprocessor(false)
	fps = true
	fps_open(tonumber(times), tonumber(dist))
end)
util.registerCommand("^//fps%s+close", TestMode.fpsclose)
util.registerCommand("^//dummy%s+(%d*)", function(extra)
	god_test(tonumber(extra) or 100)
end)
util.registerCommand("^//dummy$", function()
	god_test(100)
end)

function resumeAccelKeys()
	god = nil
	preprocessor(true)
end