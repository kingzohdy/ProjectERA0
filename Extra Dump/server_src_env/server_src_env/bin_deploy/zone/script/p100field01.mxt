;������� ,��boss�߼�����
proc 1
;	add_sys_msg_start
;	add_sys_msg �߼���
;	set_sys_msg_mode 4 0 0
;	map_msg_info
	;�߼�����1���,0δ������,1������
	get_pworld_var 28 32 1 17
	if 17 = 1
		return
	endif
	VAR 17 1
	set_pworld_var 28 32 @17 1
	;��boss
	new_mon 263005 1 17675 5007 0 0 0 1 2
endproc
