

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b越是巨大的冒险，越是需要巨大的谨慎，这与胆小、懦弱无关。
	task_add_option 1503 1 101
	task_add_option 1536 1 102

	task_add_option 1537 1 103
	task_add_option 1538 1 104 
	
	task_add_option 1507 1 106


	TASK_STAT 12035 1 20
	if 1 = 0 and 20 = 2
	  	option 150 (环次)你有没有看到过类似的细菌培养体啊？
	endif
	TASK_STAT 12235 1 20
	if 1 = 0 and 20 = 2
	  	option 160 (环次)这是给你的信，打开看看吧
	endif
	talk
endproc 


proc 101
    task_do_option 1503 1
endproc
proc 102
    task_do_option 1536 1
endproc
proc 103
    task_do_option 1537 1
endproc
proc 104
    task_do_option 1538 1
endproc
proc 106
    task_do_option 1507 1
endproc


PROC 150	
    	DIALOG 1
   	ADD_TALK $_npc$:       
    	ADD_TALK  #b#b死亡的气息已经弥漫在整个沙漠，我们现在只能眼睁睁看着死亡吞噬家园。

	OPTION 250 #(环次)我会帮助你们的！
	TALK
ENDPROC
proc 250
	
	GET_ITEM_NUM_INPAK 5300504 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg 你身上没有这件物品
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 5300504 1 0
    	task_fini 12035 1
	endif
endproc

proc 160
    	DIALOG 1
   	ADD_TALK $_npc$:       
    	ADD_TALK  #b#b新鲜的鲜血已经成为这些不死生物的追求，我们已经陷入恐慌之中了。

	OPTION 260 #(环次)我们一起想办法！
	TALK
endproc
proc 260
	
	GET_ITEM_NUM_INPAK 5300514 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg 你身上没有这件物品
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 5300514 1 0
    	task_fini 12235 1
	endif
endproc
;proc 2
;	get_trig_task 1 2
;	if 2 = 1210
;		task_fini 1210 1
;	endif
;
;endproc 