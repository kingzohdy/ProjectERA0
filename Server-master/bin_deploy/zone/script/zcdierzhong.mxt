proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b��֪��������򿴲����õ�ϸ�����ꡣ
	add_talk #b#b��ȥ��ϸ����������ս�Ĵ�������һ���ҵİ��ˣ�Ϊ��������ֻ�ܳ�ȥ������׬Ǯ�����Ѿ��ܾ�û�������ˡ�
	add_talk #b#b�����Ҫȥϸ�����꣬�鷳����ҿ������ò��ã�ֻҪ������Ҳ�á�
	
	;��һ�
	task_stat 4438 1 51
	if 51 = 2
		option 510 ����ο�ʡ�����������
	endif
	
	
	strvar RetTsk
	task_stat 3123 1 RetTsk
	if RetTsk = 2
		option 200 ����ʥ���Ϲ������������
	endif
	talk
	
endproc
proc 200 
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����Ҹ�лʥ���Ϲ�����������һ��������ʥ���������㣬$_player$��лл�㲻�����࣡
		option 201 #����ʥ������
	talk
endproc 
proc 201
	GET_ITEM_NUM_INPAK 5300844 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ��
		set_sys_msg_mode 1 0 0
		msg_info
	ENDIF
	if 11 >= 1
			
		dec_item_num_inpak 5300844 1 30
		if 30 != -1
			task_fini 3123 1
		endif
	endif
endproc 

;��һ�
proc 510
	GET_ITEM_NUM_INPAK 5052308 1 10
	STRVAR flag
	var flag 0
	if 10 > 0
		DEC_ITEM_NUM_INPAK 5052308 1 11
		if 11 = 0
			var flag 1
			task_fini 4438 1
			
			DIALOG 1
			add_talk $_npc$:
			add_talk #b#b��һֱ��Ϊ��������Ѿ�����������û�뵽�����˼ǵ��ң�
			option 511 #�����飬����ͻ��ս�����
			talk
		endif
	endif
	
	if flag = 0
		add_sys_msg_start
		add_sys_msg ���ı�����û�С�ο�����ʡ���
		set_sys_msg_mode 1 0 0
		msg_info
		PLAYER_CLOSE_DIALOG1 0 0
	endif
endproc