;������鱦��

proc 2

	GET_MON_INFO 0 11 0 0 0 0 13 14
	RAND 12 5
		if 12 = 0
			add_sys_msg_start
			add_sys_msg $_player$�������䣬����������һ����ŭ�ĹŴ��Ƕ�ʿ����
			set_sys_msg_mode 4 0 0
			map_msg_info
			NEW_LIFE_MON 283014 1 @13 @14 0 0 0 0 30 1 19
			TARGET_ADD_STATUS 3 @19 1095 1 7200 2
		else
			mul 12 3
			set_pworld_var 8 32 @12 1
			ADD_PWORLD_RESOURCE 0 0 @12 2
			add_sys_msg_start
			add_sys_msg $_player$�������䣬���$@12$ˮ��
			set_sys_msg_mode 4 0 0
			map_msg_info
		endif
	;��������1/5���ʿ���boss��4/5���ʻ��ˮ��
endproc

