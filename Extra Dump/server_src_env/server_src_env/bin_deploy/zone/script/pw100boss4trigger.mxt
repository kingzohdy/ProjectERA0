;������� ,boss ��������
proc 1
	;shadow32,boss��Ӱ������ʾ
	var 32 0
	get_pworld_var 88 32 1 32
	if 32 > 0
		return
	endif
	;pworld33,�Ƿ�pworldover��,1����û����,0������
	var 33 0
	get_pworld_var 92 32 1 33
	if 33 = 0
		return
	endif
	;��������ؼ���
	get_pworld_var 8 32 1 12
	if 12 = 0
		add_sys_msg_start
		add_sys_msg �ڰ���̳����Ӱ���ϱ����ţ�����ǰ�������ڵ��ƻ���� 
		set_sys_msg_mode 1 0 0
		map_msg_info
		return
	endif
	if 12 = 1
		add_sys_msg_start
		add_sys_msg �����������������ƻ�������ȫ�����Ӱ���� 
		set_sys_msg_mode 1 0 0
		map_msg_info
		return
	endif 
	if 12 = 2
		add_sys_msg_start
		add_sys_msg �����ʣ�µĽ���ƻ�������ȫ�����Ӱ���� 
		set_sys_msg_mode 1 0 0
		map_msg_info
		return
	endif
	if 12 = 3
		add_sys_msg_start
		add_sys_msg �������Ѱ��Ͳ�ס��������źڰ���̳�� 
		set_sys_msg_mode 1 0 0
		map_msg_info
		DELETE_MON_BY_DEFID 261058 1
		add 12 1
		set_pworld_var 8 32 @12 1	
		DELETE_MON_BY_DEFID 294031 1
		var 32 1
		;shadow32,boss��Ӱ������ʾ
		
		set_pworld_var 88 32 @32 1
	endif
endproc
