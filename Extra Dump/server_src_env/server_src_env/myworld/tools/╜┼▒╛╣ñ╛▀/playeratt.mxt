	; 1---�Ա�  2---�ȼ�  3---ְҵ
	;
	;CAREER_WARRIOR = 1, 
    ;CAREER_MAGICOR = 2, 
    ;CAREER_GUNER = 3, 
	
    ;GENDER_NO = 0, 
    ;GENDER_MAN = 1, 
    ;GENDER_WOMAN = 2, 
	
PROC 1	
	GET_PLAYER_BASE_ATT 4 1 2 3
		DIALOG 0
	IF 1 = 2
		ADD_TALK $_npc$:
		ADD_TALK ��ã�������$_player$�ɣ� СMM,��ӭ���� I WORLD!!!
	ELSE 
		ADD_TALK $_npc$:
		ADD_TALK ��ã�������$_player$�ɣ� С����,��ӭ���� I WORLD!!!
	ENDIF
		NEXT 10
		TALK
ENDPROC

PROC 10
	DIALOG 1
	ADD_TALK $_npc$:
	ADD_TALK �η熪�����ꆪ����ʼ������
	OPTION 50 #��Ҫ���Ӵ���
	OPTION 51 #ADD_MONEY!!!
	OPTION 52 #ADD_EXP!!!
	TALK
ENDPROC

PROC 50
	IS_CAPTAIN 1 2
	IF 2 = 0 AND 1 = 0
		;ADD_TALK #���Ƕӳ�!
		TEAM_TRANS 1002 3800 3800 1
	ELSE
		DIALOG 0
		ADD_TALK $_npc$:
		ADD_TALK #�㲻�Ƕӳ�!!�������Ӵ���!!
		TALK
	ENDIF
	
ENDPROC

PROC 51
	ADD_MONEY 10000 1
ENDPROC

PROC 52
	ADD_EXP 10000 1
ENDPROC


