

proc 1
	DIALOG 1
	;STRVAR id
	;GET_PLAYER_ID 1 id
	;HAS_STATUS  1219 1
	;IF 1 = 0
	;	TARGET_ADD_STATUS 1 @id 1219 1 1 1
	;ENDIF
	add_talk $_npc$:
	add_talk  #b#b人生其实在不知不觉中早已悄悄改变，当你勇敢走向黑暗挑战时，就会发现黑暗早已不在。  


	task_add_option 2240 1 113
	
	task_add_option 1719 1 11
	task_add_option 1720 1 12
	task_add_option 1721 1 13
	task_add_option 1722 1 14
	task_add_option 1723 1 15

	
	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300925 1 num1
	if num1 > 0
		TASK_STAT 3305 1 Ret
		if Ret = 2
			option 700 曲别针换别墅
		endif
	endif
	talk
endproc 
PROC 11
	task_do_option 1719 1
ENDPROC
PROC 12
	task_do_option 1720 1
ENDPROC
PROC 13
	task_do_option 1721 1
ENDPROC
PROC 14
	task_do_option 1722 1
ENDPROC
PROC 15
	task_do_option 1723 1
ENDPROC

PROC 113
	task_do_option 2240 1
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
	task_fini 12062 1
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
	task_fini 12262 1
	endif
ENDPROC


proc 700
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b人在江湖飘，还有什么比了解各地风俗更好的呢？
	add_talk #b#b别想借机提价，伙计，风俗志这种东西就像大海里的水，遍地都是。

		option 701 #交换

	talk
	
endproc
proc 701
	
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300925 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300925 1 1
			
		;	rand Rad 100
		;	if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300924 1 0 0
		;	endif
		;	if @Rad >= 80 and @Rad < 95
		;		ADD_ITEM_NUM_INPAK 5300927 1 0 0
		;	endif
		;	if @Rad >= 95 and @Rad < 99
		;		ADD_ITEM_NUM_INPAK 5300928 1 0 0
		;	endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300929 1 0 0
		;	endif
		else
			add_sys_msg_start
			add_sys_msg 背包空间不足！
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg 你身上没有可以交换的物品！
		set_sys_msg_mode 1 0 0
		msg_info
	endif
endproc 