;4号BOSS

proc 2
	strvar hp
	GET_MON_INFO 0 0 0 hp 0 0 0 0
	if hp = 0
		get_pworld_var 4 32 1 10
		DEL_ID_MON @10 1
		
;		add_sys_msg_start
;		add_sys_msg 古武大喊：这还是不科学啊！！！也嗝屁了~~~~~~
;		set_sys_msg_mode 4 0 0
;		map_msg_infoOPEN_PWORLD_AWARD_UI
PWORLD_BOX_START 0
		OPEN_PWORLD_AWARD_UI
		
		NEW_NPC 1 201065 26800 7900 110 60fubencs.mac
	endif
endproc 

proc 9
	DESTROY_MON 1
	strvar momid
	NEW_MON_BY_DIR 205119 1 26600 8300 0 0 0 30 1 momid
	TARGET_ADD_STATUS 3 @momid 1095 1 25920000 0 

endproc 

