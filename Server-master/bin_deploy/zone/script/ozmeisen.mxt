

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b不管什么神或人，请让那些恐怖的怪物消失吧，我只是希望可以好好的耕种我的地啊。
	

;20~30
	
	task_add_option 1494 1 110
	task_add_option 1495 1 111
	task_add_option 1496 1 112
	
	

	TASK_STAT 12025 1 20
	if 1 = 0 and 20 = 2
	  	option 150 (环次)学院长让我来打探一下雪原的情况
	endif

	TASK_STAT 12225 1 20
	if 1 = 0 and 20 = 2
	  	option 160 (环次)我带来了协议书，请您看一下
	endif
	
	talk
endproc 


PROC 110
	task_do_option 1494 1
ENDPROC
PROC 111
	task_do_option 1495 1
ENDPROC
PROC 112
	task_do_option 1496 1
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
	task_fini 12225 1
	endif
ENDPROC
proc 2
	get_trig_task 1 2

	if 2 = 1495
		task_fini 1495 1
	endif 
	if 2 = 1496
;			add_item_num_inpak 5300335 5 1 0
	endif 

endproc
