;npc �±�

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����һ���±�����֪��˭�ܹ���
	add_talk #b#b�������������У�·��֮���񶮣�
	add_talk #b#b�����������أ�������ζ���ء�
	add_talk #b#b�������±���У���������ѽڣ�
	add_talk #b#b
	
	GET_PLAYER_BASE_ATT 13 0 12 0
	if @13 = 0 and @12 >= 40
		add_talk #b#b#c186506�����ڼ䣬ÿ�춼��������ȡ�±���У��������±������һ���ḻ��ʣ�����������Щ�±����⻹��һЩ����������֮����±�Ӵ��#n
		option 200 ��ȡ�±����
	elseif @13 = 0 and @12 < 40 
		add_talk #b#b#c186506�����ڻ�С������40��������������ȡ�±���аɣ������ͨ�������������������Ȼ���ˣ���ô��������ֶ����أ�����5����ϲ�±������ɣ�#n
		GET_PLAYER_DAILY_FLAG 0 15 22
		if @22 = 0
			option 101 ��ȡ��ϲ�±�
		endif
	else
	endif
		
		TASK_ADD_OPTION 4633 0 100
	talk
endproc 

proc 100
	TASK_DO_OPTION 4633 0
endproc

proc 101
	GET_PLAYER_DAILY_FLAG 0 15 11
	;0��ʾδ����
	if 11 = 1
		PLAYER_CLOSE_DIALOG1 0 0
		return
	endif
	
	strvar ret1
	GET_PACK_SPACE 0 ret1
	if @ret1 = 0
		add_sys_msg_start
		add_sys_msg �����ռ䲻�㣬�޷���ȡ
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif

	strvar Ret 
	SET_PLAYER_DAILY_FLAG Ret 15 1
	if Ret = 0
		ADD_BIND_ITEM_NUM_INPAK 5012135 5 1 14 
		add_sys_msg_start
		add_sys_msg ����ڿ��֣�
		set_sys_msg_mode 3 0 0
		msg_info
	endif
	PLAYER_CLOSE_DIALOG1 0 0
endproc

proc 200
	GET_DAYONLINE 1 10
	
	var 11 @10
	if 10 >= 3600
		dev 10 3600
		MOD 11 3600
		dev 11 60
	else
		var 10 0
		dev 11 60
	endif
	
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bÿ������6����������ʱ�䣬��������ʱ����ۼơ�
	add_talk #b#b
	add_talk #b#b�����������ʱ��Ϊ��$@10$ʱ$@11$��
	add_talk #b#b
	add_talk #b#b#c186506�����±�����������ɻ������
	add_talk #b#b��Ƥ�±�������������գ���ɺ��þ���
	add_talk #b#b��Բ�±�������ʱ�򿪼����������մ��ͽ���
	add_talk #b#b�����±����Զ��˻����#n
	
	GET_PLAYER_DAILY_FLAG 0 9 20
	GET_PLAYER_DAILY_FLAG 0 12 21
	GET_PLAYER_DAILY_FLAG 0 14 22
	
	if @20 = 0
		option 201 ��ȡ�����±����
	endif
	
	if @21 = 0
		option 202 ��ȡ������2Сʱ�±����
	endif
	
	if @22 = 0
		option 203 ��ȡ������4Сʱ�±����
	endif
		
	talk
endproc

;�±����x1
proc 201
	GET_PLAYER_DAILY_FLAG 0 9 11
	;0��ʾδ����
	if 11 = 1
		PLAYER_CLOSE_DIALOG1 0 0
		return
	endif
	
	strvar ret1
	GET_PACK_SPACE 0 ret1
	if @ret1 = 0
		add_sys_msg_start
		add_sys_msg �����ռ䲻�㣬�޷���ȡ
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif

	strvar Ret 
	SET_PLAYER_DAILY_FLAG Ret 9 1
	if Ret = 0
		ADD_BIND_ITEM_NUM_INPAK 5052118 1 1 14 
		add_sys_msg_start
		add_sys_msg �±�����Ѿ���������ı������򿪿�����ʲô��ζ�İɣ�
		set_sys_msg_mode 3 0 0
		msg_info
	endif
	PLAYER_CLOSE_DIALOG1 0 0
endproc

;�±����x4
proc 202
	GET_DAYONLINE 1 10
	
	GET_PLAYER_DAILY_FLAG 0 12 11
	;0��ʾδ����
	if 11 = 1
		PLAYER_CLOSE_DIALOG1 0 0
		return
	endif
	
	if 10 < 7200
		add_sys_msg_start
		add_sys_msg ��Ǹ������������ʱ�䲻��2Сʱ�����Ժ�������ȡ��
		set_sys_msg_mode 4 0 0
		msg_info
		return
	else
		strvar ret1
		GET_PACK_SPACE 0 ret1
		if @ret1 = 0
			add_sys_msg_start
			add_sys_msg �����ռ䲻�㣬�޷���ȡ
			set_sys_msg_mode 4 0 0
			msg_info
			return
		endif
	
		strvar Ret 
		SET_PLAYER_DAILY_FLAG Ret 12 1
		if Ret = 0
			ADD_BIND_ITEM_NUM_INPAK 5052118 4 1 14 
			add_sys_msg_start
			add_sys_msg �±�����Ѿ���������ı������򿪿�����ʲô��ζ�İɣ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		PLAYER_CLOSE_DIALOG1 0 0
	endif
endproc

;�±����x8+2
proc 203
	GET_DAYONLINE 1 10
	
	GET_PLAYER_DAILY_FLAG 0 14 11
	;0��ʾδ����
	if 11 = 1
		PLAYER_CLOSE_DIALOG1 0 0
		return
	endif
	
	if 10 < 14400
		add_sys_msg_start
		add_sys_msg ��Ǹ������������ʱ�䲻��4Сʱ�����Ժ�������ȡ��
		set_sys_msg_mode 4 0 0
		msg_info
		return
	else
		strvar ret1
		GET_PACK_SPACE 0 ret1
		if @ret1 = 0
			add_sys_msg_start
			add_sys_msg �����ռ䲻�㣬�޷���ȡ
			set_sys_msg_mode 4 0 0
			msg_info
			return
		endif
	
		strvar Ret 
		SET_PLAYER_DAILY_FLAG Ret 14 1
		if Ret = 0
			ADD_BIND_ITEM_NUM_INPAK 5052118 10 1 15
			add_sys_msg_start
			add_sys_msg �±�����Ѿ���������ı������򿪿�����ʲô��ζ�İɣ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		PLAYER_CLOSE_DIALOG1 0 0
	endif
endproc
