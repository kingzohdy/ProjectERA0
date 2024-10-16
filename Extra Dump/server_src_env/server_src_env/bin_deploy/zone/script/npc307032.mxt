;领主的遗产

proc 1

	dialog 1
	strvar jilu ret
	boss_baoxiang_touch_get jilu 2
	
	GET_BAOXIANG_TOUCH_LIMIT ret
	
	add_talk $_npc$:
	add_talk #b#b世界领主死亡后找到的遗产，说不准能搜寻出什么珍宝。
	add_talk #b#b
	add_talk #b#b#c005ebe参与击杀领主战斗的人可搜寻领主的遗产#n
	
	if ret = -1
		add_talk #b#b
		add_talk #b#b#c186506你未参与击杀领主，无法搜寻领主遗产#n
	endif
	if jilu = 1
		add_talk #b#b#c186506你已经搜寻过领主的遗产了#n
	endif

	if jilu = 0 and ret = 1
		option 100 #搜寻领主的遗产
	endif
	talk
endproc

proc 100
	strvar ret num zhi2 zhi
	GET_PACK_SPACE 0 num
	
	rand zhi2 100
	if zhi2 < 38
		if @num < 6
			ADD_SYS_MSG_START
			ADD_SYS_MSG 背包需要6个格子才装得下哦！请先整理背包再来领取奖励
			SET_SYS_MSG_MODE 3 0 0
			MSG_INFO
		else
			boss_baoxiang_touch_set ret
			if ret = 1
				if zhi2 < 30
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5250101 1 1 0
					rand zhi 1000
					if zhi < 920
						if zhi < 400
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052183 1 1 0
						endif
						if zhi >= 400 and zhi < 915
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052174 1 1 0
						endif
						if zhi >= 915 and zhi < 920
							ADD_ITEM_NUM_INPAK_NOLIMIT 5052193 1 1 0
							call 102
						endif
						ADD_SYS_MSG_START
						ADD_SYS_MSG 你从领主的遗产中找到了一小袋绑银、一些强化石、慧然果子和战备箱！
						SET_SYS_MSG_MODE 3 0 0
						MSG_INFO
					endif
					if zhi >= 920
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5060002 1 1 0
						
						ADD_SYS_MSG_START
						ADD_SYS_MSG 你从领主的遗产中找到了一小袋绑银、一些强化石、慧然果子和一块合金！
						SET_SYS_MSG_MODE 3 0 0
						MSG_INFO
					endif
				endif
				if zhi2 >= 30 and zhi2 < 38
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5250102 1 1 0
					rand zhi 1000
					if zhi < 920
						if zhi < 400
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052183 1 1 0
						endif
						if zhi >= 400 and zhi < 915
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052174 1 1 0
						endif
						if zhi >= 915 and zhi < 920
							ADD_ITEM_NUM_INPAK_NOLIMIT 5052193 1 1 0
							call 102
						endif
						ADD_SYS_MSG_START
						ADD_SYS_MSG 你从领主的遗产中找到了一小袋绑银、一些强化石、独悟果子和战备箱！
						SET_SYS_MSG_MODE 3 0 0
						MSG_INFO
					endif
					if zhi >= 920
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5060002 1 1 0
						
						ADD_SYS_MSG_START
						ADD_SYS_MSG 你从领主的遗产中找到了一小袋绑银、一些强化石、独悟果子和一块合金！
						SET_SYS_MSG_MODE 3 0 0
						MSG_INFO
					endif
				endif
				
				call 101
			endif
		endif
	else
		if @num < 6
			ADD_SYS_MSG_START
			ADD_SYS_MSG 背包需要6个格子才装得下哦！请先整理背包再来领取奖励
			SET_SYS_MSG_MODE 3 0 0
			MSG_INFO
		else
			boss_baoxiang_touch_set ret
			if ret = 1
				rand zhi 1000
				if zhi < 920
					if zhi < 400
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052183 1 1 0
					endif
					if zhi >= 400 and zhi < 915
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052174 1 1 0
					endif
					if zhi >= 915 and zhi < 920
						ADD_ITEM_NUM_INPAK_NOLIMIT 5052193 1 1 0
						call 102
					endif
					ADD_SYS_MSG_START
					ADD_SYS_MSG 你从领主的遗产中找到了一小袋绑银、一些强化石和战备箱！
					SET_SYS_MSG_MODE 3 0 0
					MSG_INFO
				endif
				if zhi >= 920
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5060002 1 1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG 你从领主的遗产中找到了一小袋绑银、一些强化石和一块合金！
					SET_SYS_MSG_MODE 3 0 0
					MSG_INFO
				endif
				
				call 101
			endif	
		endif
	endif
endproc

proc 101
	strvar val con chongzhu xilian xinghui
	
	ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052817 1 1 0
	
	GET_ID_PLAYER_INFO 0 19 con	
	if con != -1
		var val @con
		dev val 10000
		mul val 130
		ADD_EXP 0 @val 0
	endif
	
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
		
	rand xilian 5
		if xilian = 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200042 1 1 0
		endif
		if xilian = 1
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200042 2 1 0
		endif
		if xilian = 2
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200042 3 1 0
		endif
		if xilian = 3
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200042 4 1 0
		endif
		if xilian = 4
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200042 5 1 0
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
	ADD_SYS_MSG $_player$从“领主的遗产”中找到了一个英雄的战备箱！
	SET_SYS_MSG_MODE 18 0 0
	SVR_MSG_INFO
endproc
