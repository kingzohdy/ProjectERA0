proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����������������˸��ָ������鷳����һ�����û���ӹ��Ǹ���һ��ħצ��������û�뵽��������ܿ����㣬$_player$��������Ϊʲô�����أ�
		
		task_add_option 3172 1 100
		task_add_option 3173 1 101
		task_add_option 3171 1 102
		
		strvar Rac Ret
		task_stat 3401 1 Ret
		if Ret = 2 
			option 120 #�����Σ�գ��һ��ǸϽ��ؼҰɡ�
		endif
	talk
	
endproc
proc 120
	strvar Num
	GET_ITEM_NUM_INPAK 5300522 1 Num
	if Num < 1
		ADD_ITEM_NUM_INPAK 5300522 1 1 0
	endif
	DEL_NPC 304047 1
endproc 

PROC 100
	task_do_option 3172 1
ENDPROC
PROC 101
	task_do_option 3173 1
ENDPROC
PROC 102
	task_do_option 3171 1
ENDPROC
proc 2
	get_trig_task 1 2 
	if 2 = 3172
;		task_fini 3172 1
		DEL_NPC 304047 1
	endif
	if 2 = 3173
		task_fini 3173 1
	endif
endproc 