


proc 1

	dialog 1
	
	add_talk $_npc$:
	add_talk #b#b�ɱ�������ģ�$_player$����Ȼ���￴���������Ѿã��������������ƻķ��˵��γǣ����ʿ������ɥ����
		task_add_option 1963 1 100
		task_add_option 1965 1 101
		
		task_add_option 2260 1 102
		task_add_option 2261 1 103
		
		
		strvar Ret
		task_stat 1965 1 Ret
		if Ret = 2
		;	option #120 ��ȡ�ؾ�
		
		endif
		
	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300931 1 num1
	if num1 > 0
		TASK_STAT 3305 1 Ret
		if Ret = 2
			option 700 �����뻻����
		endif
	endif
	talk
	
endproc


proc 100
	task_do_option 1963 1
endproc 
proc 101
	task_do_option 1965 1
endproc 
proc 102
	task_do_option 2260 1
endproc 
proc 103
	task_do_option 2261 1
endproc 

proc 120	
	APPLY_TMP_ITEM 291003 0 0

endproc


proc 700
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���뵱������ʿ�����Ǻ�ʿ������һֱ�����ܹ���Ϊ���ˣ���Ϊ��������Ϊ���˾�����Ӣ�ۡ����������ƽӹ�ɳܣ���������־Ȥ��ͬ�ķ��ϣ��ҾͰ���һ�Ѱɣ�ϣ������Ҳ�����˰���һ�ѣ�
		option 701 #����

	talk
	
endproc
proc 701
	
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300931 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300931 1 1
			
		;	rand Rad 100
		;	if @Rad < 95
				ADD_ITEM_NUM_INPAK 5300932 1 0 0
		;	endif
		;	if @Rad >= 95 and @Rad < 99
		;		ADD_ITEM_NUM_INPAK 5300933 1 0 0
		;	endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300934 1 0 0
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



