;¬����



proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b������ʲô�ˣ�������һ���ض�����Ϊ��˼����ʽ��ʱ��Ҳ����ˣ����ԣ����Ǻ������ܹ���һЩϸ���Ϸֱ����Щ���������ĸ�ʱ����Ҳ���Ը�����Ʒ��ת�����о�ʱ����ת�䡣
	
	
	task_add_option 2022 1 100
	task_add_option 2023 1 101
	task_add_option 2024 1 102

	
	
	
	talk
	
endproc 

PROC 100
	task_do_option 2022 1
ENDPROC
PROC 101
	task_do_option 2023 1
ENDPROC
PROC 102
	task_do_option 2024 1
ENDPROC


proc 2
	
	get_trig_task 1 2 
;	if 2 = 2001
;		task_fini 2001 1
;	endif
;	if 2 = 2017
;	
;	endif
endproc

