;5级宝石包

proc 1
    strvar num ret
	GET_PACK_SPACE 0 num
	
	if @num < 1
		ADD_SYS_MSG_START
		ADD_SYS_MSG 请空出1个背包格子再开启吧！
		SET_SYS_MSG_MODE 1 0 0
		MSG_INFO
		return
	endif
	
	DEC_ITEM_NUM_INPAK 5052985 1 ret
	
	if  @ret = 0
		rand 10 7
			switch @10
				case 0
					ADD_BIND_ITEM_NUM_INPAK 5211004 1 1 0
				endcase
				case 1
					ADD_BIND_ITEM_NUM_INPAK 5211104 1 1 0
				endcase
				case 2
					ADD_BIND_ITEM_NUM_INPAK 5211204 1 1 0
				endcase
				case 3
					ADD_BIND_ITEM_NUM_INPAK 5212004 1 1 0
				endcase
				case 4
					ADD_BIND_ITEM_NUM_INPAK 5212404 1 1 0
				endcase
				case 5
					ADD_BIND_ITEM_NUM_INPAK 5213304 1 1 0
				endcase
				case 6
					ADD_BIND_ITEM_NUM_INPAK 5213404 1 1 0
				endcase
			endswitch
	endif
endproc
