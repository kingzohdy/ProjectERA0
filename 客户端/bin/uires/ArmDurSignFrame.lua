
local t_ArmDurSignIconList = {};

function ArmDurSignFrame_OnLoad()
	this:RegisterEvent("GE_CONTAINER_ALL_UPDATE");
	this:RegisterEvent("GE_WEAR_ARM_UPDATE");
end

function ArmDurSignFrame_OnEvent()
	if arg1 == "GE_CONTAINER_ALL_UPDATE" or arg1 == "GE_WEAR_ARM_UPDATE" then
		UpdateArmDurSign();
	end
end

function ReSetArmDurSign()
	local t_hideIcons = {	"ArmDurSignFrameRight_Tex", "ArmDurSignFrameBody_Tex", "ArmDurSignFrameLeft_Tex",
							"ArmDurSignFrameHead_Tex",	"ArmDurSignFrameShoulder_Tex",	"ArmDurSignFrameClothing_Tex",
							"ArmDurSignFrameFoot_Tex", "ArmDurSignFrameMask_Tex"
						};
	util.HideIcons( t_hideIcons );
end

local nNormal  = 0;
local nBreak   = 1;
local nUnUse   = 2;
local nShow = 1;
local nHide = 2;

function UpdateArmDurSign()
	ReSetArmDurSign();
	local MainPlayer = ActorMgr:getMainPlayer();
	local equip = MainPlayer:getEquip();
	local bShow = false;
	for  i = 1, EQUIP_POS_END do
		local nState = nNormal;
		local item = equip:getItem(i);
		if item:getItemType() ~= ITEM_SLOTTYPE_NONE and item:getItemId() ~= 0 then
			local itemDef = item:getItemDef();

			local nGridIdx	= item:getGridIdx();				--装备身上的格子数
			local nCurDur	= item:getItemDur();				--当前耐久度
			local nMaxDur	= item:getItemMaxDur();				--最大耐久度

			if nCurDur == nMaxDur then
				nState = nNormal;
			elseif nCurDur ==  0 then
				nState = nUnUse;
			elseif nCurDur <=  (nMaxDur*(9/100)) then
				nState = nBreak;
			end

			if nState == nUnUse or nState == nBreak or item:isArmFengYin() then
				bShow = true;
			end
			
			SetPlayerArmDurDialog( nState );

			if nState ~= nNormal then
				CheckArmDurSignFrameBreakXinShou()
			end
			if item:isArmFengYin() then
				CheckArmDurSignFrameFengyinXinShou()
			end

			if nGridIdx == EQUIP_POS_RIGHTHAND then		--右手
				if item:isArmFengYin() then
					-- 封印
					ArmDurSignFrameRight_Tex:SetTexUV(1007, 203, 17, 64);
					ArmDurSignFrameRight_Tex:Show();
					ArmDurSignFrameBody_Tex:Show();
				elseif nState == nUnUse then
					-- 装备耐久标识:武器红
					ArmDurSignFrameRight_Tex:SetTexUV(1001, 565, 17, 64);
					ArmDurSignFrameRight_Tex:Show();
					ArmDurSignFrameBody_Tex:Show();					
				elseif nState == nNormal then
					ArmDurSignFrameRight_Tex:Hide();
				elseif nState == nBreak then
					-- 装备耐久标识:武器黄
					ArmDurSignFrameRight_Tex:SetTexUV(981, 565, 17, 64);
					ArmDurSignFrameRight_Tex:Show();
					ArmDurSignFrameBody_Tex:Show();
				end
			elseif nGridIdx == EQUIP_POS_LEFT then		--左手
				if item:isArmFengYin() then
					-- 封印
					ArmDurSignFrameLeft_Tex:SetTexUV(777, 237, 26, 37);
					ArmDurSignFrameLeft_Tex:Show();
					ArmDurSignFrameBody_Tex:Show();
				elseif nState == nUnUse then
					-- 装备耐久标识:盾红
					ArmDurSignFrameLeft_Tex:SetTexUV(906, 565, 26, 37);
					ArmDurSignFrameLeft_Tex:Show();
					ArmDurSignFrameBody_Tex:Show();
				elseif nState == nNormal then
					ArmDurSignFrameLeft_Tex:Hide();				
				elseif nState == nBreak then
					-- 装备耐久标识:盾黄
					ArmDurSignFrameLeft_Tex:SetTexUV(879, 565, 26, 37);
					ArmDurSignFrameLeft_Tex:Show();
					ArmDurSignFrameBody_Tex:Show();
				end
			elseif nGridIdx == EQUIP_POS_SHOULDER then	--肩
				if item:isArmFengYin() then
					-- 封印
					ArmDurSignFrameShoulder_Tex:SetTexUV(336, 34, 30, 13);
					ArmDurSignFrameShoulder_Tex:Show();
					ArmDurSignFrameBody_Tex:Show();
				elseif nState == nUnUse then
					-- 装备耐久标识:肩甲红
					ArmDurSignFrameShoulder_Tex:SetTexUV(125, 653, 30, 13);
					ArmDurSignFrameShoulder_Tex:Show();
					ArmDurSignFrameBody_Tex:Show();
				elseif nState == nNormal then
					ArmDurSignFrameShoulder_Tex:Hide();				
				elseif nState == nBreak then
					-- 装备耐久标识:肩甲黄
					ArmDurSignFrameShoulder_Tex:SetTexUV(222, 225, 30, 13);
					ArmDurSignFrameShoulder_Tex:Show();
					ArmDurSignFrameBody_Tex:Show();
				end
			elseif nGridIdx == EQUIP_POS_HEAD then		--头
				if item:isArmFengYin() then
					-- 封印
					ArmDurSignFrameHead_Tex:SetTexUV(349, 48, 17, 19);
					ArmDurSignFrameHead_Tex:Show();
					ArmDurSignFrameBody_Tex:Show();
				elseif nState == nUnUse then
					-- 装备耐久标识:头红
					ArmDurSignFrameHead_Tex:SetTexUV(189, 182, 17, 19);
					ArmDurSignFrameHead_Tex:Show();
					ArmDurSignFrameBody_Tex:Show();
				elseif nState == nNormal then
					ArmDurSignFrameHead_Tex:Hide();				
				elseif nState == nBreak then
					-- 装备耐久标识:头黄
					ArmDurSignFrameHead_Tex:SetTexUV(171, 182, 17, 19);
					ArmDurSignFrameHead_Tex:Show();
					ArmDurSignFrameBody_Tex:Show();
				end
			elseif nGridIdx == EQUIP_POS_BODY then		--衣服
				if item:isArmFengYin() then
					-- 封印
					ArmDurSignFrameClothing_Tex:SetTexUV(738, 237, 38, 47);
					ArmDurSignFrameClothing_Tex:Show();
					ArmDurSignFrameBody_Tex:Show();
				elseif nState == nUnUse then
					-- 装备耐久标识:衣服红
					ArmDurSignFrameClothing_Tex:SetTexUV(839, 466, 38, 47);
					ArmDurSignFrameClothing_Tex:Show();
					ArmDurSignFrameBody_Tex:Show();
				elseif nState == nNormal then
					ArmDurSignFrameClothing_Tex:Hide();				
				elseif nState == nBreak then
					-- 装备耐久标识:衣服黄
					ArmDurSignFrameClothing_Tex:SetTexUV(839, 418, 38, 47);
					ArmDurSignFrameClothing_Tex:Show();
					ArmDurSignFrameBody_Tex:Show();
				end
			elseif nGridIdx == EQUIP_POS_FACE then		--面饰
				if item:isArmFengYin() then
					-- 封印
					ArmDurSignFrameMask_Tex:SetTexUV(339, 48, 9, 13);
					ArmDurSignFrameMask_Tex:Show();
					ArmDurSignFrameBody_Tex:Show();
				elseif nState == nUnUse then
					-- 装备耐久标识:面饰红
					ArmDurSignFrameMask_Tex:SetTexUV(739, 426, 9, 13);
					ArmDurSignFrameMask_Tex:Show();
					ArmDurSignFrameBody_Tex:Show();
				elseif nState == nNormal then
					ArmDurSignFrameMask_Tex:Hide();
				elseif nState == nBreak then
					-- 装备耐久标识:面饰黄
					ArmDurSignFrameMask_Tex:SetTexUV(739, 440, 9, 13);
					ArmDurSignFrameMask_Tex:Show();
					ArmDurSignFrameBody_Tex:Show();
				end
			elseif nGridIdx == EQUIP_POS_SHOE then
				if item:isArmFengYin() then
					-- 封印
					ArmDurSignFrameFoot_Tex:SetTexUV(804, 237, 23, 37);
					ArmDurSignFrameFoot_Tex:Show();
					ArmDurSignFrameBody_Tex:Show();
				elseif nState == nUnUse then
					-- 装备耐久标识:腿红
					ArmDurSignFrameFoot_Tex:SetTexUV(957, 565, 23, 37);
					ArmDurSignFrameFoot_Tex:Show();
					ArmDurSignFrameBody_Tex:Show();
				elseif nState == nNormal then
					ArmDurSignFrameMask_Tex:Hide();
				elseif nState == nBreak then
					-- 装备耐久标识:腿黄
					ArmDurSignFrameFoot_Tex:SetTexUV(933, 565, 23, 37);
					ArmDurSignFrameFoot_Tex:Show();
					ArmDurSignFrameBody_Tex:Show();
				end
			end
		end
	end

	if bShow and not ArmDurSignFrame:IsShown() then
		ArmDurSignFrame:Show();
	elseif not bShow and ArmDurSignFrame:IsShown() then
		ArmDurSignFrame:Hide();
	end
