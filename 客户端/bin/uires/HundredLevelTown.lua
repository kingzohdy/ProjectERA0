local floor = 1
local historyFloor = 0
local isPass = false
local isFail = false
local passTime

local pworldID = 300

local itemIcon = {}
local monsterTypeIcon = {}
local scrollFrame = {unit = 468}
local selectIdx = 0

local tFloor = { 1,5,10,20,30,40,50,60,70,80,90 }
--ͨ�ؽ���ö�ٱ���ս��Ʒ
function getNextBoxHundredLevelTownLayer(obj, layer)
	local ret = GameBCT.getNextBoxLayer(obj, layer)
	if ret == 0 then
		return nil
	end
	
	return ret
end
function EnumBoxHundredLevelTownLayer()
	return getNextBoxHundredLevelTownLayer, GameBCT, 0
end

--�����������(������һ�ػ�ȫ��ͨ��)
function ShowHundredLevelTownPass()
	if floor > 100 then
	else
		HundredLevelTownFinishDialog:Show()
	end
end

--���յ���ʷ��߲���Ϣ
function SetHundredLevelTownHistoryFloor(i)
	historyFloor = i > 0 and i or 0
	--����Ϣ�����Ž����Ƿ���ȡ�ĸ��µ���Ϣ
	if HundredLevelTownMainFrame:IsShown() then
		for i = 1, 100 do
			local openBtn = getglobal( "HundredLevelTownMainFrameScrollFrameBoxOpen"..i)
			local closeBtn = getglobal("HundredLevelTownMainFrameScrollFrameBoxClose"..i)
			if GameBCT:IsAwardReceive( i - 1 ) then
				openBtn:Show()
				closeBtn:Hide()
			else
				openBtn:Hide()
				closeBtn:Show()
				local closeUVAnimationTex = getglobal( closeBtn:GetName().."UVAnimationTex" )
				closeUVAnimationTex:SetUVAnimation( 48, true )		
				--�����ǰ���ͻ�ƽ���������ȡ�ˣ���ʾ��˸��Ч
				if i <= historyFloor then
					closeUVAnimationTex:Show()
				else
					closeUVAnimationTex:Hide()
				end
			end
			--�������û����������Ӳ���ʾ
			if GameBCT:GetAwardItem( i, 0 ).itemNum == 0 then
				openBtn:Hide()
				closeBtn:Hide()
			end
			local icon = getglobal("HundredLevelTownMainFrameScrollFrameBoxClose"..i.."Icon")
			local IconPath = GetItemEquipIconPath().."5300295.tga"
			-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
			if not IsInExistence(IconPath) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			icon:SetTexture(IconPath)
			icon:SetTexRelUV(1, 1)
		end
	end
	if HundredLevelTownFinishFrame:IsShown() then
		for i = 1, 100 do
			local openBtn = getglobal("HundredLevelTownFinishFrameScrollFrameBoxOpen"..i)
			local closeBtn = getglobal("HundredLevelTownFinishFrameScrollFrameBoxClose"..i)
			if GameBCT:IsAwardReceive( i - 1 ) then
				openBtn:Show()
				closeBtn:Hide()
			else
				openBtn:Hide()
				closeBtn:Show()
				local closeUVAnimationTex = getglobal( closeBtn:GetName().."UVAnimationTex" )
				closeUVAnimationTex:SetUVAnimation( 48, true )		
				--�����ǰ���ͻ�ƽ���������ȡ�ˣ���ʾ��˸��Ч
				if i <= historyFloor then
					closeUVAnimationTex:Show()
				else
					closeUVAnimationTex:Hide()
				end
			end
			--�������û����������Ӳ���ʾ
			if GameBCT:GetAwardItem( i, 0 ).itemNum == 0 then
				openBtn:Hide()
				closeBtn:Hide()
			end
			local icon = getglobal("HundredLevelTownFinishFrameScrollFrameBoxClose"..i.."Icon")
			local IconPath = GetItemEquipIconPath().."5300295.tga"
			-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
			if not IsInExistence(IconPath) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			icon:SetTexture(IconPath)
			icon:SetTexRelUV(1, 1)
		end
	end
end

--�������½���ٲ���
function ReEnterHundredLevelTown(current, start, pass)
	floor = current
	isPass = start + pass > current
	if isPass then
		SetHundredLevelTownFinishDialog(floor)
	else
		SetHundredLevelTownProcessingDialog(floor)
	end
	MMMapName:SetText(string.format("���֮��(%d��)", current))
end

function HundredLevelTownOpenBtn_OnLoad()
	this:RegisterEvent("GE_BCT_ENTER")
	this:RegisterEvent("GE_BCT_LEAVE")
	this:RegisterEvent("GE_ENTER_PLAYERLOGIN")
end

--�ٲ���������ʼ��
function HundredLevelTownMainFrame_OnLoad()
	HundredLevelTownMainFrameSelectScrollDownBtn:Disable()
	HundredLevelTownMainFrameSelectScrollUpBtn:Disable()
	local btn, text
	for i = 1, 10 do
		btn = getglobal("HundredLevelTownMainFrameSelectBtn"..i)
		text = getglobal("HundredLevelTownMainFrameSelectBtn"..i.."AreaName")
		if text then
			text:SetText(string.format("%02d", (i==1 and 1 or (i - 1) * 5)))
		end
	end
	
	for i = 1, 100 do
		--[[
		if GameBCT:hasBox(i) then
			btn = getglobal("HundredLevelTownMainFrameScrollFrameBoxClose"..i)
			btn:Show()
			local icon = getglobal("HundredLevelTownMainFrameScrollFrameBoxClose"..i.."Icon")
			local IconPath = GetItemEquipIconPath()..GameBCT:getBoxIcon(i)..".tga"
			-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
			if not IsInExistence(IconPath) then
				IconPath = DEFAULT_ITEM_PATH;
			end
			icon:SetTexture(IconPath)
			icon:SetTexRelUV(1, 1)
		end
		--]]
		local openBtn = getglobal( "HundredLevelTownMainFrameScrollFrameBoxOpen"..i)
		local closeBtn = getglobal("HundredLevelTownMainFrameScrollFrameBoxClose"..i)
		if GameBCT:IsAwardReceive( i - 1 ) then
			openBtn:Show()
			closeBtn:Hide()
		else
			openBtn:Hide()
			closeBtn:Show()
			local closeUVAnimationTex = getglobal( closeBtn:GetName().."UVAnimationTex" )
			closeUVAnimationTex:SetUVAnimation( 48, true )		
			--�����ǰ���ͻ�ƽ���������ȡ�ˣ���ʾ��˸��Ч
			if i <= historyFloor then
				closeUVAnimationTex:Show()
			else
				closeUVAnimationTex:Hide()
			end
		end
		--�������û����������Ӳ���ʾ
		if GameBCT:GetAwardItem( i, 0 ).itemNum == 0 then
			openBtn:Hide()
			closeBtn:Hide()
		end
		local icon = getglobal("HundredLevelTownMainFrameScrollFrameBoxClose"..i.."Icon")
		local IconPath = GetItemEquipIconPath().."5300295.tga"
		-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
		if not IsInExistence(IconPath) then
			IconPath = DEFAULT_ITEM_PATH;
		end
		icon:SetTexture(IconPath)
		icon:SetTexRelUV(1, 1)
	end
