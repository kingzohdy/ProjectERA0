
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��������¡��ֶ����������������������ҵļ�ʱ���֣�������ڶ����������������Ա�ˣ���Ϊ���Ǹе��ѹ���
	add_talk #b#b����������ѣ�������Ȥ���Ҷ�һ���𣬲����ҵ������㣬��ʱ���֣����������úܲҡ�

	task_add_option 4101 1 102
	task_add_option 1110 1 103
	task_add_option 4119 1 104

	task_add_option 4207 1 105
	task_add_option 4210 1 106
	task_add_option 4211 1 107
;50��


	task_add_option 4120 1 110

;	TASKCYCLE_ADD_OPTION 101 1 120
	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300765 1 num1
	if num1 > 0
		TASK_STAT 3304 1 Ret
		if Ret = 2
			option 500 �����뻻����
		endif
	endif

	talk
endproc 

PROC 102
	task_do_option 4101 1
ENDPROC
PROC 103
	task_do_option 1110 1
ENDPROC
PROC 104
	task_do_option 4119 1
ENDPROC
PROC 105
	task_do_option 4207 1
ENDPROC
PROC 106
	task_do_option 4210 1
ENDPROC
PROC 107
	task_do_option 4211 1
ENDPROC

PROC 110
	task_do_option 4120 1
ENDPROC

PROC 120	
	TASKCYCLE_DO_OPTION 101 1
ENDPROC


proc 2
	get_trig_task 1 2
	if 2 = 4207
		task_fini 4207 1
	endif 
	if 2 = 4211 
		task_fini 4211 1
	endif 

endproc

proc 500
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�ˣ�#Name#���ͺ��ҽ����ɣ��ҵ��������ղ����������еľ����򣬼�Ȼ��Ը��������ɳɾͱ��������룬Ϊʲô��˳�����һ���أ�

	option 501 #����

	talk
endproc
	
proc 501
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300765 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300765 1 1
			
			rand Rad 100
			if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300766 1 0 0
			endif
			if @Rad >= 80 and @Rad < 95
				ADD_ITEM_NUM_INPAK 5300783 1 0 0
			endif
			if @Rad >= 95 and @Rad < 99
				ADD_ITEM_NUM_INPAK 5300784 1 0 0
			endif
			if @Rad = 99
				ADD_ITEM_NUM_INPAK 5300767 1 0 0
			endif
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



