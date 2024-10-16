

proc 5
	strvar num ret
	BCT_GET_CURR_LAYER num ret
	if ret = 0 and num = 30
		NEW_NPC 1 304047 6400 4300 0 zhbuluoke.mac
	endif
endproc 