

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b巨大的财富总是先让人有巨大的安全感，然后就会让人滋生巨大的底气，接着就会想有更多的财富。
;	task_add_option 1576 1 101
;	task_add_option 1577 1 102
;	task_add_option 1578 1 103
 ;	task_add_option 1579 1 104
	TASKCYCLE_ADD_OPTION 103 1 140
	TASK_STAT 12044 1 20
	if 1 = 0 and 20 = 2
	  	option 150 (环次)学院长让我来询问一下关于地宫的消息
	endif
	TASK_STAT 12244 1 20
	if 1 = 0 and 20 = 2
	  	option 160 (环次)这是一本古老的图书，希望你参照这本书能给我一些消息
	endif
	talk
endproc 
proc 140
	TASKCYCLE_DO_OPTION 103 1
endproc

PROC 101
	task_do_option 4250 1
ENDPROC
PROC 102
	task_do_option 4241 1
ENDPROC
PROC 103
	task_do_option 4244 1
ENDPROC
PROC 104
	task_do_option 4245 1
ENDPROC

PROC 150
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b古老的地宫深埋着神秘的讯息，一切都将会随着我们的探寻找到真实的信息。我会将资料交给学院长的。
	option 250 #(环次)好的
	TALK
ENDPROC
PROC 250
	GET_ITEM_NUM_INPAK 5300505 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg 你身上没有这件物品
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 5300505 1 0
	task_fini 12044 1
	endif
ENDPROC

;proc 2
;	get_trig_task 1 2
;	if 2 = 4250
;		task_fini 4250 1
;	endif  
; 
;endproc
