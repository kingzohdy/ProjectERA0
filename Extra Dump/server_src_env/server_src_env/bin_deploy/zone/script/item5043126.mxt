;����ս     ,ʱ��֮Կ
proc 1
	; strvar enable open
	; get_world_city_enable_time 1 enable
	; if enable = 0
		; return
	; endif
	; get_world_city_open_time 1 open
	; if open > 0
		; return
	; endif
	; set_desig 1 126	
	; set_world_city_enable_time 1 0
	; strvar sec
	; get_time_sec sec
	
	; dev sec 86400
	; mul sec 86400
	; get_now_time 0 0 0 2 0 0 0
	; mul 2 -1
	; add 2 11
	; mul 2 86400
	; add 2 @sec
	; set_world_city_open_time 1 @2
	
	;;����
	; add_sys_msg_start
	; add_sys_msg ΰ���[$_Player$]������ͨ��ʱ����п����������ͨ����ʱ��ǳ�������ս����������20��00��ʽ����  
	; set_sys_msg_mode 1 0 0
	; svr_msg_info
	;;SET_EVENT_BULLETIN 1 300 30 ΰ��ġ�$_Player$��������ͨ��ʱ����п����������ͨ����ʱ��ǳ�������ս����������20��00��ʽ����
	;;Ƶ��6��ʾ
	; add_sys_msg_start
	; add_sys_msg [$_Player$]���ɹ�������ͨ��ʱ����п����ĵ�ͨ�������[�����߳ƺ�]  
	; set_sys_msg_mode 6 0 0
	; msg_info
endproc
