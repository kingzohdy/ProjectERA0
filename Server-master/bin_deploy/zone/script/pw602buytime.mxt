;�����ػ���Ŧ�    ,����buff
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��Ŧ�ճ���һЩά��������������ά�֣���Ŧ��Ҫ��һ�����쳣����ҿ϶���֪���ġ�
	option 200 #������Ŧ�����޵�
	option 300 #�з�Ϯ��ʱ���Ӻ�
	talk
endproc
proc 200
	choose 2 201 202 ����󹫻���Ŧ30���ڴ����޵�״̬����Ҫ����20��ң�ȷ������
endproc
proc 201
	strvar sec
	get_time_sec sec
	;monmemid10,��Ŧ��memid
	var 10 0
	get_pworld_var 0 32 1 10
	if 10 = 0
		add_sys_msg_start
		add_sys_msg �޷����д˲���  
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	;cool13,�޵���ȴʱ��
	var 13 0
	get_pworld_var 12 32 1 13
	if 13 > @sec
;		add_sys_msg_start
;		add_sys_msg $@13$:$@sec$
;		set_sys_msg_mode 3 0 0
;		map_msg_info
		add_sys_msg_start
		add_sys_msg ����ʧ�ܣ�������Ŧ�޵�3������ֻ�ܹ���һ��  
		set_sys_msg_mode 1 0 0
		msg_info
		return
	endif
		
	DEC_MONEY_TEST 0 20 60 1
	if 1 != 0
		add_sys_msg_start
		add_sys_msg ��Ҳ��㣬������Ŧ�޷������޵�״̬  
		set_sys_msg_mode 1 0 0
		msg_info
		return
	endif
	dec_money 0 20 60 1
	
	if 10 > 0
	;TARGET_ADD_STATUS   #Ŀ������ #memid #statusID #statusLV #statusTM #����ֵ
		TARGET_ADD_STATUS 3 @10 3170 1 30 1
		if 1 = 0
;			add_sys_msg_start
;			add_sys_msg $@13$:$@sec$
;			set_sys_msg_mode 3 0 0
;			map_msg_info
			add_sys_msg_start
			add_sys_msg ���򹫻���Ŧ�޵гɹ���������Ŧ30���ڴ����޵�״̬ 
			set_sys_msg_mode 4 0 0
			map_msg_info
			add sec 180
			;cool13,�޵���ȴʱ��
			var 13 @sec
			set_pworld_var 12 32 @13 1
		endif
	endif	
endproc
proc 202
;	call 203
endproc
proc 203
	;monmemid10,��Ŧ��memid
	var 10 0
	get_pworld_var 0 32 1 10
	if 10 = 0
		return
	endif
	TARGET_ADD_STATUS 3 @10 3170 1 30 1
endproc
proc 300
	choose 2 301 302 ������ܽ��з�Ϯ��ʱ���Ӻ�20�룬��Ҫ����10��ң�ȷ������
endproc
proc 301
	DEC_MONEY_TEST 0 10 60 1
	if 1 != 0
		add_sys_msg_start
		add_sys_msg ��Ҳ��㣬�з�Ϯ��ʱ���޷��Ӻ�  
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	is_pworld_over 1
	if 1 = 1
		add_sys_msg_start
		add_sys_msg �޷����д˲���  
		set_sys_msg_mode 4 0 0
		msg_info
		return 
	endif
	dec_money 0 10 60 1
	GET_PWORLD_wave_MON_COUNT_DOWN 10
	add 10 20
	SET_PWORLD_wave_MON_COUNT_DOWN 1 @10
endproc
proc 302
endproc
