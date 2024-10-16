;精灵商人 316002


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b我这里有各种精灵的魂哦，都是上等的品种！
	
;	OPTION 110 #(买卖)打开精灵商店
	
	task_add_option 3110 1 100
	task_add_option 3111 1 101
	task_add_option 3112 1 102
	
	
	task_add_option 3113 1 103
	task_add_option 3114 1 104
;	task_add_option 1082 1 105
	
;	strvar Ret
;	task_stat 1082 1 Ret
;	if Ret = 2
;		
;		option 300 时间城太大了，能否送我一程？
;	endif
	
	TASK_STAT 3111 1 20
	HAS_FAIRY 1 30 1
	ROLE_ITEM_NUM 5620000 31 1
	if 20 = 3 and 30 = 0 and 31 = 0
		option 170 #购买精灵
	endif

	strvar Ret
	TASK_STAT 3304 1 Ret
	if Ret = 2
		option 500 曲别针换别墅
	endif

	
	talk
endproc 
proc 170
	OPEN_NPC_SHOP 
endproc

;proc 511
;	new_mon 202005 1 7779 8542 0 0 0 0 0
;
;	add_sys_msg_start
;	add_sys_msg 恶魔之影已经出现，记住他！杀了他！
;	set_sys_msg_mode 1 0 0
;	msg_info
;endproc 


proc 500
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b嗨，$_player$，看在我送你精灵的份上，就和我交换吧，我的梦想是收藏世界上所有的精灵序……
	
	option 501 #交换

	talk
endproc
proc 501
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300765 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300765 1 1
			
			rand Rad 100
			if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300766 1 0 0
			endif
			if @Rad >= 80 and @Rad < 95
				ADD_ITEM_NUM_INPAK 5300767 1 0 0
			endif
			if @Rad >= 95 and @Rad < 99
				ADD_ITEM_NUM_INPAK 5300768 1 0 0
			endif
			if @Rad = 99
				ADD_ITEM_NUM_INPAK 5300769 1 0 0
			endif
		else
			add_sys_msg_start
			add_sys_msg 背包空间不足！
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg 你身上没有可以交换的物品！
		set_sys_msg_mode 1 0 0
		msg_info
	endif
endproc 


proc 100
	task_do_option 3110 1
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
;proc 105
;	task_do_option 1082 1
;endproc
PROC 110
	OPEN_NPC_SHOP
ENDPROC
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

proc 300
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b当然可以，您可是个大人物呢。
	add_talk #b#b不过，我是直接将您送到目的地，中途不会停的哟！
	add_talk #b#b您确定要乘坐直达班车吗？
		option 301 #请直接将我送到目的地吧！
		option 302 #不用啦，我自己过去，还能逛逛时间城！
	talk
endproc
proc 301
	
;	NEW_OWNER_TMP_ITEM 1460072 29400 26600 0 0 0 312 1095 1 3600 1
	AUTO_MOVE 0 1000 26400 38700 1460072 0 3081 1 3600
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


