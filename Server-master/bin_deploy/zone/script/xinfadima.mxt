;������ 
; Я������

proc 1
	dialog 1	
	add_talk $_npc$:
	add_talk #b#b�־�������Ϊ������ϣ�������ػ����ɣ���ǿ����ֻ�ܾ����Լ���ΰ����˲��ܾ������ˣ����ţ�ϣ���Ǽ��ö�����û׼���Ǽ���õĶ��������Ҵ�û��һ���ö��������š�
;	task_add_option 1064 1 101	
	task_add_option 1065 1 102

	task_add_option 1066 1 105 
	task_add_option 1068 1 107
	talk
endproc

proc 102
    task_do_option 1065 1
endproc

proc 105
    task_do_option 1066 1
endproc

proc 107
    task_do_option 1068 1
endproc


proc 2
	get_trig_task 1 2
	if 2 = 1068
		task_fini 1068 1
		
	;	NEW_OWNER_TMP_ITEM 1460066 10100 26500 0 0 0 307 1095 1 3600 1
	endif
endproc

