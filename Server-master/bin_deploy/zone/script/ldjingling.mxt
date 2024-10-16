;精灵商人 316002


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b我这里有各种精灵的魂哦，都是上等的品种！
	
	task_add_option 1870 1 100
	task_add_option 3111 1 101
	task_add_option 3112 1 102
	
	
	task_add_option 3113 1 103
	task_add_option 3114 1 104
	task_add_option 3110 1 105

	
	TASK_STAT 3111 1 20
	HAS_FAIRY 1 30 1
	ROLE_ITEM_NUM 5620000 31 1
	if 20 = 3 and 30 = 0 and 31 = 0
		option 170 #购买精灵
	endif


	
	talk
endproc 
proc 170
	OPEN_NPC_SHOP 
endproc


proc 100
	task_do_option 1870 1
endproc
proc 101
	task_do_option 3111 1
endproc
proc 102
	task_do_option 3112 1
endproc
proc 103
	task_do_option 3113 1
endproc
proc 104
	task_do_option 3114 1
endproc
proc 105
	task_do_option 3110 1
endproc

proc 302
	return
endproc 
proc 2
	get_trig_task 1 2 
;	if 2 = 1082
;		DIALOG 1
;		add_talk $_npc$:
;		add_talk #b#b我这里有各种精灵的魂哦，都是上等的品种！
;			task_fini 1082 1
;			option 300 时间城太大了，能否送我一程？
;		talk
;	endif
	if 2 = 3111
		
		strvar Num_1
		get_pworld_var 6 8 1 Num_1
		var Num_1 0
		set_pworld_var 6 8 @Num_1 1
	endif
	if 2 = 3114
		strvar Num_1
		get_pworld_var 6 8 1 Num_1
		var Num_1 0
		set_pworld_var 6 8 @Num_1 1
	
	endif
;		DIALOG 1
;		add_talk $_npc$:
;		add_talk #b#b我这里有各种精灵的魂哦，都是上等的品种！
;
;		strvar Map
;		GET_PLAYER_MAPINFO 0 Map 0 0 0
;		if @Map = 50400
;			get_task_var 3114 0 8 1 So
;			var So 1
;			set_task_var 3114 0 8 @So 1
;			
;			strvar Ret3
;			task_stat 3114 1 Ret3
;			if Ret3 = 2 and So = 1
;				option 511 #召唤恶魔之影
;			endif
;		endif
;		if @Map = 1000	
;		
;			get_task_var 3114 0 8 1 So
;			strvar Ret4
;			task_stat 3114 1 Ret4
;			if Ret4 = 2 and So = 0
;			
;			option 510 #进入雷霆崖
;			endif
;		endif
;	
;		talk
;	endif
endproc


