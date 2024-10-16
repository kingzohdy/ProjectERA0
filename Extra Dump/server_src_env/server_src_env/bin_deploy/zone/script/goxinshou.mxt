proc 1
	strvar Ret
	GET_ID_PLAYER_INFO 0 5 Ret

	if Ret = 1000
		transmit 2000 31300 21200 1
	endif
	if Ret = 2400
		transmit 2000 10900 48000 1
	endif
	
endproc
