
;查理曼


proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b我永远相信只要永不放弃，我们一定能够重建落日故乡！
	
		task_add_option 1902 1 100
		task_add_option 1903 1 101
		task_add_option 1904 1 102
		task_add_option 1905 1 103
		task_add_option 1920 1 104
		task_add_option 1921 1 105
		
		task_add_option 1932 1 106
		task_add_option 1933 1 107
		task_add_option 2001 1 108
	
	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300930 1 num1
	if num1 > 0
		TASK_STAT 3305 1 Ret
		if Ret = 2
			option 700 曲别针换别墅
		endif
	endif
	talk
endproc

PROC 100
	task_do_option 1902 1
ENDPROC
PROC 101
	task_do_option 1903 1
ENDPROC
PROC 102
	task_do_option 1904 1
ENDPROC
PROC 103
	task_do_option 1905 1
ENDPROC
PROC 104
	task_do_option 1920 1
ENDPROC
PROC 105
	task_do_option 1921 1
ENDPROC
PROC 106
	task_do_option 1932 1
ENDPROC
PROC 107
	task_do_option 1933 1
ENDPROC
PROC 108
	task_do_option 2001 1
ENDPROC


proc 2
	get_trig_task 1 2
	if 2 = 1905
		task_fini 1905 1
	endif
	if 2 = 1921
		task_fini 1921 1
	endif
	if 2 = 2001
		task_fini 2001 1
	endif
endproc



proc 700
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b真难得现在还有人对古老的精灵王国感兴趣，竟将藏书保存的这样完好无缺。
	add_talk #b#b与我交换吧，这样有助于你实现梦想。
		option 701 #交换

	talk
	
endproc
proc 701
	
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300930 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300930 1 1
			
		;	rand Rad 100
		;	if @Rad < 80
		;		ADD_ITEM_NUM_INPAK 5300931 1 0 0
		;	endif
		;	if @Rad >= 80 and @Rad < 95
		;		ADD_ITEM_NUM_INPAK 5300932 1 0 0
		;	endif
		;	if @Rad >= 95 and @Rad < 99
				ADD_ITEM_NUM_INPAK 5300933 1 0 0
		;	endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300934 1 0 0
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



