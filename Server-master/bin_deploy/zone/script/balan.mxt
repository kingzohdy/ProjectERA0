

proc 1
	STRVAR a m2
	get_task_var 1731 1 8 1 m2
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bҪ֪������Щ�����ע�����ᱻ����������ģ����ǵ�ÿһƬ��ë������ҫ�����ɵĹ�â������һ�����ڵĶ����������������ģ�����Щֻ��������������˵��ﲻ�ˣ�Ҳ�޷������ģ�
	
	task_add_option 1702 1 11
	task_add_option 1703 1 12
	task_add_option 1704 1 13
	task_add_option 1705 1 14


	TASK_STAT 1731 1 a
	if a = 2 and m2 = 0
		OPTION 301 #�������������
	endif
	
	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300779 1 num1
	if num1 > 0
		TASK_STAT 3304 1 Ret
		if Ret = 2
			option 500 �����뻻����
		endif	
	endif
	strvar Ret num2
	GET_ITEM_NUM_INPAK 5300923 1 num2
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
	add_talk #b#b��˵�����������Ż����ߣ�������������������һ�ε���˼�ǡ����øϽ����߸����У�
	add_talk #b#b���ҽ����ɣ��㲻��Կ��ģ�
	option 501 #����
		
	talk
endproc
proc 501
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300779 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300779 1 1
			
		;	rand Rad 100
		;	if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300780 1 0 0
		;	endif
		;	if @Rad >= 80 and @Rad < 95
		;		ADD_ITEM_NUM_INPAK 5300781 1 0 0
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




PROC 11
	task_do_option 1702 1
ENDPROC
PROC 12
	task_do_option 1703 1
ENDPROC
PROC 13
	task_do_option 1704 1
ENDPROC
PROC 14
	task_do_option 1705 1
ENDPROC


proc 2
	get_trig_task 1 2
	if 2 = 1705 
		task_fini 1705 1
	endif
endproc
PROC 301
	get_task_var 1731 1 8 1 m2
	if m2 = 0
		DEC_ITEM_NUM_INPAK 5300476 1 1
		if 1 = -1
			add_sys_msg_start
			add_sys_msg ��û���⣬���ҳ�ʲô��
			set_sys_msg_mode 4 0 0
			msg_info		
		else
			var @m2 1
			set_task_var 1731 1 8 @m2 1
				if 1 = -1
					return
				endif
		endif
	endif	
ENDPROC

proc 700
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��˵�����������Ż����ߣ�������������������һ�ε���˼�ǡ����øϽ����߸����У�
	add_talk #b#b���ҽ����ɣ��㲻��Կ��ģ�
		option 701 #����

	talk
	
endproc
proc 701
	
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300923 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300923 1 1
			
		;	rand Rad 100
		;	if @Rad < 80
		;		ADD_ITEM_NUM_INPAK 5300924 1 0 0
		;	endif
		;	if @Rad >= 80 and @Rad < 95
				ADD_ITEM_NUM_INPAK 5300925 1 0 0
		;	endif
		;	if @Rad >= 95 and @Rad < 99
		;		ADD_ITEM_NUM_INPAK 5300926 1 0 0
		;	endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300927 1 0 0
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
