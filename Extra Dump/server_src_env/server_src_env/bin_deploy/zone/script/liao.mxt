

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��ϲ���е�ʶ��֪ʶ���Ҵ������ˡ����ǲ�����Σ�գ��ܹ�Ӧ��һ��ͻ��״�������Ҷ���Ȩ�����ף���������һЩ���˲����ѵ����顣��Ȼ����ؼ����ǣ���������������ĸ����ã���������û��ʲô������һ���������ָ����ѵ����ˣ�
	add_talk #b#b
	add_talk #c186506ע�⣺��������������������һ�Ρ�
	add_talk #c186506ע�⣺��ȡ����󣬽�����ʹ�á��Һ�æ������������ɡ�#n
	
	task_add_option 1651 1 101
	task_add_option 1652 1 102
	task_add_option 1653 1 103
	task_add_option 1654 1 104
	task_add_option 1655 1 105
	task_add_option 1660 1 106
	TASKCYCLE_ADD_OPTION 106 1 107
	
	
	task_add_option 1662 1 108
	task_add_option 1663 1 109
	task_add_option 1629 1 110
	task_add_option 3000 1 111
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1

	if lv >= 45 and lv < 55
		option 220 ��Ҫ������ɡ����顿ʮ���ſ�
	endif
	talk
	
endproc 

PROC 101
	task_do_option 1651 1
ENDPROC
PROC 102
	task_do_option 1652 1
ENDPROC
PROC 103
	task_do_option 1653 1
ENDPROC
PROC 104
	task_do_option 1654 1
ENDPROC
PROC 105
	task_do_option 1655 1
ENDPROC
PROC 106
	task_do_option 1660 1
ENDPROC
PROC 107
	TASKCYCLE_DO_OPTION 106 1
ENDPROC

PROC 108
	task_do_option 1662 1
ENDPROC
PROC 109
	task_do_option 1663 1
ENDPROC
PROC 110
	task_do_option 1629 1
ENDPROC
PROC 111
	task_do_option 3000 1
ENDPROC


proc 2
	get_trig_task 1 2
	if @2 = 1655
		task_fini 1655 1
	endif    
	if 2 = 1663
		
		add_item_num_inpak 5300862 5 1 0	
	endif
	if 2 = 3000
		task_fini 3000 1
	endif
endproc
proc 220
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	if lv >= 45 and lv < 55
		PLAYER_BUSY_UI 17 1
	endif
endproc 