;���ﱦ��

	;ADD_SYS_MSG_START
	;ADD_SYS_MSG 
	;SET_SYS_MSG_MODE 0 0 0
	;MSG_INFO
	

proc 1

;	strvar Num RetX
;	GET_PACK_SPACE 0 Num
;	if @Num < 2

;		ADD_SYS_MSG_START
;		ADD_SYS_MSG �����������������Ʒ����Ԥ��2�������ո�λ���ٿ�����
;		SET_SYS_MSG_MODE 1 0 0
;		MSG_INFO
;		return
;	endif
	

	ITEM_ADD_ONEDAY 5030204 1
	;ʹ�ô���+1
;	GET_ITEM_ONEDAY_INFO 5030204 1 800 901
;	;���ʹ�ô���	
;	if 800 <= 10
;	;ʹ��20������  
;		DEC_ITEM_NUM_INPAK 5052800 1 RetX
;	else
;	
;	endif
	
endproc

	