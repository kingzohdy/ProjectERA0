;�뿪����3�Ĵ���Ա

PROC 1 
	
	DIALOG 1

	ADD_TALK �ҿ��������뿪����2

	OPTION 101 �뿪����2
	OPTION 102 #�뿪
    TALK               
ENDPROC
PROC 101
	DIALOG 1

	ADD_TALK $_npc$��
	ADD_TALK ��ȷ��Ҫ�뿪������

	OPTION 201 #��Ҫ�뿪
    TALK 
ENDPROC
PROC 201
	OUT_PWORLD_BY_SINGLE
ENDPROC  

PROC 102
	RETURN
ENDPROC

