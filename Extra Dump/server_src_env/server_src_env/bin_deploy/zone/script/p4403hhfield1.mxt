;��������߼�����

proc 1
	;�ж��Ƿ�����ʱ��е
	GET_ROLE_TMP_ITEM 0 10
	if @10 = 1461002
		DEL_ROLE_TMP_ITEM 0 0
	else
		return
	endif
endproc 

