


proc 1
	
	DIALOG 1	
	add_talk $_npc$:
	strvar rety
	ENTER_WORLD_CITY_CHECK rety
	if rety = -1

	add_talk #b#b��....��.....�ԣ�����
	add_talk #b#bһ��δ��ɵ�����Ʒ����˵���������֮ʱ����ҿ���������հ����������������������©��ͳ����

	
	
	else
	
	add_talk #b#b���Ǽ�Ȩ������ҫ����Ǯ��һ���������������۷�λ�õ�������
	add_talk #b#b�����������ǣ�������հ���ɣ�������......����������
	add_talk #b#b
	add_talk #b#bÿ��ɶԵ���#c186506����#n����#c186506�Ӽ���#n1�Σ��ɻ�ô������顣
		option 300 ����һ��
		option 350 �ӳ�����
		
		strvar ret
		is_world_city_owner ret
		if ret = 1
			strvar pos
			player_clan_pos_info 1 0 0 0 Pos
			if Pos = 8
			;ֻ�г����ܿ���
				option 400 ��ȡ��������			
			endif
		endif
	endif
	talk
endproc  
proc 301

endproc
proc 300
	strvar ret
	PLAYER_WORSHIP_EFFIGY_TEST 1 ret
	if ret = 0
		choose 2 310 301 ȷ������200���ҳ���һ�£�
	endif
;	ȷ������XXX����/��������һ�£�
endproc 
proc  310
	PKG_IS_UNLOCK 10 20
	if 10 = 0 and 20 = 0
		return
	endif
	DEC_MONEY_TEST 0 200 20 10
	if 10 = -1
		add_sys_msg_start
		add_sys_msg ���Ҳ��㣬�޷�����
		set_sys_msg_mode 3 0 0
		msg_info
	else
		PLAYER_WORSHIP_EFFIGY 1 20
		if 20 = 0
			;��������
			DEC_MONEY 0 200 20 1		
		    ;ADD_EXP #playerid #����ֵ #���ر�־
			
		endif
	endif
endproc 

proc 350
	strvar ret
	PLAYER_WORSHIP_EFFIGY_TEST 0 ret
	if ret = 0
		choose 2 351 301 ȷ������200������һö��������
	endif
endproc
proc 351



			PKG_IS_UNLOCK 10 20
			if 10 = 0 and 20 = 0
				return
			endif
			DEC_MONEY_TEST 0 200 20 10
			if 10 = -1
				add_sys_msg_start
				add_sys_msg ���Ҳ��㣬�޷��Գ�����
				set_sys_msg_mode 3 0 0
				msg_info
			else
				PLAYER_WORSHIP_EFFIGY 0 20
				if 20 = 0
					;��������
					DEC_MONEY 0 200 20 1		
				    ;ADD_EXP #playerid #����ֵ #���ر�־
					
				endif
			endif

	
endproc 

proc 400
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�𾴵ĳ������ˣ���ÿ����ڴ˴���ȡǰһ���ҳ��ݻ��Ӽ�����������ֳɡ�
	add_talk #b#b���ǣ�ֻ��ӵ���������Լ��ĵ��񣬲ſ�����ȡ����ֳɡ�
	
	CITY_EFFIGY_INFO 10 20
	
;	add_talk #b#b�ʻ���$@10$
;	add_talk #b#b��������$@20$

	
	
		option 410 #��ȡ��������
		option 411 #�������
		
	talk
endproc 
proc 411
endproc
proc 410

;	�ʻ�10 ����20
	CITY_EFFIGY_INFO 10 20
	add 10 @20
	mul 10 100
	
	choose 2 415 411 ���յ����������Ϊ��$@10$���ң��Ƿ�ȷ����ȡ��
	
endproc


proc 415

	strvar ret
	is_world_city_owner ret
	if ret = 1
		strvar pos
		player_clan_pos_info 1 0 0 0 Pos
		if Pos = 8


			PLAYER_EFFIGY_AWARD 1
	
		else
			add_sys_msg_start
			add_sys_msg Ȩ�޲��㣬ֻ��ʱ��ǳ����ſɽ��д˲���
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg Ȩ�޲��㣬ֻ��ʱ��ǳ����ſɽ��д˲���
		set_sys_msg_mode 3 0 0
		msg_info
	
	endif
endproc 



