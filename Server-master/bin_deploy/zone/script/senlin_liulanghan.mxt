

proc 1
	dialog 1
	add_talk $_npc$:

		add_talk #b#b��Ϊһ��רҵ�����˺�,�ҵ�װ��Ҳ����רҵ��.��Խ��˵�е�Ϭ����,��������׷��!����˵������һ���ܹ������ҩˮ,����Ч���ǳ�Ϭ����.
		add_talk #b#b��������ܸ���һƿ����ҩˮ,��Ϊ����,�ҿ��԰���������Ԧ���ߵ�С����,�����ǿ��Ա���ܶ��鷳,ֱ����սԦ����.
		add_talk #b#b��Ȼ,ֱ�Ӹ�Ǯ�Ļ�,��Ҳ���ᷴ�Ե�.
	
		option 150 #�����˺�һƿ����ҩˮ 
		option 160 #ҩˮ̫�鷳,����ֱ�Ӹ���100���Ұ�
		option 170 #���Լ��ܸ㶨,���������!

	talk
	
endproc 
proc 150

	dialog 3
	add_talk $_npc$:
	add_talk #b#b���ύһƿ����ҩˮ
	add_talk #b#b

		NPC_COMMIT_OPT 151
		
	talk
endproc 
proc 151
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

	if m_ID = 5006100 or m_ID = 5006101 or m_ID = 5006102 or m_ID = 5006103 or m_ID = 5006106
		switch m_ID
			case 5006100
				DEC_ITEM_NUM_BYIDX 5006100 1 @m_Pos1 @m_Pos2 RetTask
				call 152
				PLAYER_CLOSE_DIALOG 0 0
			endcase
			case 5006101
				DEC_ITEM_NUM_BYIDX 5006101 1 @m_Pos1 @m_Pos2 RetTask
				call 152
				PLAYER_CLOSE_DIALOG 0 0
			endcase
			case 5006102
				DEC_ITEM_NUM_BYIDX 5006102 1 @m_Pos1 @m_Pos2 RetTask
				call 152
				PLAYER_CLOSE_DIALOG 0 0
			endcase
			case 5006103
				DEC_ITEM_NUM_BYIDX 5006103 1 @m_Pos1 @m_Pos2 RetTask
				call 152
				PLAYER_CLOSE_DIALOG 0 0
			endcase
			case 5006106
				DEC_ITEM_NUM_BYIDX 5006106 1 @m_Pos1 @m_Pos2 RetTask
				call 152
				PLAYER_CLOSE_DIALOG 0 0
			endcase
		endswitch
	else
	
		add_sys_msg_start
		add_sys_msg �۸������˼����񲻺�?������û�б���ҩˮ!
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc 
proc 152


	;���npcʵ��ð��
	strvar mid roc
	GET_MAP_NPC_ID 304012 mid 1
	mon_speech @mid 0 2 1 �����ȥ��Ԧ���ߵ�С�������~
	
	del_npc 304012 1
	;ɾnpc
	call 199
	;ɾ����
	get_pworld_var 101 8 1 roc
	var roc 2
	set_pworld_var 101 8 @roc 1
	
	get_pworld_var 104 8 1 44
	var 44 0
	set_pworld_var 104 8 @44 1

	;��ǣ�ֱ��ˢBOSS
	get_pworld_var 108 8 1 50 
	var 50 1
	set_pworld_var 108 8 @50 1
endproc
	
	
proc 160
	strvar moy
	DEC_MONEY_TEST 0 100 20 moy
	if moy = -1
		
		add_sys_msg_start
		add_sys_msg ����ûǮ,�ͱ�װ��ү!
		set_sys_msg_mode 3 0 0
		msg_info
	else
		dec_money 0 100 20 50
		if 50 = 0 
			call 152
		endif
	endif
	
endproc 
proc 170
	return
endproc
proc 199
	;ר��
	DELETE_MON_BY_DEFID 201276 10
	DELETE_MON_BY_DEFID 201281 1
	DELETE_MON_BY_DEFID 201280 1
	DELETE_MON_BY_DEFID 201279 1
	DELETE_MON_BY_DEFID 201278 1
	DELETE_MON_BY_DEFID 201277 1
	DELETE_MON_BY_DEFID 201282 1
	DELETE_MON_BY_DEFID 201283 1
	DELETE_MON_BY_DEFID 201284 1
	DELETE_MON_BY_DEFID 201285 1
	
	;����
	DELETE_MON_BY_DEFID 201206 10
	DELETE_MON_BY_DEFID 201215 1
	DELETE_MON_BY_DEFID 201211 1
	DELETE_MON_BY_DEFID 201210 1
	DELETE_MON_BY_DEFID 201209 1
	DELETE_MON_BY_DEFID 201208 1
	DELETE_MON_BY_DEFID 201207 1
	DELETE_MON_BY_DEFID 201212 1
	DELETE_MON_BY_DEFID 201213 1
	DELETE_MON_BY_DEFID 201214 1
	
	
	; add_sys_msg_start
	; add_sys_msg $@10$ 
	; set_sys_msg_mode 1 0 0
	; msg_info
endproc 






