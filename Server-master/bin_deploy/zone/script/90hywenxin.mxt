; ��ܰ

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b������һҹ֮�䣬������ô�ͱ����أ�����ԶҲ�޷������������޴Ȱ��ĸ�ĸͻȻ����������������������˺����Ƭ�Ĳ����ݵ����飬��Ҳ�޷���������еĸ��ܣ�
	
		task_add_option 2105 1 100
		task_add_option 2106 1 101
		task_add_option 2107 1 102
		task_add_option 2108 1 103
		task_add_option 2110 1 104
		task_add_option 2111 1 105

		strvar Ret
		TASK_STAT 2107 1 Ret
		if Ret = 2
			option 150 �Ҿ��������б�Ҫ����
		endif
		
		strvar Ret1
		TASK_STAT 2111 1 Ret1
		get_task_var 2111 4 8 1 20
		if Ret1 = 2 and 20 = 0
			option 160 #���ǳ����ɣ�
		endif
	talk
endproc

PROC 100
	task_do_option 2105 1
ENDPROC
PROC 101
	task_do_option 2106 1
ENDPROC
PROC 102
	task_do_option 2107 1
ENDPROC
PROC 103
	task_do_option 2108 1
ENDPROC
PROC 104
	task_do_option 2110 1
ENDPROC
PROC 105
	task_do_option 2111 1
ENDPROC


proc 2
	get_trig_task 1 2
	if 2 = 2107
		dialog 1
		add_talk $_npc$:
		add_talk #b#b������һҹ֮�䣬������ô�ͱ����أ�����ԶҲ�޷������������޴Ȱ��ĸ�ĸͻȻ����������������������˺����Ƭ�Ĳ����ݵ����飬��Ҳ�޷���������еĸ��ܣ�	
			option 150 �Ҿ��������б�Ҫ����
		talk
	endif
	if 2 = 2111
		dialog 1
		add_talk $_npc$:
		add_talk #b#b������һҹ֮�䣬������ô�ͱ����أ�����ԶҲ�޷������������޴Ȱ��ĸ�ĸͻȻ����������������������˺����Ƭ�Ĳ����ݵ����飬��Ҳ�޷���������еĸ��ܣ�	
			option 160 #���ǳ����ɣ�
		talk
	endif
endproc
proc 160
	;����NPC
	strvar Mid
	CALL_MON_BY_PATROL 1 254023 0 0 11300 20700 503 0 Mid
	if Mid >= 0
		get_task_var 2111 0 32 1 10
		var 10 @Mid 
		set_task_var 2111 0 32 @10 1
		
		get_task_var 2111 4 8 1 20
		var 20 1
		set_task_var 2111 4 8 @20 1
	endif
endproc


proc 150
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�ҡ�����һ��Ҫ��Ӧ�ң�$_player$���ҿ��ó���������һ�������¸ҡ�������Ϊ���е������ˣ���ͽ��ҵ�����ɥʬ��ɱ�˰ɣ�ֻ��Ҫ�㶯һ����ָͷ���Ϳ����ˣ������ˣ�$_player$��
		
		option 151 ɥʬ���㡭��
	talk
endproc 
proc 151
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�ҡ��Ҹոձ�ɥʬҧ�ˡ����б�ɥʬҧ�����ˣ�����󶼻���ɥʬ���Ҳ�������ô���ģ����������˺��ҵĵܵܣ���������ɸ�����˺�֮ǰ��ɱ���ң�
	
		option 152 ��ҧ���ͻ���ɥʬ��
	talk
endproc 
proc 152
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�ǰ���ԭ�����Ǵ�ׯ���Ի���ȥ���ԣ���ɥʬϮ������Ȼɥʬ����Ȯҧ���ˣ����Ǽ������Ȯ����ҧ���Ի���Ȼ��һ��ʮ��ʮ���١����Һ͵ܵ��ǹ¶�����������ס�ıȽ�ƫԶ�������������ѡ� 
	
		option 153 ����֪��������ʲôϮ�����Ի���
	talk
endproc 
proc 153
	dialog 1
	add_talk $_npc$:
	add_talk #b#b���Ǻ������ֻ����˵��һ���������С����֣��ڹ��컯��֮���˵����˿�ʳ�����������Ǹ��������С��Ѿ�����������ˣ�
	
		option 154 �Ǹ����е��أ�
	talk
endproc 
proc 154
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�ǾͲ�̫����ˣ���˵�Ǳ��͵�ҽԺ�����ˡ���#Name#���Ҳ�����ɥʬ�������ɳ��˵Ĺ���������˺��ҵĵܵܣ������Ӧ�Ұɣ�����û�б���֮ǰ��ɱ���ң�

		option 155 #�ܱ�Ǹ���Ҳ��ܴ�Ӧ�㣡
	talk
endproc 
proc 155
	task_fini 2107 1
	return
endproc 













