

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b我是伊丽莎白

	TASK_STAT 12050 1 20
	if 1 = 0 and 20 = 2
	  	option 150 (环次)送给你象征荣誉的勋章
	endif

	TASK_STAT 12250 1 20
	if 1 = 0 and 20 = 2
	  	option 160 (环次)你看一下，这可是新玩意，想投资么？
	endif
	talk
endproc 


PROC 150
    	DIALOG 1
   	ADD_TALK $_npc$       
    	ADD_TALK  我只是想让这些可怜的动物不要因为人类的错误而死去。

	OPTION 250 #(环次)知道了！
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
	task_fini 12050 1
	endif
ENDPROC



