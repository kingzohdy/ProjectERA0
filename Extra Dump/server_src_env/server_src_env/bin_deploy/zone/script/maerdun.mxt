

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bľ�ģ���ʯ��ˮ����������Ҫ�����һ�У���Ȼ����Щ������ȡ֮������֮���ߵġ�
	task_add_option 1655 1 101
	task_add_option 1656 1 102
	task_add_option 1657 1 103
 	task_add_option 1658 1 104
	task_add_option 1659 1 105
	task_add_option 1660 1 106
	TASKCYCLE_ADD_OPTION 106 1 107
	
	TASK_STAT 1659 1 20
	if @1 = 0 and @20 = 2
	  	option 150 �ٸ��Ҹ�ըҩ����
	endif

	STRVAR mon
	
	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300778 1 num1
	if num1 > 0
		TASK_STAT 3304 1 Ret
		if Ret = 2
			option 500 �����뻻����
		endif	
	endif
	strvar Ret num2
	GET_ITEM_NUM_INPAK 5300922 1 num2
	if num2 > 0
		TASK_STAT 3305 1 Ret
		if Ret = 2
			option 700 �����뻻����
		endif	
	endif
	talk
endproc 

proc 500
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bһ�ڼۣ���ʲô�������ܺ��һ���
	
	option 501 #����

	talk
endproc
proc 501
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300778 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300778 1 1
			
		;	rand Rad 100
		;	if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300779 1 0 0
		;	endif
		;	if @Rad >= 80 and @Rad < 95
		;		ADD_ITEM_NUM_INPAK 5300780 1 0 0
		;	endif
		;	if @Rad >= 95 and @Rad < 99
		;		ADD_ITEM_NUM_INPAK 5300781 1 0 0
		;	endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300782 1 0 0
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



PROC 101
	task_do_option 1655 1
ENDPROC
PROC 102
	task_do_option 1656 1
ENDPROC
PROC 103
	task_do_option 1657 1
ENDPROC
PROC 104
	task_do_option 1658 1
ENDPROC
PROC 105
	task_do_option 1659 1
ENDPROC
PROC 106
	task_do_option 1660 1
ENDPROC
PROC 107
	TASKCYCLE_DO_OPTION 106 1
ENDPROC
PROC 150
	GET_PACK_SPACE 1 1
	
			if @1 > 0
				GET_ITEM_NUM_INPAK 5300456 0 mon
				if @mon = 0
					ADD_ITEM_NUM_INPAK 5300456 1 1 1
						if @1 = -1
							return
						endif
				else
					add_sys_msg_start
					add_sys_msg ���Ѿ�ӵ��ըҩ���ˡ�
					set_sys_msg_mode 1 0 0
					msg_info	
				endif
			endif
ENDPROC



proc 700
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bһ�ڼۣ���ʲô�������ܺ��һ���
		option 701 #����

	talk
	
endproc
proc 701
	
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300922 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300922 1 1
			
		;	rand Rad 100
		;	if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300923 1 0 0
		;	endif
		;	if @Rad >= 80 and @Rad < 95
		;		ADD_ITEM_NUM_INPAK 5300924 1 0 0
		;	endif
		;	if @Rad >= 95 and @Rad < 99
		;		ADD_ITEM_NUM_INPAK 5300925 1 0 0
		;	endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300926 1 0 0
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
