;;;;��������



proc 1
	dialog 1

	add_talk $_npc$:
	add_talk #b#b���˵�Ѫ���ܰ�������ҪΪ���Ǳ���ѩ�ޣ�
	task_add_option 1311 1 100
	task_add_option 1338 1 101
	task_add_option 1339 1 102
	task_add_option 1340 1 103
	task_add_option 1312 1 104
	
	strvar Ret
	task_stat 1312 1 Ret
	if Ret = 2
;		option 300 �Ƿ���Ҫ������һ�̣�
	endif
	talk
endproc 
proc 100
    task_do_option 1311 1
endproc

proc 101
    task_do_option 1338 1
endproc

proc 102
    task_do_option 1339 1
endproc
proc 103
    task_do_option 1340 1
endproc
proc 104
    task_do_option 1312 1
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
	
	NEW_OWNER_TMP_ITEM 1460060 23200 10400 0 0 0 301 1095 1 3600 1
endproc 
proc 302
	return
endproc 
proc 2
	get_trig_task 1 2
	if 2 = 1312
;		dialog 1

;		add_talk $_npc$:
;		add_talk #b#b���˵�Ѫ���ܰ�������ҪΪ���Ǳ���ѩ�ޣ�
			task_fini 1312 1
;			option 300 �Ƿ���Ҫ������һ�̣�		
;;		;;;Э��
	;	talk
	endif

endproc 



