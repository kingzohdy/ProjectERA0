

proc 1
	DIALOG 1
	add_talk $_npc$:
    add_talk #b#b我曾经看到杰尼狄普偷偷的跟人会面，若是好事干嘛不光明正大的进行呢？所以一定是什么见不得人的勾当，我在想要不要向费尔南斯神父揭穿他假面具呢，可又没有证据…哎，$_player$，你来帮我拿个主意吧。
	
	
;20~30
	task_add_option 1422 1 117
;	task_add_option 1423 1 118
	task_add_option 1424 1 119
	task_add_option 1425 1 120
	task_add_option 1410 1 121
;	task_add_option 1427 1 122
	task_add_option 1428 1 123
	

	TASK_STAT 12028 1 20
	if 1 = 0 and 20 = 2
	  	option 150 (环次)学院长让我来询问一下雪原的情况
	endif
	TASK_STAT 12228 1 20
	if 1 = 0 and 20 = 2
	  	option 160 (环次)我带来了协议书，请您看一下
	endif
	
	talk
endproc 

PROC 117
	task_do_option 1422 1
ENDPROC
;PROC 118
;	task_do_option 1423 1
;ENDPROC
PROC 119
	task_do_option 1424 1
ENDPROC
PROC 120
	task_do_option 1425 1
ENDPROC
PROC 121
	task_do_option 1410 1
ENDPROC
;PROC 122
;	task_do_option 1427 1
;ENDPROC
PROC 123
	task_do_option 1428 1
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
	task_fini 12228 1
	endif
ENDPROC
proc 2
	get_trig_task 1 2

	
	if 2 = 1426
;		GET_PLAYER_ID 1 54
;		TARGET_ADD_STATUS 1 @54 1099 1 300 53
		
;		task_fini 1426 1
	endif 

endproc
