local CodeToKey = {
	[8]		= "退格键",
	[9]		= "Tab",
	[20]	= "Caps Lock",
	[32]	= "空格键",
	[33]	= "Page Up",
	[34]	= "Page Down",
	[35]	= "End",
	[36]	= "Home",
	[37]	= "←",
	[38]	= "↑",
	[39]	= "→",
	[40]	= "↓",
	[45]	= "Insert",
	[46]	= "Delete",
	[48]	= "0",
	[49]	= "1",
	[50]	= "2",
	[51]	= "3",
	[52]	= "4",
	[53]	= "5",
	[54]	= "6",
	[55]	= "7",
	[56]	= "8",
	[57]	= "9",
	[65]	= "A",
	[66]	= "B",
	[67]	= "C",
	[68]	= "D",
	[69]	= "E",
	[70]	= "F",
	[71]	= "G",
	[72]	= "H",
	[73]	= "I",
	[74]	= "J",
	[75]	= "K",
	[76]	= "L",
	[77]	= "M",
	[78]	= "N",
	[79]	= "O",
	[80]	= "P",
	[81]	= "Q",
	[82]	= "R",
	[83]	= "S",
	[84]	= "T",
	[85]	= "U",
	[86]	= "V",
	[87]	= "W",
	[88]	= "X",
	[89]	= "Y",
	[90]	= "Z",
	[93]	= "Menu",
	[96]	= "Num 0",
	[97]	= "Num 1",
	[98]	= "Num 2",
	[99]	= "Num 3",
	[100]	= "Num 4",
	[101]	= "Num 5",
	[102]	= "Num 6",
	[103]	= "Num 7",
	[104]	= "Num 8",
	[105]	= "Num 9",
	[106]	= "Num *",
	[107]	= "Num +",
	[109]	= "Num -",
	[110]	= "Num .",
	[111]	= "Num /",
	[112]	= "F1",
	[113]	= "F2",
	[114]	= "F3",
	[115]	= "F4",
	[116]	= "F5",
	[117]	= "F6",
	[118]	= "F7",
	[119]	= "F8",
	[120]	= "F9",
	[121]	= "F10",
	[122]	= "F11",
	[123]	= "F12",
	[144]	= "Num Lock",
	[186]	= ";",
	[187]	= "=",
	[188]	= ",",
	[189]	= "-",
	[190]	= ".",
	[191]	= "/",
	[192]	= "～",
	[219]	= "[",
	[220]	= "\\",
	[221]	= "]",
	[222]	= "'",
}

function KeyToString(code)
	return code and string.format("%s%s%s%s",
		IsCtrlPress(code) and "Ctrl + " or "",
		IsShiftPress(code) and "Shift + " or "",
		IsAltPress(code) and "Alt + " or "",
		CodeToKey[code % 256] or "")
end

function KeyToShort(code)
	if not code then
		return
	end
	local key = CodeToKey[code % 256] or ""
	if #key > 4 then
		key = "…"
	else
		--[[local byte
		for i = 1, #key do
			byte = string.byte(key, i, i)
			if not (IsDigit(byte) or IsAlpha(byte)) then
				key = "…"
				break
			end
		end]]
	end
	local str = string.format("%s%s%s%s",
		IsCtrlPress(code) and "c+" or "",
		IsShiftPress(code) and "s+" or "",
		IsAltPress(code) and "a+" or "",
		(key))
	if #str > 5 then
		str = string.sub(str, 1, 4).."…"
	end
	
	return str
end

local types = {
	"常规按键",
	"快捷栏按键",
}
local hasType = {}
for index, type in ipairs(types) do
	hasType[type] = {}
end

local Name2Type = {}
local NameFunction = {}

local NameHash = {}
local HashName = {}

function RegisterFunctionName(type, name, invoke, defaultKey)
	if NameFunction[name] then
		--error("功能名字重复："..name, 2)
		return
	end
	local hash = util.StringHash(name) % 65536
	if HashName[hash] then
		--error(string.format("功能名字Hash值与%s冲突：%s", HashName[hash], name), 2)
		--return
	end
	NameHash[name] = hash
	HashName[hash] = name
	if not hasType[type] then
		table.insert(types, type)
		hasType[type] = {}
	end
	table.insert(hasType[type], name)
	local t = {}
	NameFunction[name] = t
	t.call = invoke
	t.default = defaultKey or 0
