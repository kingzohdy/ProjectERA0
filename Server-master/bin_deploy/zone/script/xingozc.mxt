
;��մ�������

proc 1

	
	strvar Lv
	get_player_base_att 1 2 Lv 4
	
	if Lv >= 10
	
		get_role_var 0 8 1 10	;������ұ���
		GET_AUTO_FLY_INFO 50 3 1

		if 10 = 1 and 50 = 1
			transmit 1000 28154 6672 1
	                
			var 10 2
			set_role_var 0 8 @10 1

			return
		endif
	;;;;;;;;;;;;;;;;;;;;;;;;;
		get_role_var 0 8 1 10
		if 10 = 2 

			transmit 1000 28200 6500 0
		else	
			
			autofly 118 1	
			set_role_var 0 8 1 1
		endif

	else
		
		FOLLOW_CANCEL 
		add_sys_msg_start
		add_sys_msg ʵ�ڱ�Ǹ,���ĵȼ�δ�ﵽ10��,��������ʱ��������...
		set_sys_msg_mode 4 0 0
		msg_info
	
	endif
endproc

