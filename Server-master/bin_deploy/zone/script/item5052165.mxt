;�»�ף�����

proc 1
    strvar ret	ret1
    ADD_MONEY 0 92130 20  ret
    if @ret != 0
    	ADD_SYS_MSG_START
	ADD_SYS_MSG �����е����ҳ���������
	SET_SYS_MSG_MODE 3 0 0
	MSG_INFO
    	return
    ENDIF
    
    DEC_ITEM_NUM_INPAK 5052165 1 ret1
ENDPROC
