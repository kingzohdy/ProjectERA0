

proc 1
	DIALOG 1
	add_talk #b#b��������ʱ�����õķ��ߣ����ţ������Ա���ͷ�����ɵµ����������ҵĿ������ԣ�������������һ��������������һ������ô�����Ҷ�һ����
	add_talk #b#b#c186506���۸�ְҵ�ķ���#n
	task_add_option 1091 1 101
	task_add_option 1092 1 102
	task_add_option 2013 1 105


;50��



	task_add_option 1029 1 108
	task_add_option 1030 1 109



	OPTION 103 #(����)���̵�

	task_stat 4488 1 20
	if 20 = 2
		option 200 #��������˰��ȶ�����һ������
	endif
	talk
endproc 


PROC 101
	task_do_option 1091 1
ENDPROC
PROC 102
	task_do_option 1092 1
ENDPROC
PROC 103
	OPEN_NPC_SHOP
ENDPROC

PROC 105 
	task_do_option 2013 1
ENDPROC

PROC 108
	task_do_option 1029 1
ENDPROC
PROC 109
	task_do_option 1030 1
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
	task_stat 4488 1 100
	if 100 = 2
		if m_ID = 5012123 or m_ID = 5012124
			switch m_ID
				case 5012123
					DEC_ITEM_NUM_BYIDX 5012123 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4488 1
					call 502
				endcase
				case 5012124
					DEC_ITEM_NUM_BYIDX 5012123 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4488 1
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
	if 2 = 1092	
		task_fini 1092 1
	endif
	if 2 = 2013	
		task_fini 2013 1
	endif
	if 2 = 2228	
		task_fini 2228 1
	endif
	if 2 = 1030	
		task_fini 1030 1
	endif

endproc
