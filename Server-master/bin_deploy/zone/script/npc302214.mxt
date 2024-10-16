;302214	传令兵


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b如你所见，空气中早已笼罩着战争的味道。
	
	strvar zhuangtai
	TASK_STAT 3720 0 zhuangtai
	if zhuangtai = 2
		option 100 #我是来转达工作密件的
	endif
	
	talk
endproc

proc 100
	strvar ret num
	GET_ITEM_NUM_INPAK 5301110 0 num
	if num > 0
		DEC_ITEM_NUM_INPAK 5301110 @num ret
		if ret = 0
			TASK_FINI 3720 0
			
			ADD_SYS_MSG_START
			ADD_SYS_MSG 你交出了工作密件，顺利完成【传达工作】任务
			SET_SYS_MSG_MODE 3 0 0
			MSG_INFO
		endif
	else
		ADD_SYS_MSG_START
		ADD_SYS_MSG 身上找不到工作密件，暂时无法完成【传达工作】任务
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	endif
endproc
