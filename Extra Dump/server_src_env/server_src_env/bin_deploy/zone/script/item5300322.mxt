;����ڰ�ħ��


proc 1

	STRVAR Ret Direc MapID EndX EndY 
	USE_TREASURE_PIC 5300322 1000 Ret Direc MapID EndX EndY 
	dev EndX 100
	dev EndY 100

	 
	if Ret = 0 
		dec_item_num_inpak 5300322 1 0
		add_item_num_inpak 5300292 1 1 0
		
		GET_PLAYER_MAPINFO 0 20 21 22 23
		effect 0 0 playEffct($@22$,$@23$,"effect\\entity\\9552_16.ent")	
		
		add_sys_msg_start
		add_sys_msg ���ҵ��˺ڰ����䣡
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	if Ret = 1
		add_sys_msg_start
		add_sys_msg Ŀ�ĵز��ڴ˵�ͼ��
		set_sys_msg_mode 3 0 0
		msg_info
	endif
;	if Ret = 2
;		add_sys_msg_start
;		add_sys_msg Ŀ���Ѿ��ܽ��ˣ�����$@EndX$ $@EndY$����
;		set_sys_msg_mode 3 0 0
;		msg_info
;	endif
	if Ret > 0
		if Direc = 1
				add_sys_msg_start
				add_sys_msg Ŀ����ڱ�����Զ����
				set_sys_msg_mode 3 0 0
				msg_info
		endif
		if Direc = 2
				add_sys_msg_start
				add_sys_msg Ŀ����ڶ�������Զ����
				set_sys_msg_mode 3 0 0
				msg_info
		endif
		if Direc = 3
				add_sys_msg_start
				add_sys_msg Ŀ����ڶ�����Զ����
				set_sys_msg_mode 3 0 0
				msg_info
		endif
		if Direc = 4
				add_sys_msg_start
				add_sys_msg Ŀ����ڶ��Ϸ���Զ����
				set_sys_msg_mode 3 0 0
				msg_info
		endif
		if Direc = 5
				add_sys_msg_start
				add_sys_msg Ŀ������Ϸ���Զ����
				set_sys_msg_mode 3 0 0
				msg_info
		endif
		if Direc = 6
				add_sys_msg_start
				add_sys_msg Ŀ��������Ϸ���Զ����
				set_sys_msg_mode 3 0 0
				msg_info
		endif
		if Direc = 7
				add_sys_msg_start
				add_sys_msg Ŀ�����������Զ����
				set_sys_msg_mode 3 0 0
				msg_info
		
		endif
		if Direc = 8
				add_sys_msg_start
				add_sys_msg Ŀ�������������Զ����
				set_sys_msg_mode 3 0 0
				msg_info
		endif
		
	
	endif

endproc


