;����ѧϰԱ

proc 1
	dialog 1
	
	add_talk $_npc$:
	add_talk #b#bǰ������η�ڰ���Ү�����ʿ��������������Ү�����ʿ��������𣬳���ڰ�����ì�𺳣����Ʒ��顭��ֱ��̫������
	
	task_add_option 1319 1 101	
	task_add_option 1320 1 102
	task_add_option 1321 1 103
	task_add_option 1322 1 104

	OPTION 150 #ѧϰ����
		
	talk
endproc

proc 101
    task_do_option 1319 1
endproc
proc 102
    task_do_option 1320 1
endproc
proc 103
    task_do_option 1321 1
endproc
proc 104
    task_do_option 1322 1
endproc


proc 150
    OPEN_UI 1
endproc
proc 2
	get_trig_task 1 600
	if 600 = 1322
			ADD_ITEM_NUM_INPAK 5300388 1 1 0
			ADD_ITEM_NUM_INPAK 5300390 1 1 0
			ADD_ITEM_NUM_INPAK 5300391 1 1 0
	endif
ENDPROC














