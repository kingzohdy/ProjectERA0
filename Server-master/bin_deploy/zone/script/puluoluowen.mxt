
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b������������ģ�Ҳ�����ǿ�ģ��������ǿ�����ζ�ġ������ʤ����Ҳ����ʧ�ܣ����㲻��������
		
	
		TASKCYCLE_ADD_OPTION 109 1 140
	talk
endproc 
proc 140
	TASKCYCLE_DO_OPTION 109 1
endproc

