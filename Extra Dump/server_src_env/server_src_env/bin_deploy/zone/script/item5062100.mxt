;ѱ����� ���� ���� ����

proc 1
	STRVAR Overlay Ret MonMemID	fConvoy
	;GET_MON_INFO #���ر�־ #����id #ʵ��id #Ѫ�� #��ͼid #��ͼʵ�� #����X #Y
	GET_MON_INFO 0 8 MonMemID 0 0 0 0 0

	;IS_MY_CONVOY_MON  #����ʵ���ɣ� #���ر�־
	IS_MY_CONVOY_MON  @MonMemID fConvoy

	var 6 0
	var 5 0

	If fConvoy = 0
		If 8 = 264000 Or 8 = 264001 Or 8 = 264002 Or 8 = 264003 Or 8 = 264004 Or 8 = 264005 Or 8 = 264006 Or 8 = 264007 Or 8 = 264008 Or 8 = 264009 Or 8 = 264010 Or 8 = 264011 Or 8 = 264012 Or 8 = 264013 Or 8 = 264014 Or 8 = 264015
		
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
							TARGET_ADD_STATUS 3 @MonMemID 1203 1 6 0
							add_sys_msg_start
							add_sys_msg ������ƣ����������Ҫ��Ϣ���ƶ��ٶȽ��ͣ�
							set_sys_msg_mode 1 0 0
							MSG_INFO
							
							return
						endif
					endif
					
					;�Ӽ���
					TARGET_ADD_STATUS 3 @MonMemID 1202 1 4 0
					
					GET_ID_MON_STATUS_INFO  @MonMemID 1202 Ret 0 Overlay 0 0
					if Ret = -1
						return
					endif


					;4�ξ�����
					If Overlay = 3
						add_sys_msg_start
						add_sys_msg ������е�裬�ٶ�ò�Ƶ������ˣ�
						set_sys_msg_mode 1 0 0
						MSG_INFO
					endif
			

		Else
			add_sys_msg_start
			add_sys_msg ���ڵ��˻���ʱ�����Լ��Ļ���ʹ�ã�
			set_sys_msg_mode 1 0 0
			msg_info
		endif
		
	Else
		add_sys_msg_start
		add_sys_msg ���ڵ��˻���ʱ�����Լ��Ļ���ʹ�ã�
		set_sys_msg_mode 1 0 0
		msg_info
	endif

endproc			