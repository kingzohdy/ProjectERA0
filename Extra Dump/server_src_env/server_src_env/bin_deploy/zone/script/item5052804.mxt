;±¶œ‰


proc 1
	strvar Num
	GET_PACK_SPACE 0 Num
	if @Num < 3

		ADD_SYS_MSG_START
		ADD_SYS_MSG  «Î‘§¡Ù3∏ˆŒª÷√‘Ÿø™∆Ù±¶œ‰£°
		SET_SYS_MSG_MODE 1 0 0
		MSG_INFO
		return
	else
		
		DEC_ITEM_NUM_INPAK 5052804 1 1
		ADD_ITEM_NUM_INPAK 5050210 5 1 0
		ADD_ITEM_NUM_INPAK 5002110 1 1 0
		ADD_ITEM_NUM_INPAK 5250321 1 1 0
	endif

endproc 