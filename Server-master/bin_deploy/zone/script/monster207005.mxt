

;;;;;

proc 2
	strvar locq
	get_pworld_var 2 8 1 locq
	GET_PLAYER_MAPINFO 1 30 1 1 1 
	if locq = 0 and 30 = 51300
		strvar x y
		GET_MON_INFO 0 0 0 0 0 0 x y
		NEW_MON 207005 4 @x @y	0 0 0 0 0 
		add_sys_msg_start
		add_sys_msg һ����������������֮��˲�临����ҷ��ѳ��ĸ���
		set_sys_msg_mode 3 0 0
		map_msg_info
		
		strvar locf
		get_pworld_var 4 8 1 locf 
		;ֻ��ʾһ��
		if locf = 0
			strvar time
			get_pworld_var 5 8 1 time
			var time 1
			set_pworld_var 5 8 @time 1	

			;ֻһ��
			var locf 10
			set_pworld_var 4 8 @locf 1
			
		endif
	endif
endproc  

