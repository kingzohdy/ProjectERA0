;������Ŧ


proc 1 

	strvar Ret
	is_in_my_clan_city  Ret
	if Ret = 0 
		return
	endif
	DIALOG 1
;	add_talk $_npc$:
	add_talk #b#b���ǹ�����������������������Ŧ����ô����Ҳ�ò�����������
	add_talk #b#b������ŦҲ�����ṩһЩС���ܸ���Աʹ��
	add_talk #b#b
	add_talk #b#b������ó��д��;�����ģ�����Ҳ������ͻص�֮ǰ�ĵط���
	add_talk #b#b�������ħ����¯����Ҳ���Դ������ﴫ�͹�ȥ
		
		option 150 ���һ�ȥ��
		
		task_stat 3250 1 20
		task_stat 3251 1 21
		task_stat 3252 1 22
		task_stat 3253 1 23
		task_stat 3254 1 24
		if 20 = 2 or 21 = 2 or 22 = 2 or 23 = 2 or 24 = 2
			option 160 #��¯�ر�����
		endif
		strvar map
		HAS_STRONG_GLORY 30 map
		strvar mapid
		has_strong_glory 1 mapid
		
		if 30 = 0 and map = 2202 and mapid = 2202
			option 300 ���͵��������������ġ���֧��130������
		endif
		if 30 = 0 and map = 2200 and mapid = 2200
			option 301 ���͵�����ѩԭ���������ġ���֧��220������
		endif
		if 30 = 0 and map = 2300 and mapid = 2300
			option 302 ���͵�����ɳĮ���������ġ���֧��100������
		endif
		if 30 = 0 and map = 2400 and mapid = 2400
			option 303 ���͵����ջ�Ұ���������ġ���֧��250������
		endif
		if 30 = 0 and map = 2500 and mapid = 2500
			option 304 ���͵�Խ�����֡��������ġ���֧��350������
		endif
		if 30 = 0 and map = 3404 and mapid = 3404
			option 305 ���͵���ɳ�ݵء��������ġ���֧��810������
		endif
		
	talk
 endproc 
 proc 150
 	DIALOG 1
;	add_talk $_npc$:
	add_talk #b#b���������͵����빫�����֮ǰ�ĵط���ȷ��Ҫ��ȥ��
		option 151 #��Ҫ��ȥ
	talk
endproc
proc 151
	OUT_PWORLD_BY_SINGLE
endproc
 
proc 160
	DIALOG 1
;	add_talk $_npc$:
	add_talk #b#b������ȡ����¯������ô����������ﴫ�ͳ�ȥ������Ҫ����һ�㹱�׶�Ŷ����
	add_talk #b#b
	add_talk #c0181f9�㵱ǰ������ص�Ϊ��
	task_stat 3250 1 20
	if 20 = 2
		add_talk #b#b���ջ�Ұ����Ҫ����#c186506 3 #n#c0181f9�㹱�׶�
		option 170 #��Ҫȥɱа�����ڱ�	
		option 180 #��Ҫȥɱ��Ӹ�Ⱦ��
		
	endif
	task_stat 3251 1 30
	if 30 = 2
		add_talk #b#bԽ�����֣���Ҫ����#c186506 4 #n#c0181f9�㹱�׶�
		option 170 #��Ҫȥɱ�꼪սʿ
		option 180 #��Ҫȥɱ�����Ա�
	endif
	task_stat 3252 1 40
	if 40 = 2
		add_talk #b#b���ֵأ���Ҫ����#c186506 9 #n#c0181f9�㹱�׶�
		option 170 #��Ҫȥɱ��³��ħ��
		option 180 #��Ҫȥɱ����ʿ��
	endif
	task_stat 3253 1 50
	if 50 = 2
		add_talk #b#b����ѹȣ���Ҫ����#c186506 15 #n#c0181f9�㹱�׶�
		option 170 #��Ҫȥɱ�����
		option 180 #��Ҫȥɱ��˪�ƻ���
	endif
	task_stat 3254 1 60
	if 60 = 2
		add_talk #b#b������ԭ����Ҫ����#c186506 20 #n�㹱�׶�
		option 170 #��Ҫȥɱ����Ⱦ�Ĵ���
		option 180 #��Ҫȥɱ����Ⱦ�Ŀ�
	endif
	
		option 1 ����
	talk
