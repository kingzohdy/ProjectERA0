;�ս�
; Я������\����ͭ�Ĵ�

proc 1

	get_role_var 0 8 1 10	;������ұ���
	GET_AUTO_FLY_INFO 50 3 1
	
	if 10 = 1 and 50 = 1
		transmit 1000 28154 6672 1
                
		var 10 2
		set_role_var 0 8 @10 1

		return
	endif

	dialog 1
	add_talk $_npc$:
	add_talk #b#b����$_player$������˵���ܾ����ء�ʱ��ǵİ������׵´������������أ�����10���ˣ��Ҿͽ����͵�ʱ���ȥ���������׵´��˻��и������Ҫ������Ҫ������������أ�
	add_talk #b#b��ô������׼����ǰ��ʱ�������
	
;	task_add_option 1077 1 102
	task_add_option 1074 1 104
	task_add_option 1076 1 105
	
	strvar Ret
	TASK_STAT 1076 1 Ret
	if Ret = 1 or Ret = 3
		option 120 #ȥʱ���
	endif
	talk
endproc

proc 104
    task_do_option 1074 1
endproc
proc 105
    task_do_option 1076 1
endproc

proc 2

	get_trig_task 1 50
	if 50 = 1076
		dialog 1
		add_talk $_npc$:
		add_talk #b#b����$_player$������˵���ܾ����ء�ʱ��ǵİ������׵´������������أ�����10���ˣ��Ҿͽ����͵�ʱ���ȥ���������׵´��˻��и������Ҫ������Ҫ������������أ�
		add_talk #b#b��ô������׼����ǰ��ʱ�������	
	
			task_fini 1076 10
			option 120 #ȥʱ���
		talk
	endif
endproc

proc 120

	get_player_base_att 1 2 3 4
	
	if 3 >= 10
		get_role_var 0 8 1 10
		if 10 = 2 
			transmit 1000 28154 6672 1
		else	
			
			autofly 102 1	
			set_role_var 0 8 1 1
		endif

	else
		
		add_sys_msg_start
		add_sys_msg ʵ�ڱ�Ǹ,���ĵȼ�δ�ﵽ10��,��������ʱ��������...
		set_sys_msg_mode 4 0 0
		msg_info
		FOLLOW_CANCEL 
	endif


endproc





