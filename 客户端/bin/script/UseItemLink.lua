local t_ItemLink = 
	{
		--[��ƷID] = { link = "���ӵ�ַ" };
		[5063000] = { link = "http://www.wyyl.com" };
		[5063001] = { link = "http://www.wyyl.com" };
	};

function GetItemLink(nItemId)
	return t_ItemLink[nItemId];
end