

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�㡫û���������������𣿣��㡫�Ҳ���������������������㡫���������������㡫��˵���Ҳ�����������һ�������㶮���𣿣��㡫
	add_talk #b#b#c186506���۸�ְҵ������#n
	task_add_option 1090 1 102
	task_add_option 1091 1 103

	task_add_option 2022 1 104
	task_add_option 2023 1 105

;����

	task_stat 4487 1 20
	if 20 = 2
		option 200 #�������������ɵ�����һ������
	endif
	
	OPTION 101 #(����)���̵�
	option 300 #�һ�70��������������
	option 400 #����80��������������
	
	talk
endproc 


PROC 101
	OPEN_NPC_SHOP
ENDPROC

PROC 102
	task_do_option 1090 1
ENDPROC
PROC 103
	task_do_option 1091 1
ENDPROC
PROC 104
	task_do_option 2022 1
ENDPROC
PROC 105
	task_do_option 2023 1
ENDPROC

proc 200
	dialog 3
	add_talk $_npc$:
	add_talk #b#b��������ĸ����з���#c186506��˼��#n��#c186506������#n
	add_talk #b#b�����û�����ӵĻ�����ȥ�����Ƕ������ɣ�
	
	NPC_COMMIT_OPT 201
	
	talk
endproc 
proc 201
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
	task_stat 4487 1 100
	if 100 = 2
		if m_ID = 5012123 or m_ID = 5012124
			switch m_ID
				case 5012123
					DEC_ITEM_NUM_BYIDX 5012123 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4487 1
					call 502
				endcase
				case 5012124
					DEC_ITEM_NUM_BYIDX 5012124 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4487 1
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


proc 2
	get_trig_task 1 2
	if 2 = 1091
		task_fini 1091 1
	endif 
	if 2 = 2023 
		task_fini 2023 1
	endif 
endproc

proc 300
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�㡫û���������������𣿣��㡫�Ҳ���������������������㡫���������������㡫��˵���Ҳ�����������һ�������㶮���𣿣��㡫
	add_talk #b#b#c186506ע�⣺ֻ�ܶһ������Լ�ר��ְҵ�Ĵ���װ��#n
	option 330 #�һ�70�������������������
	option 340 #�һ�70��������ĸ���������
	
	talk
endproc

proc 330
	choose 2 310 301 �һ���Ҫ����ϼ�20�����ع�����300��\n�Ƿ�ȷ�϶һ���
endproc

proc 310
	strvar lv job gezi num1 num2 ret1 ret2
	GET_PLAYER_BASE_ATT 0 0 lv job
	GET_PACK_SPACE 0 gezi
	
	GET_ITEM_NUM_INPAK 5060302 0 num1
	GET_ITEM_NUM_INPAK 5060304 0 num2
	
	if lv < 70
		ADD_SYS_MSG_START
		ADD_SYS_MSG �һ�70��������ġ���������������ȼ���Ҫ�ﵽ70��
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	elseif num1 < 20
		ADD_SYS_MSG_START
		ADD_SYS_MSG �����ﲻ��20������ϼ�������ʱ�޷��һ�70��������ġ���������
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	elseif num2 < 300
		ADD_SYS_MSG_START
		ADD_SYS_MSG �����ﲻ��300�����ع����ʡ�����ʱ�޷��һ�70��������ġ���������
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	elseif @gezi < 1
		ADD_SYS_MSG_START
		ADD_SYS_MSG ������Ҫ1�����Ӳ�װ����Ŷ�������������ٶһ�
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	else
		DEC_ITEM_NUM_INPAK 5060302 20 ret1
		DEC_ITEM_NUM_INPAK 5060304 300 ret2
		if ret1 = 0 and ret2 = 0
			if job = 17
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 1016006 1 1 0
			endif
			if job = 18
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 1106006 1 1 0
			endif
			if job = 19
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 1206006 1 1 0
			endif
			if job = 20
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 1516006 1 1 0
			endif
			if job = 33
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 1036006 1 1 0
			endif
			if job = 34
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 1116006 1 1 0
			endif
			if job = 35
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 1256006 1 1 0
			endif
			if job = 36
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 1506006 1 1 0
			endif
		endif
	endif
endproc

proc 340
	choose 2 320 301 �һ���Ҫ����ϼ�20�����ع�����300��\n�Ƿ�ȷ�϶һ���
endproc

proc 320
	strvar lv job gezi num1 num2 ret1 ret2
	GET_PLAYER_BASE_ATT 0 0 lv job
	GET_PACK_SPACE 0 gezi
	
	GET_ITEM_NUM_INPAK 5060302 0 num1
	GET_ITEM_NUM_INPAK 5060304 0 num2
	
	if lv < 70
		ADD_SYS_MSG_START
		ADD_SYS_MSG �һ�70��������ġ���������������ȼ���Ҫ�ﵽ70��
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	elseif num1 < 20
		ADD_SYS_MSG_START
		ADD_SYS_MSG �����ﲻ��20������ϼ�������ʱ�޷��һ�70��������ġ���������
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	elseif num2 < 300
		ADD_SYS_MSG_START
		ADD_SYS_MSG �����ﲻ��300�����ع����ʡ�����ʱ�޷��һ�70��������ġ���������
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	elseif @gezi < 1
		ADD_SYS_MSG_START
		ADD_SYS_MSG ������Ҫ1�����Ӳ�װ����Ŷ�������������ٶһ�
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	else
		DEC_ITEM_NUM_INPAK 5060302 20 ret1
		DEC_ITEM_NUM_INPAK 5060304 300 ret2
		if ret1 = 0 and ret2 = 0
			if job = 17
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 1066006 1 1 0
			endif
			if job = 18
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 1186006 1 1 0
			endif
			if job = 19
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 1286006 1 1 0
			endif
			if job = 20
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 1586006 1 1 0
			endif
			if job = 33
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 1046006 1 1 0
				call 601
			endif
			if job = 34
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 1186016 1 1 0
			endif
			if job = 35
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 1266006 1 1 0
			endif
			if job = 36
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 1586016 1 1 0
			endif
		endif
	endif
endproc

proc 400
	OPEN_UI 92
endproc
