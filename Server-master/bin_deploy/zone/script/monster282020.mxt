;������
proc 1
	strvar Mid Time
	var Time 0
	set_mon_var 0 8 @Time 1
	
	GET_MON_INFO 1 1 Mid 1 1 1 1 1	
	TARGET_ADD_STATUS 3 @Mid 1009 2 58 1
	TARGET_ADD_STATUS 3 @Mid 1051 2 58 111
	
	MON_SPEECH @Mid 1 2 1  ����һ�в������ںڰ������ˡ�
	

	
endproc 

proc 2
		strvar Hp Rac Loc_5
		
		get_pworld_var 19 8 1 Loc_5
		var Loc_5 1
		set_pworld_var 19 8 @Loc_5 1
		

		
		add_sys_msg_start
		add_sys_msg ��������Ҫ�������ػ����һ����������ҽ�׹���޾�����Ԩ��
		set_sys_msg_mode 3 0 0
		map_msg_info 
endproc 

proc 4
	get_mon_var 0 8 1 Time
	add Time 1
	set_mon_var 0 8 @Time 1
	
	if Time = 1

		new_mon_and_forced_move 261009 0 1 0 6100 7100 0 5600 7800
		new_mon_and_forced_move 261010 0 1 0 5700 6900 0 5200 7500
		add_sys_msg_start
		add_sys_msg ��1С�ӳ�����
		set_sys_msg_mode 3 0 0
		map_msg_info 
	endif
	if Time = 4
		new_mon_and_forced_move 261009 0 1 0 6100 7100 0 5600 7800
		new_mon_and_forced_move 261010 0 1 0 5700 6900 0 5200 7500
		                
	endif
	if Time = 7
		new_mon_and_forced_move 261009 0 1 0 6100 7100 0 5600 7800
		new_mon_and_forced_move 261010 0 1 0 5700 6900 0 5200 7500
		add_sys_msg_start
		add_sys_msg ��2С�ӳ�����
		set_sys_msg_mode 3 0 0
		map_msg_info 
	endif
	if Time = 10
		new_mon_and_forced_move 261009 0 1 0 6100 7100 0 5600 7800
		new_mon_and_forced_move 261010 0 1 0 5700 6900 0 5200 7500
		                
	endif
	if Time = 13
		new_mon_and_forced_move 261009 0 1 0 6100 7100 0 5600 7800
		new_mon_and_forced_move 261010 0 1 0 5700 6900 0 5200 7500
		add_sys_msg_start
		add_sys_msg ��3С�ӳ�����
		set_sys_msg_mode 3 0 0
		map_msg_info 
	endif
	if Time = 16
		new_mon_and_forced_move 261009 0 1 0 6100 7100 0 5600 7800
		new_mon_and_forced_move 261010 0 1 0 5700 6900 0 5200 7500
		                
	endif
	if Time = 19
		new_mon_and_forced_move 261009 0 1 0 6100 7100 0 5600 7800
		new_mon_and_forced_move 261010 0 1 0 5700 6900 0 5200 7500
		add_sys_msg_start
		add_sys_msg ��4С�ӳ�����
		set_sys_msg_mode 3 0 0
		map_msg_info 
	endif
	if Time = 22
		new_mon_and_forced_move 261009 0 1 0 6100 7100 0 5600 7800
		new_mon_and_forced_move 261010 0 1 0 5700 6900 0 5200 7500	
	                   
	endif
	if Time = 25
		new_mon_and_forced_move 261009 0 1 0 6100 7100 0 5600 7800
		new_mon_and_forced_move 261010 0 1 0 5700 6900 0 5200 7500
		add_sys_msg_start
		add_sys_msg ��5С�ӳ�����
		set_sys_msg_mode 3 0 0
		map_msg_info 		
	endif
	if Time = 28
		new_mon_and_forced_move 261009 0 1 0 6100 7100 0 5600 7800
		new_mon_and_forced_move 261010 0 1 0 5700 6900 0 5200 7500
		                
	endif
	if Time > 28
		var Time 30
		set_mon_var 0 8 @Time 1
		
	endif
endproc 







