


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���Խ��Խ����̴����٣��ɺ޵�����Щ���˲������˲ƻ����������������Ѿ�û�м����̻�ҳ����ˣ�������ʱ���һ���Ϊ����̫æ����Թ�������Ҿ�Ȼ��ʼ������ͷ�ܶ�����ͷ����
		task_add_option 1355 1 100
		task_add_option 1356 1 101
		task_add_option 1377 1 102
		task_add_option 1379 1 103

	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300783 1 num1
	if num1 > 0
		TASK_STAT 3304 1 Ret
		if Ret = 2
			option 500 �����뻻����
		endif
	endif
	talk
	
endproc 

PROC 100
	task_do_option 1355 1
ENDPROC
PROC 101
	task_do_option 1356 1
ENDPROC
PROC 102
	task_do_option 1377 1
ENDPROC
PROC 103
	task_do_option 1379 1
ENDPROC
proc 2

	get_trig_task 1 2
	if 2 = 1356
		task_fini 1356 1
	endif

endproc 
	
proc 500
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��ô�����ѧ�ʣ�ֻ���ҲŻ����Ȥ��������һ��������������������־�ͷ���ۻ��ˣ������Ҵ���ˣ����һ��ɣ�����ԥʲô������������������ͷ������Ϊ�Ȿ����Ϊ����ô������ԥ�ˣ���������û��������ˣ�

	option 501 #����

	talk
endproc
	
proc 501
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300783 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300783 1 1
			
		;	rand Rad 100
		;	if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300784 1 0 0
		;	endif
		;	if @Rad >= 80 and @Rad < 95
		;		ADD_ITEM_NUM_INPAK 5300767 1 0 0
		;	endif
		;	if @Rad >= 95 and @Rad < 99
		;		ADD_ITEM_NUM_INPAK 5300768 1 0 0
		;	endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300769 1 0 0
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
	
	
	
	
	
	
	
	
	