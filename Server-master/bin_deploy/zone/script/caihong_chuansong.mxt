

proc 1
	strvar lv
	task_stat 3003 0 50

	get_player_base_att 1 2 3 4
	if 3 >= 18 
		if 50 = 3 
			transmit 4400 23725 27202 1
		else
			add_sys_msg_start
			add_sys_msg ��Ҫ��ɳ���ϵ��������ܽ��룡
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg ʵ�ڱ�Ǹ,���ĵȼ�δ�ﵽ18��,��������ʱ��������...
		set_sys_msg_mode 4 0 0
		msg_info
	endif

endproc 

