;����� ��˹�� 302035

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��˵��������������޲Ƹ���
	
	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300777 1 num1
	if num1 > 0
		TASK_STAT 3304 1 Ret
		if Ret = 2
			option 500 �����뻻����
		endif	
	endif
	strvar Ret num2
	GET_ITEM_NUM_INPAK 5300921 1 num2
	if num2 > 0
		TASK_STAT 3305 1 Ret
		if Ret = 2
			option 700 �����뻻����
		endif
	endif
	
	OPTION 101 #����󾮸���ð�գ�
;	option 102 #���ø���


	talk
endproc

proc 101
;	ENTER_PWORLD 103 0 1
	
;		OPEN_PWORLD_UI 103
		OPEN_PWORLD_UI 2
endproc
proc 102

	RESET_PWORLD 103
endproc
proc 500
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���ֿ��ñ�ѧ�Ŀ�Ŀ�������˻��ղأ����ҽ����ɣ�Ϊ��������룡
	option 501 #����

	talk
endproc
proc 501
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300777 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300777 1 1
			
		;	rand Rad 100
		;	if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300778 1 0 0
		;	endif
		;	if @Rad >= 80 and @Rad < 95
		;		ADD_ITEM_NUM_INPAK 5300779 1 0 0
		;	endif
		;;	if @Rad >= 95 and @Rad < 99
			;	ADD_ITEM_NUM_INPAK 5300780 1 0 0
			;endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300781 1 0 0
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


proc 700
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���ֿ��ñ�ѧ�Ŀ�Ŀ�������˻��ղأ����ҽ����ɣ�Ϊ��������룡
		option 701 #����

	talk
	
endproc
proc 701
	
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300921 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300921 1 1
			
		;	rand Rad 100
		;	if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300922 1 0 0
		;	endif
		;	if @Rad >= 80 and @Rad < 95
		;		ADD_ITEM_NUM_INPAK 5300923 1 0 0
		;	endif
		;	if @Rad >= 95 and @Rad < 99
		;		ADD_ITEM_NUM_INPAK 5300924 1 0 0
		;	endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300925 1 0 0
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





