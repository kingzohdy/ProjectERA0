;������ʿ
proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#bʿ��ɱ�������裡ֻҪ�����Ž�һ�ģ���û��սʤ���˵ĵ��ˡ�
	task_add_option 1326 430 30
;	task_add_option 1327 430 31
;	task_add_option 4035 430 32
	talk
	
endproc

proc 30
	
	task_do_option 1326 430
	
endproc


