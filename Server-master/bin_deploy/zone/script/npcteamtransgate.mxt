;���鴫���� ,���鴫����
PROC 1	
	strvar lv
	get_mon_info 1 2 0 0 0 0 0 0 
	GET_PLAYER_BASE_ATT 1 1 lv 1
	;ʱ���
	if 2 = 333069
		TRANSMIT 1000 26600 28500 1
	endif
	;�γ�
	if 2 = 333070
		if lv >= 70
			TRANSMIT 3200 6300 21100 1
		else
		
			add_sys_msg_start
			add_sys_msg �˵�ͼ̫Σ���ˣ�Ϊ������������������ǵ�70���Ժ���ȥ�ɣ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	;����ʥ��
	if 2 = 333071
		if lv >= 60
			TRANSMIT 3100 13100 1200 1
		else
		
			add_sys_msg_start
			add_sys_msg �˵�ͼ̫Σ���ˣ�Ϊ������������������ǵ�60���Ժ���ȥ�ɣ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	;������
	if 2 = 333072
		if lv >= 70
			TRANSMIT 3304 25600 48600 1
		else
		
			add_sys_msg_start
			add_sys_msg �˵�ͼ̫Σ���ˣ�Ϊ������������������ǵ�70���Ժ���ȥ�ɣ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	;��Ŭ��˹��̳
	if 2 = 333073
		if lv >= 30
			TRANSMIT 3302 4500 7200 1
		else
		
			add_sys_msg_start
			add_sys_msg �˵�ͼ̫Σ���ˣ�Ϊ������������������ǵ�30���Ժ���ȥ�ɣ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	;��Ŭ��˹���
	if 2 = 333074
		if lv >= 30
			TRANSMIT 3300 1400 7500 1
		else
		
			add_sys_msg_start
			add_sys_msg �˵�ͼ̫Σ���ˣ�Ϊ������������������ǵ�30���Ժ���ȥ�ɣ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	;��Ŭ��˹����
	if 2 = 333075
		if lv >= 30
			TRANSMIT 3303 10500 3700 1
		else
		
			add_sys_msg_start
			add_sys_msg �˵�ͼ̫Σ���ˣ�Ϊ������������������ǵ�30���Ժ���ȥ�ɣ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	;Խ����
	if 2 = 333076
		if lv >= 50
			TRANSMIT 3500 5300 12900 1
		else
		
			add_sys_msg_start
			add_sys_msg �˵�ͼ̫Σ���ˣ�Ϊ������������������ǵ�50���Ժ���ȥ�ɣ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	;ʮ���ſ�
	if 2 = 333077
		if lv >= 40
			TRANSMIT 3400 3500 3100 1
		else
		
			add_sys_msg_start
			add_sys_msg �˵�ͼ̫Σ���ˣ�Ϊ������������������ǵ�40���Ժ���ȥ�ɣ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	
ENDPROC
