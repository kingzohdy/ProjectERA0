;;;;ʥ���



proc 1
	strvar Tim Loc
	var Tim 0
	set_event_var 0 32 @Tim 1
	
	; add_sys_msg_start
	; add_sys_msg ʥ���Ϲ����Ѿ�������ѷŵ�ʱ��ǵĴ�ʥ������������ҿ�����ѽ��
	; set_sys_msg_mode 6 0 0
	; SVR_MSG_INFO
	
endproc 

proc 2
	add_sys_msg_start
	add_sys_msg ����ʥ���Ϲ����������Ѿ�����������ף���ʥ���ڿ��֣�
	set_sys_msg_mode 1 0 0
	SVR_MSG_INFO
endproc 

proc 3

	;;;;10�㿪ʼˢ����
	get_event_var 0 32 1 Tim
	add Tim 1
	set_event_var 0 32 @Tim 1
	
	if Tim = 1
		add_sys_msg_start
		add_sys_msg ʥ���Ϲ����Ѿ�������ѷŵ�ʱ��ǵĴ�ʥ������������ҿ�����ѽ��
		set_sys_msg_mode 1 0 0
		SVR_MSG_INFO
		; ˢ����	
		call 110
	endif
	
	; if Tim = 630
		; add_sys_msg_start
		; add_sys_msg 10���Ӻ�ʥ���Ϲ�������ʱ��ǵĴ�ʥ�����¶����������·����Ҫ���ѽ�� 
		; set_sys_msg_mode 1 0 0
		; SVR_MSG_INFO
	; endif
	; if Tim = 690
		; add_sys_msg_start
		; add_sys_msg 5���Ӻ�ʥ���Ϲ�������ʱ��ǵĴ�ʥ�����¶����������·����Ҫ���ѽ�� 
		; set_sys_msg_mode 1 0 0
		; SVR_MSG_INFO
	; endif
	
	if Tim >= 720
		var Tim 0
		set_event_var 0 32 @Tim 1
	endif

endproc 


proc 110
	 ;ˢ����
	NEW_MAP_MON 352101 20 1000 29504 33330 800 1 0 -1 600 0 0
	NEW_MAP_MON 352101 20 1000 38847 34238 800 0 0 -1 600 0 0
	NEW_MAP_MON 352101 20 1000 21414 46004 800 0 0 -1 600 0 0
	NEW_MAP_MON 352101 20 1000 29448 29696 800 0 0 -1 600 0 0
	NEW_MAP_MON 352101 20 1000 23301 29856 800 0 0 -1 600 0 0
	NEW_MAP_MON 352101 20 1000 23349 33149 800 0 0 -1 600 0 0
	NEW_MAP_MON 352101 20 1000 38795 29661 800 0 0 -1 600 0 0
	NEW_MAP_MON 352101 20 1000 37960 32411 800 0 0 -1 600 0 0
	NEW_MAP_MON 352101 20 1000 39694 31108 800 0 0 -1 600 0 0
	NEW_MAP_MON 352101 20 1000 38432 43607 800 0 0 -1 600 0 0
endproc



