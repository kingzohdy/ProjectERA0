PROC 1
	NEW_MAP_LIFE_NPC 1 1000 304118 27400 30400 0 0 npc304118.mac
ENDPROC

PROC 2
	GET_MAP_INST 1000 10
    del_npc 304118 0
	add_sys_msg_start
	add_sys_msg 五一劳动节活动已经圆满结束！
	set_sys_msg_mode 6 0 0
	svr_msg_info
ENDPROC