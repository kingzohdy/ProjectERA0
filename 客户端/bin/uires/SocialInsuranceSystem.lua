
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
	local szInsuranceDesc = [[#cb9b9b91.ֻҪ���߳���һ�죬���߽����#cffe955���ع������#cb9b9b9������
2.��©�μӵĻ���´�ͨ�غ󽫻�þ��鲹����
3.������߼����10���������#cffe955���������ϡ�#cb9b9b9������]];
	local szWelfareDesc = [[#cb9b9b920��   ����   #cffe955������ǹ�֡�
#cb9b9b930��   ����   #cffe955���������
#cb9b9b940��   ����   #cffe955��������װ��
#cb9b9b9���ࡰ�ȼ�������������Ϸ�У�]];
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