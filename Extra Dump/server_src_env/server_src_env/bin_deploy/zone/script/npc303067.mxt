proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��ã���������ᡣ
	add_talk #b#bʱ������Ϊ�˹�����������ȥ̽�ա���������Դ�����ϻ������̻Ὠ����һϵ�еĽ�����ʩ��
	add_talk #b#b����һ����������������� #c186506Ұ��BOSS�͸���BOSS#n ���ɻ�ã�����Ϊһ�ֻ��Ҵ�����������潱��һҳ�С�
	add_talk #b#b�Ѽ��������Ȼ�������ﹺ�����ǵ���Ʒ�ɡ�
	add_talk #b#b���ߣ�Ը����Խ��Խǿ��
	
	strvar LV
	GET_PLAYER_BASE_ATT 1 1 LV 1
	
	if @LV < 70
		option 101 #(����)������������̵�
	endif
	if @LV >= 70 and @LV  <= 85
		option 102 #(����)������������̵�
	endif
	
		option 200 1�����ǻԱ��꡿�һ�1����������¶��
		option 210 10�����ǻԱ��꡿�һ�10����������¶��
		option 220 50�����ǻԱ��꡿�һ�50����������¶��
	
	talk
endproc

proc 101
	OPEN_SHOP_BY_ID 10000
	
	task_fini 2266 1
endproc
proc 102
	OPEN_SHOP_BY_ID 10001
	
	task_fini 2266 1
endproc

proc 200
	strvar ret unl
	PKG_IS_UNLOCK ret unl
	if ret = 0 and unl = 1
		choose 2 201 202 �������һ�1����������¶����Ҫ1�����ǻԱ��꡿�����Ƿ�ȷ�϶һ���
	endif
endproc

proc 201
	strvar ret1 num ret2 pmid num1
	GET_ITEM_NUM_INPAK 5200020 ret1 num
	GET_PACK_SPACE 0 num1
	

	if @num1 < 1
		ADD_SYS_MSG_START
		ADD_SYS_MSG ������Ҫ1�����Ӳ�װ����Ŷ�������������ٶһ�
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
		return
	endif
		
		
	if ret1 = 0 and num > 0
		DEC_ITEM_NUM_INPAK 5200020 1 ret2
		if ret2 = 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5233512 1 1 0
				
			add_sys_msg_start
			add_sys_msg ���ѳɹ��һ���1����������¶��
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg �����ϵ��ǻԱ��겻�㣬��ʱ�޷��һ�
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc

proc 210
	strvar ret unl
	PKG_IS_UNLOCK ret unl
	if ret = 0 and unl = 1
		choose 2 211 202 �������һ�10����������¶����Ҫ10�����ǻԱ��꡿�����Ƿ�ȷ�϶һ���
	endif
endproc

proc 211
	strvar ret1 num ret2 pmid num1
	GET_ITEM_NUM_INPAK 5200020 ret1 num
	GET_PACK_SPACE 0 num1
	
	if @num1 < 1
		ADD_SYS_MSG_START
		ADD_SYS_MSG ������Ҫ1�����Ӳ�װ����Ŷ�������������ٶһ�
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
		return
	endif
		
		
	if ret1 = 0 and num > 9
		DEC_ITEM_NUM_INPAK 5200020 10 ret2
		if ret2 = 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5233512 10 1 0
			
			add_sys_msg_start
			add_sys_msg ���ѳɹ��һ���10����������¶��
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg �����ϵ��ǻԱ��겻�㣬��ʱ�޷��һ�
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc

proc 220
	strvar ret unl
	PKG_IS_UNLOCK ret unl
	if ret = 0 and unl = 1
		choose 2 221 202 �������һ�50����������¶����Ҫ50�����ǻԱ��꡿�����Ƿ�ȷ�϶һ���
	endif
endproc

proc 221
	strvar ret1 num ret2 pmid num1
	GET_ITEM_NUM_INPAK 5200020 ret1 num
	GET_PACK_SPACE 0 num1
	
	if @num1 < 1
		ADD_SYS_MSG_START
		ADD_SYS_MSG ������Ҫ1�����Ӳ�װ����Ŷ�������������ٶһ�
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
		return
	endif
		
		
	if ret1 = 0 and num > 49
		DEC_ITEM_NUM_INPAK 5200020 50 ret2
		if ret2 = 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5233512 50 1 0
			
			add_sys_msg_start
			add_sys_msg ���ѳɹ��һ���50����������¶��
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg �����ϵ��ǻԱ��겻�㣬��ʱ�޷��һ�
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc
