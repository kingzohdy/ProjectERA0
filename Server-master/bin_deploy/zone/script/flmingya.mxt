
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b你显然不是盗墓贼，那么是朋友了？
		
		task_add_option 2043 1 100
		task_add_option 2044 1 101
		task_add_option 2045 1 102
		task_add_option 2046 1 103
		task_add_option 2047 1 104
		task_add_option 2051 1 105
		task_add_option 2052 1 106
		task_add_option 2042 1 107

	; strvar Ret num1
	; GET_ITEM_NUM_INPAK 5300933 1 num1
	; if num1 > 0
		; TASK_STAT 3305 1 Ret
		; if Ret = 2
			; option 700 曲别针换别墅
		; endif	
	; endif
	talk
endproc 

proc 100
	task_do_option 2043 1
endproc 
proc 101
	task_do_option 2044 1
endproc 
proc 102
	task_do_option 2045 1
endproc 
proc 103
	task_do_option 2046 1
endproc 
proc 104
	task_do_option 2047 1
endproc 
proc 105
	task_do_option 2051 1
endproc 
proc 106
	task_do_option 2052 1
endproc 
proc 107
	task_do_option 2042 1
endproc 


proc 2
	get_trig_task 1 2
	if 2 = 2047
		task_fini 2047 1
	endif

endproc

proc 700
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b人是人他妈生的，妖是妖他妈生的，那人妖……啊，不是人妖，是妖人，啊哦~~什么是妖人？这真是个值得研究的课题。
		option 701 #交换

	talk
	
endproc
proc 701
	
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300933 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300933 1 1
			
			ADD_ITEM_NUM_INPAK 5300934 1 0 0

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