end

local function UpdateSelect()
	local btn, text
	for i=1,10 do
		btn = getglobal("HundredLevelTownMainFrameSelectBtn"..i)
		text = getglobal("HundredLevelTownMainFrameSelectBtn"..i.."AreaName")
		step = tFloor[i + selectIdx]
		text:SetText( string.format("%02d", step ))
		if step <= historyFloor + 1 then
			text:SetTextColor( 194, 179, 145 )
			btn:Enable()
		else
			text:SetTextColor( 60, 60, 50 )
			btn:Disable()
		end
	end

	for i = 1, 100 do
		btn = getglobal("HundredLevelTownMainFrameScrollFrameHistory"..i)
		if i == historyFloor then
			btn:Show()
		else
			btn:Hide()
		end
	end
	--��ʱ���°ٲ�ͻ�ƽ�������״̬
	for i = 1, 100 do
		local btn = getglobal( "HundredLevelTownMainFrameScrollFrameBoxOpen"..i)
		if btn then
			btn:Hide()
		end
	end
	for i = 1, 100 do
		local openBtn = getglobal( "HundredLevelTownMainFrameScrollFrameBoxOpen"..i)
		local closeBtn = getglobal("HundredLevelTownMainFrameScrollFrameBoxClose"..i)
		if GameBCT:IsAwardReceive( i - 1 ) then
			openBtn:Show()
			closeBtn:Hide()
		else
			openBtn:Hide()
			closeBtn:Show()
			local closeUVAnimationTex = getglobal( closeBtn:GetName().."UVAnimationTex" )
			closeUVAnimationTex:SetUVAnimation( 48, true )		
			--�����ǰ���ͻ�ƽ���������ȡ�ˣ���ʾ��˸��Ч
			if i <= historyFloor then
				closeUVAnimationTex:Show()
			else
				closeUVAnimationTex:Hide()
			end
		end
		--�������û����������Ӳ���ʾ
		if GameBCT:GetAwardItem( i, 0 ).itemNum == 0 then
			openBtn:Hide()
			closeBtn:Hide()
		end
		local icon = getglobal("HundredLevelTownMainFrameScrollFrameBoxClose"..i.."Icon")
		local IconPath = GetItemEquipIconPath().."5300295.tga"
		-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
		if not IsInExistence(IconPath) then
			IconPath = DEFAULT_ITEM_PATH;
		end
		icon:SetTexture(IconPath)
		icon:SetTexRelUV(1, 1)
	end
