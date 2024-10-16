;吉萨



proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b考古界近百年来没有起色的最大原因就是有些做事说话极其不负责任但又装出一副诲人不倦的样子的人长期占据权威位置。
	
	
	task_add_option 2005 1 100
	task_add_option 2006 1 101
	task_add_option 2007 1 102
	task_add_option 2008 1 103
	task_add_option 2009 1 104
	task_add_option 2018 1 105
	
;	strvar Ret num1
;	GET_ITEM_NUM_INPAK 5300932 1 num1
;	if num1 > 0
;		TASK_STAT 3305 1 Ret
;		if Ret = 2
;			option 700 曲别针换别墅
;		endif
;	endif
	talk
	
endproc 

PROC 100
	task_do_option 2005 1
ENDPROC
PROC 101
	task_do_option 2006 1
ENDPROC
PROC 102
	task_do_option 2007 1
ENDPROC
PROC 103
	task_do_option 2008 1
ENDPROC
PROC 104
	task_do_option 2009 1
ENDPROC
PROC 105
	task_do_option 2018 1
ENDPROC
proc 2
	
	get_trig_task 1 2 
;	if 2 = 2001
;		task_fini 2001 1
;	endif
	if 2 = 2009
		task_fini 2009 1
	endif
	if 2 = 2018
		task_fini 2018 1
	endif
endproc





proc 700
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b你想要什么？别墅？哈，别逗了，就你这本破书，顶多换块砖头！
	add_talk #b#b哎~别走啊，我也没说不换是吧。
		option 701 #交换

	talk
	
endproc
proc 701
	
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300932 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300932 1 1
			
			rand Rad 100
			if @Rad < 99
				ADD_ITEM_NUM_INPAK 5300933 1 0 0
			endif
			if @Rad = 99
				ADD_ITEM_NUM_INPAK 5300934 1 0 0
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
