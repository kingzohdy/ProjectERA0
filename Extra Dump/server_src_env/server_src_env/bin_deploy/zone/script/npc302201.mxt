;302201	��¶���


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�����κڰ�����ڴ������������Ժ�����ƽ������Ͼ���
	add_talk #b#b#Name#����ʱ������ս������ʱһ��������
	
	task_add_option 3104 1 102
	
	TASK_STAT 3104 10 11
	if @10 = 0 and @11 = 2
		task_fini 3104 500
	endif
	
	talk
endproc 

proc 102
    task_do_option 3104 1
endproc

