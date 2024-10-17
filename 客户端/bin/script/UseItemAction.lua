
--[道具ID] = {动作ID,使用者身上特效名,目标身上特效},
local tUserItemActionTable = { [5300416] = {10471,"","9566_16"},	--巴掌
							   [5300415] = {10470,"9565_16",""},	--鞭子
							   [5300414] = {10136,"",""},			--敲晕，拖走
								};

function getActionIDbyItemID( itemID )
	if tUserItemActionTable[itemID] == nil then
		return 0,"","";
	else
		return tUserItemActionTable[itemID][1],tUserItemActionTable[itemID][2],tUserItemActionTable[itemID][3];
	end	
end

