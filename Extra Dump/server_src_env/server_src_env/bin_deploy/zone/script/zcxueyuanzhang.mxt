
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b可怜的瑞德、浦尔、吉格和伊万，他们早该听我的及时收手，结果现在都得替我做赛马裁判员了，我为他们感到难过。
	add_talk #b#b那年轻的朋友，你有兴趣跟我赌一把吗，不过我得提醒你，及时收手，否则你会输得很惨。

	task_add_option 4101 1 102
	task_add_option 1110 1 103
	task_add_option 4119 1 104

	task_add_option 4207 1 105
	task_add_option 4210 1 106
	task_add_option 4211 1 107
;50级


	task_add_option 4120 1 110

;	TASKCYCLE_ADD_OPTION 101 1 120
	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300765 1 num1
	if num1 > 0
		TASK_STAT 3304 1 Ret
		if Ret = 2
			option 500 曲别针换别墅
		endif
	endif

	talk
endproc 

PROC 102
	task_do_option 4101 1
ENDPROC
PROC 103
	task_do_option 1110 1
ENDPROC
PROC 104
	task_do_option 4119 1
ENDPROC
PROC 105
	task_do_option 4207 1
ENDPROC
PROC 106
	task_do_option 4210 1
ENDPROC
PROC 107
	task_do_option 4211 1
ENDPROC

PROC 110
	task_do_option 4120 1
ENDPROC

PROC 120	
	TASKCYCLE_DO_OPTION 101 1
ENDPROC


proc 2
	get_trig_task 1 2
	if 2 = 4207
		task_fini 4207 1
	endif 
	if 2 = 4211 
		task_fini 4211 1
	endif 

endproc

proc 500
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b嗨，#Name#，就和我交换吧，我的梦想是收藏世界上所有的精灵序，既然你愿意帮助唐纳成就别墅的梦想，为什么不顺便帮我一下呢？

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
				ADD_ITEM_NUM_INPAK 5300783 1 0 0
			endif
			if @Rad >= 95 and @Rad < 99
				ADD_ITEM_NUM_INPAK 5300784 1 0 0
			endif
			if @Rad = 99
				ADD_ITEM_NUM_INPAK 5300767 1 0 0
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



