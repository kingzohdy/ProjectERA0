;阿克萨


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b我没有太多的时间用来闲聊！如果你只是想打发时间，不妨到竞技场那儿去看看，那里经常有人竞技，或许你可以从中学到些实用的技巧。
	
 	task_add_option 3404 1 100
;	task_add_option 3405 1 101
 	task_add_option 3401 1 102
 	task_add_option 3402 1 103
	
 	task_add_option 3168 1 104
 	task_add_option 3169 1 105
 	task_add_option 3409 1 106
	
 	task_add_option 1801 1 107
	
	; strvar Ret num1
	; GET_ITEM_NUM_INPAK 5300926 1 num1
	; if num1 > 0
		; TASK_STAT 3305 1 Ret
		; if Ret = 2
			; option 700 曲别针换别墅
		; endif
	; endif
	strvar Rac Ret Num
	task_stat 3402 1 Ret
	if Ret = 2 
	
		get_task_var 3402 0 8 1 30
		var 30 0
		set_task_var 3402 0 8 @30 1
		GET_ID_PLAYER_INFO 0 7 Rac
		if Rac = 1
		;战士
			option 130 #进入幻境
		endif
		if Rac = 2
		;法师
			option 131 #进入幻境
		endif
		if Rac = 3
		;枪手
			option 132 #进入幻境
		endif
		if Rac = 4
		;牧师
			option 133 #进入幻境
		endif
	endif
	
	ROLE_ITEM_NUM 5030021 Num 1
	GET_ID_PLAYER_INFO 0 7 Rac
	if Ret = 3 and Num = 0 
		if Rac = 1 or Rac = 2 or Rac = 3 or Rac = 4
			option 150 #获得学识之书
		endif
	endif


	
	talk
endproc 






proc 150
	OPEN_NPC_SHOP 
endproc 

PROC 100
	task_do_option 3404 1
ENDPROC
PROC 101
	task_do_option 3405 1
ENDPROC
PROC 102
	task_do_option 3401 1
ENDPROC
PROC 103
	task_do_option 3402 1
ENDPROC
PROC 104
	task_do_option 3168 1
ENDPROC
PROC 105
	task_do_option 3169 1
ENDPROC
PROC 106
	task_do_option 3409 1
ENDPROC
PROC 107
	task_do_option 1801 1
ENDPROC


proc 130
	ENTER_PWORLD 509 0 0
endproc 
proc 131
	ENTER_PWORLD 510 0 0
endproc 
proc 132
	ENTER_PWORLD 511 0 0
endproc 
proc 133
	ENTER_PWORLD 512 0 0
endproc 

proc 2
	get_trig_task 1 2 
	if 2 = 1801
		task_fini 1801 1
	endif
	if 2 = 3402
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b我没有太多的时间用来闲聊！如果你只是想打发时间，不妨到竞技场那儿去看看，那里经常有人竞技，或许你可以从中学到些实用的技巧。
	
			strvar Rac
			GET_ID_PLAYER_INFO 0 7 Rac
			if Rac = 1
			;战士
				option 130 #进入幻境
			endif
			if Rac = 2
			;法师
				option 131 #进入幻境
			endif
			if Rac = 3
			;枪手
				option 132 #进入幻境
			endif
			if Rac = 4
			;牧师
				option 133 #进入幻境
			endif
		talk
	endif
endproc 


; proc 700
	; DIALOG 1
	; add_talk $_npc$:
	; add_talk #b#b如果当真按照书里的内容催动咒语，只怕伤人伤已！不知道是谁出版了这篇误人子弟的东西……
	; add_talk #b#b跟我交换吧，你也不希望有人因你的行为而受到伤害，对吧？

		; option 701 #交换

	; talk
	
; endproc
; proc 701
	
	; strvar ItmNum Num Rad
	; GET_ITEM_NUM_INPAK 5300926 1 ItmNum
	; if ItmNum > 0
			
		; GET_PACK_SPACE 0 Num
		; if Num > 0
			; DEC_ITEM_NUM_INPAK 5300926 1 1
			
			; rand Rad 100
			; if @Rad < 80
				; ADD_ITEM_NUM_INPAK 5300927 1 0 0
			; endif
			; if @Rad >= 80 and @Rad < 95
				; ADD_ITEM_NUM_INPAK 5300928 1 0 0
			; endif
			; if @Rad >= 95 and @Rad < 99
				; ADD_ITEM_NUM_INPAK 5300929 1 0 0
			; endif
			; if @Rad = 99
				; ADD_ITEM_NUM_INPAK 5300930 1 0 0
			; endif
		; else
			; add_sys_msg_start
			; add_sys_msg 背包空间不足！
			; set_sys_msg_mode 1 0 0
			; msg_info
		; endif
	; else
		; add_sys_msg_start
		; add_sys_msg 你身上没有可以交换的物品！
		; set_sys_msg_mode 1 0 0
		; msg_info
	; endif
; endproc 
