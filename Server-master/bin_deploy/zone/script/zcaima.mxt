


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�͸��ң���ƣ���ĺ�ƶ���ģ�����һ��������ɺ����Ĵ��ڣ������������İ��ϱ������Ŀ�������Ⱥ�����޼ҿɹ鱥���粨�����ǣ�һ���͸��ң���վ��ʱ��ǵķ�����߾�����֮������ġ�
	add_talk #b#b
	add_talk #b#b����֮��������÷�ʽ��ͨ����ɱ�����ȡ���ɽ���Ĺ�����
	
;	strvar Ret 
;	task_stat 3676 1 Ret
;	if Ret = 2 
;		option 300 ��������ƾ֤��Ҫ��ǩ��
;	endif
	
	option 150 #(����)�����̵�
	task_stat 4496 1 20
	if 20 = 2
		option 200 #������֮���������˰�������һ������
	endif
	
	talk
	
endproc 	
proc 150
	OPEN_NPC_SHOP 
endproc

;proc 300
;	dialog 1
;	add_talk $_npc$:
;	add_talk #b#b�ҾͲµ��ǹ������ȡƾ֤��������ʱ�ǵ�С�ĵ㣬�����в�������Ʒ��
;		option 301 #�ó���ȡƾ֤�ð���ǩ��
;
;	talk
;endproc 

proc 301
	GET_ITEM_NUM_INPAK 5301003 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ��
		set_sys_msg_mode 1 0 0
		msg_info
	ENDIF
	if 11 >= 1
		dec_item_num_inpak 5301003 1 30
		if 30 != -1
			ADD_ITEM_NUM_INPAK 5300879 1 1 0
			
		endif
	endif
endproc


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
	task_stat 4496 1 100
	if 100 = 2
		if m_ID = 5012123 or m_ID = 5012124
			switch m_ID
				case 5012123
					DEC_ITEM_NUM_BYIDX 5012123 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4496 1
					call 502
				endcase
				case 5012124
					DEC_ITEM_NUM_BYIDX 5012124 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4496 1
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