end

function KeyMapTypes()
	local i = 1
	return function()
		if i > #types then
			return
		end
		i  = i + 1
		return types[i - 1]
	end
end

function KeyMapFunctions(t)
	local i = 1
	return function()
		local names = hasType[t]
		if not names then
			return
		end
		if i > #names then
			return
		end
		i  = i + 1
		return names[i - 1]
	end
end

function KeyMapTypesFunctions()
	local itype = 1
	local iname = 1
	local function iterate()
		if itype > #types then
			return
		end
		local t = types[itype]
		local names = hasType[t]
		if not names then
			return
		end
		if iname > #names then
			itype = itype + 1
			iname = 1
			return iterate()
		end
		iname = iname + 1
		return t, names[iname - 1]
	end
	return iterate
end

function CallNameFunction(name, press, code)
	local t = NameFunction[name]
	if not t then
		--error("不具备该功能："..name, 2)
		return
	end
	return t.call(press, code)
end

local updateSkip
local FunctionKey = {}
local KeyFunction = {}
local SandboxKeyMap
local keyToTrySet

function OnAccelerator(code, press)
	if getGameState() ~= STATE_INWORLD then
		return
	end
	if not CodeToKey[code % 256] then
		return
	end
	if keyToTrySet then
		if press then
			StartTrySetKeyMap()
			local old
			old, keyToTrySet = keyToTrySet
			SetKeyMap(old, code)
		end
	else
		local name = KeyFunction[code]
		--[[if SandboxKeyMap then
			name = SandboxKeyMap.KeyFunction[code]
		end]]
		if name then
			CallNameFunction(name, press, code)
		end
	end
end

function IsKeyMapChange()
	return SandboxKeyMap ~= nil
end

function StartTrySetKeyMap()
	if not SandboxKeyMap then
		SandboxKeyMap = {}
		SandboxKeyMap.FunctionKey = {}
		SandboxKeyMap.KeyFunction = {}
		for k, v in pairs(FunctionKey) do
			SandboxKeyMap.FunctionKey[k] = v
		end
		for k, v in pairs(KeyFunction) do
			SandboxKeyMap.KeyFunction[k] = v
		end
	end
end

function EndTrySetKeyMap(apply)
	keyToTrySet = nil
	if SandboxKeyMap and not apply then
		FunctionKey = SandboxKeyMap.FunctionKey
		KeyFunction = SandboxKeyMap.KeyFunction
		OnAccelUpdate()
	elseif SandboxKeyMap then
		SaveKeyMap()
	end
	SandboxKeyMap = nil
end

function SkipUpdate(skip)
	updateSkip = skip
	if not skip then
		OnAccelUpdate()
	end
end

function SetKeyMap(name, code)
	if code and code ~= 0 then
		local old = KeyFunction[code]
		if old then
			if not updateSkip then
				OnAccelOverlay(old)
			end
			FunctionKey[old] = 0
		end
		KeyFunction[code] = name
	end
	local old = FunctionKey[name]
	FunctionKey[name] = code
	if old then
		KeyFunction[old] = nil
	end
	if not updateSkip then
		OnAccelUpdate()
	end
end

function TrySetKeyMap(name)
	local old
	old, keyToTrySet = keyToTrySet, name
	return old
end

function GetKeyMap(name)
	local t = NameFunction[name]
	if not t then
		--error("不具备该功能："..name, 2)
		return
	end
	return FunctionKey[name], t.default
end

function ResetKeyMap(name)
	if name then
		SetKeyMap(name, NameFunction[name].default)
	else
		SkipUpdate(true)
		for name, t in pairs(NameFunction) do
			SetKeyMap(name, t.default)
		end
		SkipUpdate(false)
	end
end

