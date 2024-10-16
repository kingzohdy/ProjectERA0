;苏木的木箱

proc 1

;	strvar Rad_1 Rad_2 Rad_3 Rad_4 Num Num1 Num2 Num3

    strvar Rad Num Num1 Num2 Num3 Num4
		GET_PACK_SPACE 1 Num
		GET_PACK_SPACE 0 Num1
		GET_ITEM_NUM_INPAK 5300561 0 Num2
		GET_ITEM_NUM_INPAK 5300562 0 Num3
		GET_ITEM_NUM_INPAK 5300563 0 Num4

	if @Num < 1
		ADD_SYS_MSG_START
		ADD_SYS_MSG 请空出1个任务背包格子再开启吧！
		SET_SYS_MSG_MODE 1 0 0
		MSG_INFO
	elseif @Num2 < 3
		DEC_ITEM_NUM_INPAK 5030015 1 Rad
		if @Rad = 0
			ADD_ITEM_NUM_INPAK 5300561 1 1 0
		endif
	elseif @Num3 < 3
		DEC_ITEM_NUM_INPAK 5030015 1 Rad
		if @Rad = 0
			ADD_ITEM_NUM_INPAK 5300562 1 1 0
		endif
	elseif @Num4 < 3
		DEC_ITEM_NUM_INPAK 5030015 1 Rad
		if @Rad = 0
			ADD_ITEM_NUM_INPAK 5300563 1 1 0
		endif
	elseif @Num1 < 1
		ADD_SYS_MSG_START
		ADD_SYS_MSG 请空出1个杂物背包格子再开启吧！
		SET_SYS_MSG_MODE 1 0 0
		MSG_INFO
	else
		DEC_ITEM_NUM_INPAK 5030015 1 Rad
		if @Rad = 0
			ADD_ITEM_NUM_INPAK 5011003 1 1 0
		endif
		; return
	endif

	; strvar Rad	
	; DEC_ITEM_NUM_INPAK 5030015 1 Rad

    ; if  @Rad = 0
		; if @Num2 < 3
			; DEC_ITEM_NUM_INPAK 5030015 1 Rad
			; if @Rad = 0
				; ADD_ITEM_NUM_INPAK 5300561 1 1 0
			; endif
		; elseif @Num3 < 3
			; DEC_ITEM_NUM_INPAK 5030015 1 Rad
			; if @Rad = 0
				; ADD_ITEM_NUM_INPAK 5300562 1 1 0
			; endif
		; elseif @Num4 < 3
			; DEC_ITEM_NUM_INPAK 5030015 1 Rad
			; if @Rad = 0
				; ADD_ITEM_NUM_INPAK 5300563 1 1 0
			; endif
		; elseif @Num1 < 1
			; ADD_SYS_MSG_START
			; ADD_SYS_MSG 请空出1个杂物背包格子再开启吧！
			; SET_SYS_MSG_MODE 1 0 0
			; MSG_INFO
		; else
			; DEC_ITEM_NUM_INPAK 5030015 1 Rad
			; if @Rad = 0
				; ADD_ITEM_NUM_INPAK 5011003 1 1 0
			; endif
		; endif
	; endif


;	rand Rad_1 3
	; var Rad_1 3
	; var Rad_2 0
	; while Rad_2 < @Rad_1
	; add Rad_2 1
		; rand Rad_3 3
		; switch Rad_3
			; case 0
				; ADD_ITEM_NUM_INPAK 5300561 1 1 0
			; endcase 
			; case 1
				; ADD_ITEM_NUM_INPAK 5300562 1 1 0
			; endcase
			; case 2
				; ADD_ITEM_NUM_INPAK 5300563 1 1 0
			; endcase
		; endswitch
	
	; endwhile
	
	; rand Rad_4 10
	; if @Rad_4 < 3
		; ADD_ITEM_NUM_INPAK 5000003 1 1 0		
	; endif
	; if @Rad_4 >= 3 and @Rad_4 < 7
		; ADD_ITEM_NUM_INPAK 5001003 1 1 0
	; endif
	
endproc 
