;航海大亨逻辑区域

proc 1
	;判断是否有临时机械
	GET_ROLE_TMP_ITEM 0 10
	if @10 = 1461002
		DEL_ROLE_TMP_ITEM 0 0
	else
		return
	endif
endproc 

