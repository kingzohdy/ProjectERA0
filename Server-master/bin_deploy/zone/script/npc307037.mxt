;������ı���

proc 1

	dialog 1
	strvar jilu
	boss_baoxiang_touch_get jilu 1
	
	add_talk $_npc$:
	add_talk #b#b���������������ı��أ�˵��׼����Ѱ��ʲô�䱦��
	add_talk #b#b
	add_talk #b#b#c005ebe�¼���Ĺ�Ա�޷���Ѱ������ı���#n
	
	if jilu = 1
		add_talk #b#b#c186506���Ѿ���Ѱ��������ı�����#n
	endif

	if jilu = 0
		option 100 #��Ѱ������ı���
	endif
	talk
endproc

proc 100
	strvar ret num zhi lv ret1 pos zhi1
	GET_PACK_SPACE 0 num

	GET_PLAYER_BASE_ATT 0 0 lv 0
	if lv < 75
		if @num < 1
			ADD_SYS_MSG_START
			ADD_SYS_MSG ������Ҫ1�����Ӳ�װ����Ŷ������������������ȡ����
			SET_SYS_MSG_MODE 3 0 0
			MSG_INFO
		else
			boss_baoxiang_touch_set ret
			if ret = 1
				if lv < 50
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052172 1 1 0
				endif
				if lv >= 50 and lv < 60
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052173 1 1 0
				endif
				if lv >= 60 and lv < 70
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052174 1 1 0
				endif
					
				if lv >= 70 and lv < 75
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052175 1 1 0
				endif
			endif
		endif
	else
		PLAYER_CLAN_POS_INFO ret1 0 0 0 pos
		if ret1 = 0 and pos = 8
			if @num < 3
				ADD_SYS_MSG_START
				ADD_SYS_MSG ������Ҫ3�����Ӳ�װ����Ŷ������������������ȡ����
				SET_SYS_MSG_MODE 3 0 0
				MSG_INFO
			else
				boss_baoxiang_touch_set ret
				if ret = 1
					if lv >= 75 and lv < 80
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052175 1 1 0
					endif
					if lv >= 80
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052176 1 1 0
					endif
						
					rand zhi 100
					if zhi < 20
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5063000 3 1 0
					endif
					if zhi >= 20 and zhi < 40
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5063000 4 1 0
					endif
					if zhi >= 40 and zhi < 60
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5063000 5 1 0
					endif
					if zhi >= 60 and zhi < 80
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5063000 6 1 0
					endif
					if zhi >= 80
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5063000 7 1 0
					endif
					
					rand zhi1 100
					if zhi1 < 10
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5063001 1 1 0
						
						ADD_SYS_MSG_START
						ADD_SYS_MSG ��ӳ�����ı������ҵ���һЩ���Һ��ġ�һ��ս�����һ������֮Ѫ��
						SET_SYS_MSG_MODE 3 0 0
						MSG_INFO
					else
						ADD_SYS_MSG_START
						ADD_SYS_MSG ��ӳ�����ı������ҵ���һЩ���Һ��ĺ�һ��ս���䣡
						SET_SYS_MSG_MODE 3 0 0
						MSG_INFO
					endif
				endif
			endif
		else
			if @num < 2
				ADD_SYS_MSG_START
				ADD_SYS_MSG ������Ҫ2�����Ӳ�װ����Ŷ������������������ȡ����
				SET_SYS_MSG_MODE 3 0 0
				MSG_INFO
			else
				boss_baoxiang_touch_set ret
				if ret = 1
					if lv >= 75 and lv < 80
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052175 1 1 0
					endif
					if lv >= 80
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052176 1 1 0
					endif
						
					rand zhi 100
					if zhi < 20
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5063000 3 1 0
					endif
					if zhi >= 20 and zhi < 40
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5063000 4 1 0
					endif
					if zhi >= 40 and zhi < 60
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5063000 5 1 0
					endif
					if zhi >= 60 and zhi < 80
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5063000 6 1 0
					endif
					if zhi >= 80
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5063000 7 1 0
					endif
							
					ADD_SYS_MSG_START
					ADD_SYS_MSG ��ӳ�����ı������ҵ���һЩ���Һ��ĺ�һ��ս���䣡
					SET_SYS_MSG_MODE 3 0 0
					MSG_INFO
				endif
			endif
		endif
	endif
endproc
