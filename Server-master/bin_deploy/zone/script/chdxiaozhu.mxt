
;С��
proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b555555555555555555555����ͬ�����ˣ����զ����ô���أ�
	add_talk #b#b<����ô�ˣ�>
	add_talk #b#b#059#059#059#059#059#059���������������һ���ɶ��PK��ɱ����55555����ɱ���Һö�ΰ���
	add_talk #b#b<����Ϊʲô�������أ�>
	add_talk #b#b#047#047��Ҫ����PK���������������Ҫ���ұ����Һø�����PK��ȥ����ѽ��

	strvar ret
	task_stat 30012 1 ret
	if ret = 2
		option 150 #������PK��ķ�λ�ɣ������㱨��
	endif
	talk
endproc
proc 150
	
	strvar num
	GET_MAP_MON_NUM 4400 241008 1 num 1
;	GET_MON_NUM 1 num
	if num = 0
	;	NEW_MON 241008 1 43100 37800 0 0 0 0 0
		strvar lv
		GET_PLAYER_BASE_ATT 1 1 lv 1
		
		NEW_LIFE_DYN_MON 1 241008 1 40000 38100 0 0 0 0 0 @lv 0 180
		add_sys_msg_start
		add_sys_msg pk���ڶ��߲�Զ�����֣�
		set_sys_msg_mode 3 0 0
		msg_info
	else
		add_sys_msg_start
		add_sys_msg pk���Ѿ��ڶ��߲�Զ���Ⱥ��ʱ�ˡ���������
		set_sys_msg_mode 3 0 0
		msg_info
	
	endif
endproc 