end
function HundredLevelTownMainFrame_OnShow()
	HundredLevelTownFinishFrame:Hide()
	this:SetPoint("center", "UIClient", "center", 0, 0)
	local data = PworldLimitInfo:getPworldData(pworldID)
	HundredLevelTownMainFrameItemLink:SetLinkTextColor(120, 230, 50)
	HundredLevelTownMainFrameItemLink:SetText(string.format("Ĭ�Ͻ������  1��\n���߽������  1��(����#L���֮��@@%d#n)", data.itemId), 194, 179, 145)
	HundredLevelTownMainFrameFreeTime:SetText(string.format("%d/%d", data.freeTimeUse>data.freeTimeTotal and data.freeTimeTotal or data.freeTimeUse, data.freeTimeTotal))
	HundredLevelTownMainFrameItemTime:SetText(string.format("%d/%d", data.itemTimeUse, data.itemTimeTotal))
	HundredLevelTownMainFrameVipTime:SetText( string.format("%d/%d", (data.freeTimeTotal-data.freeTimeUse)<0 and math.abs(data.freeTimeTotal-data.freeTimeUse) or 0, VipManager:getBCTVipNum()) )
	ResetHundredLevelTownScrollFrame(HundredLevelTownMainFrameScrollFrame, 0)
	HundredLevelTownMainFrameSetFloor(1)
	
	SetScrollBar( "HundredLevelTownMainFrameSelectScrollBar", 10, #tFloor )
	HundredLevelTownMainFrameSelectScrollBar:SetValue( 1 )
	selectIdx = 1

	UpdateSelect()
end
function HundredLevelTownMainFrame_OnHide()
	HundredLevelTownIntroduce:Hide()
end

function HundredLevelTownMainFrameSelectScollUpBtn_OnClick()
	local slider = getglobal( "HundredLevelTownMainFrameSelectScrollBar" );
	if not slider:IsShown() then
		return;
	end

	local nValue = slider:GetValue() - slider:GetValueStep();
	if nValue >= 0 then
		slider:SetValue( nValue );
	end
end

function HundredLevelTownMainFrameSelectScollDownBtn_OnClick()
	local slider	= getglobal( "HundredLevelTownMainFrameSelectScrollBar" );
	if not slider:IsShown() then
		return;
	end

	local nMaxValue = slider:GetMaxValue();
	local nValue	= slider:GetValue() + slider:GetValueStep();

	if nValue <= nMaxValue then
		slider:SetValue( nValue );
	end
end

function HundredLevelTownMainFrameSelectSlider_OnValueChanged()
	local val = this:GetValue()
	selectIdx = 1 - val
	UpdateSelect()
end
--�����б��ѡ��
function HundredLevelTownSelectBtn_OnClick()
	util.closeParent(this)
	local name = this:GetName()
	if string.find(name, "HundredLevelTownMainFrameSelect") then--������ѡ��
		local text = getglobal(name.."AreaName")
		if text then
			HundredLevelTownMainFrameSetFloor(tonumber(text:GetText()))
		end
	elseif string.find(name, "HundredLevelTownIntroduceSelect") then--���Ĺ��Ե�ѡ��
		local text = getglobal(name.."AreaName")
		if text then
			SetHundredLevelTownIntroduce(tonumber(text:GetText()))
		end
	end
end

--�ٲ���ͳ������ս��Ʒ����
function HundredLevelTownFinishFrameTotalItem_OnClick()
	if arg3 == "LeftButton" or arg3 == "RightButton" then
		local id = tonumber(arg2)
		if id then
			ShowItemTips(LUA_ITEMTIPS_TYPE_LINK, LUA_ITEMTIPS_ITEMDEF, getItemDef(id))
		end
	end
end

--��ʼ���ٲ���ͳ�����
function SetHundredLevelTownFinishFrame(maxLayer, layerNum, totalExp)
	HundredLevelTownFinishFrameFloorCount:SetText(string.format("���δ��֮�Ĺ�ͨ��%d��", layerNum))
	if layerNum == 0 then
		HundredLevelTownFinishFrameFloorMax:SetText("��", 194, 179, 145)
	else
		HundredLevelTownFinishFrameFloorMax:SetText(string.format("��#c78e632��%d��#n��", maxLayer), 194, 179, 145)
	end
	HundredLevelTownFinishFrameTotalExp:SetText(totalExp)
	local remain = GameWheelLottery:remain()
	if remain > 0 then
		HundredLevelTownFinishFrameItemLink:SetLinkTextColor(120, 230, 50)
		--HundredLevelTownFinishFrameItemLink:SetText(string.format("����ͨ��������%d�γ齱���ᣬ#L�����ȡ#n", remain), 194, 179, 145)
		HundredLevelTownFinishFrameItemLink:SetText( "", 255,255,255 )
		HundredLevelTownMainFrameShowHeroList:Hide()
		HundredLevelTownMainFrameShowGameWheel:Show()
		HundredLevelTownMainFrameShowGameWheelCount:SetText( remain .."�����˳齱��" )
		HundredLevelTownMainFrameShowGameWheelEffectTex:SetUVAnimation( 80, true )
	else
		HundredLevelTownFinishFrameItemLink:SetText("���ź�������ͨ����û�л�ó齱����", 194, 179, 145)
		HundredLevelTownMainFrameShowHeroList:Show()
		HundredLevelTownMainFrameShowGameWheel:Hide()
	end
	local layers = {}
	local items = {}
	for l in EnumBoxHundredLevelTownLayer() do
		table.insert(layers, l)
		local box = GameBCT:getLayerItem(l)
		if box then
			table.insert(items, box.id)
		end
	end	
	--[[
	for i = 1, 100 do
		btn = getglobal("HundredLevelTownFinishFrameScrollFrameBoxOpen"..i)
		if btn then
			btn:Hide()
		end
	end
	for _, v in ipairs(layers) do
		btn = getglobal("HundredLevelTownFinishFrameScrollFrameBoxOpen"..v)
		if btn then
			btn:Show()
		end
	end
	--]]
	for i = 1, 100 do
		local openBtn = getglobal("HundredLevelTownFinishFrameScrollFrameBoxOpen"..i)
		local closeBtn = getglobal("HundredLevelTownFinishFrameScrollFrameBoxClose"..i)
		if GameBCT:IsAwardReceive( i - 1 ) then
			openBtn:Show()
			closeBtn:Hide()
		else
			openBtn:Hide()
			closeBtn:Show()
			local closeUVAnimationTex = getglobal( closeBtn:GetName().."UVAnimationTex" )
			closeUVAnimationTex:SetUVAnimation( 48, true )		
			--�����ǰ���ͻ�ƽ���������ȡ�ˣ���ʾ��˸��Ч
			if i <= historyFloor then
				closeUVAnimationTex:Show()
			else
				closeUVAnimationTex:Hide()
			end
		end
		--�������û����������Ӳ���ʾ
		if GameBCT:GetAwardItem( i, 0 ).itemNum == 0 then
			openBtn:Hide()
			closeBtn:Hide()
		end
		local icon = getglobal("HundredLevelTownFinishFrameScrollFrameBoxClose"..i.."Icon")
		local IconPath = GetItemEquipIconPath().."5300295.tga"
		-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
		if not IsInExistence(IconPath) then
			IconPath = DEFAULT_ITEM_PATH;
		end
		icon:SetTexture(IconPath)
		icon:SetTexRelUV(1, 1)
	end
	local len = 0
	local newLine = false
	local text
	local num = 1
	for _, v in ipairs(items) do
		local item = getItemDef(v)
		if item then
			if text then
				text = text .. "  "
				len = len + 2
			else
				text = ""
			end
			len = len + #item.Name
			if len > 48 then
				if newLine then
					text = text .. "..."
					break
				end
				newLine = true
				text = text .. "\n"
				len = #item.Name
			end
			if GameWheelLottery:remain() == 0 or num <= 4 then
				text = text .. string.format("#L#c%s%s@@%d#n", getItemColor(item.Important,item.SuitSetID), item.Name, v)
			end
			num = num + 1
		end
	end
	HundredLevelTownFinishFrameTotalItem:SetText(text or "", 255, 255, 255)
	for i = 1, 100 do
		btn = getglobal("HundredLevelTownFinishFrameScrollFrameHistory"..i)
		if i == historyFloor then
			btn:Show()
		else
			btn:Hide()
		end
		btn = getglobal("HundredLevelTownFinishFrameScrollFrameCurrent"..i)
		if i == floor then
			btn:Show()
		else
			btn:Hide()
		end
	end
	HundredLevelTownMoveToFloor(floor)
	HundredLevelTownFinishFrame:Show()
end

--��ʼ���ٲ���ͳ�����
function HundredLevelTownFinishFrame_OnShow()
	this:SetPoint("center", "UIClient", "center", 0, 0)
	local data = PworldLimitInfo:getPworldData(pworldID)
	HundredLevelTownFinishFrameFreeTime:SetText(string.format("%d/%d", data.freeTimeUse>data.freeTimeTotal and data.freeTimeTotal or data.freeTimeUse, data.freeTimeTotal))
	HundredLevelTownFinishFrameItemTime:SetText(string.format("%d/%d", data.itemTimeUse, data.itemTimeTotal))
	HundredLevelTownFinishFrameVipTime:SetText( string.format("%d/%d", (data.freeTimeTotal-data.freeTimeUse)<0 and math.abs(data.freeTimeTotal-data.freeTimeUse) or 0, VipManager:getBCTVipNum()) )
	
	local i = floor
	HundredLevelTownFinishFrameFloorNum:SetText(string.format("��%d��", i))
	HundredLevelTownFinishFrameExp:SetText(GameBCT:getAwardExp(i))
	if GameBCT:hasBox(i) then--��ʾ�������
		SetHundredLevelTownItemIcon(HundredLevelTownFinishFrameBox, -i)
		HundredLevelTownFinishFrameBox:Show()
	else--��ʾ�ձ���
		HundredLevelTownFinishFrameBox:Hide()
	end
	--����ͻ�ƽ���
	local awardItemInfo = GameBCT:GetAwardItem( i, 0 )
	local ItemBtn = getglobal( "HundredLevelTownFinishFrameBreakAwardItem1" )
	if awardItemInfo.itemID == 0 or awardItemInfo.itemNum == 0 then
		ItemBtn:Hide()
		local numFont = getglobal( "HundredLevelTownFinishFrameBreakAwardItemNum1" )
		numFont:SetText( "��" )
		ItemBtn:SetClientUserData( 0, 0 )
	else
		ItemBtn:Show()
		local btnIcon = getglobal( ItemBtn:GetName().."Icon" )			
		local ItemDef = getItemDef( awardItemInfo.itemID )
		local IconPath = GetItemEquipIconPath()..ItemDef.IconID..".tga"
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end
		btnIcon:SetTexture( IconPath )
		local numFont = getglobal( "HundredLevelTownFinishFrameBreakAwardItemNum1" )
		numFont:SetText( awardItemInfo.itemNum )
		ItemBtn:SetClientUserData( 0, awardItemInfo.itemID )
	end
	
	local monsterType = GameBCT:getMonsterType(i)
	for n = 1, 3 do
		local btn = getglobal("HundredLevelTownFinishFrameMonster"..n)
		if btn then
			btn:Hide()
		end
	end
	for n = 1, monsterType.Num do
		local btn = getglobal("HundredLevelTownFinishFrameMonster"..n)
		if btn then
			btn:Show()
		else
			break;
			--btn = CreateButton("HundredLevelTownFinishFrameMonster"..n, "HundredLevelTownMonsterIconTemplate", HundredLevelTownFinishFrameRight2)
		end
		local icon = getglobal("HundredLevelTownFinishFrameMonster"..n.."Icon")
		if icon then
			local IconID = GameBCT:getMonsterIcon(monsterType.MonsterID[n - 1])
			icon:SetTexture(GetMonsterIconPath(IconID))
			icon:SetTexRelUV(1, 1)
		end
		monsterTypeIcon[n] = string.format("#c32E6D3%d��%s#n", monsterType.Level[n - 1], GameBCT:getMonsterName(monsterType.MonsterID[n - 1]))
	end
	local current = HundredLevelTownFinishFrameScrollFrame:GetVerticalOffset()
	local target = scrollFrame.target
	if not target then
		return
	end
	target = target / 10 * scrollFrame.unit
	HundredLevelTownFinishFrameScrollFrame:IncreaseVerticalScroll(target - current)
end

function HundredLevelTownFinishFrame_OnHide()
	GameWheelLotteryFrame:Hide()
	HundredLevelTownLotteryRemindDialog:Hide()
	HundredLevelTownIntroduce:Hide()
	GameWheelLottery:reset()
end

--�رհٲ���ͳ�����
function HundredLevelTownFinishFrame_OnClose()
	if not GameWheelLotteryFrameClose:IsEnable() then
		return GameWheelLotteryFrame:Active()
	end
	GameWheelLotteryFrame:Hide()
	if GameWheelLottery:remain() > 0 then
		return HundredLevelTownLotteryRemindDialog:Show()
	end
	return HundredLevelTownFinishFrame:Hide()
end

--�����齱
function HundredLevelTownLotteryRemindDialog_OnQuit()
	util.closeParent(this)
	HundredLevelTownFinishFrame:Hide()
end

--�ٲ����������ʾָ��¥����Ϣ
function HundredLevelTownMainFrameSetFloor(i)
	floor = i
	HundredLevelTownMainFrameResultAreaName:SetText(string.format("%02d", i))
	HundredLevelTownMainFrameFloorNum:SetText(string.format("��%d��", i))
	HundredLevelTownMainFrameExp:SetText(GameBCT:getAwardExp(i))
	if GameBCT:hasBox(i) then--��ʾ�������
		SetHundredLevelTownItemIcon(HundredLevelTownMainFrameBox, -i)
		HundredLevelTownMainFrameBox:Show()
	else--��ʾ�ձ���
		HundredLevelTownMainFrameBox:Hide()
	end
	local monsterType = GameBCT:getMonsterType(i)
	for n = 1, 3 do
		local btn = getglobal("HundredLevelTownMainFrameMonster"..n)
		if btn then
			btn:Hide()
		end
	end
	for n = 1, monsterType.Num do
		local btn = getglobal("HundredLevelTownMainFrameMonster"..n)
		if btn then
			btn:Show()
		else
			break;
			--btn = CreateButton("HundredLevelTownMainFrameMonster"..n, "HundredLevelTownMonsterIconTemplate", HundredLevelTownMainFrameRight2)
		end
		local icon = getglobal("HundredLevelTownMainFrameMonster"..n.."Icon")
		if icon then
			local IconID = GameBCT:getMonsterIcon(monsterType.MonsterID[n - 1])
			icon:SetTexture(GetMonsterIconPath(IconID))
			icon:SetTexRelUV(1, 1)
		end
		monsterTypeIcon[n] = string.format("#c32E6D3%d��%s#n", monsterType.Level[n - 1], GameBCT:getMonsterName(monsterType.MonsterID[n - 1]))
	end
	for n = 1, 100 do
		btn = getglobal("HundredLevelTownMainFrameScrollFrameCurrent"..n)
		if n == i then
			btn:Show()
		else
			btn:Hide()
		end
	end
	HundredLevelTownMoveToFloor(i)
	--����ͻ�ƽ���
	local awardItemInfo = GameBCT:GetAwardItem( i, 0 )
	local btn = getglobal( "HundredLevelTownMainFrameBreakAwardItem1" )
	if awardItemInfo.itemID == 0 or awardItemInfo.itemNum == 0 then
		btn:Hide()
		local numFont = getglobal( "HundredLevelTownMainFrameBreakAwardItemNum1" )
		numFont:SetText( "��" )
		btn:SetClientUserData( 0, 0 )
	else
		btn:Show()
		local btnIcon = getglobal( btn:GetName().."Icon" )			
		local ItemDef = getItemDef( awardItemInfo.itemID )
		local IconPath = GetItemEquipIconPath()..ItemDef.IconID..".tga"
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end
		btnIcon:SetTexture( IconPath )
		local numFont = getglobal( "HundredLevelTownMainFrameBreakAwardItemNum1" )
		numFont:SetText( awardItemInfo.itemNum )
		btn:SetClientUserData( 0, awardItemInfo.itemID )
	end
end

--��ʾ�ٲ�������tips
function HundredLevelTownMonsterIcon_OnEnter()
	local name = this:GetName()
	local _, _, n = string.find(name, "(%d+)")
	if n then
		n = tonumber(n)
	end
	if not n then
		return
	end
	SetGameTooltips(name, monsterTypeIcon[n] or "")
end
function HundredLevelTownMonsterIcon_OnLeave()
	GameTooltipRichText:SetText("",255,255,255)
	GameTooltip:Hide()
end

--�򿪴��֮�˹������
function HundredLevelTownMainFrameItemLink_OnClick()
	if arg3 == "LeftButton" or arg3 == "RightButton" then
		local id = tonumber(arg2)
		if id then
			MarketManager:selectMarketItem(id)
		end
	end
end

--�򿪳齱����
function HundredLevelTownFinishFrameItemLink_OnClick()
	HundredLevelTownMainFrameShowGameWheelEffectTex:Hide()
	if GameWheelLotteryFrame:IsShown() then
		GameWheelLotteryFrame:Active()
	else
		GameWheelLotteryFrame:Show()
	end
end

function HundredLevelTownOpenMenu()
	if isPass then
		ShowHundredLevelTownPass()
	elseif isFail then
		HundredLevelTownFailDialog:Show()
	elseif not passTime then
		HundredLevelTownProcessingDialog:Show()
	end
end

--��ع��̵��С��ͼ���
function HundredLevelTownOpenBtn_OnClick()
	HideMinimapOpenPworldXinShouBtn(this)
	HundredLevelTownOpenMenu()
end

--��ع���С��ͼ���tips
function HundredLevelTownOpenBtn_OnEnter()
	SetGameTooltips(this:GetName(), "������Ϣ")
end
function HundredLevelTownOpenBtn_OnLeave()
	GameTooltipRichText:SetText("",255,255,255)
	GameTooltip:Hide()
end

function HundredLevelTownOpenBtn_OnEvent()
	if arg1 == "GE_BCT_ENTER" then
		isPass = false
		isFail = false
		this:Show()
		SetHundredLevelTownProcessingDialog(floor)
		MMMapName:SetText(string.format("���֮��(%d��)", floor))
	elseif arg1 == "GE_BCT_LEAVE" then
		this:Hide()
	elseif arg1 == "GE_ENTER_PLAYERLOGIN" then
		floor = 1
		isPass = false
		historyFloor = 0
		this:Hide()
	end
end

--�ӳٵ����������
function HundredLevelTownOpenBtn_OnUpdate()
	if passTime then
		local now = getServerTime()
		local diff = (now.sec - passTime.sec) + (now.usec - passTime.usec) * 0.001
		if diff > 1.5 then
			passTime = nil
			isPass = true
			HundredLevelTownOpenBtn_OnClick()
		end
	end
end

--�򿪰ٲ��������
function openHundredLevelTown(npcId)
	if HundredLevelTownFinishFrame:IsShown() then
		HundredLevelTownFinishFrame:Hide()
	end
	if not HundredLevelTownMainFrame:IsShown() and not HundredLevelTownConfirmUseFreeDialog:IsShown() and
			not HundredLevelTownPayDialog:IsShown() then
		HundredLevelTownMainFrame:Show()
		util.testNpcDialogDistance(npcId, HundredLevelTownMainFrame, HundredLevelTownConfirmUseFreeDialog, HundredLevelTownPayDialog)
	end
end

--ȷ�Ͻ��븱���Ի����ʼ��
function HundredLevelTownConfirmUseFreeDialog_OnShow()
	HundredLevelTownConfirmUseFreeDialogTextMiddle:SetText(string.format("��%d��", floor))
	util.ShowInCenter(this)
end
function HundredLevelTownPayDialog_OnShow()
	HundredLevelTownPayDialogTextMiddle:SetText(string.format("��%d��", floor))
	util.ShowInCenter(this)
end

--���������Ʒչʾ���ʼ��
function SetHundredLevelTownShowRandomItems(i)
	HundredLevelTownShowRandomItemsTitleMiddle:SetText(string.format("��%d��", i))
	local t = GameBCT:getBoxItem(i)
	for n = 1, t.Num do
		SetHundredLevelTownItemIcon(getglobal("HundredLevelTownShowRandomItemsItem"..n), t.ItemID[n - 1], t.ItemNum[n - 1])
	end
	for n = t.Num + 1, 15 do
		SetHundredLevelTownItemIcon(getglobal("HundredLevelTownShowRandomItemsItem"..n))
	end
	HundredLevelTownShowRandomItems:Show()
end
--ͻ�ƽ�����Ʒ��ʾ
function SetHundredLevelTownBreakAwardItems(i)
	HundredLevelTownBreakAwardItemsTitleMiddle:SetText(string.format("��%d��", i))
	local awardItemInfo = GameBCT:GetAwardItem( i, 0 )
	if awardItemInfo.itemID == 0 or awardItemInfo.itemNum == 0 then
		return
	end
	local btn = getglobal( "HundredLevelTownBreakAwardItemsItem" )
	local btnBK = getglobal( "HundredLevelTownBreakAwardItemsItemBackTexture" )
	btnBK:Show()
	local btnIcon = getglobal( btn:GetName().."IconTexture" )
	local ItemDef = getItemDef( awardItemInfo.itemID )
	local IconPath = GetItemEquipIconPath()..ItemDef.IconID..".tga"
	if not IsInExistence( IconPath ) then
		IconPath = DEFAULT_ITEM_PATH;
	end
	btnIcon:SetTexture( IconPath )
	local numFont = getglobal( "HundredLevelTownBreakAwardItemsNum" )
	numFont:SetText( "x"..awardItemInfo.itemNum )
	btn:SetClientUserData( 0, awardItemInfo.itemID )
	HundredLevelTownBreakAwardItems:Show()
	HundredLevelTownBreakAwardItems:SetClientUserData( 0, i )
end

--���ðٲ�������ͼ����
function SetHundredLevelTownItemIcon(which, id, num)
	if not which then
		return
	end
	local name = which:GetName()
	itemIcon[name] = id
	local font = getglobal(name.."NumFont")
	if font then
		if num and num > 1 then
			font:SetText(num)
		else
			font:SetText("")
		end
	end
	local icon = getglobal(name.."Icon")
	if icon then
		if id and id ~= 0 then
			if id > 0 then
				local ItemDef = getItemDef(id)
				local IconPath = DEFAULT_ITEM_PATH
				if ItemDef then
					IconPath = GetItemEquipIconPath()..ItemDef.IconID..".tga"
				end
				-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
				if not IsInExistence(IconPath) then
					IconPath = DEFAULT_ITEM_PATH;
				end
				icon:SetTexture(IconPath)
				icon:SetTexRelUV(1, 1)
				SetEquipImportantEffectUV(ItemDef, name.."UVAnimationTex")
			elseif id < 0 then
				local IconPath = GetItemEquipIconPath()..GameBCT:getBoxIcon(-id)..".tga"
				-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
				if not IsInExistence(IconPath) then
					IconPath = DEFAULT_ITEM_PATH;
				end
				icon:SetTexture(IconPath)
				icon:SetTexRelUV(1, 1)
			end
		else
			icon:SetTexture("uires\\ZhuJieMian\\1.dds")
			icon:SetTexUV(794, 517, 44, 44)
			local UVAnimationTex = getglobal(name.."UVAnimationTex")
			if UVAnimationTex then
				UVAnimationTex:Hide()
			end
		end
	end
end

--����ٲ�������ͼ��
function HundredLevelTownItemIcon_OnClick()
	local id = itemIcon[this:GetName()]
	if id and id < 0 then--ֻ�����������Ե��
		SetHundredLevelTownShowRandomItems(-id)
	end
end

--��ʾ�ٲ�����������
function HundredLevelTownItemIcon_OnEnter()
	local name = this:GetName()
	local id = itemIcon[name]
	if id then
		if id > 0 then--��Ʒtips
			ShowItemTips(LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, getItemDef(id))
		elseif id < 0 then--�������tips
			local tex = getglobal(name.."Highlight")
			if tex then
				tex:Show()
			end
		end
	end
end
function HundredLevelTownItemIcon_OnLeave()
	ItemButton_OnLeave()
	local name = this:GetName()
	local tex = getglobal(name.."Highlight")
	if tex then
		tex:Hide()
	end
end

--������������ͼ��
function HundredLevelTownScrollFrameBox_OnEnter()
	local tex = getglobal(this:GetName().."Highlight")
	if tex then
		tex:Show()
	end
end
function HundredLevelTownScrollFrameBox_OnLeave()
	local tex = getglobal(this:GetName().."Highlight")
	if tex then
		tex:Hide()
	end
end
function HundredLevelTownScrollFrameBox_OnClick()
	--SetHundredLevelTownShowRandomItems(this:GetClientID())
	SetHundredLevelTownBreakAwardItems( this:GetClientID() )
end

function HundredLevelTownBreakAwardItemsGetBtn_OnClick()
	local layer = HundredLevelTownBreakAwardItems:GetClientUserData( 0 )
	if layer > historyFloor then
		ShowMidTips( "�ݲ�����ȡ�ò㽱��" )
		HundredLevelTownBreakAwardItems:Hide()
		return
	end
	GameBCT:GetAward( layer )
	HundredLevelTownBreakAwardItems:Hide()
	HundredLevelTownBreakAwardItems:Hide()
end

function HundredLevelTownBreakAwardItem_OnEnter()
	local itemID = this:GetClientUserData( 0 )
	local ItemDef = getItemDef( itemID )
	if ItemDef ~= nil then
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, ItemDef );
	end
