

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�����Ѿ�ʧ�ܺܶ���ˣ���������Ȼ���ܷ�������������Ҳ�����ˣ��Ǽž�����ľ�Ҫ��Ϊ�ž����ˡ�
	
	task_add_option 1607 1 112
	task_add_option 1608 1 113
	task_add_option 1609 1 114
	task_add_option 1610 1 115

	
	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300776 1 num1
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
	add_talk #b#b�ڲ���Ϣ���������̻��½��˴������������ͣ������ϵ��Ȿ���Ͼ�Ҫ���������Ͻ����ְɣ�
	add_talk #b#b���ţ����ٹ����������ϵ��Ȿֻ�������ϲ����ˣ���ʱ����޶���������
	
	option 501 #����

	talk
endproc
proc 501
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300776 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300776 1 1
			
		;	rand Rad 100
		;	if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300777 1 0 0
		;	endif
		;	if @Rad >= 80 and @Rad < 95
		;		ADD_ITEM_NUM_INPAK 5300778 1 0 0
		;	endif
		;	if @Rad >= 95 and @Rad < 99
		;		ADD_ITEM_NUM_INPAK 5300779 1 0 0
		;	endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300780 1 0 0
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



PROC 112
	task_do_option 1607 1
ENDPROC
PROC 113
	task_do_option 1608 1
ENDPROC
PROC 114
	task_do_option 1609 1
ENDPROC
PROC 115
	task_do_option 1610 1
ENDPROC



proc 2
	get_trig_task 1 2 

	if 2 = 1610 
		task_fini 1610 1
	endif
endproc
