;�峤 
; Я������

proc 1
	dialog 1	
	add_talk $_npc$:
	add_talk #b#bÿ���˶������Լ�������ڱ��˿������������Ц�˷�ʱ�䣬�Լ�ȴ���ú���Ҫ������Ը��Ϊ֮���������������$_player$�����������ʲô�أ�


	task_add_option 1049 1 100
	task_add_option 1050 1 101
	
	
;	task_add_option 9999 1 110
	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300912 1 num1
	if num1 > 0
		TASK_STAT 3305 1 Ret
		if Ret = 2
			option 700 �����뻻����
		endif
	endif
	TASK_STAT 3284 1 20
	TASK_STAT 3288 1 21
	TASK_STAT 3515 1 22
	TASK_STAT 3537 1 23
	TASK_STAT 3561 1 24
	TASK_STAT 3607 1 25
	
	
	if 21 = 2 or 20 = 2 or 22 = 2 or 23 = 2 or 24 = 2 or 25 = 2
	  	option 150 ��������-����Э������
	endif
	
	talk
endproc
proc 100
    task_do_option 1049 1
endproc
proc 101
    task_do_option 1050 1
endproc

;proc 110
;    task_do_option 9999 1
;endproc



PROC 150
	
	DIALOG 1
	ADD_TALK $_npc$       
	ADD_TALK #b#b���ҾͲ�����������������ˣ�����������ʶ�ȱ��
		
	OPTION 250 #����Э������
	TALK
ENDPROC

PROC 250
	
	GET_ITEM_NUM_INPAK 5300533 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ��
		set_sys_msg_mode 1 0 0
		msg_info
	ENDIF
	if 11 >= 1
		dec_item_num_inpak 5300533 1 0
		TASK_STAT 3284 1 70
		TASK_STAT 3288 1 71
		TASK_STAT 3515 1 72
		TASK_STAT 3537 1 73
		TASK_STAT 3561 1 74
		TASK_STAT 3607 1 75
		if 70 = 2
			task_fini 3284 1
		endif
		if 71 = 2
			task_fini 3288 1
		endif
		if 72 = 2
			task_fini 3515 1
		endif
		if 73 = 2
			task_fini 3537 1
		endif	
		if 74 = 2
			task_fini 3561 1
		endif		
		if 75 = 2
			task_fini 3607 1
		endif		
	endif
ENDPROC


proc 2
	get_trig_task 1 2
	if 2 = 1050
		task_fini 1050 1
	endif

endproc
proc 700
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���ǰ��������±�Խ��Խ���ˣ����ǿ�ϲ�����顣���±��ֲ���˵�״�������ɾͲ�һ�����������ǲ���ܾ������ѵ�ͬʱ����Ϊ�ֵģ��԰ɣ�

		option 701 #����

	talk
	
endproc
proc 701
	
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300912 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300912 1 1
			
		;	rand Rad 100
		;	if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300913 1 0 0
		;	endif
		;	if @Rad >= 80 and @Rad < 95
		;		ADD_ITEM_NUM_INPAK 5300914 1 0 0
		;	endif
		;	if @Rad >= 95 and @Rad < 99
		;		ADD_ITEM_NUM_INPAK 5300915 1 0 0
		;	endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300916 1 0 0
		;	endif
		else
			add_sys_msg_start
			add_sys_msg �����ռ䲻�㣡
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg ������û�п��Խ�������Ʒ��
		set_sys_msg_mode 1 0 0
		msg_info
	endif
endproc 
