;70����,��4�������ľ�����
proc 1
	call 100
endproc
proc 4
	call 100
endproc
proc 100
	;�߼�����boss1������,0δ������,1��һ�ű���,2��һ�ű���,3�����ű���
	get_pworld_var 80 32 1 30
	if 30 < 1
		return
	endif
	strvar x y num num2
	GET_MON_INFO 1 0 0 0 0 0 x y
	GET_MON_NUM_BY_DEFID 206313 num
	if num < 0
		return
	endif
	if num < 1
		new_mon 206313 1 @x @y 0 0 0 1 0
	endif
	GET_MON_NUM_BY_DEFID 206319 num2
	if num2 < 0
		return
	endif
	if num2 < 1
		new_mon 206319 1 @x @y 0 0 0 1 0
	endif
	if num < 1 or num2 < 1
		add_sys_msg_start
		add_sys_msg ���������������������ߣ�
		set_sys_msg_mode 1 0 0
		map_msg_info
	endif
endproc
