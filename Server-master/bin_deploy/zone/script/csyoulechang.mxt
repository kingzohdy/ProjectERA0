;���ֳ�
proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��ӭ���ټ����������գ��ҹ�˾���С��ͻ����ϡ��ĳ�ŵ��ͨ���ṩ���ɿ���׼�㡢���㡢��ݡ������ʷ����������㲢��Խ�ͻ�������������һ�Ȼ�����ְܾ䷢�ķ��а�ȫ��߽���
;	add_talk #b#b���Ϊ��л���ͻ���֧����񰮣��ҹ�˾�ؾٰ�����Żݻ������ʱ����ڴ���ҹ�˾�ĺ��࣬���۾���Զ����ֻ��Ҫ90���ң�
;	add_talk #b#bֻ��90���ң����Ϳ������̴�����·�̣���û�п�������9ǧ������9�٣�ֻҪ90���ң�
	
	option 105 ʱ����ں���
	option 104 #���纽��

	talk
	
endproc
proc 105
	dialog 1
	add_talk $_npc$:
	add_talk #b#b���Ϊ��л���ͻ���֧����񰮣��ҹ�˾�ؾٰ�����Żݻ��������ҹ�˾��ʱ����ں��࣬����Զ�������Ͼ��ܵ��

	OPTION 50 #ȥ����֮��
	OPTION 60 #ȥ�ھŽ���
	OPTION 70 #ȥ���ɸ�
	OPTION 80 #ȥӢ�۹㳡

	talk
endproc
proc 104
	MAP_TRANS 1
endproc
proc 50
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 90 and @ret >= 90
		choose 1 101 99 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 90
		choose 1 101 99 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 101
	endif
endproc 
proc 99
	return
endproc

proc 101
	RIDE_OFF 1
	strvar p_ID Moy Ret RetB
	IS_CAN_BIANSHEN RetB 1
	if RetB = 1	
		DEC_MONEY_TEST 0 90 21 Moy
		if Moy = -1
			ADD_SYS_MSG_START
			ADD_SYS_MSG �������е����Ҳ��㣬�޷�������
			SET_SYS_MSG_MODE 1 0 0
			MSG_INFO
		else
			AUTOFLY 106 Ret
			if Ret = 0
				DEC_MONEY 0 90 21 10		
			endif
		endif

	endif
endproc
proc 60
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 90 and @ret >= 90
		choose 1 102 99 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 90
		choose 1 102 99 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 102
	endif
endproc 
proc 102
	RIDE_OFF 1
	strvar p_ID Moy Ret RetB
	IS_CAN_BIANSHEN RetB 1
	if RetB = 1	
		DEC_MONEY_TEST 0 90 21 Moy
		if Moy = -1
			ADD_SYS_MSG_START
			ADD_SYS_MSG �������е����Ҳ��㣬�޷�������
			SET_SYS_MSG_MODE 1 0 0
			MSG_INFO
		else
			AUTOFLY 108 Ret
			if Ret = 0
				DEC_MONEY 0 90 21 10		
			endif
		endif

	endif
endproc
proc 70
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 90 and @ret >= 90
		choose 1 103 99 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 90
		choose 1 103 99 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 103
	endif
endproc 
proc 103
	RIDE_OFF 1
	strvar p_ID Moy Ret RetB
	IS_CAN_BIANSHEN RetB 1
	if RetB = 1	
		DEC_MONEY_TEST 0 90 21 Moy
		if Moy = -1
			ADD_SYS_MSG_START
			ADD_SYS_MSG �������е����Ҳ��㣬�޷�������
			SET_SYS_MSG_MODE 1 0 0
			MSG_INFO
		else
			AUTOFLY 201 Ret
			if Ret = 0
				DEC_MONEY 0 90 21 10		
			endif
		endif

	endif
endproc

proc 80
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 50 and @ret >= 50
		choose 1 106 99 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 50
		choose 1 106 99 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 106
	endif
endproc 
proc 106
	
	RIDE_OFF 1
	strvar RetB Moy
	IS_CAN_BIANSHEN RetB 1
	if RetB = 1
		strvar mid
		DEC_MONEY_TEST 0 50 21 Moy
		if Moy = -1
			ADD_SYS_MSG_START
			ADD_SYS_MSG �������е����Ҳ��㣬�޷�������
			SET_SYS_MSG_MODE 1 0 0
			MSG_INFO
		else
			GET_PLAYER_ID 1 mid
			TRANSMIT_CAMERA @mid 90 1000 23500 33500 0 0
			DEC_MONEY 0 50 21 10
		endif
	endif
endproc