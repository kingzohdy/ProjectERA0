; ����


proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b���������ˣ�Ҫ��ô�ı��أ�
	add_talk #b#b�������޷����ı䣬������ֻ������
	
		task_add_option 2124 1 100
		
	
	talk
endproc

PROC 100
	task_do_option 2124 1
ENDPROC


proc 2
;	get_trig_task 1 2
;	if 2 = 2124
;		task_fini 2124 1
;	endif
endproc






