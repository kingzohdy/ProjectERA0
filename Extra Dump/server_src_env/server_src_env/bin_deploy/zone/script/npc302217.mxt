;302217	侦察兵


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b掌控了关键情报，既掌控了胜利的大门。
	
	strvar zhuangtai
	TASK_STAT 3722 0 zhuangtai
	if zhuangtai = 2
		option 100 #我是来回收情报密函的
	endif
	
	talk
endproc

proc 100
	strvar ret num
	GET_PACK_SPACE 1 num
	if num > 0
		ADD_BIND_ITEM_NUM_INPAK 5301112 1 1 ret
		if ret = 0
			TASK_FINI 3722 0
			
			ADD_SYS_MSG_START
			ADD_SYS_MSG 你成功回收了情报密函，顺利完成【回收情报】任务
			SET_SYS_MSG_MODE 3 0 0
			MSG_INFO
		endif
	else
		ADD_SYS_MSG_START
		ADD_SYS_MSG 你的背包装不下更多物品，暂时无法回收情报密函
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	endif
endproc
