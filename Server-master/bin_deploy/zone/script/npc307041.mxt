;�������Ų�

proc 1

	dialog 1
	strvar jilu ret
	boss_baoxiang_touch_get jilu 2
	GET_BAOXIANG_TOUCH_LIMIT ret
	
	add_talk $_npc$:
	add_talk #b#b���������������ҵ����Ų���˵��׼����Ѱ��ʲô�䱦��
	add_talk #b#b
	add_talk #b#b#c005ebe�����ɱ����ս�����˿���Ѱ�������Ų�#n
	
	if ret = -1
		add_talk #b#b
		add_talk #b#b#c186506��δ�����ɱ�������޷���Ѱ�����Ų�#n
	endif
	if jilu = 1
		add_talk #b#b#c186506���Ѿ���Ѱ���������Ų���#n
	endif

	if jilu = 0 and ret = 1
		option 100 #��Ѱ�������Ų�
	endif
	talk
endproc

proc 100
	strvar ret num zhi2 zhi
	GET_PACK_SPACE 0 num
	call 101
	
	rand zhi2 100
	if zhi2 < 38
		if @num < 8
			ADD_SYS_MSG_START
			ADD_SYS_MSG ������Ҫ8�����Ӳ�װ����Ŷ������������������ȡ����
			SET_SYS_MSG_MODE 3 0 0
			MSG_INFO
		else
			boss_baoxiang_touch_set ret
			if ret = 1
				if zhi2 < 30
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5250101 2 1 0
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052817 2 1 0
					call 103
				endif
				if zhi2 >= 30 and zhi2 < 38
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5250102 2 1 0
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052817 5 1 0
					call 103
				endif
			endif
		endif
	else
		if @num < 7
			ADD_SYS_MSG_START
			ADD_SYS_MSG ������Ҫ7�����Ӳ�װ����Ŷ������������������ȡ����
			SET_SYS_MSG_MODE 3 0 0
			MSG_INFO
		else
			boss_baoxiang_touch_set ret
			if ret = 1
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052817 1 1 0
				call 103
			endif	
		endif
	endif
endproc

proc 101
	strvar val con chongzhu xilian xinghui
	
	GET_ID_PLAYER_INFO 0 19 con	
	if con != -1
		var val @con
		dev val 10000
		mul val 170
		ADD_EXP 0 @val 0
	endif
	
	rand chongzhu 5
		if chongzhu = 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200033 1 1 0
		endif
		if chongzhu = 1
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200033 2 1 0
		endif
		if chongzhu = 2
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200033 3 1 0
		endif
		if chongzhu = 3
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200033 4 1 0
		endif
		if chongzhu = 4
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200033 5 1 0
		endif
		
	rand xilian 5
		if xilian = 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200043 1 1 0
		endif
		if xilian = 1
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200043 2 1 0
		endif
		if xilian = 2
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200043 3 1 0
		endif
		if xilian = 3
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200043 4 1 0
		endif
		if xilian = 4
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200043 5 1 0
		endif
		
	rand xinghui 5
		if xinghui = 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200020 1 1 0
		endif
		if xinghui = 1
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200020 2 1 0
		endif
		if xinghui = 2
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200020 3 1 0
		endif
		if xinghui = 3
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200020 4 1 0
		endif
		if xinghui = 4
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200020 5 1 0
		endif
	
		ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052196 1 1 0
		ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5060117 1 1 0
	
	ADD_SYS_MSG_START
	ADD_SYS_MSG ����������Ų����ҵ�����ϡ���䱦��
	SET_SYS_MSG_MODE 3 0 0
	MSG_INFO
	
endproc

proc 102
	ADD_SYS_MSG_START
	ADD_SYS_MSG $_player$�ӡ��������Ų������ҵ���һ��ս���䡾�꡿��
	SET_SYS_MSG_MODE 18 0 0
	SVR_MSG_INFO
endproc

proc 103
	strvar zhi
	rand zhi 1000
		if zhi < 400
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5053044 1 1 0
		endif
		if zhi >= 400 and zhi < 945
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5060005 1 1 0
		endif
		if zhi >= 945 and zhi < 950
			ADD_ITEM_NUM_INPAK_NOLIMIT 5052177 1 1 0
		call 102
		endif
		if zhi >= 950 and zhi < 970
			ADD_ITEM_NUM_INPAK_NOLIMIT 5060300 1 1 0
		endif
		if zhi >= 970
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5060301 1 1 0
		endif
endproc
