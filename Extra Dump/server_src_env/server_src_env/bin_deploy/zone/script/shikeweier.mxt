

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk 看到了没，这片玉米地里……


	TASK_STAT 12056 1 20
	if 1 = 0 and 20 = 2
	  	option 150 (环次)这里地形真复杂，不过我给你带来了地图
	endif
	TASK_STAT 12256 1 20
	if 1 = 0 and 20 = 2
	  	option 160 (环次)你看一下，这可是新玩意，想投资么？
	endif
	talk
endproc 


PROC 150
    	DIALOG 1
   	ADD_TALK $_npc$       
    	ADD_TALK  哈哈，更多的金子，只有到最深处的地脉才能发到更多的金子，你明白吗。

	OPTION 250 #(环次)明白了！
	TALK
ENDPROC
PROC 250
	GET_ITEM_NUM_INPAK 5300508 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg 你身上没有这件物品
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 5300508 1 0
	task_fini 12056 1
	endif
ENDPROC

;proc 2
;	get_trig_task 1 2
;	if 2 = 4256 
;		task_fini 4256 1
;	endif  
;
;endproc
