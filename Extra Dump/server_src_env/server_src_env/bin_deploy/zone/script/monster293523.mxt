
proc 2
	rand 50 1000
	ID_PLAYER_BASE_ATT 0 1 1 41 1
	mul 41 @41
	
	;29%
	if @50 >= 0 and @50 < 290
		dev 41 10
		ADD_MONEY 0 @41 20 123 ;���˼ӽ�Ǯ
		if 123 < 0
			add_sys_msg_start
			add_sys_msg �������е������Ѿ��ﵽ����
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		;�������ϵͳ����ʾ
	endif
	
	;29%
	if @50 >= 290 and @50 < 580			
		ADD_EXP 0 @41 123   ;���˼Ӿ���
		if 123 < 0
			add_sys_msg_start
			add_sys_msg ����ǰ�洢�ľ���ֵ�Ѿ��ﵽ���ޣ��������ȼ��������޷�������þ���
			set_sys_msg_mode 3 0 0
			msg_info
		else
			add_sys_msg_start
			add_sys_msg �����$@41$�㾭��
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif

	;29%
	if @50 >= 580 and @50 < 870
		dev 41 10
		ADD_MONEY 0 @41 21 123 ;���˼ӽ�Ǯ
		if 123 < 0
			add_sys_msg_start
			add_sys_msg �������еİ����Ѿ��ﵽ����
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		;�������ϵͳ����ʾ
	endif
	
	;1%   ������
	if @50 >= 870 and @50 < 880
		dev 41 10
		ADD_MONEY 0 @41 20 123 ;���˼ӽ�Ǯ
		if 123 < 0
			add_sys_msg_start
			add_sys_msg �������е������Ѿ��ﵽ����
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		;�������ϵͳ����ʾ
	endif
	
	;1%  ����
	if @50 >= 880 and @50 < 890		
		ADD_EXP 0 @41 123   ;���˼Ӿ���
		if 123 < 0
			add_sys_msg_start
			add_sys_msg ����ǰ�洢�ľ���ֵ�Ѿ��ﵽ���ޣ��������ȼ��������޷�������þ���
			set_sys_msg_mode 3 0 0
			msg_info
		else
			add_sys_msg_start
			add_sys_msg �����$@41$�㾭��
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif

	;1%    �����
	if @50 >= 890 and @50 < 900
		dev 41 10
		ADD_MONEY 0 @41 21 123 ;���˼ӽ�Ǯ
		if 123 < 0
			add_sys_msg_start
			add_sys_msg �������еİ����Ѿ��ﵽ����
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		;�������ϵͳ����ʾ
	endif
	
	if @50 >= 900
	
		GET_PACK_SPACE 0 11
		strvar ibind x1

		if 11 < 1
			add_sys_msg_start
			add_sys_msg �����ռ䲻�㣡
			set_sys_msg_mode 4 0 0
			msg_info
		else		
			;4%
			if @50 >= 900 and @50 < 940
				;5233000	�û���ʵ
				var x1 5233000
				ADD_BIND_ITEM_NUM_INPAK @x1 5 1 14
				add_sys_msg_start
				add_sys_msg $_player$������ź������ı����в��Ž����Ʊ����������ϴ˷����õ��˻û���ʵ�����Ǻ��˵�ͷ�������ܵ���
				set_sys_msg_mode 18 0 0
				SVR_MSG_INFO 
			endif
					
			;3%
			if @50 >= 940 and @50 < 970
				;5052936	�ͽ�ϻ
				var x1 5052936
				ADD_BIND_ITEM_NUM_INPAK @x1 5 1 14
				add_sys_msg_start
				add_sys_msg $_player$������ź������ı����в��Ž����Ʊ����������ϴ˷����õ����ͽ�ϻ�����Ǻ��˵�ͷ�������ܵ���
				set_sys_msg_mode 18 0 0
				SVR_MSG_INFO 
			endif
				
			;3%
			if @50 >= 970 and @50 < 1000
				;5012127	��ɫ����
				var x1 5012127
				ADD_BIND_ITEM_NUM_INPAK @x1 1 1 14
				 
				add_sys_msg_start
				add_sys_msg $_player$������ź������ı����в��Ž����Ʊ����������ϴ˷����õ�����ɫ���������Ǻ��˵�ͷ�������ܵ���
				set_sys_msg_mode 18 0 0
				SVR_MSG_INFO 
			endif
		endif
	endif
endproc

