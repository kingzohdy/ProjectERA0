; Ƥ��ϣ

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�������У������˶�����ͬһ�װ��ĸ裬����̲�֮��Ц����������ͨ�õ����ԣ�Ȼ������������ʱ���֣������ϵ���������Ҫ�������ˡ����ǲ����Ҵ����ʲô������ô����Ҫ�������ˣ�ȴ�������Ԯ��֮�֡���
	
		task_add_option 2101 1 100
		task_add_option 2102 1 101
		task_add_option 2103 1 102
		task_add_option 2104 1 103
		task_add_option 2105 1 104

	talk
endproc

PROC 100
	task_do_option 2101 1
ENDPROC
PROC 101
	task_do_option 2102 1
ENDPROC
PROC 102
	task_do_option 2103 1
ENDPROC

PROC 103
	task_do_option 2104 1
ENDPROC

PROC 104
	task_do_option 2105 1
ENDPROC



proc 2
	get_trig_task 1 2
	if 2 = 2104
		
		ADD_ITEM_NUM_INPAK 5300891 1 1 0
		ADD_ITEM_NUM_INPAK 5300892 1 1 0
	endif
	if 2 = 2105
		task_fini 2105 1
	endif

endproc
