;�����ػ���Ŧ�    ,��Ŧ��
proc 1
	;monsay10,��Ŧ�ֱ�����
	var 10 0
	set_mon_var 0 32 @10 1
	;monhp11,��Ŧ���ϸ�tick��Ѫ��
	var 11 0
	set_mon_var 4 32 @11 1
	GET_MON_INFO 1 0 10 0 0 0 0 0 
	;monmemid10,��Ŧ��memid
	
	set_pworld_var 0 32 @10 1
	var 11 1
	;monstate11,��Ŧ��״̬ 0δˢ 1��� 2����
	
	set_pworld_var 4 32 @11 1
endproc
proc 2
	var 11 2
	;monstate11,��Ŧ��״̬ 0δˢ 1��� 2����
	
	set_pworld_var 4 32 @11 1
endproc
;tick
proc 4
	strvar hp
	GET_MON_INFO 1 0 10 hp 0 0 0 0 
	;monmemid10,��Ŧ��memid
	
	set_pworld_var 0 32 @10 1
	;monsay10,��Ŧ�ֱ�����
	var 10 0
	get_mon_var 0 32 1 10
	;monhp11,��Ŧ���ϸ�tick��Ѫ��
	var 11 0
	get_mon_var 4 32 1 11
	if 11 > @hp
		var 20 @10
		mod 20 7
		if 20 = 0
			;��ʾ
			add_sys_msg_start
			add_sys_msg $_Monster$���ܵ��з��������뾡��֧Ԯ 
			set_sys_msg_mode 14 0 0
			map_msg_info
		endif
		add 10 1
		;monsay10,��Ŧ�ֱ�����
		
		set_mon_var 0 32 @10 1
	endif
	var 11 @hp
	;monhp11,��Ŧ���ϸ�tick��Ѫ��
	
	set_mon_var 4 32 @11 1
endproc
