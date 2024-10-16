;绿野凶林宝箱

proc 1
	dialog 1
	strvar jilu ret
	boss_baoxiang_touch_get jilu 2
	
	GET_BAOXIANG_TOUCH_LIMIT ret
	
	add_talk $_npc$:
	add_talk #b#b凶林霸主这么多年来收刮的大量财宝。
	add_talk #b#b
	add_talk #b#b#c005ebe参与击杀霸主战斗的人可搜寻领主的遗产#n
	
	if ret = -1
		add_talk #b#b
		add_talk #b#b#c186506你未参与击杀霸主，无法搜寻霸主财宝#n
	endif
	if jilu = 1
		add_talk #b#b#c186506你已经搜寻过霸主的财宝了#n
	endif

	if jilu = 0 and ret = 1
		option 100 #搜寻霸主的财宝
	endif
	talk
endproc

proc 100
	strvar ret num zhi2 zhi
	strvar taskflag
	var taskflag 0  ; 0初始化，1表示100%给奖励，2表示给过了
	GET_PACK_SPACE 0 num

	TASK_STAT 4641 153 154
	if @154 = 2
		var taskflag 1  ;   100%给奖励5301120
	endif
	
	if @taskflag = 1
		if @num < 2
			ADD_SYS_MSG_START
			ADD_SYS_MSG 背包需要2个格子才装得下哦！请先整理背包再来领取奖励
			SET_SYS_MSG_MODE 3 0 0
			MSG_INFO
			return 	
		endif
	else
		if @num < 1
			ADD_SYS_MSG_START
			ADD_SYS_MSG 背包需要1个格子才装得下哦！请先整理背包再来领取奖励
			SET_SYS_MSG_MODE 3 0 0
			MSG_INFO
			return 
		endif
	endif
	
	rand zhi2 100
	if zhi2 < 85
		boss_baoxiang_touch_set ret
		if ret = 1
			ADD_BIND_ITEM_NUM_INPAK 5052817 1 1 0
			if @taskflag = 1
				ADD_BIND_ITEM_NUM_INPAK 5301120 1 1 0
			endif
		endif
	elseif zhi2 < 95
		boss_baoxiang_touch_set ret
		if ret = 1	
			rand zhi 100
			if @zhi < 50
				ADD_ITEM_NUM_INPAK 5233000 1 1 0
			else
				ADD_ITEM_NUM_INPAK 5233500 1 1 0
			endif
			
			if @taskflag = 1
				ADD_BIND_ITEM_NUM_INPAK 5301120 1 1 0
			endif
		endif	
	else
		boss_baoxiang_touch_set ret
		if ret = 1
			rand zhi 100
			if @zhi < 50
				ADD_ITEM_NUM_INPAK 5233000 1 1 0
			else
				ADD_ITEM_NUM_INPAK 5233500 1 1 0
			endif
			
			if @taskflag = 1
				ADD_BIND_ITEM_NUM_INPAK 5301120 1 1 0
			endif
		endif	
	endif
endproc
