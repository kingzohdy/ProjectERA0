;70����,������⻷2�ű�
proc 1
	var 10 0
	set_mon_var 0 32 @10 1
endproc
;�����ű�
proc 2
	;ɾ�컷
	delete_mon_by_defid 284049 1
	delete_mon_by_defid 284088 1
	delete_mon_by_defid 284089 1
	delete_mon_by_defid 284090 1
	delete_mon_by_defid 284091 1
	delete_mon_by_defid 284092 1
	delete_mon_by_defid 284093 1
	delete_mon_by_defid 284094 1
	delete_mon_by_defid 284095 1
	
	;�������������
	strvar num all
	mon_get_player_num_in_rect 1 400 400 num
	GET_PWORLD_ROLE_NUM all 1		
	;����⻷վ�Դ���
	VAR 14 0
	get_pworld_var 16 32 1 14
	if num = @all
		add 14 1
	else
		var 14 0
		;�����׶�,��Ҫ����tickʱҪ������proc
		var 13 0
		set_pworld_var 12 32 @13 1
		mon_skill 10352 1
		effect 0 0 playEffct(19620,29736,"effect\\entity\\1000900_8.ent")
		add_sys_msg_start
		add_sys_msg ���ź�������ʧ���ˣ���������ѡ������������
		set_sys_msg_mode 3 0 0
		map_msg_info
	endif
	set_pworld_var 16 32 @14 1 
	;�������ʱ�����
	;10��ʱ��
	var 10 0
	set_pworld_var 0 32 @10 1
	
endproc
proc 4
	;�������������
	strvar num all
	mon_get_player_num_in_rect 1 400 400 num
	GET_PWORLD_ROLE_NUM all 1
	if num = @all
		get_mon_var 0 32 1 10
		if 10 = 0
			add_sys_msg_start
			add_sys_msg ��ϲ�㣬����ˣ���������֣�ֱ����������ɹ���
			set_sys_msg_mode 3 0 0
			map_msg_info
		else
			if 10 = 1
				add_sys_msg_start
				add_sys_msg ������ͨ��δ���ĵ�·�ϣ�������ȷ��ѡ�����Ҫ��
				set_sys_msg_mode 3 0 0
				map_msg_info
			else
				add_sys_msg_start
				add_sys_msg ����Ҫ���ǣ���������ȷ��ѡ����ܼ������ȥ��
				set_sys_msg_mode 3 0 0
				map_msg_info
			endif
		endif
		
		add 10 1
		set_mon_var 0 32 @10 1
	else
		add_sys_msg_start
		add_sys_msg ���Ǳ���Ҫͬ��Э��������ͨ���˹أ�
		set_sys_msg_mode 3 0 0
		map_msg_info
	endif
endproc
