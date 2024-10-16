;克里斯

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b欢迎来到脱脱酒馆，本店酒水半价，小菜免单，$_player$，要不要进来休息一会儿呢？

	TASKCYCLE_ADD_OPTION 105 1 140

	TASK_STAT 12051 1 20
	if 1 = 0 and 20 = 2
	  	option 150 送给你象征荣誉的勋章
	endif
	TASK_STAT 12251 1 20
	if 1 = 0 and 20 = 2
	  	option 160 你看一下，这可是新玩意，想投资么？
	endif
	talk
endproc 
proc 140
	TASKCYCLE_DO_OPTION 105 1
endproc 

PROC 150
    	DIALOG 1
   	ADD_TALK $_npc$:       
    	ADD_TALK  #b#b我只是想让这些可怜的动物不要因为人类的错误而死去。

	OPTION 250 #知道了！
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
	task_fini 12051 1
	endif
ENDPROC

;proc 2
;	get_trig_task 1 2 
;	if 2 = 4345
;		task_fini 4345 1
;	endif
;
;endproc

