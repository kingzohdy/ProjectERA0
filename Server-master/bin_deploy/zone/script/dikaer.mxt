

proc 1
	STRVAR a m3
	GET_TASK_VAR 1731 2 8 1 m3
	DIALOG 1
	add_talk $_npc$:
	add_talk  #b#b��ֻ���ڱ�Ҫ��ʱ��Ż��¸ң��¸Ҳ���������Ҫ�����������������¸����ܹ�Ϊ�˹��ҵĴ�����Զ����ð�չ�����ɨƽһ���谭���ҷ�չ���ϰ�������Ϊ��������
	
	task_add_option 1718 1 11
	task_add_option 1719 1 12
	task_add_option 1723 1 13
	task_add_option 1724 1 14
	task_add_option 1725 1 15
	task_add_option 1726 1 16
	task_add_option 1727 1 17
	task_add_option 1728 1 18
	task_add_option 1737 1 19
	task_add_option 1738 1 20


	TASK_STAT 1731 1 a
	if a = 2 and m3 = 0
		OPTION 301 #�������������
	endif
	
	
	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300780 1 num1
	if num1 > 0
		TASK_STAT 3304 1 Ret
		if Ret = 2
			option 500 �����뻻����
		endif
	endif
	talk
endproc 

PROC 11
	task_do_option 1718 1
ENDPROC
PROC 12
	task_do_option 1719 1
ENDPROC
PROC 13
	task_do_option 1723 1
ENDPROC
PROC 14
	task_do_option 1724 1
ENDPROC
PROC 15
	task_do_option 1725 1
ENDPROC
PROC 16
	task_do_option 1726 1
ENDPROC
PROC 17
	task_do_option 1727 1
ENDPROC
PROC 18
	task_do_option 1728 1
ENDPROC
PROC 19
	task_do_option 1737 1
ENDPROC
PROC 20
	task_do_option 1738 1
ENDPROC


proc 500
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�������治��������ɶ��Ǽ�ֵǧ�����ر������һ��ɣ�����ԥ����
	option 501 #����
		
	talk
endproc
proc 501
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300780 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300780 1 1
			
		;	rand Rad 100
			
		;	if @Rad < 95
				ADD_ITEM_NUM_INPAK 5300781 1 0 0
		;	endif
		;	if @Rad >= 95 and @Rad < 99
		;		ADD_ITEM_NUM_INPAK 5300782 1 0 0
		;	endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300785 1 0 0
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



PROC 301
	GET_TASK_VAR 1731 2 8 1 m3
	if m3 = 0
		DEC_ITEM_NUM_INPAK 5300476 1 1
		if 1 = -1
			add_sys_msg_start
			add_sys_msg ��û���⣬���ҳ�ʲô��
			set_sys_msg_mode 4 0 0
			msg_info		
		else
			var @m3 1
			set_task_var 1731 2 8 @m3 1
				if 1 = -1
					return
				endif
		endif
	endif	
ENDPROC
proc 2
	get_trig_task 1 2
	if 2 = 1724 
		task_fini 1724 1
	endif
endproc