endproc 
proc 170
	task_stat 3250 1 5
	if 5 = 2
		GET_ID_PLAYER_CONTRI 10 0 20
		if 10 = 0 and 20 >= 3
			ADD_ID_PLAYER_CONTRI 11 0 -3 0 0
			if 11 = 0
				TRANSMIT 2400 6100 15600 1
			endif
		else
			call 200
		endif
	endif
	
	task_stat 3251 1 5
	if 5 = 2
		GET_ID_PLAYER_CONTRI 10 0 20
		if 10 = 0 and 20 >= 4
			ADD_ID_PLAYER_CONTRI 11 0 -4 0 0
			if 11 = 0
				TRANSMIT 2500 13200 28200 1 
			endif
		else
			call 200
		endif
	endif
	task_stat 3252 1 5
	if 5 = 2
		GET_ID_PLAYER_CONTRI 10 0 20
		if 10 = 0 and 20 >= 9
			ADD_ID_PLAYER_CONTRI 11 0 -9 0 0
			if 11 = 0
				TRANSMIT 2104 18700 14400 1 
			endif
		else
			call 200
		endif
	endif
	task_stat 3253 1 5
	if 5 = 2
		GET_ID_PLAYER_CONTRI 10 0 20
		if 10 = 0 and 20 >= 15
			ADD_ID_PLAYER_CONTRI 11 0 -15 0 0
			if 11 = 0
				TRANSMIT 2201 9700 9000 1 
			endif
		else
			call 200
		endif
	endif
	task_stat 3254 1 5
	if 5 = 2
		GET_ID_PLAYER_CONTRI 10 0 20
		if 10 = 0 and 20 >= 20
			ADD_ID_PLAYER_CONTRI 11 0 -20 0 0
			if 11 = 0
				TRANSMIT 3402 15900 21300 1
			endif
		else
			call 200
		endif
	endif
endproc

proc 180
	task_stat 3250 1 5
	if 5 = 2
		GET_ID_PLAYER_CONTRI 10 0 20
		if 10 = 0 and 20 >= 3
			ADD_ID_PLAYER_CONTRI 11 0 -3 0 0
			if 11 = 0
				TRANSMIT 2400 16900 16600 1
			endif
		else
			call 200
		endif
	endif
	
	task_stat 3251 1 5
	if 5 = 2
		GET_ID_PLAYER_CONTRI 10 0 20
		if 10 = 0 and 20 >= 4
			ADD_ID_PLAYER_CONTRI 11 0 -4 0 0
			if 11 = 0
				TRANSMIT 2500 28100 30000 1 
			endif
		else
			call 200
		endif
	endif
	task_stat 3252 1 5
	if 5 = 2
		GET_ID_PLAYER_CONTRI 10 0 20
		if 10 = 0 and 20 >= 9
			ADD_ID_PLAYER_CONTRI 11 0 -9 0 0
			if 11 = 0
				TRANSMIT 2104 23600 11200 1 
			endif
		else
			call 200
		endif
	endif
	task_stat 3253 1 5
	if 5 = 2
		GET_ID_PLAYER_CONTRI 10 0 20
		if 10 = 0 and 20 >= 15
			ADD_ID_PLAYER_CONTRI 11 0 -15 0 0
			if 11 = 0
				TRANSMIT 2201 23100 33100 1 
			endif
		else
			call 200
		endif
	endif
	task_stat 3254 1 5
	if 5 = 2
		GET_ID_PLAYER_CONTRI 10 0 20
		if 10 = 0 and 20 >= 20
			ADD_ID_PLAYER_CONTRI 11 0 -20 0 0
			if 11 = 0
				TRANSMIT 3402 28000 6800 1
			endif
		else
			call 200
		endif
	endif
endproc

 proc 200
	ADD_SYS_MSG_START
	ADD_SYS_MSG ��Ĺ��׶Ȳ��㣬�޷����ͣ�
	SET_SYS_MSG_MODE 4 0 0
	MSG_INFO
 endproc 
 
proc 299

endproc 
proc 300
	GET_PLAYER_BASE_ATT 1 1 30 1
	if 30 < 10
		ADD_SYS_MSG_START
		ADD_SYS_MSG ����ȼ�С��10�����޷�����
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
		return
	endif
	
	
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 130 and @ret >= 130
		choose 1 310 299 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 130
		choose 1 310 299 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 310
	endif
endproc
proc 301
	GET_PLAYER_BASE_ATT 1 1 30 1
	if 30 < 10
		ADD_SYS_MSG_START
		ADD_SYS_MSG ����ȼ�С��10�����޷�����
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
		return
	endif
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 220 and @ret >= 220
		choose 1 311 299 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 220
		choose 1 311 299 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 311
	endif
