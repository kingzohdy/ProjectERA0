;回廊教官
; 携带任务\副本503

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b每个人都会经过这个阶段，见到一座山，就想知道山后面是什么，大部分人会因为山势险峻或害怕失望而绕行。我很想告诉他，如果想知道山那边有什么，就亲自去看看吧，也会有意外的收获。
	task_add_option 1079 1 101	
	task_add_option 1075 1 102
	task_add_option 1080 1 103
	task_add_option 1071 1 104
	
	task_add_option 1074 1 105
	task_add_option 1083 1 106
	task_add_option 3004 1 107
	
	 
	TASK_STAT 1075 1 3
	if 3 = 2
		OPTION 110 #进入天空回廊
;		OPTION 111 #重置天空回廊
	ENDIF
	
	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300763 1 num1
	if num1 > 0
		TASK_STAT 3304 1 Ret
		if Ret = 2
			option 500 曲别针换别墅
		endif
	endif
	
	
	task_stat 3004 1 20
	if 20 = 2
		option 120 我想要宠物，请送我过去吧！
	endif
	

	
	
	talk
endproc
proc 101
    task_do_option 1079 1
endproc
proc 102
    task_do_option 1075 1
endproc
proc 103
    task_do_option 1080 1
endproc
proc 104
    task_do_option 1071 1
endproc
proc 105
    task_do_option 1074 1
endproc
proc 106
    task_do_option 1083 1
endproc
proc 107
    task_do_option 3004 1
endproc

proc 110
	enter_pworld 503 0 1
endproc

;proc 111
;	RESET_PWORLD 503
;endproc
proc 120
	AUTO_MOVE 20 1000 24200 13200 1460049 0 3081 1 3600
endproc
proc 500
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b啊，我正好缺少你手中的这一章节，跟我交换吧，相信我，只有我对这本魔法帝国年表感兴趣。

		option 501 #交换

	talk
endproc

proc 501
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300763 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300763 1 1
			
		;	rand Rad 100
		;	if @Rad < 80
		;		ADD_ITEM_NUM_INPAK 5300764 1 0 0
		;	endif
		;	if @Rad >= 80 and @Rad < 95
		;		ADD_ITEM_NUM_INPAK 5300765 1 0 0
		;	endif
		;	if @Rad >= 95 and @Rad < 99
				ADD_ITEM_NUM_INPAK 5300766 1 0 0
		;	endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300783 1 0 0
		;	endif
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

proc 2
	strvar Retask
	get_trig_task 1 Retask
	if @Retask = 1086
		task_fini 1086 1
	endif
	
	if @Retask = 1075
;		RESET_PWORLD 503
		dialog 1
		add_talk $_npc$:
		add_talk #b#b每个人都会经过这个阶段，见到一座山，就想知道山后面是什么，大部分人会因为山势险峻或害怕失望而绕行。我很想告诉他，如果想知道山那边有什么，就亲自去看看吧，也会有意外的收获。
		OPTION 110 #进入天空回廊
		talk
	endif

endproc

