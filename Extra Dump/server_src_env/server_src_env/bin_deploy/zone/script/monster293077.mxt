;鑻卞媷

proc 1
		
	;		task_fini 3003 0
		add_sys_msg_start
		add_sys_msg 对“可捕捉”的松鼠枪手使用任务追踪右侧的“捕兽夹”！
		set_sys_msg_mode 5 0 0
		map_msg_info
		get_pworld_var 100 8 1 40
		var 40 10
		set_pworld_var 100 8 @40 1
endproc  
proc 2
;	strvar Mid Hp
;	GET_MON_INFO 0 0 Mid Hp 0 0 0 0
;	if Hp > 0
;		EFFECT_ID 0 @Mid playMontion(%d,"10254_5_1")
;		ADD_ITEM_NUM_INPAK 5610004 1 0 0
;	endif
endproc 
;proc 4
;	strvar Mid Rethp Loc
;	get_mon_var 0 8 1 Loc
;	if Loc != 1
;		GET_MON_INFO 1 1 Mid 1 1 1 1 1 
;		GET_MON_HP_PERCENT @Mid Rethp
;		if Rethp < 50 
;			add_sys_msg_start
;			add_sys_msg 现在怪物血量较少，再用“捕兽夹”试试！
;			set_sys_msg_mode 3 0 0
;			map_msg_info
;			
;			GET_MAP_NPC_ID 317009 60 0 
;			mon_speech @60 0 1 0 现在怪物血量较少，再用“捕兽夹”试试！
;
;			var Loc 1
;			set_mon_var 0 8 @Loc 1
;		endif
;	endif
;endproc 