
;������ܰ
proc 1
	strvar Mid
	var Mid 0 
	set_task_var 2111 0 32 @Mid 1
	
	;�ܲ����л��͹ֵ���
	var 20 0
	set_task_var 2111 4 8 @20 1
	
	strvar time
	var time 0
	set_task_var 2111 5 8 @time 1

endproc 
proc 4
	get_task_var 2111 0 32 1 Mid 
	DEL_ID_MON @Mid 1
endproc 
proc 5
	
	get_task_var 2111 0 32 1 Mid 
	DEL_ID_MON @Mid 1
endproc 

proc 6
	strvar x y m
;	GET_PLAYER_MAPINFO 0 0 0 x y
	get_task_var 2111 0 32 1 Mid 
	
	strvar locgo
	get_task_var 2111 4 8 1 locgo
	if locgo = 1
	
		get_task_var 2111 5 8 1 time 
		add time 1
		set_task_var 2111 5 8 @time 1
		
	;	add_sys_msg_start
	;	add_sys_msg $@time$
	;	set_sys_msg_mode 1 0 0
	;	msg_info
		
		if time = 2
			MON_SPEECH @Mid 1 1 1 ǰ��ɣ˿����Ҫ����һ����������Ͽ�ȣ����Ǳ�����ͨ����
		endif
		if time = 10
			MON_SPEECH @Mid 1 1 1 ���ǲ����ߵ�̫���ˣ�
		endif
		if time = 19
			MON_SPEECH @Mid 1 1 1 �㿴����С����ϸ�ļ�ֱ�Ϳ�Ҫ�ϵ���
		endif
		if time = 28
			MON_SPEECH @Mid 1 1 1 �������б�Ϯ���־�֢�ˣ������Ǻܵ�����ʲô����ͻȻ��·�ߴܳ���
		endif
		if time = 37
			MON_SPEECH @Mid 1 1 1 �Ϳ�Ҫ���ˣ��������Ͽ�ȣ���Ҫ���ˣ�ϣ��һ·ƽ����
		endif
		if time = 46
			MON_SPEECH @Mid 1 1 1 ������̫Զ������
		endif
		if time = 55
			MON_SPEECH @Mid 1 1 1 ����ĺ���Ľ�㣬������������˺��ӣ���һ������ѧ������
		endif
		if time = 64
			MON_SPEECH @Mid 1 1 1 ��������һ����н�������
		endif
		if time = 73
			MON_SPEECH @Mid 1 1 1 �������߷�һ��֮����Ҳ�����ϵ۶��������������������˭�Ĵ�
		endif
		if time = 82
			MON_SPEECH @Mid 1 1 1 ����������һ����Ӱ��������������ǧ�ꡭȻ���������綼���ˡ�
		endif
		if time = 91
			MON_SPEECH @Mid 1 1 1 ���ǵ�����Ҳ�������������������һ�о��������
		endif
		if time = 100
			MON_SPEECH @Mid 1 1 1 �����߳������˷���ĵط��ˣ���Ըɣ˿��һ��������
		endif
		if time = 109
			MON_SPEECH @Mid 1 1 1 ���ˣ��������ɣ˿���ˣ�ǰ����λ����ʶ��
		endif
		
		GET_ID_MON_INFO @Mid 1 0 0 0 0 x y

		strvar plm px py
		GET_PLAYER_MAPINFO 1 1 1 px py
		GET_POINT_DIST @x @y @px @py plm
		
		if plm > 1000
			add_sys_msg_start
			add_sys_msg ����뻤��Ŀ����̫ܰԶ���뾡��ص���ܰ���
			set_sys_msg_mode 3 0 0
			msg_info	
		endif
		if plm > 2500
			add_sys_msg_start
			add_sys_msg �Ĵ����Ҳ�����ܰ���ټ������ˣ������ˡ�
			set_sys_msg_mode 3 0 0
			msg_info
			TASK_FAIL 2111 1
		endif
		
		GET_POINT_DIST @x @y 31300 31400 m	
		if m <= 500
			task_fini 2111 1 
		;	get_task_var 2111 0 32 1 Mid 
			DEL_ID_MON @Mid 1
			
			var time 110
			set_task_var 2111 5 8 @time 1
			
		endif
	endif
endproc 

