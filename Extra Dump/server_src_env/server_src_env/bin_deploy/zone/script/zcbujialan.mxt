
;����

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���ں�ͬ־�����ںñ�����
	add_talk #b#b��ӭǰ���һ��������۴����͡��ֿ�Կ�ס���
	add_talk #b#b���ζһ��������Ԫ��������ͬ־Ϊ���Ƿ���
	add_talk #b#bֻҪ��ӵ���㹻�ġ��۴����������ɻ�ø��ֿ۴���Կ�ף�
	add_talk #b#b��лʱ������Ĵ���֧�֣���л��λ�ĳ�����ע��
	add_talk #b#b
	add_talk #b#b#c186506ʹ�ñ����۴���õĿ۴����������ڴ˶һ������۴���ֿ�Կ��#n
	strvar RetTsk
	task_stat 3130 1 RetTsk
	
	if RetTsk = 2
		option 200 ����ʥ���Ϲ������������
	endif
	option 120 #�����̵�
	
	task_stat 4498 1 20
	if 20 = 2
		option 250 #����λ����Ա����������һ������
	endif
	
	option 300 #С�������󱳰�

	
	talk
	
endproc 
proc 120
	OPEN_NPC_SHOP 
endproc
proc 200 
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bÿ��ʥ���Ϲ���������������ң�$_player$���������лл������һ����Ŭ������Ϊһ������Ϊ���ˣ�
		option 201 #����ʥ������
	talk
endproc 
proc 201
	GET_ITEM_NUM_INPAK 5300844 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ��
		set_sys_msg_mode 1 0 0
		msg_info
	ENDIF
	if 11 >= 1
			
		dec_item_num_inpak 5300844 1 30
		if 30 != -1
			task_fini 3130 1
		endif
	endif
endproc 

proc 250
	dialog 3
	add_talk $_npc$:
	add_talk #b#b��������ĸ����з���#c186506��˼��#n��#c186506������#n
	add_talk #b#b�����û�����ӵĻ�����ȥ�����Ƕ������ɣ�
	
	NPC_COMMIT_OPT 251
	
	talk
endproc 
proc 251
	strvar m_ID m_Pos1 m_Pos2 m_Col Ret RetTask
	GET_COMMIT_ITEM_INFO 1 m_ID
	if m_ID = -1
		add_sys_msg_start
		add_sys_msg �㻹û���ύ��Ʒ��
		set_sys_msg_mode 1 0 0
		msg_info		
		return 
	endif
	
	GET_COMMIT_ITEM_INFO 2 m_Pos1
	GET_COMMIT_ITEM_INFO 3 m_Pos2
	GET_COMMIT_ITEM_INFO 4 m_Col
	task_stat 4498 1 100
	if 100 = 2
		if m_ID = 5012123 or m_ID = 5012124
			switch m_ID
				case 5012123
					DEC_ITEM_NUM_BYIDX 5012123 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4498 1
					call 502
				endcase
				case 5012124
					DEC_ITEM_NUM_BYIDX 5012124 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4498 1
					call 502
				endcase
			endswitch
		else
			add_sys_msg_start
			add_sys_msg ��Ʒ��������Ҫ��δ�ܳɹ��Ͻ�
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	
	endif
endproc 
proc 502
	add_sys_msg_start
	add_sys_msg �ɹ��Ͻ���Ʒ���������
	set_sys_msg_mode 3 0 0
	msg_info
	PLAYER_CLOSE_DIALOG 0 0
endproc 

proc 300
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�����5��С�������Ҹ���1���󱳰���û�б������������ˣ��Բ��Բ��Բ���
	add_talk #b#b
	; add_talk #b#b#c1865065#n����#c005ebe4�񱳰�#n���ɶһ�#c1865061#n����#c005ebe6�񱳰�#n��
	add_talk #b#b#c1865065#n����#c005ebe6�񱳰�#n���ɶһ�#c1865061#n����#c005ebe8�񱳰�#n��
	add_talk #b#b#c1865065#n����#c005ebe8�񱳰�#n���ɶһ�#c1865061#n����#c005ebe10�񱳰�#n��
	add_talk #b#b#c1865065#n����#c005ebe10�񱳰�#n���ɶһ�#c1865061#n����#c005ebe12�񱳰�#n��
	add_talk #b#b#c1865065#n����#c005ebe12�񱳰�#n���ɶһ�#c1865061#n����#c005ebe14�񱳰�#n��
	
	; option 301 #��Ҫ����6�񱳰���
	option 302 ��Ҫ����8�񱳰���
	option 303 ��Ҫ����10�񱳰���
	option 304 ��Ҫ����12�񱳰���
	option 305 ��Ҫ����14�񱳰���
	
	talk
endproc

