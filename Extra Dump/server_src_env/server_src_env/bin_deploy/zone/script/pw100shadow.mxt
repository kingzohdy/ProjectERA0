;������� ,��Ӱ��������
proc 1
	;shadow29,��Ӱ������ʾ
	var 29 0
	get_pworld_var 76 32 1 29
	if 29 > 0
		return 
	endif
	;shadow29,��Ӱ������ʾ
	var 29 1
	set_pworld_var 76 32 @29 1
	add_sys_msg_start
	add_sys_msg �������ƻ��������밵Ӱ��磬���ܽ����Ӱ���� 
	set_sys_msg_mode 1 0 0
	map_msg_info
endproc