end

--�յ�ͨ����Ϣ
function SetHundredLevelTownFinishDialog(i)
	if historyFloor < i then
		historyFloor = i
	end
	floor = i + 1
	HundredLevelTownProcessingDialog:Hide()
	passTime = getServerTime()
end

--��ʼ���ٲ����������
function HundredLevelTownFinishDialog_OnShow()
	local i = floor - 1
	HundredLevelTownFinishDialogFloorMiddle:SetText(string.format("��%d��", i + 1))
	HundredLevelTownFinishDialogInfo:SetText(GameBCT:getDesciption(i + 1))
	local history = GameBCT:getHistory(i)
	if history then
		if history.TopName == "" then
			HundredLevelTownFinishDialogWho:SetText("��")
		else
			HundredLevelTownFinishDialogWho:SetText(history.TopName)
		end
		if history.SelfWhen == 0 then
			HundredLevelTownFinishDialogWhen:SetText("��")
		else
			HundredLevelTownFinishDialogWhen:SetText(os.date("%Y.%m.%d", history.SelfWhen))
		end
		if history.TopTime == 0 and history.TopTimeMs == 0 then
			HundredLevelTownFinishDialogTheFirst:SetText("��", 194, 179, 145)
		else
			HundredLevelTownFinishDialogTheFirst:SetText(string.format("#c78e632%02d#n��#c78e632%02d.%02d#n��", math.floor(history.TopTime / 60), history.TopTime % 60, history.TopTimeMs), 194, 179, 145)
		end
		if history.SelfTime == 0 and history.SelfTimeMs == 0 then
			HundredLevelTownFinishDialogMine:SetText("��", 194, 179, 145)	
		else
			HundredLevelTownFinishDialogMine:SetText(string.format("#c78e632%02d#n��#c78e632%02d.%02d#n��", math.floor(history.SelfTime / 60), history.SelfTime % 60, history.SelfTimeMs), 194, 179, 145)	
		end
	end
	history = GameBCT:getHistory(i + 1)
	if history then
		HundredLevelTownFinishDialogNextWho:SetText(history.TopName)
		if history.TopTime == 0 then
			HundredLevelTownFinishDialogNextTheFirst:SetText("��", 194, 179, 145)
		else
			HundredLevelTownFinishDialogNextTheFirst:SetText(string.format("#c78e632%02d#n��#c78e632%02d.%02d#n��", math.floor(history.TopTime / 60), history.TopTime % 60, history.TopTimeMs), 194, 179, 145)
		end
	end

	local nextBoss = 5 *( math.floor((floor-1)/5) + 1 )
	if nextBoss <= historyFloor then
		HundredLevelTownFinishDialogBossLeft:Show()
		HundredLevelTownFinishDialogBossRight:Show()
		HundredLevelTownFinishDialogFrameRightTitle5:Show()
		HundredLevelTownFinishDialogBossMiddle:SetText( nextBoss )
		HundredLevelTownFinishDialogBossBtn:Show()
		HundredLevelTownFinishDialogBossBtn:SetClientUserData( 0, nextBoss )
	else
		HundredLevelTownFinishDialogBossLeft:Hide()
		HundredLevelTownFinishDialogBossRight:Hide()
		HundredLevelTownFinishDialogFrameRightTitle5:Hide()
		HundredLevelTownFinishDialogBossMiddle:SetText( "" )
		HundredLevelTownFinishDialogBossBtn:Hide()
	end
	util.ShowInCenter(this)
