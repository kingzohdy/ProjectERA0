
local numTex = {
["0"] = {x = 335, y = 472},
["1"] = {x = 404, y = 472},
["2"] = {x = 358, y = 472},
["3"] = {x = 499, y = 472},
["4"] = {x = 381, y = 472},
["5"] = {x = 427, y = 472},
["6"] = {x = 452, y = 472},
["7"] = {x = 381, y = 435},
["8"] = {x = 358, y = 435},
["9"] = {x = 476, y = 472},
};

local function ResolveNumber( num )
	for i = 7, 1, -1 do
		local tex = getglobal(this:GetName() .. "DigitDisplyer" .. "Digit" .. i);
		local integer = math.floor(num / 10);
		local fract = num - integer * 10;
		num = integer;
		fract = tostring(fract);
		tex:SetTexUV(numTex[fract].x, numTex[fract].y, tex:GetWidth(), tex:GetHeight());
	end
end

function SocialInsuranceSystem_OnShow()
	local szInsuranceDesc = [[#cb9b9b91.只要离线超过一天，上线将获得#cffe955【回归礼包】#cb9b9b9补偿。
2.遗漏参加的活动，下次通关后将获得经验补偿。
3.低于最高级玩家10级，将获得#cffe955【天天向上】#cb9b9b9补偿。]];
	local szWelfareDesc = [[#cb9b9b920级   ——   #cffe955【松鼠枪手】
#cb9b9b930级   ——   #cffe955【永久坐骑】
#cb9b9b940级   ——   #cffe955【荣誉套装】
#cb9b9b9更多“等级福利”尽在游戏中！]];
	local rich = getglobal(this:GetName().."InsuranceDesc");
	rich:SetText(szInsuranceDesc, 185, 185, 185);
	rich = getglobal(this:GetName().."WelfareDesc");
	rich:SetText(szWelfareDesc, 185, 185, 185);
	ResolveNumber(1234567);
end

function SocialInsuranceSystem_OnEvent()

end

function EndowmentBtn_OnClick()

end

function JoblessBtn_OnClick()

end

function MedicalBtn_OnClick()

end

function GrowBtn_OnClick()

end

function PrecreateBtn_OnClick()

end

function NewcomeBtn_OnClick()

end

function OfficerBtn_OnClick()

end

function LevelBtn_OnClick()

end

function EarthBtn_OnClick()

end