;npc������

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b������ô���׾ͱ��ҵ��˰����´���һ��Ҫ�Ҹ�˭Ҳ���ֲ����ҵĵط����������Ķ��أ��������룬�ꡭ�ҵúú����롭
	
	GET_PLAYER_DAILY_FLAG 0 12 10
	if 10 = 0
		option 200 ��˵���кö���Ҫ���ң�
	endif
	
	task_stat 4481 1 20
		if 20 = 2
			option 202 #��������ץ�����ˣ�
		endif
	talk
endproc 

proc 202
	task_fini 4481 1
endproc

proc 200
	dialog 1
	add_talk $_npc$:
	add_talk #b#b���㲻�㣬��β���������Ҫ�ٲ�һ�Σ�����㻹���ҵ��Ҿ��������������������������èè�Ļ����Ҿ�����5����Ҷ�����Ͼ�Ҫ��������ˣ�����ǰ����ӱز����ٵ�ԭ��֮һӴ��
	
	option 201 #�ðɣ���������Ȱ���Ҷ���ҡ�
	talk
endproc

proc 201
	GET_PLAYER_DAILY_FLAG 0 12 10
	if @10 = 1
	;����ȡ
		add_sys_msg_start
		add_sys_msg ����ȡ
		set_sys_msg_mode 4 0 0
		msg_info
		PLAYER_CLOSE_DIALOG1 0 0
		;strvar npcmid
		;GET_MAP_NPC_ID  302162 npcmid 1
		;MON_SPEECH  @npcmid 0 0 1 �ҽ����Ѿ���������Ҷ�ˣ��㶼�������棬�ߣ���Ҳ���������ˣ�
	else
		GET_PACK_SPACE 0 11
	
		if 11 < 1
			add_sys_msg_start
			add_sys_msg �����ռ䲻�㣡
			set_sys_msg_mode 4 0 0
			msg_info
		else	
			SET_PLAYER_DAILY_FLAG 0 12 1
			
			ADD_BIND_ITEM_NUM_INPAK 5060041 5 1 13
			add_sys_msg_start
			add_sys_msg ��������Ҷ
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
endproc
