

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���ݶ�˹��ʲô���£���ֻӵ����СС�ĵ���֮�أ�����׷���ߣ�������ӵ�л�ɳ�������ǧǧ��ȴ���������������������Կ���
	task_add_option 1553 1 101
	task_add_option 1554 1 102



	talk
endproc 


PROC 101
	task_do_option 1553 1
ENDPROC
PROC 102
	task_do_option 1554 1
ENDPROC



;proc 2
;	get_trig_task 1 2
;	if 2 = 1550
;		task_fini 1550 1
;	endif 
;endproc
