


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b嗝…一…嗝…二…嗝…三…嗝…六…嗝…这次来了个六…六胞胎…还是昨天那…嗝…那家伙的爹妈更给力…嗝…，十……十胞胎…嗝……
		task_add_option 1360 1 100
		task_add_option 1361 1 101
		task_add_option 1362 1 102

	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300784 1 num1

	TASK_STAT 3304 1 Ret
	if num1 > 0
		if Ret = 2
			option 500 曲别针换别墅
		endif
	endif
	talk
	
endproc 

PROC 100
	task_do_option 1360 1
ENDPROC
PROC 101
	task_do_option 1361 1
ENDPROC
PROC 102
	task_do_option 1362 1
ENDPROC

proc 2

	get_trig_task 1 2
	if 2 = 1362
		task_fini 1362 1
	endif

endproc 
	
proc 500
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b听说这里罗列了当今世界最昂贵的佳酿，并按照品评报告、评分进行了排序分类，历史和资料整理的也十分完备，当然，最要命的是这里只介绍最知名的顶极佳酿，所以要打算按图索骥的话，还需要腰包里多准备些银两。
	add_talk #b#b像你这样的不懂酒的人拿着实在没什么用，不如和我换吧。
	option 501 #交换

	talk
endproc
	
proc 501
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300784 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300784 1 1
			
		;	rand Rad 100
		;	if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300767 1 0 0
		;	endif
		;	if @Rad >= 80 and @Rad < 95
		;		ADD_ITEM_NUM_INPAK 5300768 1 0 0
		;	endif
		;	if @Rad >= 95 and @Rad < 99
		;		ADD_ITEM_NUM_INPAK 5300769 1 0 0
		;	endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300770 1 0 0
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
	
	