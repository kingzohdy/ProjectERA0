; ���Թ���ű�
;
PROC 1 
	MAP_MSG_INFO �������
	MON_SKILL 2100 1
ENDPROC

PROC 2
	MAP_MSG_INFO ��������
	GET_MON_NUM 1 2
	IF 1 = -1
		RETURN
	ENDIF

	IF 2 = 0
		MAP_MSG_INFO ���һ�����ﱻ���
	ENDIF	
ENDPROC


  

