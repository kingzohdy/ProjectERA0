
;���촺�ڻ
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�������ռ�����Ӳ�ң�������ܸ���һЩ����Ը�����ҵı������㽻����
	add_talk #b#b#c186506��������ֻ��1��27��--2��10����ʱ��Ƕ���

		option 150 #(����)���̵�
	
	STRVAR chunjie
	GET_EVENT_STAT chunjie 302
	
	if chunjie = 1
		option 200 #�������ͺ���  �������ǻԡ�
	endif
	
	talk
endproc 

proc 150
	OPEN_NPC_SHOP 
endproc

proc 200
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#b#b �������ͺ����������ǻԡ�
	add_talk #b#b#b#b#b#b#b#c1865061��27��-2��10��#n
	add_talk #b#b
	add_talk #b ÿ���ۻ�����2Сʱ����ȡ1�Ρ��ǻԱ��ꡱ
	add_talk #b#b#b#c005ebeÿ�ο���ȡ���20�����ǻԱ��ꡱ��
	
	STRVAR chunjie
	GET_EVENT_STAT chunjie 302
	
	if chunjie = 1
		option 210 #��ϲ���ƣ��ǻ�����
	endif
	
	talk 
endproc

proc 210
	strvar num ol chunjie zhi
	GET_PACK_SPACE 0 num
	GET_DAYONLINE 0 ol
	GET_EVENT_STAT chunjie 302
	GET_PLAYER_DAILY_FLAG 0 5 zhi
	
	if chunjie = 1
		if zhi = 0
			if ol > 7200
				if @num < 1
					ADD_SYS_MSG_START
					ADD_SYS_MSG ��ճ�1�񱳰�������ǰ����ȡ
					SET_SYS_MSG_MODE 3 0 0
					MSG_INFO
				else
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200020 20 1 0
					
					set_player_daily_flag 0 5 1
					
					add_sys_msg_start
					add_sys_msg ɯɯ��������������20�����ǻԱ��ꡱ��
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			else
				var 10 7200
				sub 10 @ol
				dev 10 60
				add 10 1
				
				ADD_SYS_MSG_START
				ADD_SYS_MSG ɯɯ���ڳﱸ���������ǻԡ������У���$@10$���Ӻ���ǰ����ȡ
				SET_SYS_MSG_MODE 3 0 0
				MSG_INFO
			endif
		else
			ADD_SYS_MSG_START
			ADD_SYS_MSG ���������ȡ���ۻ�����2Сʱ�ġ��������ǻԡ����񣬲���̫̰��Ŷ
			SET_SYS_MSG_MODE 3 0 0
			MSG_INFO
		endif
	else
		add_sys_msg_start
		add_sys_msg ���ꡰ���ں��񡱻�Ѿ������ˣ���ʱ�޷���ȡ
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc
