;������������

proc 1
	dialog 1
	add_talk ������ϣ�
	add_talk #b#b����ʯͷ������һ������ѭ���Ĺ��̣�����ɱ����֮�󣬾ͻ�������һ����Ҳ����˵������������Ŭ�������ǵ������ǲ�����ģ�
	add_talk #b#b
	add_talk �ܱ����
	add_talk #b#b#c186506ʯͷ �� ����
	add_talk #b#b���� �� ����
	add_talk #b#b���� �� ʯͷ#n
	add_talk ʤ��������
	add_talk #b#b����ʱ����ʱ������֮��Ĺ���#c186506����ͬһ����#n
	
	get_task_var 4614 1 8 1 10
	get_task_var 4614 2 8 1 11
	get_task_var 4614 3 8 1 12
	if 10 = 0 and 11 = 0 and 12 = 0
		add_talk #b#b
		add_talk ����Ҫ�󣺿�ʼ������ҽ�����������Ҫ��
	elseif 10 = 1
		add_talk #b#b
		add_talk ����Ҫ��������еļ���ʯͷ��������Ϊ#c005ebe����#n
	elseif 11 = 1
		add_talk #b#b
		add_talk ����Ҫ��������еļ���ʯͷ����������Ϊ#c005ebeʯͷ#n
	elseif 12 = 1
		add_talk #b#b
		add_talk ����Ҫ��������еļ���ʯͷ����������Ϊ#c005ebe����#n
	else
	endif
	
	strvar ret
	task_stat 4614 1 ret
	if ret = 2
		option 100 #��ʼ����
		option 200 #�������
	endif
		option 300 #ллָ��
	
	talk
endproc

proc 100
	strvar num ran
	GET_MON_NUM 0 num
	
	if num <= 0
		NEW_MON 230056 1 12822 12737 1500 0 0 0 0
		NEW_MON 230057 1 12822 12737 1500 0 0 0 0
		NEW_MON 230058 1 12822 12737 1500 0 0 0 0
		
		add_sys_msg_start
		add_sys_msg ���ܳ����˷�����˼��ʯͷ��������
		set_sys_msg_mode 4 0 0
		msg_info
		
		rand ran 2
		switch @ran
			case 0
				get_task_var 4614 1 8 0 10
				VAR 10 1
				set_task_var 4614 1 8 @10 0
				call 500
			endcase
			case 1
				get_task_var 4614 2 8 0 11
				VAR 11 1
				set_task_var 4614 2 8 @11 0
				call 500
			endcase
			case 2
				get_task_var 4614 3 8 0 12
				VAR 12 1
				set_task_var 4614 3 8 @12 0
				call 500
			endcase
		endswitch
	else
		add_sys_msg_start
		add_sys_msg ���Ȱ����ڵļ���ʯͷ�������
		set_sys_msg_mode 4 0 0
		msg_info
	endif
	call 1
endproc

proc 200
	call 502
endproc

proc 300
;�ر����
endproc


proc 500
	get_task_var 4614 0 8 0 50
	VAR 50 1
	set_task_var 4614 0 8 @50 0
endproc

proc 502
	strvar nMon_1 nMon_2 nMon_3
	GET_MAP_MON_NUM 51400 230056 0 nMon_1 0
	GET_MAP_MON_NUM 51400 230057 0 nMon_2 0
	GET_MAP_MON_NUM 51400 230058 0 nMon_3 0
	
	GET_TASK_VAR 4614 1 8 0 10
	if @10 = 1 and @nMon_1 >= 30
		TASK_FINI 4614 0
	else
		add_sys_msg_start
		add_sys_msg ������еļ���ʯͷ��������Ϊ����
		set_sys_msg_mode 3 0 0
		msg_info
	endif
	
	GET_TASK_VAR 4614 2 8 0 11
	if @11 = 1 and @nMon_2 >= 30
		TASK_FINI 4614 0
	else
		add_sys_msg_start
		add_sys_msg ������еļ���ʯͷ��������Ϊʯͷ
		set_sys_msg_mode 3 0 0
		msg_info
	endif
	
	GET_TASK_VAR 4614 3 8 0 12
	if @12 = 1 and @nMon_3 >= 30
		TASK_FINI 4614 0
	else
		add_sys_msg_start
		add_sys_msg ������еļ���ʯͷ��������Ϊ����
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc
