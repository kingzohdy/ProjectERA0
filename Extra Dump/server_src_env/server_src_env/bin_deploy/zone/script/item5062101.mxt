;��ʦ���� Ѻ�� ���� ���� ����


proc 1
	STRVAR Overlay Ret MonMemID	fConvoy
	;GET_MON_INFO #���ر�־ #����id #ʵ��id #Ѫ�� #��ͼid #��ͼʵ�� #����X #Y
	GET_MON_INFO 0 8 MonMemID 0 0 0 0 0

	;IS_MY_CONVOY_MON  #����ʵ���ɣ� #���ر�־
	IS_MY_CONVOY_MON @MonMemID fConvoy

	var 6 0
	var 5 0

	If fConvoy = 0
		If 8 = 264016 Or 8 = 264017 Or 8 = 264018 Or 8 = 264019 Or 8 = 264020 Or 8 = 264021 Or 8 = 264022 Or 8 = 264023 Or 8 = 264024 Or 8 = 264025 Or 8 = 264026 Or 8 = 264027 Or 8 = 264028 Or 8 = 264029 Or 8 = 264030 Or 8 = 264031
		
					;�м���
					GET_ID_MON_STATUS_INFO  @MonMemID 1203 Ret 0 6 0 0
					if Ret = 0
						return
					endif
								
					;��ü���״̬��Ϣ
					GET_ID_MON_STATUS_INFO  @MonMemID 1202 Ret 0 Overlay 0 0
					;û�����״̬
					if Ret = 0
						;����
						If Overlay = 3
							TARGET_ADD_STATUS 3 @MonMemID 1203 2 6 0
								add_sys_msg_start
								add_sys_msg �������������ȣ���Ҫ��ȴ���ƶ��ٶȽ��ͣ�
								set_sys_msg_mode 1 0 0
								TEAM_MSG_INFO
							
							return
						endif
					endif
					
					;�Ӽ���
					TARGET_ADD_STATUS 3 @MonMemID 1202 2 4 0
					
					GET_ID_MON_STATUS_INFO  @MonMemID 1202 Ret 0 Overlay 0 0
					if Ret = -1
						return
					endif


					;4�ξ�����
					If Overlay = 3
						add_sys_msg_start
						add_sys_msg ������е�裬�ٶ�ò�Ƶ������ˣ�
						set_sys_msg_mode 1 0 0
						TEAM_MSG_INFO
					endif
			

		Else
			add_sys_msg_start
			add_sys_msg ���ڶ��˻���ʱ�����Լ��Ļ���ʹ�ã�
			set_sys_msg_mode 1 0 0
			msg_info
		endif
		
	Else
		add_sys_msg_start
		add_sys_msg ���ڶ��˻���ʱ�����Լ��Ļ���ʹ�ã�
		set_sys_msg_mode 1 0 0
		msg_info
	endif

endproc			