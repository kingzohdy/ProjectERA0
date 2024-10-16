

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk  #b#b我们必须要找到能够替代元素石的能源，我们必须将世界掌握在自己手中！否则，我们的家人、朋友，包括我们自己都要活在一个比死还要痛苦的世界！
	
	
	task_add_option 1705 1 11
	task_add_option 1706 1 12
	task_add_option 1707 1 13
	task_add_option 1708 1 14
	task_add_option 1729 1 15
	task_add_option 1730 1 16
	task_add_option 1731 1 17


	TASK_STAT 12059 1 20
	if 1 = 0 and 20 = 2
	  	option 150 (环次)这是变异的培养体，你有没有发现类似这种的生物体？
	endif
	TASK_STAT 12259 1 20
	if 1 = 0 and 20 = 2
	  	option 160 (环次)这是炼金需要的一些材料，你愿意帮忙收集一些么？
	endif
	talk
endproc 

PROC 11
	task_do_option 1705 1
ENDPROC
PROC 12
	task_do_option 1706 1
ENDPROC
PROC 13
	task_do_option 1707 1
ENDPROC
PROC 14
	task_do_option 1708 1
ENDPROC
PROC 15
	task_do_option 1729 1
ENDPROC
PROC 16
	task_do_option 1730 1
ENDPROC
PROC 17
	task_do_option 1731 1
ENDPROC

PROC 150
    	DIALOG 1
   	ADD_TALK $_npc$       
    	ADD_TALK  灾难已经降临到这片土地，恐怖的事情一件接着一件发生。

	OPTION 250 #(环次)知道了！
	TALK
ENDPROC
PROC 250
	GET_ITEM_NUM_INPAK 5300509 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg 你身上没有这件物品
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 5300509 1 0

	task_fini 12059 1
	endif
ENDPROC

PROC 160
    	DIALOG 1
   	ADD_TALK $_npc$       
    	ADD_TALK  #b#b少年，既然你能走到这里，那就继续往前吧。。

	OPTION 260 #(环次)谢谢你了！
	TALK
ENDPROC
PROC 260
	GET_ITEM_NUM_INPAK 5300519 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg 你身上没有这件物品
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 5300519 1 0

	task_fini 12259 1
	endif
ENDPROC

proc 2
	get_trig_task 1 2
	if 2 = 1708 
		task_fini 1708 1
	endif
endproc
