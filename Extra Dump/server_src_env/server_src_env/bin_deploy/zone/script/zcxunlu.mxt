
proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��ඣ�ʥ���ڿ����ٲ���װ��һ��Ӵ��
	add_talk #b#bÿ�����ʱ��ʥ���Ϲ�����Ϊ���׼���˺ܶ�ܶ�ʥ��װ�磡
	add_talk #b#b�����ҲҪװ��һ������Ҫ����ÿ��������ȡ���俩���ٺ٣�
	add_talk #b#b����ҲҪ�����������ı�װ�ɶԣ���Ү��
	
		option 100 #��ʥ����װ�ɶԡ�
		
	talk
endproc

proc 100
	dialog 1
	get_player_daily_flag 20 20 21
	get_player_daily_flag 30 21 31
	get_player_daily_flag 40 22 41
	STRVAR time
	GET_DAYONLINE 0 time
	
	add_talk #b#b#b#b#b#b#b#c6d00d2��ʥ����װ�ɶԡ�#n
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b ��#c186506ʥ��������#n��
	add_talk  ÿ���ۻ�����#c1865061��2��4Сʱ#n�ɷֱ���ȡ1������
	add_talk #b �����䣺���л����á�#c005ebeʥ��ʱװ#n������
	
	if 40 = 0 and 41 = 1
		add_talk #b#b
		add_talk #b#c005ebe��������ȡ��3�����䣬����ǵ�������ȡӴ��#n
	endif
	
	
	if 20 = 0 and 21 = 0
		option 101 #��ȡ��ʥ�������䡱���ۻ�����1Сʱ��
	endif
	if 30 = 0 and 31 = 0
		if time > 3600
			option 102 #��ȡ��ʥ�������䡱���ۻ�����2Сʱ��
		endif
	endif
	if 40 = 0 and 41 = 0
		if time > 7200
			option 103 #��ȡ��ʥ�������䡱���ۻ�����4Сʱ��
		endif
	endif
		
	talk
endproc

proc 101
	STRVAR time ret num ret2
	GET_DAYONLINE 0 time
	
	if time > 3600
		get_player_daily_flag ret 20 num
		if ret = 0 and num = 0
			GET_PACK_SPACE 0 num
			if @num > 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5012107 3 1 ret2
				
				strvar Mid
				GET_PLAYER_ID 0 Mid
				EFFECT_ID 0 @Mid playMontion(%d,"201800_5@reset")
				
				add_sys_msg_start
				add_sys_msg ѱ¹������������ĵݸ���3����ʥ�������䡱
				set_sys_msg_mode 3 0 0
				msg_info
				if ret2 = 0
					var num 1
					SET_PLAYER_DAILY_FLAG 0 20 @num
				endif
			else
				ADD_SYS_MSG_START
				ADD_SYS_MSG ��ճ�1�񱳰�������ǰ����ȡ
				SET_SYS_MSG_MODE 3 0 0
				MSG_INFO
			endif
		else
			add_sys_msg_start
			add_sys_msg ���������ȡ���ۻ�����1Сʱ�ġ�ʥ�������䡱�������
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		var 10 3600
		sub 10 @time
		dev 10 60
		ADD_SYS_MSG_START
		ADD_SYS_MSG ѱ¹�����������������У���$@10$���Ӻ���ǰ����ȡ
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	endif
endproc

proc 102
	STRVAR time ret num ret2
	GET_DAYONLINE 0 time
	
	if time > 7200
		get_player_daily_flag ret 21 num
		if ret = 0 and num = 0
			GET_PACK_SPACE 0 num
			if @num > 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5012107 4 1 ret2
				
				strvar Mid
				GET_PLAYER_ID 0 Mid
				EFFECT_ID 0 @Mid playMontion(%d,"201800_5@reset")
				
				add_sys_msg_start
				add_sys_msg ѱ¹������������ĵݸ���4����ʥ�������䡱
				set_sys_msg_mode 3 0 0
				msg_info
				if ret2 = 0
					var num 1
					SET_PLAYER_DAILY_FLAG 0 21 @num
				endif
			else
				ADD_SYS_MSG_START
				ADD_SYS_MSG ��ճ�1�񱳰�������ǰ����ȡ
				SET_SYS_MSG_MODE 3 0 0
				MSG_INFO
			endif
		else
			add_sys_msg_start
			add_sys_msg ���������ȡ���ۻ�����2Сʱ�ġ�ʥ�������䡱�������
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		var 10 7200
		sub 10 @time
		dev 10 60
		ADD_SYS_MSG_START
		ADD_SYS_MSG ѱ¹�����������������У���$@10$���Ӻ���ǰ����ȡ
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	endif
endproc

proc 103
	STRVAR time ret num ret2
	GET_DAYONLINE 0 time
	
	if time > 14400
		get_player_daily_flag ret 22 num
		if ret = 0 and num = 0
			GET_PACK_SPACE 0 num
			if @num > 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5012107 5 1 ret2
				
				strvar Mid
				GET_PLAYER_ID 0 Mid
				EFFECT_ID 0 @Mid playMontion(%d,"201800_5@reset")
				
				add_sys_msg_start
				add_sys_msg ѱ¹������������ĵݸ���5����ʥ�������䡱
				set_sys_msg_mode 3 0 0
				msg_info
				if ret2 = 0
					var num 1
					SET_PLAYER_DAILY_FLAG 0 22 @num
				endif
			else
				ADD_SYS_MSG_START
				ADD_SYS_MSG ��ճ�1�񱳰�������ǰ����ȡ
				SET_SYS_MSG_MODE 3 0 0
				MSG_INFO
			endif
		else
			add_sys_msg_start
			add_sys_msg ���������ȡ���ۻ�����4Сʱ�ġ�ʥ�������䡱�������
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		var 10 14400
		sub 10 @time
		dev 10 60
		ADD_SYS_MSG_START
		ADD_SYS_MSG ѱ¹�����������������У���$@10$���Ӻ���ǰ����ȡ
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	endif
endproc
