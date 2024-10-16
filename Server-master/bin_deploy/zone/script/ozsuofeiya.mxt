
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b莫里亚蒂，无论你做了什么，光明的旅途都不会在这里结束，死亡不过是另一条路，一条我们人人都得走的路，你也不例外。
	

	
;20~30
	
	task_add_option 1402 1 115
	task_add_option 1403 1 116
	task_add_option 1404 1 117
	task_add_option 1405 1 118
	task_add_option 1406 1 119
	task_add_option 1415 1 120
;	task_add_option 1419 1 121
	task_add_option 1417 1 122
	task_add_option 1418 1 123
	task_add_option 1416 1 124
		
	
	
	TASK_STAT 12030 1 20
	if 1 = 0 and 20 = 2
	  	option 150 (环次)我想得到一些消息
	endif

	TASK_STAT 12230 1 20
	if 1 = 0 and 20 = 2
	  	option 160 (环次)我带来了协议书，请您看一下
	endif
	talk
endproc 


PROC 115
	task_do_option 1402 1
ENDPROC
PROC 116
	task_do_option 1403 1
ENDPROC
PROC 117
	task_do_option 1404 1
ENDPROC
PROC 118
	task_do_option 1405 1
ENDPROC
PROC 119
	task_do_option 1406 1
ENDPROC

PROC 120
	task_do_option 1415 1
ENDPROC
;PROC 121
;	task_do_option 1419 1
;ENDPROC
PROC 122
	task_do_option 1417 1
ENDPROC
PROC 123
	task_do_option 1418 1
ENDPROC
PROC 124
	task_do_option 1416 1
ENDPROC


proc 160
    	DIALOG 1
   	ADD_TALK $_npc$:       
    	ADD_TALK #b#b如果我知道更多的炼金材料的消息，我一定会通知炼金术师阁下的。

	OPTION 260 #(环次)谢谢你的支持！
	TALK
endproc
PROC 260
	GET_ITEM_NUM_INPAK 5300513 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg 你身上没有这件物品
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
		dec_item_num_inpak 5300513 1 0
	task_fini 12230 1
	endif
ENDPROC


proc 2
	get_trig_task 1 2
	if 2 = 1406
		task_fini 1406 1
	endif
	if 2 = 1418
		task_fini 1418 1
	endif
;	if 2 = 1419
;		task_fini 1419 1
;	endif
endproc
