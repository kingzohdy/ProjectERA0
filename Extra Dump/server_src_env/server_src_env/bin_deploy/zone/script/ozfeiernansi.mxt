

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���������ϵĸ���Ը�˶��𸸵���Ϊʥ�������ǵ�ծ����ͬ�������˵�ծ�������������׶񣬰��š�
	

;��ҹ����
	
	task_add_option 2241 1 138
	task_add_option 2245 1 139

;20~30�޸�

	task_add_option 1401 1 140
	task_add_option 1402 1 141
	task_add_option 1419 1 142
	task_add_option 3131 1 143
;30~40
;	task_add_option 1501 1 101
	
	
	task_add_option 1375 1 102
	

	TASK_STAT 3226 1 20
	TASK_STAT 3256 1 21
	TASK_STAT 3286 1 22
	TASK_STAT 3518 1 23
	TASK_STAT 3540 1 24
	TASK_STAT 3564 1 25
	TASK_STAT 3610 1 26
	if 21 = 2 or 20 = 2 or 22 = 2 or 23 = 2 or 24 = 2 or 25 = 2 or 26 = 2
	  	option 150 ��������-����Э������
	endif
	
	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300768 1 num1
	if num1 > 0
		TASK_STAT 3304 1 Ret
		if Ret = 2
			option 500 �����뻻����
		endif	
	endif
		
	talk
endproc 
proc 500
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bǰЩ���ӣ�����ĳЩ������ʥ�������ţ����ֱ�Ƕ��ϵ۲�����Ϊ�˱������˷���ͬ�������������ҽ����ɡ�
	option 501 #����

	talk
endproc
proc 501
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300768 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300768 1 1
			
		;	rand Rad 100
		;	if @Rad < 80
		;		ADD_ITEM_NUM_INPAK 5300769 1 0 0
		;	endif
		;	if @Rad >= 80 and @Rad < 95
				ADD_ITEM_NUM_INPAK 5300770 1 0 0
		;	endif
		;	if @Rad >= 95 and @Rad < 99
		;		ADD_ITEM_NUM_INPAK 5300771 1 0 0
		;	endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300772 1 0 0
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



;PROC 101
;	task_do_option 1501 1
;ENDPROC
PROC 102
	task_do_option 1375 1
ENDPROC

PROC 138
	task_do_option 2241 1
ENDPROC
PROC 139
	task_do_option 2245 1
ENDPROC
PROC 140
	task_do_option 1401 1
ENDPROC
PROC 141
	task_do_option 1402 1
ENDPROC
PROC 142
	task_do_option 1419 1
ENDPROC
PROC 143
	task_do_option 3131 1
ENDPROC



PROC 150
	
	DIALOG 1
	ADD_TALK $_npc$       
	ADD_TALK #b#bԸ�ϵ۱������ǣ�����������һ�н���Ϊ��������ϣ����
		
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
		TASK_STAT 3226 1 70
		TASK_STAT 3256 1 71
		TASK_STAT 3286 1 72
		TASK_STAT 3518 1 73
		TASK_STAT 3540 1 74
		TASK_STAT 3564 1 75
		TASK_STAT 3610 1 76
		if 70 = 2
			task_fini 3226 1
		endif
		if 71 = 2
			task_fini 3256 1
		endif	
		if 72 = 2
			task_fini 3286 1
		endif	
		if 73 = 2
			task_fini 3518 1
		endif	
		if 74 = 2
			task_fini 3540 1
		endif	
		if 75 = 2
			task_fini 3564 1
		endif	
		if 76 = 2
			task_fini 3610 1
		endif	
	endif
ENDPROC



proc 2
	get_trig_task 1 2

;	if 2 = 2241
;		task_fini 2241 1
;	endif
	if 2 = 1402
		task_fini 1402 1
	endif
	if 2 = 3131
;		task_fini 3131 1
	endif
	if 2 = 1501
		task_fini 1501 1
	endif
endproc
