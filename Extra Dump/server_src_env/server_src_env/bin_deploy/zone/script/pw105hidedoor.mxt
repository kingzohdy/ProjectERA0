;70����,���ع�npc
proc 1
	;���ع�npc�Ի����,0δ������,1����������ͨ��
	get_pworld_var 44 32 1 21
	if 21 > 0
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b��������ˡ�
		talk
		return
	endif	
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���ƺ���Ҫ��ʲô��������ͨ����
	option 100 ����ʲô
        talk
endproc
proc 100
	DIALOG 3
	add_talk $_npc$:
	add_talk #b#b�������ʲô��
	NPC_COMMIT_OPT 101
        talk
	
endproc
proc 101
	
	strvar m_ID m_Pos1 m_Pos2 m_Col 
	GET_COMMIT_ITEM_INFO 1 m_ID
	GET_COMMIT_ITEM_INFO 2 m_Pos1
	GET_COMMIT_ITEM_INFO 3 m_Pos2
	GET_COMMIT_ITEM_INFO 4 m_Col
	if m_ID = -1
		add_sys_msg_start
		add_sys_msg �㻹û���ύ��Ʒ��
		set_sys_msg_mode 1 0 0
		msg_info
		return
	endif
	if m_ID = 5015005 
		DEC_ITEM_NUM_BYIDX @m_ID 1 @m_Pos1 @m_Pos2 1
		PLAYER_CLOSE_DIALOG 0 0
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b������˶������������ȥ��
		
		talk
		call 102
	else
		;����tick�õ�,��ֵΪ0�رմ˹���
		get_pworld_var 20 32 1 15
		if 15 > 0
			DEC_ITEM_NUM_BYIDX @m_ID 1 @m_Pos1 @m_Pos2 1
			PLAYER_CLOSE_DIALOG 0 0
			DIALOG 1
			add_talk $_npc$:
			add_talk #b#b��Ȼ��������˵㶫��������Ϊ�˲��ԣ��һ���������ɡ�
			call 102
			talk
		else
			PLAYER_CLOSE_DIALOG 0 0
			DIALOG 1
			add_talk $_npc$:
			add_talk #b#b������˶������Ҳ��������ȥ�ġ�
			
			talk
		endif
	endif
endproc
proc 102
	;���ع�npc�Ի����,0δ������,1����������ͨ��
	get_pworld_var 44 32 1 21
	if 21 = 0
		var 21 1
		set_pworld_var 44 32 @21 1
	endif	
endproc
