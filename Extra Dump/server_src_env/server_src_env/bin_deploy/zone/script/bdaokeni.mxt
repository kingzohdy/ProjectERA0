


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��һ��������������������ʲô��
	add_talk #b#b���ܱ��Ҹ�����
	add_talk #b#b�����ˣ������ˣ������ˣ�������ˣ�Ψһʣ�µģ��������������ˡ���Ϊʲô���������˲�����

		task_add_option 1351 1 100
		task_add_option 1352 1 101
		task_add_option 1353 1 102
		task_add_option 1354 1 103
		task_add_option 1355 1 104
	strvar Ret
	task_stat 1355 0 Ret
	if Ret = 2
		option 150 #�ٸ���һ�λ���ɣ����һ���͵���

	endif
	talk
	
endproc 

PROC 100
	task_do_option 1351 1
ENDPROC
PROC 101
	task_do_option 1352 1
ENDPROC
PROC 102
	task_do_option 1353 1
ENDPROC
PROC 103
	task_do_option 1354 1
ENDPROC
PROC 104
	task_do_option 1355 1
ENDPROC
proc 150

;	NEW_OWNER_TMP_ITEM 1460078 46900 5700 0 0 0 0 0 0 0 1
		APPLY_TMP_ITEM 1460078 0 1
	
endproc 
proc 2
	get_trig_task 1 2
	if 2 = 1355
		call 150
;		NEW_OWNER_TMP_ITEM 1460060 9800 8800 0 0 0 184 1095 1 3600 1
	endif

endproc 
	
	