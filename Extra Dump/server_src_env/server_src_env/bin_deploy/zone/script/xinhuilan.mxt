;���Ƚ̹�
; Я������\����503

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#bÿ���˶��ᾭ������׶Σ�����һ��ɽ������֪��ɽ������ʲô���󲿷��˻���Ϊɽ���վ�����ʧ�������С��Һ���������������֪��ɽ�Ǳ���ʲô��������ȥ�����ɣ�Ҳ����������ջ�
	task_add_option 1079 1 101	
	task_add_option 1075 1 102
	task_add_option 1080 1 103
	task_add_option 1071 1 104
	
	task_add_option 1074 1 105
	task_add_option 1083 1 106
	task_add_option 3004 1 107
	
	 
	TASK_STAT 1075 1 3
	if 3 = 2
		OPTION 110 #������ջ���
;		OPTION 111 #������ջ���
	ENDIF
	
	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300763 1 num1
	if num1 > 0
		TASK_STAT 3304 1 Ret
		if Ret = 2
			option 500 �����뻻����
		endif
	endif
	
	
	task_stat 3004 1 20
	if 20 = 2
		option 120 ����Ҫ��������ҹ�ȥ�ɣ�
	endif
	

	
	
	talk
endproc
proc 101
    task_do_option 1079 1
endproc
proc 102
    task_do_option 1075 1
endproc
proc 103
    task_do_option 1080 1
endproc
proc 104
    task_do_option 1071 1
endproc
proc 105
    task_do_option 1074 1
endproc
proc 106
    task_do_option 1083 1
endproc
proc 107
    task_do_option 3004 1
endproc

proc 110
	enter_pworld 503 0 1
endproc

;proc 111
;	RESET_PWORLD 503
;endproc
proc 120
	AUTO_MOVE 20 1000 24200 13200 1460049 0 3081 1 3600
endproc
proc 500
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����������ȱ�������е���һ�½ڣ����ҽ����ɣ������ң�ֻ���Ҷ��Ȿħ���۹�������Ȥ��

		option 501 #����

	talk
endproc

proc 501
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300763 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300763 1 1
			
		;	rand Rad 100
		;	if @Rad < 80
		;		ADD_ITEM_NUM_INPAK 5300764 1 0 0
		;	endif
		;	if @Rad >= 80 and @Rad < 95
		;		ADD_ITEM_NUM_INPAK 5300765 1 0 0
		;	endif
		;	if @Rad >= 95 and @Rad < 99
				ADD_ITEM_NUM_INPAK 5300766 1 0 0
		;	endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300783 1 0 0
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

proc 2
	strvar Retask
	get_trig_task 1 Retask
	if @Retask = 1086
		task_fini 1086 1
	endif
	
	if @Retask = 1075
;		RESET_PWORLD 503
		dialog 1
		add_talk $_npc$:
		add_talk #b#bÿ���˶��ᾭ������׶Σ�����һ��ɽ������֪��ɽ������ʲô���󲿷��˻���Ϊɽ���վ�����ʧ�������С��Һ���������������֪��ɽ�Ǳ���ʲô��������ȥ�����ɣ�Ҳ����������ջ�
		OPTION 110 #������ջ���
		talk
	endif

endproc

