proc 1

	strvar Ret
	GET_ID_PLAYER_INFO 0 5 Ret
	if Ret = 3100
		transmit 2100 29600 9900 1
	endif
	if Ret = 2200
		transmit 2100 14900 22900 1
	endif	
	if Ret = 2104
		transmit 2100 20500 3800 1
	endif
endproc
