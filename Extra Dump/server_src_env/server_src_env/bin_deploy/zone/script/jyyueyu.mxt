
;̽��

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b����<<Խ��>>û?�൱���ʰ�,���˼���,��Ҳ���Խ�һ��Խ����������,�ٺ�~,������ȻûԽ���������ţ��,������,ÿ������������,ÿ���Ѷȶ��������,�������շ�Ҳ�ǲ�һ����,��һ��1000����,�ڶ�����Ҫ5000����,��������Ҫ20000����!

	;��һ�
	task_stat 4439 1 51
	if 51 = 2
		option 510 ����ο�ʡ�����������
	endif
	
		get_yueyu_num 10 20
		if 10 = 0 and 20 = 0
			option 150 #��ҪԽ��
		endif
		if 10 = 0 and 20 = 1
			option 160 #��ҪԽ��
		endif
		if 10 = 0 and 20 = 2
			option 170 #��ҪԽ��
		endif

	talk
	
endproc 

proc 150
	GET_PLAYER_PKVAL_TYPE 0 50
	if 50 = 3
		;CHOOSE #����ֵ #yes�¼��� #no�¼��� #��ʾ��Ϣ
		choose 1 151 152 �����1��Խ��,��Ҫ֧��1000����,�Ƿ�ȷ��?
	else
		add_sys_msg_start
		add_sys_msg ����ɱ��ֵ����200�����������뿪����,����ҪԽ����!
		set_sys_msg_mode 3 0 0
		MSG_INFO 
	endif
endproc 
proc 152
	return
endproc
proc 151
	get_player_id 1 30
	DEC_MONEY_TEST @30 1000 20 40
	
	if 40 = 0
		GET_PLAYER_ID 1 30	
		dec_money @30 1000 20 35
		if 35 = 0
			add_yueyu_num 36
			if 36 = 0
				TRANSMIT_CAMERA @30 135 1000 26200 28800 0 0
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg ���Ҳ��㣡
		set_sys_msg_mode 4 0 0
		MSG_INFO 
	endif
endproc 

proc 160
	GET_PLAYER_PKVAL_TYPE 0 50
	if 50 = 3
		;CHOOSE #����ֵ #yes�¼��� #no�¼��� #��ʾ��Ϣ
		choose 1 161 152 �����2��Խ��,��Ҫ֧��5000����,�Ƿ�ȷ��?
	else
		add_sys_msg_start
		add_sys_msg ����ɱ��ֵ����200�����������뿪����,����ҪԽ����!
		set_sys_msg_mode 3 0 0
		MSG_INFO 
	endif
endproc 
proc 161
	get_player_id 1 30
	DEC_MONEY_TEST @30 5000 20 40
	if 40 = 0
		GET_PLAYER_ID 1 30	
		dec_money @30 5000 20 35
		if 35 = 0
			add_yueyu_num 36
			if 36 = 0
				TRANSMIT_CAMERA @30 135 1000 26200 28800 0 0
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg ���Ҳ��㣡
		set_sys_msg_mode 4 0 0
		MSG_INFO 
	endif
endproc
proc 170
	GET_PLAYER_PKVAL_TYPE 0 50
	if 50 = 3
		;CHOOSE #����ֵ #yes�¼��� #no�¼��� #��ʾ��Ϣ
		choose 1 171 152 �����3��Խ��,��Ҫ֧��20000����,�Ƿ�ȷ��?
	else
		add_sys_msg_start
		add_sys_msg ����ɱ��ֵ����200�����������뿪����,����ҪԽ����!
		set_sys_msg_mode 3 0 0
		MSG_INFO 
	endif
endproc 
proc 171
	get_player_id 1 30
	DEC_MONEY_TEST @30 20000 20 40
	if 40 = 0
		GET_PLAYER_ID 1 30	
		dec_money @30 20000 20 35
		if 35 = 0
			add_yueyu_num 36
			if 36 = 0
				TRANSMIT_CAMERA @30 135 1000 26200 28800 0 0
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg ���Ҳ��㣡
		set_sys_msg_mode 4 0 0
		MSG_INFO 
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
			task_fini 4439 1
			
			DIALOG 1
			add_talk $_npc$:
			add_talk #b#b���Ƕ�˵���Ƿ��ӣ��ҿ����ǲ��������ˣ����������࣡������ţ���Ⱥɵ����ô�������ŵļ�į����Ը������������һ������Ϸ��
			option 511 #�´Ρ����´�һ������
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