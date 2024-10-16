

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b没有吃没有穿
	add_talk #b#b自有那敌人送上前
	add_talk #b#b没有枪没有炮
	add_talk #b#b敌人给我们造
	add_talk #b#b我们生长在这里
	add_talk #b#b每一寸土地都是我们自己的
	add_talk #b#b无论谁要抢占去
	add_talk #b#b我们就和他拼到底

	task_add_option 1625 1 118
	task_add_option 1626 1 119
	task_add_option 1627 1 120
	task_add_option 1628 1 121

	TASK_STAT 12048 1 20
	if 1 = 0 and 20 = 2
	  	option 150 (环次)这是基金通知函，我希望得到一些消息
	endif

	TASK_STAT 12248 1 20
	if 1 = 0 and 20 = 2
	  	option 160 (环次)我给你带来了风车模型
	endif
	talk
endproc 


PROC 118
	task_do_option 1625 1
ENDPROC
PROC 119
	task_do_option 1626 1
ENDPROC
PROC 120
	task_do_option 1627 1
ENDPROC
PROC 121
	task_do_option 1628 1
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
	task_fini 12048 1
	endif
ENDPROC