proc 301
	strvar ABn Num1 Num2 Bsl ret1 ret2 ret3
	GET_ITEM_NUM_INPAK 5251103 0 Num1
	GET_ITEM_NUM_INPAK 5251003 0 Num2
	
	var ABn @Num1
	add ABn @Num2
	if ABn >= 5
		if Num1 >= 5
			DEC_ITEM_NUM_INPAK 5251103 5 ret3
			
			if ret3 = 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251016 1 1 0
					
				add_sys_msg_start
				add_sys_msg ��ϲ��ɹ��һ���1����6�񱳰�����
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		else
			DEC_ITEM_NUM_INPAK 5251103 @Num1 ret1
			
			if ret1 = 0
				var Bsl 5
				sub Bsl @Num1
				DEC_ITEM_NUM_INPAK 5251003 @Bsl ret2
				
				if ret2 = 0
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251016 1 1 0
					
					add_sys_msg_start
					add_sys_msg ��ϲ��ɹ��һ���1����6�񱳰�����
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg ��ı�������Ŷ��Ҫ5����4�񱳰������ܻ���6�񱳰�����
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc

proc 302
	strvar Num ret
	GET_ITEM_NUM_INPAK 5251016 0 Num
	
	if Num >= 5
		DEC_ITEM_NUM_INPAK 5251016 5 ret
		if ret = 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251104 1 1 0
			
			add_sys_msg_start
			add_sys_msg ��ϲ��ɹ��һ���1����8�񱳰�����
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg ��ı�������Ŷ��Ҫ5����6�񱳰������ܻ���8�񱳰�����
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc

proc 303
	strvar ABn Num1 Num2 Bsl ret1 ret2 ret3
	GET_ITEM_NUM_INPAK 5251104 0 Num1
	GET_ITEM_NUM_INPAK 5251004 0 Num2
	
	var ABn @Num1
	add ABn @Num2
	if ABn >= 5
		if Num1 >= 5
			DEC_ITEM_NUM_INPAK 5251104 5 ret3
			
			if ret3 = 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251105 1 1 0
					
				add_sys_msg_start
				add_sys_msg ��ϲ��ɹ��һ���1����10�񱳰�����
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		else
			DEC_ITEM_NUM_INPAK 5251104 @Num1 ret1
			
			if ret1 = 0
				var Bsl 5
				sub Bsl @Num1
				DEC_ITEM_NUM_INPAK 5251004 @Bsl ret2
				
				if ret2 = 0
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251105 1 1 0
					
					add_sys_msg_start
					add_sys_msg ��ϲ��ɹ��һ���1����10�񱳰�����
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg ��ı�������Ŷ��Ҫ5����8�񱳰������ܻ���10�񱳰�����
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc

proc 304
	strvar ABn Num1 Num2 Bsl ret1 ret2 ret3
	GET_ITEM_NUM_INPAK 5251105 0 Num1
	GET_ITEM_NUM_INPAK 5251005 0 Num2
	
	var ABn @Num1
	add ABn @Num2
	if ABn >= 5
		if Num1 >= 5
			DEC_ITEM_NUM_INPAK 5251105 5 ret3
			
			if ret3 = 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251106 1 1 0
					
				add_sys_msg_start
				add_sys_msg ��ϲ��ɹ��һ���1����12�񱳰�����
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		else
			DEC_ITEM_NUM_INPAK 5251105 @Num1 ret1
			
			if ret1 = 0
				var Bsl 5
				sub Bsl @Num1
				DEC_ITEM_NUM_INPAK 5251005 @Bsl ret2
				
				if ret2 = 0
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251106 1 1 0
					
					add_sys_msg_start
					add_sys_msg ��ϲ��ɹ��һ���1����12�񱳰�����
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg ��ı�������Ŷ��Ҫ5����10�񱳰������ܻ���12�񱳰�����
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc

proc 305
	strvar ABn Num1 Num2 Bsl ret1 ret2 ret3
	GET_ITEM_NUM_INPAK 5251106 0 Num1
	GET_ITEM_NUM_INPAK 5251006 0 Num2
	
	var ABn @Num1
	add ABn @Num2
	if ABn >= 5
		if Num1 >= 5
			DEC_ITEM_NUM_INPAK 5251106 5 ret3
			
			if ret3 = 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251107 1 1 0
					
				add_sys_msg_start
				add_sys_msg ��ϲ��ɹ��һ���1����14�񱳰�����
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		else
			DEC_ITEM_NUM_INPAK 5251106 @Num1 ret1
			
			if ret1 = 0
				var Bsl 5
				sub Bsl @Num1
				DEC_ITEM_NUM_INPAK 5251006 @Bsl ret2
				
				if ret2 = 0
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251107 1 1 0
					
					add_sys_msg_start
					add_sys_msg ��ϲ��ɹ��һ���1����14�񱳰�����
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg ��ı�������Ŷ��Ҫ5����12�񱳰������ܻ���14�񱳰�����
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc
