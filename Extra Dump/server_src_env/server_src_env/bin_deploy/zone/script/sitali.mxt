

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�ܶ�ʱ������������һ�����Ϳ�һ�����ġ��֣������޷��Ƕ���������ͷ�ԺͶĵ���������֮�������������������Ϊʱ�˲��ã������˰��ݶ�˹���ѡ�
	
	task_add_option 1593 1 101
	task_add_option 1594 1 102
	task_add_option 1595 1 103
	task_add_option 1596 1 104
	task_add_option 1597 1 105
	
	task_add_option 1555 1 106

	strvar Ret
	task_stat 1503 1 Ret
	if Ret = 2
		option 150 ˹������������˵�����ң�
	endif

	talk
endproc 


PROC 101
	task_do_option 1593 1
ENDPROC
PROC 102
	task_do_option 1594 1
ENDPROC
PROC 103
	task_do_option 1595 1
ENDPROC
PROC 104
	task_do_option 1596 1
ENDPROC
PROC 105
	task_do_option 1597 1
ENDPROC
PROC 106
	task_do_option 1555 1
ENDPROC
proc 150
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b$_player$������������̫���ˣ��������кܶ�ܶ�����Ҫ��������һλ�����������ǡ��侲���¸ҡ�Ϭ���ĺð��֡�������̫�����˷��ˣ��˷ܵ��Ҷ�����Ӧ���Ȱ��ļ��½������������������룬������һ�룬���Һú���һ�롭��	
	task_fini 1503 1
		option 151 #�����������룬�Ҳ��ż���
	talk
endproc 
proc 151
	return
endproc 
proc 2
	get_trig_task 1 2
	if 2 = 1597
;		GET_MAP_MON_NUM 2300 293067 0 90 0

	endif 
endproc
