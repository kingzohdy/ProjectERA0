

proc 1
	STRVAR a m3
	GET_TASK_VAR 1731 2 8 1 m3
	DIALOG 1
	add_talk $_npc$:
	add_talk  #b#b我只是在必要的时候才会勇敢，勇敢并不代表你要到处闯祸。真正的勇敢是能够为了国家的存亡不远万里冒险工作，扫平一切阻碍国家发展的障碍，甚至为此牺牲。
	
	task_add_option 1718 1 11
	task_add_option 1719 1 12
	task_add_option 1723 1 13
	task_add_option 1724 1 14
	task_add_option 1725 1 15
	task_add_option 1726 1 16
	task_add_option 1727 1 17
	task_add_option 1728 1 18
	task_add_option 1737 1 19
	task_add_option 1738 1 20


	TASK_STAT 1731 1 a
	if a = 2 and m3 = 0
		OPTION 301 #佐齐尔的至尊烤肉
	endif
	
	
	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300780 1 num1
	if num1 > 0
		TASK_STAT 3304 1 Ret
		if Ret = 2
			option 500 曲别针换别墅
		endif
	endif
	talk
endproc 

PROC 11
	task_do_option 1718 1
ENDPROC
PROC 12
	task_do_option 1719 1
ENDPROC
PROC 13
	task_do_option 1723 1
ENDPROC
PROC 14
	task_do_option 1724 1
ENDPROC
PROC 15
	task_do_option 1725 1
ENDPROC
PROC 16
	task_do_option 1726 1
ENDPROC
PROC 17
	task_do_option 1727 1
ENDPROC
PROC 18
	task_do_option 1728 1
ENDPROC
PROC 19
	task_do_option 1737 1
ENDPROC
PROC 20
	task_do_option 1738 1
ENDPROC


proc 500
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b你运气真不错，我这里可都是价值千金的珍藏本，跟我换吧，别犹豫啦！
	option 501 #交换
		
	talk
endproc
proc 501
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300780 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300780 1 1
			
		;	rand Rad 100
			
		;	if @Rad < 95
				ADD_ITEM_NUM_INPAK 5300781 1 0 0
		;	endif
		;	if @Rad >= 95 and @Rad < 99
		;		ADD_ITEM_NUM_INPAK 5300782 1 0 0
		;	endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300785 1 0 0
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



PROC 301
	GET_TASK_VAR 1731 2 8 1 m3
	if m3 = 0
		DEC_ITEM_NUM_INPAK 5300476 1 1
		if 1 = -1
			add_sys_msg_start
			add_sys_msg 你没有肉，叫我尝什么？
			set_sys_msg_mode 4 0 0
			msg_info		
		else
			var @m3 1
			set_task_var 1731 2 8 @m3 1
				if 1 = -1
					return
				endif
		endif
	endif	
ENDPROC
proc 2
	get_trig_task 1 2
	if 2 = 1724 
		task_fini 1724 1
	endif
endproc
