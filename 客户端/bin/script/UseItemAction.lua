
--[����ID] = {����ID,ʹ����������Ч��,Ŀ��������Ч},
local tUserItemActionTable = { [5300416] = {10471,"","9566_16"},	--����
							   [5300415] = {10470,"9565_16",""},	--����
							   [5300414] = {10136,"",""},			--���Σ�����
								};

function getActionIDbyItemID( itemID )
	if tUserItemActionTable[itemID] == nil then
		return 0,"","";
	else
		return tUserItemActionTable[itemID][1],tUserItemActionTable[itemID][2],tUserItemActionTable[itemID][3];
	end	
end

