;;;
;l;
proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#bԭ����Ϊ����ƫƧ�ĵط��ܹ����һʱ������˭�ϵ���������������������

		task_add_option 1853 1 100
		task_add_option 1854 1 101
		task_add_option 1855 1 102
	talk

endproc
PROC 100
	task_do_option 1853 1
ENDPROC
PROC 101
	task_do_option 1854 1
ENDPROC
PROC 102
	task_do_option 1855 1
ENDPROC

