;��Ұ���ֱ���

proc 1
	dialog 1
	strvar jilu ret
	boss_baoxiang_touch_get jilu 2
	
	GET_BAOXIANG_TOUCH_LIMIT ret
	
	add_talk $_npc$:
	add_talk #b#b���ְ�����ô�������չεĴ����Ʊ���
	add_talk #b#b
	add_talk #b#b#c005ebe�����ɱ����ս�����˿���Ѱ�������Ų�#n
	
	if ret = -1
		add_talk #b#b
		add_talk #b#b#c186506��δ�����ɱ�������޷���Ѱ�����Ʊ�#n
	endif
	if jilu = 1
		add_talk #b#b#c186506���Ѿ���Ѱ�������ĲƱ���#n
	endif

	if jilu = 0 and ret = 1
		option 100 #��Ѱ�����ĲƱ�
	endif
	talk
endproc

proc 100
	strvar ret num zhi2 zhi
	strvar taskflag
	var taskflag 0  ; 0��ʼ����1��ʾ100%��������2��ʾ������
	GET_PACK_SPACE 0 num

	TASK_STAT 4641 153 154
	if @154 = 2
		var taskflag 1  ;   100%������5301120
	endif
	
	if @taskflag = 1
		if @num < 2
			ADD_SYS_MSG_START
			ADD_SYS_MSG ������Ҫ2�����Ӳ�װ����Ŷ������������������ȡ����
			SET_SYS_MSG_MODE 3 0 0
			MSG_INFO
			return 	
		endif
	else
		if @num < 1
			ADD_SYS_MSG_START
			ADD_SYS_MSG ������Ҫ1�����Ӳ�װ����Ŷ������������������ȡ����
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
