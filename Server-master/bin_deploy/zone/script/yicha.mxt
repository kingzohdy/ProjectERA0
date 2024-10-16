

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b谁最先找到这块土地下隐藏的能源，谁就能够成就一方霸业。
	add_talk #b#b
	add_talk #c186506注意：环组任务放弃后算作完成一次。
	add_talk #c186506注意：接取任务后，将不能使用“我很忙”将其立刻完成。#n
	
	task_add_option 1747 1 11
	task_add_option 1770 1 13
	TASKCYCLE_ADD_OPTION 107 1 12


	TASK_STAT 12067 1 20
	if 1 = 0 and 20 = 2
	  	option 150 (环次)这是变异的培养体，你有没有发现类似这种的生物体？
	endif

	TASK_STAT 12267 1 20
	if 1 = 0 and 20 = 2
	  	option 160 (环次)这是炼金需要的一些材料，你愿意帮忙收集一些么？
	endif
	
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	
	if lv >= 55 and lv < 65
		option 230 我要立刻完成【环组】越雾雨林
	endif
	
	talk
endproc 
PROC 11
	task_do_option 1747 1
ENDPROC
PROC 12
	TASKCYCLE_DO_OPTION 107 1
ENDPROC
PROC 13
	task_do_option 1770 1
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
	task_fini 12067 1
	endif
ENDPROC

PROC 160
    	DIALOG 1
   	ADD_TALK $_npc$       
    	ADD_TALK  在危险的丛林之中游走，一直都不会去注意有什么东西，但是如果是炼金术师要求的话，我想我会考虑一下。

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
	task_fini 12267 1
	endif
ENDPROC
;proc 2
;	get_trig_task 1 2
;	if 2 = 1770
;		task_fini 1770 1
;	endif    


;endproc
proc 230
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	if lv >= 55 and lv < 65
		PLAYER_BUSY_UI 18 1
	endif
endproc