;;;;ʥ���10������ʾһ��



proc 1
	add_sys_msg_start
	add_sys_msg ÿ��10:00-22:00��ʥ���Ϲ�����ÿ��2Сʱ��ʱ��ǵĴ�ʥ�����¶�������ȵ��ȵ�Ŷ��
	set_sys_msg_mode 1 0 0
	SVR_MSG_INFO
	
	strvar Tim 
	var Tim 0
	set_event_var 0 32 @Tim 1
endproc 

proc 2
	get_event_var 0 32 1 Tim
	var Tim 0
	set_event_var 0 32 @Tim 1
endproc 

proc 3
	get_event_var 0 32 1 Tim
	add Tim 1
	set_event_var 0 32 @Tim 1
	
	if Tim = 60
		add_sys_msg_start
		add_sys_msg 20���Ӻ�ʥ���Ϲ�������ʱ��ǵĴ�ʥ�����¶����������·����Ҫ���ѽ��
		set_sys_msg_mode 1 0 0
		SVR_MSG_INFO
	endif
	
	if Tim = 120
		add_sys_msg_start
		add_sys_msg 10���Ӻ�ʥ���Ϲ�������ʱ��ǵĴ�ʥ�����¶����������·����Ҫ���ѽ�� 
		set_sys_msg_mode 1 0 0
		SVR_MSG_INFO
	endif
	
	if Tim = 150
		add_sys_msg_start
		add_sys_msg 5���Ӻ�ʥ���Ϲ�������ʱ��ǵĴ�ʥ�����¶����������·����Ҫ���ѽ�� 
		set_sys_msg_mode 1 0 0
		SVR_MSG_INFO	
	endif
endproc 
