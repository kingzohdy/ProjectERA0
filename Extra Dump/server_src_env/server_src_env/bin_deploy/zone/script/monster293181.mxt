

proc 2
	strvar Rad Rad1
	var Rad 0
	rand Rad 10
	if Rad < 3
	;�ɹ�
		add_sys_msg_start
		add_sys_msg ���ƿ���ڲ����Ÿ��ӵ�ͼ����������ǿ�ҵĸо����������ˣ�
		set_sys_msg_mode 3 0 0
		msg_info
		
		add_item_num_inpak 5300597 1 1 0
		
	endif
	if Rad >= 3 and Rad < 10
		rand Rad1 2
		if Rad1 = 0
			add_sys_msg_start
			add_sys_msg ����û��ƿ�ǣ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		if Rad1 = 1
			add_sys_msg_start
			add_sys_msg ���ƿ��ûʲô�ر�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif	

endproc



