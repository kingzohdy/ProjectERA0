;70����,�����߼�����ʾ
proc 1
	is_pworld_over 1
	if 1 = 1
		return
	endif
	;roadtipflag31,������ʾ�߼������,0δ������,1����
	var 31 0
	get_pworld_var 84 32 1 31
	if 31 > 0
		return
	endif
	add 31 1
	;roadtipflag31,������ʾ�߼������,0δ������,1����
	var 31 @31;
	set_pworld_var 84 32 @31 1
	add_sys_msg_start
	add_sys_msg С�ģ����ﴦ��͸¶��Σ�յ���Ϣ 
	set_sys_msg_mode 3 0 0
	map_msg_info
endproc
