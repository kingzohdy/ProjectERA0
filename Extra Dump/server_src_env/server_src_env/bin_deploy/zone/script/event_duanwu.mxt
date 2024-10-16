PROC 1
	call 100
	NEW_MAP_LIFE_NPC 1 1000 302167 27300 30300 158 0 npc302167.mac
	
	NEW_MAP_LIFE_NPC_EX 1 1000 302168 27600 30800 135 0 107 npc302168.mac

ENDPROC

PROC 2
    call 100
	add_sys_msg_start
	add_sys_msg 端午节活动已经圆满结束！
	set_sys_msg_mode 6 0 0
	svr_msg_info
ENDPROC

;10秒触发一次
PROC 3	

ENDPROC

PROC 100
    GET_MAP_INST 1000 10
	del_npc 302167 0
	del_npc 302168 0
ENDPROC
