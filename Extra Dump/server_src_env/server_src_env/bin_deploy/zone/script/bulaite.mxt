

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b看看这片土地上都发生了些什么吧！这片土地曾为一群善良老实的农民们所有。但如今，一群该死的毛贼把农民们全部赶走了。当然，除了老子我！

	
;修改
	task_add_option 2021 1 117

;;;;;;;
	task_add_option 1619 1 120

	

	TASK_STAT 12049 1 20
	if 1 = 0 and 20 = 2
	  	option 150 (环次)象征荣誉的勋章送给你
	endif

	TASK_STAT 12249 1 20
	if 1 = 0 and 20 = 2
	  	option 160 (环次)你看一下，这可是新玩意，想投资么？
	endif
	talk
endproc 

PROC 117
	task_do_option 2021 1
ENDPROC
PROC 120
	task_do_option 1619 1
ENDPROC


PROC 150
    	DIALOG 1
   	ADD_TALK $_npc$:       
    	ADD_TALK  #b#b我只是想让这些可怜的动物不要因为人类的错误而死去。

	OPTION 250 #(环次)感谢你们做的一切！
	TALK
ENDPROC
PROC 250
	GET_ITEM_NUM_INPAK 5300507 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg 你身上没有这件物品
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 5300507 1 0
	task_fini 12049 1
	endif
ENDPROC


proc 2
	get_trig_task 1 2 

	if 2 = 4246 
		task_fini 4246 1
	endif
	if 2 = 1619 
		task_fini 1619 1
	endif
endproc
