proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#bͭ�Ĵ��������������ǵģ���Ҫ�߾�����ȥ��������
	task_add_option 1304 1 101
	task_add_option 1305 1 102
	task_add_option 1306 1 103 

	strvar Ret
	task_stat 1306 1 Ret
	if Ret = 2
;		option 300 �Ƿ���Ҫ������һ�̣�
	endif

	talk
endproc

proc 101
    task_do_option 1304 1
endproc
proc 102
    task_do_option 1305 1
endproc
proc 103
    task_do_option 1306 1
endproc
proc 300
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�ҿ���ֱ�ӽ����͵�Ŀ�ĵأ�������;����ͣӴ��
	
		option 301 #��ֱ�ӽ����͵�Ŀ�ĵذɣ�
		option 302 #�����������Լ���ȥ��
	talk

endproc 
proc 301
	
	NEW_OWNER_TMP_ITEM 1460059 8700 8400 0 0 0 300 1095 1 3600 10
endproc 
proc 302
	return
endproc 
proc 2
	get_trig_task 1 600

	if 600 = 1306
;		dialog 1
;		add_talk $_npc$:
;		add_talk #b#bͭ�Ĵ��������������ǵģ���Ҫ�߾�����ȥ��������
		task_fini 1306 1
;		option 300 �Ƿ���Ҫ������һ�̣�
;		talk
		;Э��
	
	endif

endproc 
