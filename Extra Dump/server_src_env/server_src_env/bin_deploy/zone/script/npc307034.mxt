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
	rand zhi2 100
	if zhi2 < 50
		if @num < 6
			ADD_SYS_MSG_START
			ADD_SYS_MSG ������Ҫ6�����Ӳ�װ����Ŷ������������������ȡ����
			SET_SYS_MSG_MODE 3 0 0
			MSG_INFO
		else
			boss_baoxiang_touch_set ret
			if ret = 1
				if zhi2 < 40
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5250101 1 1 0
					rand zhi 100
					if zhi < 40
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052181 1 1 0
					endif
					if zhi >= 40 and zhi < 95
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052172 1 1 0
					endif
					if zhi >= 95
						ADD_ITEM_NUM_INPAK_NOLIMIT 5052191 1 1 0
						call 102
					endif
					ADD_SYS_MSG_START
					ADD_SYS_MSG ����������Ų����ҵ���һС�ް�����һЩǿ��ʯ����Ȼ���Ӻ�ս���䣡
					SET_SYS_MSG_MODE 3 0 0
					MSG_INFO
				endif
				if zhi2 >= 40 and zhi2 < 50
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5250102 1 1 0
					rand zhi 100
					if zhi < 40
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052181 1 1 0
					endif
					if zhi >= 40 and zhi < 95
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052172 1 1 0
					endif
					if zhi >= 95
						ADD_ITEM_NUM_INPAK_NOLIMIT 5052191 1 1 0
						call 102
					endif
					ADD_SYS_MSG_START
					ADD_SYS_MSG ����������Ų����ҵ���һС�ް�����һЩǿ��ʯ��������Ӻ�ս���䣡
					SET_SYS_MSG_MODE 3 0 0
					MSG_INFO
				endif
				
				call 101
			endif
		endif
	else
		if @num < 6
			ADD_SYS_MSG_START
			ADD_SYS_MSG ������Ҫ6�����Ӳ�װ����Ŷ������������������ȡ����
			SET_SYS_MSG_MODE 3 0 0
			MSG_INFO
		else
			boss_baoxiang_touch_set ret
			if ret = 1
				rand zhi 100
				if zhi < 40
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052181 1 1 0
				endif
				if zhi >= 40 and zhi < 95
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052172 1 1 0
				endif
				if zhi >= 95
					ADD_ITEM_NUM_INPAK_NOLIMIT 5052191 1 1 0
					call 102
				endif
				ADD_SYS_MSG_START
				ADD_SYS_MSG ����������Ų����ҵ���һС�ް�����һЩǿ��ʯ��ս���䣡
				SET_SYS_MSG_MODE 3 0 0
				MSG_INFO
				
				call 101
			endif	
		endif
	endif
endproc

proc 101
	strvar val con chongzhu xilian xinghui
	
	ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052815 1 1 0
		
	GET_ID_PLAYER_INFO 0 19 con	
	if con != -1
		var val @con
		dev val 10000
		mul val 140
		ADD_EXP 0 @val 0
	endif
	
	rand chongzhu 5
		if chongzhu = 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200031 1 1 0
		endif
		if chongzhu = 1
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200031 2 1 0
		endif
		if chongzhu = 2
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200031 3 1 0
		endif
		if chongzhu = 3
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200031 4 1 0
		endif
		if chongzhu = 4
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200031 5 1 0
		endif
		
	rand xilian 5
		if xilian = 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200041 1 1 0
		endif
		if xilian = 1
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200041 2 1 0
		endif
		if xilian = 2
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200041 3 1 0
		endif
		if xilian = 3
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200041 4 1 0
		endif
		if xilian = 4
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200041 5 1 0
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
endproc

proc 102
	ADD_SYS_MSG_START
	ADD_SYS_MSG $_player$�ӡ��������Ų������ҵ���һ��Ӣ�۵�ս���䣡
	SET_SYS_MSG_MODE 18 0 0
	SVR_MSG_INFO
endproc
