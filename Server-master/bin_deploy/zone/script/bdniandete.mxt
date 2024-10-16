


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b最近越来越多的商船被劫，可恨的是那些盗匪不但劫了财还伤人性命，现在已经没有几个商会敢出海了，曾几何时，我还在为工作太忙而报怨，现在我竟然开始怀念人头攒动的码头来。
		task_add_option 1355 1 100
		task_add_option 1356 1 101
		task_add_option 1377 1 102
		task_add_option 1379 1 103

	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300783 1 num1
	if num1 > 0
		TASK_STAT 3304 1 Ret
		if Ret = 2
			option 500 曲别针换别墅
		endif
	endif
	talk
	
endproc 

PROC 100
	task_do_option 1355 1
ENDPROC
PROC 101
	task_do_option 1356 1
ENDPROC
PROC 102
	task_do_option 1377 1
ENDPROC
PROC 103
	task_do_option 1379 1
ENDPROC
proc 2

	get_trig_task 1 2
	if 2 = 1356
		task_fini 1356 1
	endif

endproc 
	
proc 500
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b这么高深的学问，只有我才会感兴趣啦，别人一看到里面密密麻麻的数字就头晕眼花了，别藏着掖着了，跟我换吧！还犹豫什么？在这人来人往的码头，你以为这本书会成为绝本么？别犹豫了，过了这村就没有这个店了！

	option 501 #交换

	talk
endproc
	
proc 501
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300783 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300783 1 1
			
		;	rand Rad 100
		;	if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300784 1 0 0
		;	endif
		;	if @Rad >= 80 and @Rad < 95
		;		ADD_ITEM_NUM_INPAK 5300767 1 0 0
		;	endif
		;	if @Rad >= 95 and @Rad < 99
		;		ADD_ITEM_NUM_INPAK 5300768 1 0 0
		;	endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300769 1 0 0
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
	
	
	
	
	
	
	
	
	