


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c676464<�����ŵ���̬������ر�������ô���ţ���һ�����صķ�ʽ���������ţ�һ��������ֻ����΢΢������ؿ���ʾ�������������ļ���>#n 

;		task_add_option 1368 1 100

		strvar Ret
		task_stat 1373 1 Ret
		if Ret = 2
			option 150 #�Ͻ�����ȥ�ɣ�

		endif
	
	talk
	
endproc 

;PROC 100
;	task_do_option 1368 1
;ENDPROC
	
	
proc 150

	APPLY_TMP_ITEM 1460077 0 1
endproc 
	
	