

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk  想赚大钱么？
;50级

	
	task_add_option 4156 1 108
	task_add_option 4157 1 109
	
	TASK_STAT 12016 1 20
	if 1 = 0 and 20 = 2
	  	option 150 (环次)这是学院长给你的信，他希望得到你的帮助
	endif

	TASK_STAT 12216 1 20
	if 1 = 0 and 20 = 2
	  	option 160 (环次)看，我给你带来了时间城的模型
	endif
	talk
endproc 

PROC 104
	task_do_option 2206 1
ENDPROC
PROC 105
	task_do_option 2207 1
ENDPROC

PROC 108
	task_do_option 4156 1
ENDPROC
PROC 109
	task_do_option 4157 1
ENDPROC




PROC 160
    	DIALOG 1
   	ADD_TALK $_npc$       
    	ADD_TALK  时间城的建设，这样庞大的问题，我需要一个晚上，不，两个晚上好好想想看。

	OPTION 260 #(环次)好的！
	TALK
ENDPROC
PROC 260
	GET_ITEM_NUM_INPAK 5300512 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg 你身上没有这件物品
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 5300512 1 0
	task_fini 12216 1
	endif
ENDPROC
proc 2
	get_trig_task 1 2
	if 2 = 2007
		task_fini 2007 1
	endif


endproc