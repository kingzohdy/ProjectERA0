
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����������ʱ���Ѿ������ˣ����������ǰ�����ײ�����鿴���а��������������Ļ�����������ȡ��������Ӵ��
    add_talk #b#b
	add_talk #b#b ϣ����һ��ĺ���������ܹ������������װ���$_player$��

	  option 200 #�ص�����
	talk
endproc 

PROC 200
	;����
	TRANSMIT 4403 24200 31800 0
	DEL_ROLE_TMP_ITEM 0 0
ENDPROC

