

proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b�����������ű��������ߡ����ߵ����Ʋ���
	add_talk #b#b
	add_talk #b#b#c186506Ϊ�˸�л��ҵ�Ŭ����ÿ�˿ɴӡ������߱��䡱����ȡһ�ν���#n
	
		strvar NumRet
		GET_PLAYER_DAILY_FLAG 1 8 NumRet
		if NumRet = 0
			option 150 #��ȡ����
		endif
	talk
endproc 

proc 150 
	strvar lv num
	GET_PLAYER_BASE_ATT 1 1 lv 1
	GET_PACK_SPACE 0 num
	if lv <= 40
		if @num < 2
			ADD_SYS_MSG_START
			ADD_SYS_MSG ������Ҫ2�����Ӳ�װ����Ŷ������������������ȡ����
			SET_SYS_MSG_MODE 3 0 0
			MSG_INFO
		else
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200030 10 1 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200040 10 1 0
			
			SET_PLAYER_DAILY_FLAG 0 8 1
		endif
	endif
	; if lv > 10 and lv <= 20
	; endif
	; if lv > 20 and lv <= 30
	; endif
	; if lv > 30 and lv <= 40
	; endif
	if lv > 40 and lv <= 60
		if @num < 2
			ADD_SYS_MSG_START
			ADD_SYS_MSG ������Ҫ2�����Ӳ�װ����Ŷ������������������ȡ����
			SET_SYS_MSG_MODE 3 0 0
			MSG_INFO
		else
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200031 8 1 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200041 8 1 0
			
			SET_PLAYER_DAILY_FLAG 0 8 1
		endif
	endif

	if lv > 60 and lv <= 79
		if @num < 3
			ADD_SYS_MSG_START
			ADD_SYS_MSG ������Ҫ3�����Ӳ�װ����Ŷ������������������ȡ����
			SET_SYS_MSG_MODE 3 0 0
			MSG_INFO
		else
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200032 4 1 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200042 4 1 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200020 1 1 0
			
			SET_PLAYER_DAILY_FLAG 0 8 1
		endif
	endif

	if lv > 79 and lv <= 100
		if @num < 3
			ADD_SYS_MSG_START
			ADD_SYS_MSG ������Ҫ3�����Ӳ�װ����Ŷ������������������ȡ����
			SET_SYS_MSG_MODE 3 0 0
			MSG_INFO
		else
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200033 4 1 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200043 4 1 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200020 1 1 0
				
			SET_PLAYER_DAILY_FLAG 0 8 1
		endif
	endif
	
endproc 


