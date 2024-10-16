
proc 1
	NEW_MAP_LIFE_NPC1  0 1000 302180 26100 38900 0 0 0 npc302180.mac
	NEW_MAP_LIFE_NPC1  0 2100 302180 8700 8000 100 0 0 npc302180.mac
	NEW_MAP_LIFE_NPC1  0 2300 302180 18900 11000 45 0 0 npc302180.mac
	NEW_MAP_LIFE_NPC1  0 2202 302180 32600 20800 100 0 0 npc302180.mac
	NEW_MAP_LIFE_NPC1  0 2400 302180 3000 6900 135 0 0 npc302180.mac
	NEW_MAP_LIFE_NPC1  0 2500 302180 31800 22000 90 0 0 npc302180.mac
	
	add_sys_msg_start
	add_sys_msg 不给糖果就捣乱，找布鲁鲁去要糖果吧，每半小时只能免费领一次哦。
	set_sys_msg_mode 1 0 0
	SVR_MSG_INFO
	
endproc

Proc 2
	del_npc_by_mapid 302180 1000 0
	del_npc_by_mapid 302180 2100 0
	del_npc_by_mapid 302180 2300 0
	del_npc_by_mapid 302180 2202 0
	del_npc_by_mapid 302180 2400 0
	del_npc_by_mapid 302180 2500 0
endproc
