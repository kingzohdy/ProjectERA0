local t_ItemLink = 
	{
		--[物品ID] = { link = "链接地址" };
		[5063000] = { link = "http://www.wyyl.com" };
		[5063001] = { link = "http://www.wyyl.com" };
	};

function GetItemLink(nItemId)
	return t_ItemLink[nItemId];
end