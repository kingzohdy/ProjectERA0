;
;script filename: npc2
;
PROC    1
	RAND 0 100
	IF 0 > 20
		CALL 30
	ELSE
		CALL 31
	ENDIF
ENDPROC

PROC    20
	DIALOG 2
	ADD_TALK	Ҫ���٣�
	INPUT 0 money 0 20000 ������
	NEXT 22
	TALK
ENDPROC

PROC    21
	DIALOG 3
	ADD_TALK	886
	TALK
ENDPROC

PROC    22
	GET_INPUT 0 money
	DIALOG 3
	IF 0 > 100
		ADD_TALK $money$����������
		ADD_TALK $_player$��������ȥ�ɣ�
	ELSEIF 0 > 0
		ADD_TALK $money$������С��˼�������θֹ��衣����
	ELSE
		ADD_TALK 0��
	ENDIF
	TALK
ENDPROC

PROC    23
	SET_REVIVE
	DIALOG 3
	ADD_TALK	�Ѿ��������趨�����㡣
	TALK
ENDPROC

PROC    30
	GET_LEVEL 1
	GET_EXP 2
	GET_SEX 3
	GETNUM_MONEY 4
	DIALOG 1
	ADD_TALK	$_player$, level $@1$, exp $@2$, sex $@3$, money $@4$
	ADD_TALK	�������趨�����㣿
	OPTION 23 ��
	OPTION 31 ����
	TALK
ENDPROC

PROC    31
	ADD_EXP 20

	RAND 0 100
	IF 0 > 50
		ADD_MONEY 500
	ELSE
		DEL_MONEY 200
	ENDIF

	GET_LEVEL 1
	GET_EXP 2
	GET_SEX 3
	GETNUM_MONEY 4

	DIALOG 1
	ADD_TALK	$_player$, level $@1$, exp $@2$, sex $@3$, money $@4$
	IF 4 > 1000
		CLEAR_MONEY
		ADD_TALK clear money
	ENDIF
	ADD_TALK	Ҫ���������Ǵ����--$_npc$��
	OPTION 20 Ҫ
	OPTION 21 ��Ҫ
	TALK
ENDPROC
