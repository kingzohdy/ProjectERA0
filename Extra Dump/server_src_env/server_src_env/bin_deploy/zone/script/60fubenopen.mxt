

proc 1
	dialog 1
	add_talk $_npc$:
	strvar lv
	GET_ID_PLAYER_INFO 0 8 lv
	if lv < 60
		add_talk #b#b�㻹̫С��$_player$���Ҳ�����������������Ц��ɭ�ֽ���Σ�գ�����תְ��60�������ɣ�
	
	else
		add_talk #b#b $_player$������������̫���ˣ��������˵������Ӣ���¼�����ξ����ж����������Ȼ�������ɣ�
		add_talk #b#b���ǵ�ս���Ѿ�����������ˣ������������ܽ�����ϸ�����
	
		option 150 #����ɭ�ֽ��
	endif
	
	task_add_option 1901 1 110
	task_add_option 1830 1 111
	task_add_option 1834 1 112
	task_add_option 1840 1 113
	task_add_option 1835 1 114
	
	
	
	talk
	
endproc 
proc 150
	open_pworld_ui 4

endproc

PROC 110
	task_do_option 1901 1
ENDPROC
PROC 111
	task_do_option 1830 1
ENDPROC
PROC 112
	task_do_option 1834 1
ENDPROC
PROC 113
	task_do_option 1840 1
ENDPROC
PROC 114
	task_do_option 1835 1
ENDPROC

proc 2
	get_trig_task 1 2
	if 2 = 1901
		task_fini 1901 1
	endif

endproc








