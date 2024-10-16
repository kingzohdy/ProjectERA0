

proc 1
	STRVAR id a m1
	GET_TASK_VAR 1731 0 8 1 m1
	DIALOG 1
	
	GET_PLAYER_ID 1 id
	HAS_STATUS  1219 1
	IF 1 = 0
		TARGET_ADD_STATUS 1 @id 1219 1 1 1
	ENDIF
	
	
	add_talk $_npc$:
	add_talk  #b#b世界上所有的生命都在微妙的平衡中生存，身为世界的主人，你不但要了解那种平衡，还要尊重所有生物，包括爬行的蚂蚁和跳跃的虎豹。

	task_add_option 1732 1 11
	task_add_option 1733 1 12
	task_add_option 1734 1 13
	task_add_option 1735 1 14
	task_add_option 1736 1 15
	task_add_option 1739 1 16
	task_add_option 1740 1 17
	
	TASK_STAT 12064 1 20
	if 1 = 0 and 20 = 2
	  	option 150 (环次)这是变异的培养体，你有没有发现类似这种的生物体？
	endif


	TASK_STAT 12264 1 20
	if 1 = 0 and 20 = 2
	  	option 160 (环次)这是炼金需要的一些材料，你愿意帮忙收集一些么？
	endif
	
	TASK_STAT 1731 1 a
		if a = 2 and m1 = 0
		OPTION 301 #佐齐尔的至尊烤肉
		endif
	talk
endproc 

PROC 11
	task_do_option 1732 1
ENDPROC
PROC 12
	task_do_option 1733 1
ENDPROC
PROC 13
	task_do_option 1734 1
ENDPROC
PROC 14
	task_do_option 1735 1
ENDPROC
PROC 15
	task_do_option 1736 1
ENDPROC
PROC 16
	task_do_option 1739 1
ENDPROC
PROC 17
	task_do_option 1740 1
ENDPROC


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
	task_fini 12064 1
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
	task_fini 12264 1
	endif
ENDPROC
PROC 301
	GET_TASK_VAR 1731 0 8 1 m1
	if m1 = 0
	
		DEC_ITEM_NUM_INPAK 5300476 1 1
		if 1 = -1
			add_sys_msg_start
			add_sys_msg 你没有肉，叫我尝什么？
			set_sys_msg_mode 4 0 0
			msg_info		
		else
			var @m1 1
			set_task_var 1731 0 8 @m1 1
				if 1 = -1
					return
				endif
		endif
	endif	
ENDPROC
