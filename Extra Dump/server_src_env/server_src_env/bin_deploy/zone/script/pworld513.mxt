

proc 1
	

	NEW_MON 207005 1 12789 12747 0 0 0 0 0 
	
	add_sys_msg_start
	add_sys_msg ��ǰ�ߣ������㿴�������й����Ϳ����뿪�����ˣ�
	set_sys_msg_mode 3 0 0
	map_msg_info
	
	strvar loc
	var loc 0
	set_pworld_var 0 8 @loc 1

	strvar locmon
	var locmon 0
	set_pworld_var 1 8 @locmon 1
	
	strvar locq
	var locq 0
	set_pworld_var 2 8 @locq 1
	
	;����״̬
	strvar locmonum
	var locmonum 0
	set_pworld_var 3 8 @locmonum 1
	
	;ֻ��ʾһ��
	strvar locf
	var locf 0
	set_pworld_var 4 8 @locf 1
	;
	strvar time
	var time 0
	set_pworld_var 5 8 @time 1
	
endproc 

proc 3

	;�����һ��������ʱ�� ֻ��ʾһ��
	get_pworld_var 4 8 1 locf 
	if locf = 10
		get_pworld_var 5 8 1 time
		add time 1
		set_pworld_var 5 8 @time 1
		if time = 3
			add_sys_msg_start
			add_sys_msg ��Ȼ���޷���������𣺱�ͣ�֣�����ɱ���������Ͼͻ������㣡
			set_sys_msg_mode 3 0 0
			map_msg_info
		
			var locf 20
			set_pworld_var 4 8 @locf 1
		endif
	endif

	strvar Num
	get_map_mon_num 0 207005 1 Num 1
	get_pworld_var 0 8 1 loc
	if loc = 0
		if Num >= 8
			;����ﵽ8ֻ����ˢ������
			get_pworld_var 1 8 1 locmon
			var locmon 10
			set_pworld_var 1 8 @locmon 1
			
			;�� ֻ��һ��
			var loc 1
			set_pworld_var 0 8 @loc 1
		endif
	endif
		
	strvar locmonum
	if Num >= 30
		;����ʧ��
		get_pworld_var 3 8 1 locmonum 
		var locmonum 1
		set_pworld_var 3 8 @locmonum 1 
		
		
	endif
	if Num = 0
		get_pworld_var 3 8 1 locmonum 
		var locmonum 2
		set_pworld_var 3 8 @locmonum 1 
		
	endif
endproc  