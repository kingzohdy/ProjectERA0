;npc ����

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�ڹ�ȥ��һ���У����߱���������أ��ռ����������䱦�������Ƿֱ�װ��������ڴ˹���ѽ�֮�ʣ�����λ��ʿ���š�

		option 200 #���ֵ��̵�
		
		;task_stat 4635 20 30
		;if @20 = 0 and @30 = 2
			;option 201 #�һ�����ɨ����
		;endif
		
	talk
endproc 

proc 200
	OPEN_SHOP_BY_ID 77
endproc

proc 201
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��������ɨ����ֱ��������С�����һ����ɡ�
	add_talk #b#b�߼�����ɨ�������һ�������Ҫ����С�����֮�⻹��Ҫ#c1865065000����#n��
	add_talk #b#b��������ɨ�����ɷֱ�ڰ��������������������ǰ��������Ա������4�ˣ��ͻ��ٻ����߼������
	add_talk #b#b�߼�����ɨ�����ɷֱ�ڰ��������߼����������ǰ��������Ա������4�ˣ��ͻ��ٻ������Ƽ����
	add_talk #b#b��ɱ�߼�����ɻ��#186506Ӣ�ۼ����#n
	add_talk #b#b��ɱ���Ƽ���ɻ��#186506Ӣ�ۼ����#n��#cff4e00��ȯ#n
	add_talk #b#b����#c186506Ǳ��#n���񣬻���1������С�����
	
		option 202 �һ���������ɨ����(��Ҫ����)
		option 203 �һ��߼�����ɨ����(��Ҫ����+5000����)
		
	talk
endproc

proc 202
	strvar ret1
	GET_PACK_SPACE 0 ret1
	if @ret1 = 0
		add_sys_msg_start
		add_sys_msg �����ռ䲻�㣬�޷��һ�
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	GET_ITEM_NUM_INPAK 5060058 13 15
	if @13 = 0 and @15 >= 1
		DEC_ITEM_NUM_INPAK 5060058 1 14
		if @14 = 0
			ADD_BIND_ITEM_NUM_INPAK 5060059 1 1 16
			add_sys_msg_start
			add_sys_msg �����˳�������ɨ����
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg ������û������С����
		set_sys_msg_mode 3 0 0
		msg_info
	endif
	PLAYER_CLOSE_DIALOG1 0 0
endproc

proc 203
	strvar ret1
	GET_PACK_SPACE 0 ret1
	if @ret1 = 0
		add_sys_msg_start
		add_sys_msg �����ռ䲻�㣬�޷��һ�
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	DEC_MONEY_TEST 0 5000 20 18
	if 18 < 0
		add_sys_msg_start
		add_sys_msg �������Ҳ��㡣
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif	
	
	GET_ITEM_NUM_INPAK 5060058 13 15
	if @13 = 0 and @15 >= 1
		DEC_ITEM_NUM_INPAK 5060058 1 14
		DEC_MONEY 0 5000 20 12
		if @14 = 0 and @12 = 0
			ADD_BIND_ITEM_NUM_INPAK 5060049 1 1 16
			add_sys_msg_start
			add_sys_msg �����˸߼�����ɨ����
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg ������û������С����
		set_sys_msg_mode 3 0 0
		msg_info
	endif
	PLAYER_CLOSE_DIALOG1 0 0
endproc