end

function HundredLevelTownFinishDialogBossBtn_OnClick()
	HundredLevelTownFinishDialog:Hide()
	if not isPass then
		return
	end
	GameBCT:ContinuePworld(this:GetClientUserData(0))
	floor = this:GetClientUserData(0)
end
--��ʾ����ʧ��
function SetHundredLevelTownFail(current, history)
	isFail = true
	HundredLevelTownProcessingDialog:Hide()
	if current then
		HundredLevelTownFailDialogText1Right:SetText(string.format("%d%%", current))
	end
	if history then
		HundredLevelTownFailDialogText2Right:SetText(string.format("%d%%", history))
	end
	HundredLevelTownFailDialog:Show()
end

--�ٲ������Գ�ʼ��
function HundredLevelTownIntroduce_OnLoad()
	local slider = HundredLevelTownIntroduceSelectScrollBar
	slider:SetMaxValue(90)
	slider:SetMinValue(0)
	slider:SetValueStep(1)
	local text
	for i = 1, 10 do
		text = getglobal("HundredLevelTownIntroduceSelectBtn"..i.."AreaName")
		if text then
			text:SetText(string.format("%2d", i))
		end
	end
	HundredLevelTownIntroduceLink:SetLinkTextColor(120, 230, 50)
	
	HundredLevelTownIntroduceResultAreaName:SetText(string.format("%2d", 1))