endproc
proc 302
	GET_PLAYER_BASE_ATT 1 1 30 1
	if 30 < 10
		ADD_SYS_MSG_START
		ADD_SYS_MSG ����ȼ�С��10�����޷�����
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
		return
	endif
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 100 and @ret >= 100
		choose 1 312 299 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 100
		choose 1 312 299 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 312
	endif
endproc
proc 303
	GET_PLAYER_BASE_ATT 1 1 30 1
	if 30 < 10
		ADD_SYS_MSG_START
		ADD_SYS_MSG ����ȼ�С��10�����޷�����
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
		return
	endif
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 250 and @ret >= 250
		choose 1 313 299 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 250
		choose 1 313 299 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 313
	endif
endproc
proc 304
	GET_PLAYER_BASE_ATT 1 1 30 1
	if 30 < 10
		ADD_SYS_MSG_START
		ADD_SYS_MSG ����ȼ�С��10�����޷�����
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
		return
	endif
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 350 and @ret >= 350
		choose 1 314 299 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 350
		choose 1 314 299 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 314
	endif
endproc
proc 305
	GET_PLAYER_BASE_ATT 1 1 30 1
	if 30 < 10
		ADD_SYS_MSG_START
		ADD_SYS_MSG ����ȼ�С��10�����޷�����
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
		return
	endif
	if 30 < 70
		
		ADD_SYS_MSG_START
		ADD_SYS_MSG ����ȼ�С�ڵ�ͼ�ȼ����ƣ��޷�����
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
		return
	endif
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 810 and @ret >= 810
		choose 1 315 299 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 810
		choose 1 315 299 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 315
	endif
endproc



proc 310
	strvar Moy
	DEC_MONEY_TEST 0 130 21 Moy
	if Moy = -1
		ADD_SYS_MSG_START
		ADD_SYS_MSG ������Ҳ��㣬�޷����ͣ�
		SET_SYS_MSG_MODE 4 0 0
		MSG_INFO
	else
		DEC_MONEY 0 130 21 10
		if 10 = 0
			TRANSMIT 2202 23000 8700 1
		endif
	endif
endproc 
proc 311
	DEC_MONEY_TEST 0 220 21 Moy
	if Moy = -1
		ADD_SYS_MSG_START
		ADD_SYS_MSG ������Ҳ��㣬�޷����ͣ�
		SET_SYS_MSG_MODE 4 0 0
		MSG_INFO
	else
		DEC_MONEY 0 220 21 10
		if 10 = 0
			TRANSMIT 2200 6500 18000 1
		endif
	endif
endproc 
proc 312
	DEC_MONEY_TEST 0 100 21 Moy
	if Moy = -1
		ADD_SYS_MSG_START
		ADD_SYS_MSG ������Ҳ��㣬�޷����ͣ�
		SET_SYS_MSG_MODE 4 0 0
		MSG_INFO
	else
		DEC_MONEY 0 100 21 10
		if 10 = 0
			TRANSMIT 2300 8700 12400 1
		endif
	endif
endproc 
proc 313
	DEC_MONEY_TEST 0 250 21 Moy
	if Moy = -1
		ADD_SYS_MSG_START
		ADD_SYS_MSG ������Ҳ��㣬�޷����ͣ�
		SET_SYS_MSG_MODE 4 0 0
		MSG_INFO
	else
		DEC_MONEY 0 250 21 10
		if 10 = 0
			TRANSMIT 2400 14000 15400 1
		endif
	endif
endproc 
proc 314
	DEC_MONEY_TEST 0 350 21 Moy
	if Moy = -1
		ADD_SYS_MSG_START
		ADD_SYS_MSG ������Ҳ��㣬�޷����ͣ�
		SET_SYS_MSG_MODE 4 0 0
		MSG_INFO
	else
		DEC_MONEY 0 350 21 10
		if 10 = 0
			TRANSMIT 2500 25700 25700 1
		endif
	endif
endproc 
proc 315
	DEC_MONEY_TEST 0 810 21 Moy
	if Moy = -1
		ADD_SYS_MSG_START
		ADD_SYS_MSG ������Ҳ��㣬�޷����ͣ�
		SET_SYS_MSG_MODE 4 0 0
		MSG_INFO
	else
		DEC_MONEY 0 810 21 10
		if 10 = 0
			TRANSMIT 3404 27400 13200 1
		endif
	endif
endproc 








 
 
 
 