

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��Ȼ����ʦ�˶�����̩�����£����Ҵ�δ������װ�������������̽����
	add_talk #b#b�����������У��ҳ����ҵ���װ����⼼���⣬��������װ��ӵ��ֱ�ӽ���Ϊ���ߵ��Σ�����������װ���ںϵ�������
	add_talk #b#b
	add_talk #c186506װ�����ף���50�����ϵ�׿Խװ�����׳�ʷʫ��װ

;	add_talk #b#b
;	add_talk #b#b#c186506�ɽ���[װ������]
;	add_talk #b#b�ɽ���[װ�����]
	
	strvar s1
	task_stat 2258 1 s1
	if s1 = 2 or s1 = 0
		add_talk #c186506װ���ںϣ��������60����׿ԽƷ�����ϵ�װ���ں����ԣ���ɡ��ں�����!������[61��]�������#n
	else
		add_talk #c186506װ���ںϣ��������60����׿ԽƷ�����ϵ�װ���ں�����#n
	endif

	TASK_ADD_OPTION 2258 0 150
	
	option 120 #װ������
	option 100 #װ�����
	
	if s1 = 3
		option 300 #װ���ں�
	endif

	task_stat 4495 1 20
	if 20 = 2
		option 200 #��װ�����״�ʦ�ݽܷ�����һ������
	endif
	
	talk
endproc 

proc 100
	open_ui 57
endproc
proc 120
	open_ui 76
endproc 

proc 150
	TASK_DO_OPTION 2258 0
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
	task_stat 4495 1 100
	if 100 = 2
		if m_ID = 5012123 or m_ID = 5012124
			switch m_ID
				case 5012123
					DEC_ITEM_NUM_BYIDX 5012123 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4495 1
					call 502
				endcase
				case 5012124
					DEC_ITEM_NUM_BYIDX 5012124 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4495 1
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
proc 300
	open_ui 87
endproc
proc 502
	add_sys_msg_start
	add_sys_msg �ɹ��Ͻ���Ʒ���������
	set_sys_msg_mode 3 0 0
	msg_info
	PLAYER_CLOSE_DIALOG 0 0
endproc 


