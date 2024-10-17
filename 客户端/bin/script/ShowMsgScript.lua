-------------------------------------------------
-- Loading界面显示时 缓存消息队列
-------------------------------------------------

-- t_TmpMsg = { { 消息类型, { 消息1, 消息2, 消息3 ... } } }
local t_TmpMsg = {}

function SetTmpMsg( nMsgType, szMsgContent )
	for i = 1, table.getn( t_TmpMsg ) do
		if nMsgType == t_TmpMsg[i][1] then
			table.insert( t_TmpMsg[i][2], szMsgContent );
			return;
		end
	end

	table.insert( t_TmpMsg,{ nMsgType, { szMsgContent } } );
end

function GetTmpMsg( nMsgType )
	local t_TmpContent = nil;

	for i = 1, table.getn( t_TmpMsg ) do
		if nMsgType == t_TmpMsg[i][1] then
			t_TmpContent = t_TmpMsg[i][2];
			table.remove( t_TmpMsg, i );
			break;
		end
	end
	return t_TmpContent;
end