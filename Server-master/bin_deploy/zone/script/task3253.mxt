

proc 1

		RONGLU_TASK_GET 1 3253

endproc 

proc 3
	GET_PACK_SPACE 0 30
	if 30 = 0
		call 100
	;	ADD_ITEM_ON_MAP 1 0 5052316 1 0 0 0 0
	else
		ADD_BIND_ITEM_NUM_INPAK 5052319 1 0 1
	endif
endproc 
proc 5
	GET_PACK_SPACE 0 30
	if 30 = 0
		call 100
	;	ADD_ITEM_ON_MAP 1 0 5052316 1 0 0 0 0
	else
		ADD_BIND_ITEM_NUM_INPAK 5052319 1 0 1
	endif
endproc 
proc 100
	ADD_SYS_MSG_START
	ADD_SYS_MSG 背包已满，无法获得金熔炉凭证
	SET_SYS_MSG_MODE 3 0 0
	MSG_INFO
endproc