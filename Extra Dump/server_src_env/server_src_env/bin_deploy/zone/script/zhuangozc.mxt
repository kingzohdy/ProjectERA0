;转职任务勇士岛回主城

proc 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b永远不要妄图知道自己的末日，生命的奥妙就在于活在当下。
		
;	strvar Ret num1
;	GET_ITEM_NUM_INPAK 5300925 1 num1
;	if num1 > 0
;		TASK_STAT 3305 1 Ret
;		if Ret = 2
;			option 700 曲别针换别墅
;		endif
;	endif
	
	option 120 #回时间城

	talk
	
endproc
proc 120

	transmit 1000 28100 8800 1
endproc 

	
; proc 700
	; DIALOG 1
	; add_talk $_npc$:
	; add_talk #b#b人在江湖飘，还有什么比了解各地风俗更好的呢？
	; add_talk #b#b别想借机提价，伙计，风俗志这种东西就像大海里的水，遍地都是。

		; option 701 #交换

	; talk
	
; endproc
; proc 701
	
	; strvar ItmNum Num Rad
	; GET_ITEM_NUM_INPAK 5300925 1 ItmNum
	; if ItmNum > 0
			
		; GET_PACK_SPACE 0 Num
		; if Num > 0
			; DEC_ITEM_NUM_INPAK 5300925 1 1
			
			; rand Rad 100
			; if @Rad < 80
				; ADD_ITEM_NUM_INPAK 5300926 1 0 0
			; endif
			; if @Rad >= 80 and @Rad < 95
				; ADD_ITEM_NUM_INPAK 5300927 1 0 0
			; endif
			; if @Rad >= 95 and @Rad < 99
				; ADD_ITEM_NUM_INPAK 5300928 1 0 0
			; endif
			; if @Rad = 99
				; ADD_ITEM_NUM_INPAK 5300929 1 0 0
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