end

function ShowArmDurSignXinShouBtn( data )
	local xinShouBtn	= getglobal( "ArmDurSignFrameXinShouBtn" );
	local rich			= getglobal( xinShouBtn:GetName().."Rich" );
	local nSelfDefineExtendWidth = 10;
	rich:SetClientString( data["text"] );
	ReSizeTooltip( rich:GetName(), xinShouBtn:GetName(), nSelfDefineExtendWidth );
	xinShouBtn:Show();
end

function CheckArmDurSignFrameBreakXinShou()
	-- 若第一次提示修理装备
	if not GameWizard:isDo(GAMEWIZARD_FIRST_EQUIP_NOT_ENOUGH_DUR) then
		GameWizard:doStep(GAMEWIZARD_FIRST_EQUIP_NOT_ENOUGH_DUR);
		ShowArmDurSignXinShouBtn( { text = "装备耐久度不足，可去商店修理",  } );
	end
end

function CheckArmDurSignFrameFengyinXinShou()
	if not GameWizard:isDo(GAMEWIZARD_FIRST_EQUIP_FENGYIN) then
		GameWizard:doStep(GAMEWIZARD_FIRST_EQUIP_FENGYIN);
		ShowArmDurSignXinShouBtn( { text = "装备被封印，部分属性失效，可找装备解封大师解封",  } );
	end
end

function ArmDurSignFrame_OnShow()
end

function ArmDurSignFrame_OnHide()
	local xinShouBtn = getglobal( "ArmDurSignFrameXinShouBtn" );
	xinShouBtn:Hide();
end

MAX_SHOW_TIME = 5;
local t_armTipsInfo = { startTime = 0 };

function ArmDurSignFrameXinShouBtn_OnShow()
	t_armTipsInfo = { startTime = os.clock() };
end

function ArmDurSignFrameXinShouBtn_OnHide()
	t_armTipsInfo = { startTime = 0 };
end

function ArmDurSignFrameXinShouBtn_OnUpdate()
	if os.clock() - t_armTipsInfo["startTime"] >= MAX_SHOW_TIME then
		this:Hide();
	end
end