;����ս     ,ʱ�������ս�¼�
;��Ҫÿ�������¼��ű�
proc 1
	;tipcount,��ʾ����
	var 10 0
	set_event_var 0 32 @10 1
	;tiptick,��ʾ��ʱ
	var 11 1
	set_event_var 4 32 @11 1
	strvar p sec
	var p 201 
	;GET_NOW_TIME      #�� #�� #���µڼ������� #���ڼ� #ʱ #�� #��
	get_now_time 0 0 0 1 2 3 4
	
	var 13 0
	if 1 = 6
		var 13 1
	endif
	;sat,�Ƿ�����
	
	set_event_var 12 32 @13 1
	;tipproc,��ʾproc
	var 12 0
	set_event_var 8 32 @12 1
	call 100
	call 200
endproc
proc 2
	call 209
endproc
;tick,10��һ��
proc 3
	call 200
	call 100
endproc
proc 100
	strvar kill enable open
	
	get_world_boss_first_kill_time 1 kill
	if kill = 0
		return
	endif
	
	get_world_city_enable_time 1 enable
	if enable = 0
		return
	endif
	get_world_city_open_time 1 open
	if open > 0
		return
	endif
	strvar sec
	get_time_sec sec
	if sec >= @enable
		;set_world_city_enable_time 1 0
		strvar sec
		get_time_sec sec
		
		dev sec 86400
		mul sec 86400
		get_now_time 0 0 0 2 0 0 0
		mul 2 -1
		add 2 11
		mul 2 86400
		add 2 @sec
		set_world_city_open_time 1 @2
		
		add_sys_msg_start
		add_sys_msg ͨ��ʱ����п����������ͨ���Ѿ�������ʱ��ǳ�������ս����������20��00��ʽ����  
		set_sys_msg_mode 1 0 0
		svr_msg_info
;		SET_EVENT_BULLETIN 1 300 30 ΰ��ġ�$_Player$��ͨ��ʱ����п����������ͨ���Ѿ�������ʱ��ǳ�������ս����������20��00��ʽ����
	endif
	
endproc
proc 200
	;sat,�Ƿ�����
	var 13 0
	get_event_var 12 32 1 13
	if 13 != 1
		return
	endif
	strvar open sec p endtime
	get_world_city_open_time 1 open
	if open = 0
		return
	endif
	
	is_same_week @open 1
	if 1 = 0
		return
	endif
	
	get_time_sec 13
	if 13 <= @open
		return
	endif
;	add_sys_msg_start
;	add_sys_msg $@13$
;	set_sys_msg_mode 3 0 0
;	map_msg_info
	mod 13 86400
;	add_sys_msg_start
;	add_sys_msg $@13$
;	set_sys_msg_mode 3 0 0
;	map_msg_info
	;39600 ����19��
	sub 13 39600
;	add_sys_msg_start
;	add_sys_msg $@13$
;	set_sys_msg_mode 3 0 0
;	map_msg_info
	dev 13 10
;	add_sys_msg_start
;	add_sys_msg $@13$
;	set_sys_msg_mode 3 0 0
;	map_msg_info
	var 11 @13
	;tiptick,��ʾ��ʱ
	var 11 @13
	set_event_var 4 32 @11 1
	
;	add_sys_msg_start
;	add_sys_msg $@11$
;	set_sys_msg_mode 3 0 0
;	map_msg_info
	
	;����Сʱ 10800 /10
	var endtime 900
	var 15 @endtime
	
	;tipproc,��ʾproc
	var 12 0
	get_event_var 8 32 1 12
	
	add 15 10
	if 11 >= @15
		return
	endif
	
	var 15 @endtime
	if 11 >= @15 and 12 < 209
		call 209
		;tipproc,��ʾproc
		var 12 209
		set_event_var 8 32 @12 1
		return
	endif
	var 15 @endtime
	sub 15 30
	if 11 >= @15 and 12 < 208
		call 208
		;tipproc,��ʾproc
		var 12 208
		set_event_var 8 32 @12 1
		return
	endif
	var 15 @endtime
	sub 15 60
	if 11 >= @15 and 12 < 207
		call 207
		;tipproc,��ʾproc
		var 12 207
		set_event_var 8 32 @12 1
		return
	endif
	
	var 15 @endtime
	sub 15 180
	if 11 >= @15 and 12 < 206
		call 206
		;tipproc,��ʾproc
		var 12 206
		set_event_var 8 32 @12 1
		return
	endif
	if 11 >= 360 and 12 < 205
		call 205
		;tipproc,��ʾproc
		var 12 205
		set_event_var 8 32 @12 1
		return
	endif
	if 11 >= 330 and 12 < 204
		call 204
		;tipproc,��ʾproc
		var 12 204
		set_event_var 8 32 @12 1
		return
	endif
	if 11 >= 300 and 12 < 203
		call 203
		;tipproc,��ʾproc
		var 12 203
		set_event_var 8 32 @12 1
		return
	endif
	if 11 >= 180 and 12 < 202
		call 202
		;tipproc,��ʾproc
		var 12 202
		set_event_var 8 32 @12 1
		return
	endif
	if 11 >= 0 and 12 < 201
		call 201
		;tipproc,��ʾproc
		var 12 201
		set_event_var 8 32 @12 1
		return
	endif
endproc
proc 201
	add_sys_msg_start
	add_sys_msg ʱ��ǳ�������ս����1��Сʱ����ʽ������  
	set_sys_msg_mode 1 0 0
	svr_msg_info
endproc
proc 202
	add_sys_msg_start
	add_sys_msg ʱ��ǳ�������ս����30���Ӻ���ʽ������  
	set_sys_msg_mode 1 0 0
	svr_msg_info
endproc
proc 203
	add_sys_msg_start
	add_sys_msg ʱ��ǳ�������ս����10���Ӻ���ʽ������  
	set_sys_msg_mode 1 0 0
	svr_msg_info
endproc
proc 204
	add_sys_msg_start
	add_sys_msg ʱ��ǳ�������ս����5���Ӻ���ʽ������  
	set_sys_msg_mode 1 0 0
	svr_msg_info
endproc
proc 205
;	add_sys_msg_start
;	add_sys_msg ʱ��ǳ�������ս��ʽ������  
;	set_sys_msg_mode 1 0 0
;	svr_msg_info
endproc
proc 206
	is_in_siege_city 0 2
	if 2 != 1
		return
	endif
	add_sys_msg_start
	add_sys_msg ʱ�������ս������30���Ӻ����  
	set_sys_msg_mode 1 0 0
	svr_msg_info
endproc
proc 207
	is_in_siege_city 0 2
	if 2 != 1
		return
	endif
	add_sys_msg_start
	add_sys_msg ʱ�������ս������10���Ӻ����  
	set_sys_msg_mode 1 0 0
	svr_msg_info
endproc
proc 208
	is_in_siege_city 0 2
	if 2 != 1
		return
	endif
	add_sys_msg_start
	add_sys_msg ʱ�������ս������5���Ӻ����  
	set_sys_msg_mode 1 0 0
	svr_msg_info
endproc
proc 209
	
	;_var_set(tiptick,0)//��ʾ��ʱ
endproc
