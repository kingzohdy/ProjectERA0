;npc����

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�������·�ˣ��ҿ�������ذ��ϣ�
    add_talk #b#b
	add_talk #b#b������������������������ǰ�У�
	
	option 200 #�ص�����
		
	talk
endproc 


PROC 200
	;����
	TRANSMIT 4403 24200 31800 0
	DEL_ROLE_TMP_ITEM 0 0
ENDPROC
