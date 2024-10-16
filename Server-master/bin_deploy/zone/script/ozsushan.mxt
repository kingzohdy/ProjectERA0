;蒙娜丽莎
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b美貌是一个女巫，在她的魔力下，忠诚是会在热情里溶解的。

	
;20~30
	
	task_add_option 1427 1 104
	task_add_option 1441 1 107
	
	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300767 1 num1
	if num1 > 0
		TASK_STAT 3304 1 Ret
		if Ret = 2
			option 500 曲别针换别墅
		endif	
	endif
	talk

endproc 

PROC 104
	task_do_option 1427 1
ENDPROC

PROC 107
	task_do_option 1441 1
ENDPROC

proc 500
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b那个，欧罢，和我换啦，大不了人家把自己的最最珍藏最最宝贵的东西给你看嘛。
	option 501 #交换

	talk
endproc

proc 501
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300767 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300767 1 1
			
		;	rand Rad 100
		;	if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300768 1 0 0
		;	endif
		;	if @Rad >= 80 and @Rad < 95
		;		ADD_ITEM_NUM_INPAK 5300769 1 0 0
		;	endif
		;	if @Rad >= 95 and @Rad < 99
		;		ADD_ITEM_NUM_INPAK 5300770 1 0 0
		;	endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300771 1 0 0
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


;proc 2
;	get_trig_task 1 2
;	
;	if 2 = 1438
;		task_fini 1438 1
;	endif
;	
;endproc 