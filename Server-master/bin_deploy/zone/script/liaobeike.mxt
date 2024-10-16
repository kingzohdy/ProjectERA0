

proc 1
	STRVAR playerid
	DIALOG 1
	add_talk $_npc$:
	add_talk  #b#b也许你觉得你每天都在做很多看起来毫无意义的事情，做一些可有可无的决定，但某天你的某个决定就能改变你的一生，那些与你息息相关的人的命运也会随之改变。

	task_add_option 1724 1 11
	task_add_option 1741 1 12
	task_add_option 1742 1 13
	task_add_option 1743 1 14
	task_add_option 1744 1 15
	task_add_option 1745 1 16
	task_add_option 1746 1 17
	task_add_option 1747 1 18
	
;	task_add_option 1801 1 100

	TASK_STAT 12065 1 20
	if 1 = 0 and 20 = 2
	  	option 150 (环次)这是变异的培养体，你有没有发现类似这种的生物体？
	endif
	TASK_STAT 12265 1 20
	if 1 = 0 and 20 = 2
	  	option 160 (环次)这是炼金需要的一些材料，你愿意帮忙收集一些么？
	endif
	talk
endproc 

PROC 11
	task_do_option 1724 1
ENDPROC
PROC 12
	task_do_option 1741 1
ENDPROC
PROC 13
	task_do_option 1742 1
ENDPROC
PROC 14
	task_do_option 1743 1
ENDPROC
PROC 15
	task_do_option 1744 1
ENDPROC
PROC 16
	task_do_option 1745 1
ENDPROC
PROC 17
	task_do_option 1746 1
ENDPROC
PROC 18
	task_do_option 1747 1
ENDPROC
proc 100
	task_do_option 1801 1
endproc 


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
	task_fini 12065 1
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
	task_fini 12265 1
	endif
ENDPROC
proc 2
	get_trig_task 1 2
	if 2 = 1747
		task_fini 1747 1
	endif
	if 2 = 1744
		effect 2 0 startGray()
		GET_PLAYER_ID 1 playerid
		TARGET_ADD_STATUS 1 @playerid 1018 1 120 1
	endif
	if 2 = 1801
		task_fini 1801 1
	endif
endproc
