;��ʦ����ʦ��˹

proc 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���������Ĵ��ԣ����������ǿ��
		
	get_task_var 3158 0 8 1 20
	task_stat 3158 1 25
	if 20 = 0 and 25 = 2
		option 150 #����һ����ɱ����ֹ�����Ļ�
	endif
	get_pworld_var 4 8 1 31
	task_stat 3158 1 25
	
	if 31 = 3 and 25 = 2
	
		option 151 #����������ɱ����ֹ����а��
	endif
	option 152 #�ص�ʱ���

	talk

endproc

proc 150
	
	get_task_var 3158 0 8 1 20
	var 20 1
	set_task_var 3158 0 8 @20 1
	
	GET_MAP_NPC_ID 302058 50 0 
	mon_speech @50 0 2 1 ��һ�з���ȥ��ֹ�����Ļ굽�����⡣

	EFFECT_ID 0 @50 playMontion(%d,"294005")
	
		add_sys_msg_start
		add_sys_msg �����á������������������Ļ���ƶ��ٶȣ����䵽�ﰬ˹λ��ǰ�����ɱ��
		set_sys_msg_mode 1 0 0
		msg_info
endproc
 
proc 151
	
	get_task_var 3158 2 8 1 22
	var 22 1
	set_task_var 3158 2 8 @22 1	
	
	get_pworld_var 4 8 1 31
	var 31 4
	set_pworld_var 4 8 @31 1
	
	GET_MAP_NPC_ID 302058 50 0 
	mon_speech @50 0 2 1 ���һ������һ����������֤�������µ�����а���������⣬Ҫ�ɹ�������������ͷ�ԡ�
	EFFECT_ID 0 @50 playMontion(%d,"294005")
	
	
	add_sys_msg_start
	add_sys_msg �������á���˪֮������������ǡ���������а�����ﰬ˹λ��ǰ�����ɱ��
	set_sys_msg_mode 1 0 0
	msg_info
endproc

proc 152
	transmit 1000 23800 27400 1
	get_pworld_var 0 32 1 30
	var 30 0
	set_pworld_var 0 32 @30 1
	
;	RESET_PWORLD 508
	
	task_stat 3158 1 30
	if 30 != 1
		TASK_FAIL 3158 1
	endif
	
	get_pworld_var 0 32 1 30
	get_pworld_var 4 8 1 31
	get_pworld_var 5 8 1 32
	get_pworld_var 7 8 1 34
	var 30 0
	set_pworld_var 0 32 @30 1
	var 31 0
	set_pworld_var 4 8 @31 1
	var 32 0
	set_pworld_var 5 8 @32 1
	var 34 0
	set_pworld_var 7 8 @34 1
	

endproc


