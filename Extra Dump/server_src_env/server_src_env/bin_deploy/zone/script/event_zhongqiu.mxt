;����  �¼�ID��99   �����ID��58

PROC 1
	call 100
	NEW_MAP_LIFE_NPC 1 1000 302170 27655 31468 158 0 npc302170.mac
	NEW_MAP_LIFE_NPC 1 1000 302172 27421 30644 158 0 npc302172.mac
	NEW_MAP_LIFE_NPC 1 1000 302173 27409 32226 158 0 npc302173.mac
ENDPROC

PROC 2
    call 100
	add_sys_msg_start
	add_sys_msg �����Ѿ�Բ��������
	set_sys_msg_mode 6 0 0
	svr_msg_info
ENDPROC

;10�봥��һ��
PROC 3	
	
ENDPROC


PROC 100
    GET_MAP_INST 1000 10
	del_npc 302170 0
	del_npc 302172 0
	del_npc 302173 0
ENDPROC
