;����

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�ڡ��ڡ����ڡ�
	add_talk #b#b#c186506����#n��#c186506��#n��#c186506��װ��#n�����١�#c186506װ#n���ڡ�#c186506����#n��
	add_talk #b#b���ڡ�
	add_talk #b#b#c186506��װս#n��#c186506��#n���ڡ���#c186506����#n��#c186506ȫ��#n���١���
	add_talk #b#b
	add_talk  #c005ebe��װս�����ڶ�ʱ���ڴ������ս���ı�������#n
	
	option 100 #���뿴�������װ�����
	
	talk

endproc

proc 100
	dialog 1
	add_talk ����ڵ���
	add_talk #b#b��ʱ���ڴ������ս��#c18650650%������#n
	add_talk #b#b
	add_talk ��װ��ǿ����
	add_talk #b#b��ʱ���ڴ������ս���ܵ���#c18650633%�˺�#n
	add_talk #b#b
	add_talk ������١�
	add_talk #b#b��ʱ���ڶ�������ս��#c18650610%�ƶ��ٶ�#n
	
	option 200 ��װ�������ڵ�����1000������
	option 300 ��װ��װ��ǿ������1000������
	option 400 ��װ���������١���1000������
	
	talk

endproc

proc 200
	STRVAR memID num ret ret2 ret3 ret4 suo
	GET_ROLE_RIDE_ON_MACHINE_MEM_ID memID 0
	PKG_IS_UNLOCK ret4 suo
	
	if ret4 = 0 and suo = 1
		if memID > 0
			GET_ID_MON_STATUS_INFO @memID 2106 ret3 1 num 0 0
			
			if ret3 = -1
				DEC_MONEY_TEST 0 1000 21 ret
				if ret = 0
					DEC_MONEY 0 1000 21 0
					TARGET_ADD_STATUS 3 @memID 2106 1 60 ret2
					if ret2 = 0
						GET_ID_MON_STATUS_INFO @memID 2106 0 1 num 0 0
						var 10 @num
						add 10 1
					
						add_sys_msg_start
						add_sys_msg �������ڵ����ﵽ��$@10$/5ǿ�ȣ�ս���������������
						set_sys_msg_mode 3 0 0
						msg_info
					endif
				else
					add_sys_msg_start
					add_sys_msg ���ϰ����������޷����򡰳����ڵ�����װս��
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			elseif ret3 = 0 and num < 4
				DEC_MONEY_TEST 0 1000 21 ret
				if ret = 0
					DEC_MONEY 0 1000 21 0
					TARGET_ADD_STATUS 3 @memID 2106 1 60 ret2
					if ret2 = 0
						GET_ID_MON_STATUS_INFO @memID 2106 0 1 num 0 0
						var 10 @num
						add 10 1
					
						add_sys_msg_start
						add_sys_msg �������ڵ����ﵽ��$@10$/5ǿ�ȣ�ս���������������
						set_sys_msg_mode 3 0 0
						msg_info
					endif
				else
					add_sys_msg_start
					add_sys_msg ���ϰ����������޷����򡰳����ڵ�����װս��
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			else
				add_sys_msg_start
				add_sys_msg �������ڵ����Ѵﵽ5/5ǿ�ȣ��޷�����ǿ��
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		else
			add_sys_msg_start
			add_sys_msg δ������ս���ϣ��޷����򡰳����ڵ���
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
endproc

proc 300
	STRVAR memID num ret ret2 ret3 ret4 suo
	GET_ROLE_RIDE_ON_MACHINE_MEM_ID memID 0
	PKG_IS_UNLOCK ret4 suo
	
	if ret4 = 0 and suo = 1
		if memID > 0
			GET_ID_MON_STATUS_INFO @memID 2107 ret3 1 num 0 0
			
			if ret3 = -1
				DEC_MONEY_TEST 0 1000 21 ret
				if ret = 0
					DEC_MONEY 0 1000 21 0
					TARGET_ADD_STATUS 3 @memID 2107 1 60 ret2
					if ret2 = 0
						GET_ID_MON_STATUS_INFO @memID 2107 0 1 num 0 0
						var 10 @num
						add 10 1
					
						add_sys_msg_start
						add_sys_msg ��װ��ǿ�����ﵽ��$@10$/5ǿ�ȣ�ս���������������
						set_sys_msg_mode 3 0 0
						msg_info
					endif
				else
					add_sys_msg_start
					add_sys_msg ���ϰ����������޷�����װ��ǿ������װս��
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			elseif ret3 = 0 and num < 4
				DEC_MONEY_TEST 0 1000 21 ret
				if ret = 0
					DEC_MONEY 0 1000 21 0
					TARGET_ADD_STATUS 3 @memID 2107 1 60 ret2
					if ret2 = 0
						GET_ID_MON_STATUS_INFO @memID 2107 0 1 num 0 0
						var 10 @num
						add 10 1
						
						add_sys_msg_start
						add_sys_msg ��װ��ǿ�����ﵽ��$@10$/5ǿ�ȣ�ս���������������
						set_sys_msg_mode 3 0 0
						msg_info
					endif
				else
					add_sys_msg_start
					add_sys_msg ���ϰ����������޷�����װ��ǿ������װս��
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			else
				add_sys_msg_start
				add_sys_msg ��װ��ǿ�����Ѵﵽ5/5ǿ�ȣ��޷�����ǿ��
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		else
			add_sys_msg_start
			add_sys_msg δ������ս���ϣ��޷�����װ��ǿ����
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
endproc

proc 400
	STRVAR memID num ret ret2 ret3 ret4 suo
	GET_ROLE_RIDE_ON_MACHINE_MEM_ID memID 0
	PKG_IS_UNLOCK ret4 suo
	
	if ret4 = 0 and suo = 1
		if memID > 0
			GET_ID_MON_STATUS_INFO @memID 2108 ret3 1 num 0 0
			
			if ret3 = -1
				DEC_MONEY_TEST 0 1000 21 ret
				if ret = 0
					DEC_MONEY 0 1000 21 0
					TARGET_ADD_STATUS 3 @memID 2108 1 60 ret2
					if ret2 = 0
						GET_ID_MON_STATUS_INFO @memID 2108 0 1 num 0 0
						var 10 @num
						add 10 1
					
						add_sys_msg_start
						add_sys_msg ���������١��ﵽ��$@10$/5ǿ�ȣ�ս���������������
						set_sys_msg_mode 3 0 0
						msg_info
					endif
				else
					add_sys_msg_start
					add_sys_msg ���ϰ����������޷����򡰳����ڵ�����װս��
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			elseif ret3 = 0 and num < 4
				DEC_MONEY_TEST 0 1000 21 ret
				if ret = 0
					DEC_MONEY 0 1000 21 0
					TARGET_ADD_STATUS 3 @memID 2108 1 60 ret2
					if ret2 = 0
						GET_ID_MON_STATUS_INFO @memID 2108 0 1 num 0 0
						var 10 @num
						add 10 1
						
						add_sys_msg_start
						add_sys_msg ���������١��ﵽ��$@10$/5ǿ�ȣ�ս���ƶ��ٶȻ������
						set_sys_msg_mode 3 0 0
						msg_info
					endif
				else
					add_sys_msg_start
					add_sys_msg ���ϰ����������޷����򡰶������١���װս��
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			else
				add_sys_msg_start
				add_sys_msg ���������١��Ѵﵽ5/5ǿ�ȣ��޷�����ǿ��
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		else
			add_sys_msg_start
			add_sys_msg δ������ս���ϣ��޷����򡰶������١�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
endproc
