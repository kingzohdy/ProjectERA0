;
;script filename: npc1
;
PROC    1
	DIALOG 1
	ADD_TALK	���������٣��ϰֽа����������ӻ�
	GET_SEX 1
	IF 1 = 1
		ADD_TALK	��磬��Ҫ����������������
	ELSE
		ADD_TALK	��㣬��Ҫ����������������
	ENDIF
	OPTION 21 ��
	OPTION 22 ��
   TALK
ENDPROC

PROC 21
	START_DEAL 1
	FINISH
ENDPROC

PROC 22
	START_DEAL 0
	FINISH
ENDPROC
