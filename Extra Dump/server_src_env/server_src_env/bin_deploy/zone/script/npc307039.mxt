;�������Ų�

proc 1

	dialog 1
	strvar jilu ret
	boss_baoxiang_touch_get jilu 2
	GET_BAOXIANG_TOUCH_LIMIT ret
	
	add_talk $_npc$:
	add_talk #b#b�ռ����Ҿ����������ҵ����Ų���˵��׼����Ѱ��ʲô�䱦��
	add_talk #b#b
	add_talk #b#b#c005ebe�����ɱ�ռ����Ҿ��޵��˿���Ѱ�������Ų�#n
	
	if ret = -1
		add_talk #b#b
		add_talk #b#b#c186506��δ�����ɱս�����޷���Ѱ�����Ų�#n
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
	strvar ret num suiji chongzhu xilian xinghui lv
	GET_PACK_SPACE 0 num

	if @num < 1
		ADD_SYS_MSG_START
		ADD_SYS_MSG ������Ҫ1�����Ӳ�װ����Ŷ������������������ȡ����
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	else
		boss_baoxiang_touch_set ret
		if ret = 1
			GET_PLAYER_BASE_ATT 0 0 lv 0
			rand suiji 3
			if suiji = 0
				if lv < 40
					rand chongzhu 5
					if chongzhu = 0
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200030 1 1 0
					endif
					if chongzhu = 1
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200030 2 1 0
					endif
					if chongzhu = 2
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200030 3 1 0
					endif
					if chongzhu = 3
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200030 4 1 0
					endif
					if chongzhu = 4
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200030 5 1 0
					endif
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG ����������Ų����ҵ�����������һЩ����ʯ�ͷḻ�ľ��飡
					SET_SYS_MSG_MODE 3 0 0
					MSG_INFO
				endif
				if lv >= 40 and lv < 60
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
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG ����������Ų����ҵ�����������һЩ����ʯ�ͷḻ�ľ��飡
					SET_SYS_MSG_MODE 3 0 0
					MSG_INFO
				endif
				if lv >= 60 and lv < 80
					rand chongzhu 5
					if chongzhu = 0
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200032 1 1 0
					endif
					if chongzhu = 1
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200032 2 1 0
					endif
					if chongzhu = 2
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200032 3 1 0
					endif
					if chongzhu = 3
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200032 4 1 0
					endif
					if chongzhu = 4
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200032 5 1 0
					endif
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG ����������Ų����ҵ�����������һЩ����ʯ�ͷḻ�ľ��飡
					SET_SYS_MSG_MODE 3 0 0
					MSG_INFO
				endif
				if lv >= 80
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
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG ����������Ų����ҵ�����������һЩ����ʯ�ͷḻ�ľ��飡
					SET_SYS_MSG_MODE 3 0 0
					MSG_INFO
				endif
			endif
			
			if suiji = 1
				if lv < 40
					rand chongzhu 5
					if chongzhu = 0
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200040 1 1 0
					endif
					if chongzhu = 1
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200040 2 1 0
					endif
					if chongzhu = 2
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200040 3 1 0
					endif
					if chongzhu = 3
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200040 4 1 0
					endif
					if chongzhu = 4
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200040 5 1 0
					endif
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG ����������Ų����ҵ�����������һЩϴ��ʯ�ͷḻ�ľ��飡
					SET_SYS_MSG_MODE 3 0 0
					MSG_INFO
				endif
				if lv >= 40 and lv < 60
					rand chongzhu 5
					if chongzhu = 0
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200041 1 1 0
					endif
					if chongzhu = 1
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200041 2 1 0
					endif
					if chongzhu = 2
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200041 3 1 0
					endif
					if chongzhu = 3
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200041 4 1 0
					endif
					if chongzhu = 4
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200041 5 1 0
					endif
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG ����������Ų����ҵ�����������һЩϴ��ʯ�ͷḻ�ľ��飡
					SET_SYS_MSG_MODE 3 0 0
					MSG_INFO
				endif
				if lv >= 60 and lv < 80
					rand chongzhu 5
					if chongzhu = 0
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200042 1 1 0
					endif
					if chongzhu = 1
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200042 2 1 0
					endif
					if chongzhu = 2
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200042 3 1 0
					endif
					if chongzhu = 3
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200042 4 1 0
					endif
					if chongzhu = 4
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200042 5 1 0
					endif
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG ����������Ų����ҵ�����������һЩϴ��ʯ�ͷḻ�ľ��飡
					SET_SYS_MSG_MODE 3 0 0
					MSG_INFO
				endif
				if lv >= 80
					rand chongzhu 5
					if chongzhu = 0
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200043 1 1 0
					endif
					if chongzhu = 1
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200043 2 1 0
					endif
					if chongzhu = 2
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200043 3 1 0
					endif
					if chongzhu = 3
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200043 4 1 0
					endif
					if chongzhu = 4
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200043 5 1 0
					endif
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG ����������Ų����ҵ�����������һЩϴ��ʯ�ͷḻ�ľ��飡
					SET_SYS_MSG_MODE 3 0 0
					MSG_INFO
				endif
			endif
			
			if suiji = 2
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
				
				ADD_SYS_MSG_START
				ADD_SYS_MSG ����������Ų����ҵ�����������һЩ�ǻԱ���ͷḻ�ľ��飡
				SET_SYS_MSG_MODE 3 0 0
				MSG_INFO
			endif	
			call 101
		endif
	endif
endproc

proc 101
	strvar val con money
	
	GET_ID_PLAYER_INFO 0 19 con	
	if con != -1
		var val @con
		dev val 10000
		mul val 160
		ADD_EXP 0 @val 0
	endif
	
	rand money 5000
	add money 5000
	ADD_MONEY 0 @money 21 0
endproc
