;�ͼ��ر�ͼ1

proc 1
	strvar d v
	use_cangbaotu_pic 5062011 1000  d v
	switch d
	case 0
			dec_item_num_inpak 5062011 1 0
			add_sys_msg_start
			add_sys_msg �������ķ�϶���ҵ�һ���ƾɵ���Ƥֽ
			set_sys_msg_mode 4 0 0
			msg_info
			add_item_num_inpak 5300358 1 1 0	
		
	endcase
	case 1
		add_sys_msg_start
		add_sys_msg ��ȷ��û�п������ز������ŵ�ͼ
		set_sys_msg_mode 4 0 0
		msg_info
	endcase
	case 2
		add_sys_msg_start
		switch v
		case 1
		add_sys_msg ���뱦��ֻ��һ��֮ң,������ı�������
		endcase
		case 2
		add_sys_msg ���뱦��ֻ��һ��֮ң,������Ķ���������
		endcase
		case 3
		add_sys_msg ���뱦��ֻ��һ��֮ң,������Ķ�������
		endcase
		case 4
		add_sys_msg ���뱦��ֻ��һ��֮ң,������Ķ��Ϸ�����
		endcase
		case 5
		add_sys_msg ���뱦��ֻ��һ��֮ң,��������Ϸ�����
		endcase
		case 6
		add_sys_msg ���뱦��ֻ��һ��֮ң,����������Ϸ�����
		endcase
		case 7
		add_sys_msg ���뱦��ֻ��һ��֮ң,���������������
		endcase
		case 8
		add_sys_msg ���뱦��ֻ��һ��֮ң,�����������������
		endcase
		endswitch
		set_sys_msg_mode 4 0 0
		msg_info
	endcase
	case 3
		add_sys_msg_start
		switch v
		case 1
		add_sys_msg ���ز�Զ�ˣ�������ı���
		endcase
		case 2
		add_sys_msg ���ز�Զ��,������Ķ�����
		endcase
		case 3
		add_sys_msg ���ز�Զ��,������Ķ���
		endcase
		case 4
		add_sys_msg ���ز�Զ��,������Ķ��Ϸ�
		endcase
		case 5
		add_sys_msg ���ز�Զ��,��������Ϸ�
		endcase
		case 6
		add_sys_msg ���ز�Զ�ˣ�����������Ϸ�
		endcase
		case 7
		add_sys_msg ���ز�Զ��,�����������
		endcase
		case 8
		add_sys_msg ���ز�Զ��,�������������
		endcase
		endswitch
		set_sys_msg_mode 4 0 0
		msg_info
	endcase
	case 4
		add_sys_msg_start
		switch v
		case 1
		add_sys_msg ���ؾ�˴�����Щ����,����ı���
		endcase
		case 2
		add_sys_msg ���ؾ�˴�����Щ����,����Ķ�����
		endcase
		case 3
		add_sys_msg ���ؾ�˴�����Щ����,����Ķ���
		endcase
		case 4
		add_sys_msg ���ؾ�˴�����Щ����,����Ķ��Ϸ�
		endcase
		case 5
		add_sys_msg ���ؾ�˴�����Щ����,������Ϸ�
		endcase
		case 6
		add_sys_msg ���ؾ�˴�����Щ����,��������Ϸ�
		endcase
		case 7
		add_sys_msg ���ؾ�˴�����Щ����,���������
		endcase
		case 8
		add_sys_msg ���ؾ�˴�����Щ����,�����������
		endcase
		endswitch
		set_sys_msg_mode 4 0 0
		msg_info
	endcase
	endswitch

endproc	