end

function HundredLevelTownIntroduce_OnShow()
	SetHundredLevelTownIntroduce(floor)
	util.ShowInCenter(this)
	HundredLevelTownIntroduceLink:SetText("#L�����װ��@@print(111)#n\n#L��������@@#n\n#Lװ��ǿ��@@#n\n#L����ǿ��@@#n\n#L��ʯǿ��@@#n", 194, 179, 145)
end

--�ٲ�������������ӵ���Ϸ����ģ��
function HundredLevelTownIntroduceLink_OnClick()
	if arg3 == "LeftButton" or arg3 == "RightButton" then
		local result, func = pcall(loadstring, arg2)
		if result then
			func()
		end
	end
end

--�������������б������
function HundredLevelTownIntroduce_SelectScrollUpBtn()
	local slider = HundredLevelTownIntroduceSelectScrollBar
	if not slider:IsShown() then
		return
	end
	local nValue = slider:GetValue() - slider:GetValueStep()
	if nValue >= slider:GetMinValue() then
		slider:SetValue(nValue)
	end
end
function HundredLevelTownIntroduce_SelectScrollDownBtn()
	local slider	= HundredLevelTownIntroduceSelectScrollBar
	if not slider:IsShown() then
		return
	end
	local nValue	= slider:GetValue() + slider:GetValueStep()
	if nValue <= slider:GetMaxValue() then
		slider:SetValue(nValue)
	end
end
function HundredLevelTownIntroduce_SelectScrollBarValueChange()
	local slider	 = HundredLevelTownIntroduceSelectScrollBar
	local nValue	 = slider:GetValue()
	local text
	for i = 1, 10 do
		text = getglobal("HundredLevelTownIntroduceSelectBtn"..i.."AreaName")
		if text then
			text:SetText(string.format("%2d", i + nValue))
		end
	end
