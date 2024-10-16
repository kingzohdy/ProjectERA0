

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b无论是什么让他们变成这样，我们都必须要让他们……安息……

	
;;;;;;

	
	task_add_option 1610 1 119
	task_add_option 1611 1 120
	task_add_option 1612 1 121
	task_add_option 1613 1 122
	task_add_option 1614 1 123
	task_add_option 1615 1 124
	task_add_option 1616 1 125
	task_add_option 1617 1 126
	task_add_option 1618 1 127

	TASK_STAT 12047 1 20
	if 1 = 0 and 20 = 2
	  	option 150 (环次)这是通知函，告诉我你知道的消息吧
	endif

	TASK_STAT 12247 1 20
	if 1 = 0 and 20 = 2
	  	option 160 (环次)我给你带来了风车模型
	endif
	task_stat 1615 1 20
	if 20 = 2
	
	endif
	talk
endproc 

PROC 119
	task_do_option 1610 1
ENDPROC
PROC 120
	task_do_option 1611 1
ENDPROC
PROC 121
	task_do_option 1612 1
ENDPROC
PROC 122
	task_do_option 1613 1
ENDPROC
PROC 123
	task_do_option 1614 1
ENDPROC
PROC 124
	task_do_option 1615 1
ENDPROC
PROC 125
	task_do_option 1616 1
ENDPROC
PROC 126
	task_do_option 1617 1
ENDPROC
PROC 127
	task_do_option 1618 1
ENDPROC

PROC 150
    	DIALOG 1
   	ADD_TALK $_npc$:       
    	ADD_TALK  #b#b如果奖赏合适的话，我想我是不会吝啬我的情报。

	OPTION 250 #(环次)好的！
	TALK
ENDPROC
PROC 250
	GET_ITEM_NUM_INPAK 5300506 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg 你身上没有这件物品
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 5300506 1 0
	task_fini 12047 1
	endif
ENDPROC



proc 2
	get_trig_task 1 2 

	if 2 = 1618
		task_fini 1618 1
	endif
	if 2 = 1615
	
;		effect 0 0 playEffct(10900,15000,"effect\\entity\\282013_18.ent")
		CALL_MON_BY_PATROL 1 230019 0 120 10900 15000 53 0 300
		
		;刷230019 
		add_sys_msg_start
		add_sys_msg 猎犬好像嗅出了正确的位置，跟着它走吧！
		set_sys_msg_mode 4 0 0
		msg_info
		get_task_var 1615 0 32 1 10
		var 10 @300
		set_task_var 1615 0 32 @10 1
	
	endif
	
endproc