function LoadKeyMap()
	local s = GameUserConfig:loadKeyMap()
	if not s then
		ResetKeyMap()
	else
		local f = loadstring("return "..s)
		if not f then
			ResetKeyMap()
		else
			local b, t = pcall(f)
			if not b or type(t) ~= "table" then
				ResetKeyMap()
			else
				SkipUpdate(true)
				FunctionKey = t
				for name, code in pairs(t) do
					KeyFunction[code] = name
				end
				for name in pairs(NameFunction) do
					if not t[name] then
						ResetKeyMap(name)
					end
				end
				SkipUpdate(false)
			end
		end
	end
end

function SaveKeyMap()
	local str = {}
	table.insert(str, "{")
	for name, code in pairs(FunctionKey) do
		table.insert(str, string.format("[\"%s\"]", name))
		table.insert(str, "=")
		table.insert(str, code)
		table.insert(str, ",")
	end
	table.insert(str, "}")
	local str = table.concat(str)
	GameUserConfig:storeKeyMap(str)
end

function OnAccelUpdate()
	local font
	for i = 1, MAX_SHORCUT do
		font = getglobal("MagicShortcut"..i.."Text")
		if font then
			local code
			if i >= 1 and i <= 10 then
				code = GetKeyMap("主快捷栏"..i)
			elseif i >= 13 and i <= 22 then
				code = GetKeyMap("扩展快捷栏"..(i - 12))
			elseif i >= 33 and i <= 42 then
				code = GetKeyMap("右1快捷栏"..(i - 32))
			elseif i >= 23 and i <= 32 then
				code = GetKeyMap("右2快捷栏"..(i - 22))
			end
			if code then
				font:SetText(KeyToShort(code))
			else
				font:SetText("")
			end
		end
	end
	for i = 1, MAX_TMP_SKILL_BTN do
		code = GetKeyMap("主快捷栏"..i)
		code = code and KeyToShort(code) or ""
		font = getglobal("TempMagicShortcut"..i.."Text")
		if font then
			font:SetText(code)
		end
		font = getglobal("ClanCollectTmpFrameCollectShortCut"..i.."Text")
		if font then
			font:SetText(code)
		end
		font = getglobal("ClanTankTmpFrameCollectShortCut"..i.."Text")
		if font then
			font:SetText(code)
		end
	end
	do
		local code
		
		code = GetKeyMap("主快捷栏1")
		code = code and KeyToShort(code) or ""
		TempFairyMagicShortcut1Text:SetText(code)
		
		code = GetKeyMap("主快捷栏2")
		code = code and KeyToShort(code) or ""
		TempFairyMagicShortcut2Text:SetText(code)
		
		code = GetKeyMap("主快捷栏7")
		code = code and KeyToShort(code) or ""
		ClanCollectTmpFrameCollectShortCut5Text:SetText(code)
		ClanTankTmpFrameCollectShortCut5Text:SetText(code)
		
		code = GetKeyMap("主快捷栏8")
		code = code and KeyToShort(code) or ""
		ClanCollectTmpFrameCollectShortCut6Text:SetText(code)
		ClanTankTmpFrameCollectShortCut6Text:SetText(code)
		
		code = GetKeyMap("主快捷栏9")
		code = code and KeyToShort(code) or ""
		TempMagicShortcut9Text:SetText(code)
		ClanCollectTmpFrameCollectShortCut7Text:SetText(code)
		ClanTankTmpFrameCollectShortCut7Text:SetText(code)
		
		code = GetKeyMap("主快捷栏10")
		code = code and KeyToShort(code) or ""
		TempMagicShortcut10Text:SetText(code)
		ClanCollectTmpFrameCollectShortCut8Text:SetText(code)
		ClanTankTmpFrameCollectShortCut8Text:SetText(code)
	end
	if KeySettingFrame:IsShown() then
		UpdateKeySettingFrame()
	end
	--[[
	for i = 1, 3 do
		font = getglobal("XPBufShortcut"..i.."Text")
		if font then
			code = GetKeyMap("精灵技能"..i)
			code = code and KeyToShort(code) or ""
			font:SetText(code)
		end
	end
	]]--
end

function OnAccelOverlay(name)
	if KeySettingFrame:IsShown() then
		OnAccelOverlayKeySettingFrame(name)
	end
end