end

--���ù���
function SetHundredLevelTownIntroduce(i)
	HundredLevelTownIntroduceResultAreaName:SetText(string.format("%2d", i))
	HundredLevelTownIntroduceInfo:SetText(GameBCT:getDesciption(i))
	HundredLevelTownIntroduceExp:SetText(GameBCT:getAwardExp(i))
	--[[
	if GameBCT:hasBox(i) then--��ʾ�������
		SetHundredLevelTownItemIcon(HundredLevelTownIntroduceItem, -i)
	else--��ʾ�ձ���
		SetHundredLevelTownItemIcon(HundredLevelTownIntroduceItem)
	end
	--]]
	local btn = getglobal( "HundredLevelTownIntroduceItem" )
	local icon = getglobal( btn:GetName().."IconTexture")
	local numFont = getglobal( btn:GetName().."CountFont" )
	local awardItemInfo = GameBCT:GetAwardItem( i, 0 )
	if awardItemInfo.itemID == 0 or awardItemInfo.itemNum == 0 then
		icon:SetTexture( "uires\\ui\\mask\\TouMingTongDao.tga" )
		numFont:SetText( "" )
		btn:SetClientUserData( 0, 0 )
	else
		local ItemDef = getItemDef( awardItemInfo.itemID )
		local IconPath = GetItemEquipIconPath()..ItemDef.IconID..".tga"
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH
		end
		icon:SetTexture( IconPath )
		numFont:SetText( awardItemInfo.itemNum )
		btn:SetClientUserData( 0, awardItemInfo.itemID )
	end
	GameBCT:updateHistory(i)
end

function HundredLevelTownIntroduceItem_OnEnter()
	if this:GetClientUserData(0) == 0 then return end
	local ItemDef = getItemDef( this:GetClientUserData(0) )
	if ItemDef ~= nil then
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, ItemDef )
	end
end

function HundredLevelTownWaradItemIconTemplate_OnEnter()
	if this:GetClientUserData(0) == 0 then return end
	local ItemDef = getItemDef( this:GetClientUserData(0) )
	if ItemDef ~= nil then
		ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, ItemDef )
	end
end

function UpdateHundredLevelTownIntroduceTopInfo(layer)
	if not HundredLevelTownIntroduce:IsShown() then
		return
	end
	local i = tonumber(HundredLevelTownIntroduceResultAreaName:GetText())
	if i ~= layer then
		return
	end
	
	local history = GameBCT:getHistory(layer)
	if history then
		if history.TopName == "" then
			HundredLevelTownIntroduceWho:SetText("��")
		else
			HundredLevelTownIntroduceWho:SetText(history.TopName)
		end
		if history.SelfWhen == 0 then
			HundredLevelTownIntroduceWhen:SetText("��")
		else
			HundredLevelTownIntroduceWhen:SetText(os.date("%Y.%m.%d", history.SelfWhen))
		end
		if history.TopTime == 0 and history.TopTimeMs == 0 then
			HundredLevelTownIntroduceTheFirst:SetText("��", 194, 179, 145)
		else
			HundredLevelTownIntroduceTheFirst:SetText(string.format("#c78e632%02d#n��#c78e632%02d.%02d#n��", math.floor(history.TopTime / 60), history.TopTime % 60, history.TopTimeMs), 194, 179, 145)
		end
		if history.SelfTime == 0 and history.SelfTimeMs == 0 then
			HundredLevelTownIntroduceMine:SetText("��", 194, 179, 145)
		else
			HundredLevelTownIntroduceMine:SetText(string.format("#c78e632%02d#n��#c78e632%02d.%02d#n��", math.floor(history.SelfTime / 60), history.SelfTime % 60, history.SelfTimeMs), 194, 179, 145)
		end
	end
end

--���ø�����ͳ����Ϣ
function SetHundredLevelTownProcessingDialog(i)
	HundredLevelTownProcessingDialogInfo:SetText(GameBCT:getDesciption(i))
	HundredLevelTownProcessingDialogExp:SetText(GameBCT:getAwardExp(i))
	local history = GameBCT:getHistory(i)
	if history then
		if history.TopName == "" then
			HundredLevelTownProcessingDialogWho:SetText("��")
		else
			HundredLevelTownProcessingDialogWho:SetText(history.TopName)
		end
		if history.SelfWhen == 0 then
			HundredLevelTownProcessingDialogWhen:SetText("��")
		else
			HundredLevelTownProcessingDialogWhen:SetText(os.date("%Y.%m.%d", history.SelfWhen))
		end
		if history.TopTime == 0 and history.TopTimeMs == 0 then
			HundredLevelTownProcessingDialogTheFirst:SetText("��", 194, 179, 145)
		else
			HundredLevelTownProcessingDialogTheFirst:SetText(string.format("#c78e632%02d#n��#c78e632%02d.%02d#n��", math.floor(history.TopTime / 60), history.TopTime % 60, history.TopTimeMs), 194, 179, 145)
		end
		if history.SelfTime == 0 and history.SelfTimeMs == 0 then
			HundredLevelTownProcessingDialogMine:SetText("��", 194, 179, 145)
		else
			HundredLevelTownProcessingDialogMine:SetText(string.format("#c78e632%02d#n��#c78e632%02d.%02d#n��", math.floor(history.SelfTime / 60), history.SelfTime % 60, history.SelfTimeMs), 194, 179, 145)
		end
	end
	if GameBCT:hasBox(i) then--��ʾ�������
		SetHundredLevelTownItemIcon(HundredLevelTownProcessingDialogItem, -i)
	else--��ʾ�ձ���
		SetHundredLevelTownItemIcon(HundredLevelTownProcessingDialogItem)
	end
end

--����ٲ���
function EnterHundredLevelTown()
	HundredLevelTownMainFrame:Hide()
	local data = PworldLimitInfo:getPworldData(pworldID)
	local vipNum = VipManager:isOwnVipFunc( STAR_RESULT_DADI_TOLUA ) and VipManager:getBCTVipNum() or 0;
	if data.freeTimeTotal == 0 or data.freeTimeUse < data.freeTimeTotal + vipNum then--��Ѵ���
		return HundredLevelTownConfirmUseFreeDialog:Show()
	end
	if data.itemTimeTotal == 0 or data.itemTimeUse < data.itemTimeTotal then--��Ʒʹ�ô���
		return HundredLevelTownPayDialog:Show()
	end
	return HundredLevelTownNotTimeDialog:Show()
end

--��Ϊ��������ͨ��ʧ�����ظ������
function HundredLevelTownFailDialog_OnUpdate()
	if ReviveFrame:IsShown() then
		ReviveFrame:Hide()
	end
end
function HundredLevelTownFailDialog_OnShow()
	HundredLevelTownLeaveDialog:Hide()
	HundredLevelTownProcessingDialog:Hide()
	HundredLevelTownFinishDialog:Hide()
	util.ShowInCenter(this)
end
function HundredLevelTownFailDialog_OnClose()
	if not GameWizard:isDo(GAMEWIZARD_FIRST_BLINK_BCT_FAIL_BTN) then
		GameWizard:doStep(GAMEWIZARD_FIRST_BLINK_BCT_FAIL_BTN)
		ShowMinimapOpenPworldXinShouBtn(HundredLevelTownOpenBtn, "���δ���ʧ���ˣ�����鿴����")
	end
end

--ʧ���ظ���ս
function HundredLevelTownFailDialog_OnRepeat()
	util.closeParent(this)
	GameBCT:RepeatPworld(floor)
end

--������һ����ս
function HundredLevelTownFinishDialogContinueBtn_OnClick()
	util.closeParent(this)
	if not isPass then
		return
	end
	GameBCT:ContinuePworld(floor)
end

function HundredLevelTownFinishDialog_OnClose()
	if not GameWizard:isDo(GAMEWIZARD_FIRST_BLINK_BCT_FINISH_BTN) then
		GameWizard:doStep(GAMEWIZARD_FIRST_BLINK_BCT_FINISH_BTN)
		ShowMinimapOpenPworldXinShouBtn(HundredLevelTownOpenBtn, "������ˣ�����鿴����")
	end
end

--��ѽ���ٲ���
function HundredLevelTownConfirmUseFreeDialogEnter_OnClick()
	util.closeParent(this)
	GameBCT:EnterPworld(floor, false)
end

--�õ��߽���ٲ���
function HundredLevelTownPayDialogEnter_OnClick()
	util.closeParent(this)
	local data = PworldLimitInfo:getPworldData(pworldID)
	if ActorMgr:getMainPlayer():getContainer():getItemCount(data.itemId) > 0 then
		return GameBCT:EnterPworld(floor, true)
	else
		return HundredLevelTownNotEnoughDialog:Show()
	end
end

--���߲���
function HundredLevelTownNotEnoughDialogBuy_OnClick()
	local data = PworldLimitInfo:getPworldData(pworldID)
	util.closeParent(this)
	if not MarketManagerFrame:IsShown() then
		MarketManagerFrame:Show();
	end
	--MarketManager:selectMarketItem(data.itemId)
end

--�ٲ��������ƶ�
function HundredLevelTownMoveToFloor(target)
	if target > 100 then
		target = 100
	elseif target < 1 then
		target = 1
	end
	target = target - 1
	target = target - target % 10
	scrollFrame.target = target
	if target == 0 then
		HundredLevelTownMainFrameScrollFrameUp:Hide()
		HundredLevelTownMainFrameScrollFrameDown:Show()
		HundredLevelTownFinishFrameScrollFrameUp:Hide()
		HundredLevelTownFinishFrameScrollFrameDown:Show()
	elseif target == 90 then
		HundredLevelTownMainFrameScrollFrameUp:Show()
		HundredLevelTownMainFrameScrollFrameDown:Hide()
		HundredLevelTownFinishFrameScrollFrameUp:Show()
		HundredLevelTownFinishFrameScrollFrameDown:Hide()
	else
		HundredLevelTownMainFrameScrollFrameUp:Show()
		HundredLevelTownMainFrameScrollFrameDown:Show()
		HundredLevelTownFinishFrameScrollFrameUp:Show()
		HundredLevelTownFinishFrameScrollFrameDown:Show()
	end
end

function HundredLevelTownScrollFrame_OnLoad()
	this:setUpdateTime(0)
	local name = this:GetName()
	for i = 1, 100 do
		local font = getglobal(string.format("%sFloor%dTitle", name, i))
		if font then
			font:SetText(string.format("��%d��", i))
		end
	end
end

function HundredLevelTownScrollFrame_OnUpdate()
	local current = this:GetVerticalOffset()
	local target = scrollFrame.target
	if not target then
		return
	end
	target = target / 10 * scrollFrame.unit
	local offset = math.abs(target - current)
	if offset == 0 then
		scrollFrame.lastTick = nil
		scrollFrame.lastStep = nil
		scrollFrame.target = nil
		scrollFrame.speed = nil
	else
		local lastTick = scrollFrame.lastTick or os.clock()
		local now = os.clock()
		local speed = math.max(scrollFrame.speed or 0, offset * 2)
		scrollFrame.speed = speed
		local step = (scrollFrame.lastStep or 0) + math.abs(now - lastTick) * speed
		scrollFrame.lastStep = step % 1
		scrollFrame.lastTick = now
		step = step - scrollFrame.lastStep
		if step == 0 then
			return
		elseif step > offset then
			step = offset
		end
		if current < target then
			this:IncreaseVerticalScroll(step)
		else
			this:IncreaseVerticalScroll(-step)
		end
		if this:GetVerticalOffset() == current then
			scrollFrame.lastTick = nil
			scrollFrame.lastStep = nil
			scrollFrame.speed = nil
		end
	end
end

function ResetHundredLevelTownScrollFrame(which, target)
	target = target / 10 * scrollFrame.unit
	which:IncreaseVerticalScroll(target - which:GetVerticalOffset())
	local name = which:GetName()
	local up = getglobal(name.."Up")
	local down = getglobal(name.."Down")
	if target == 0 then
		up:Hide()
		down:Show()
	elseif target == 90 then
		up:Show()
		down:Hide()
	else
		up:Show()
		down:Show()
	end
end

function HundredLevelTownScrollFrame_OnUp(which)
	local target = scrollFrame.target or math.floor(which:GetVerticalOffset() / scrollFrame.unit + 0.5) * 10
	target = target + 1
	HundredLevelTownMoveToFloor(target - 10)
end

function HundredLevelTownScrollFrame_OnDown(which)
	local target = scrollFrame.target or math.floor(which:GetVerticalOffset() / scrollFrame.unit + 0.5) * 10
	target = target + 1
	HundredLevelTownMoveToFloor(target + 10)
end

function HundredLevelTownScrollFrameHistory_OnEnter()
	SetGameTooltips(this:GetName(), "��ʷ��߲�")
end

function HundredLevelTownScrollFrameHistory_OnLeave()
	GameTooltipRichText:SetText("",255,255,255)
	GameTooltip:Hide()
end

function HundredLevelTownScrollFrameCurrent_OnEnter()
	SetGameTooltips(this:GetName(), "��ǰ��")
end

function HundredLevelTownScrollFrameCurrent_OnLeave()
	GameTooltipRichText:SetText("",255,255,255)
	GameTooltip:Hide()
end

function HundredLevelTownOpenBtn_OnShow()
	if not GameWizard:isDo(GAMEWIZARD_FIRST_BLINK_BCT_PROCESS_BTN) and not isPass then
		GameWizard:doStep(GAMEWIZARD_FIRST_BLINK_BCT_PROCESS_BTN)
		ShowMinimapOpenPworldXinShouBtn(HundredLevelTownOpenBtn, "����ɲ鿴������Ϣ")
	end
	if not GameWizard:isDo(GAMEWIZARD_FIRST_BLINK_BCT_FINISH_BTN) and isPass then
		GameWizard:doStep(GAMEWIZARD_FIRST_BLINK_BCT_FINISH_BTN)
		ShowMinimapOpenPworldXinShouBtn(HundredLevelTownOpenBtn, "������ˣ�����鿴����")
	end
end