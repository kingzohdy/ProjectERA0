

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��������ϣ��кܶණ��������������Ҫ���������������ǣ��������Ǵ����������أ�
	
	TASKCYCLE_ADD_OPTION 107 1 12
	
	
	task_add_option	2248 1 103
	
	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300781 1 num1
	if num1 > 0
		TASK_STAT 3304 1 Ret
		if Ret = 2
			option 500 �����뻻����
		endif
	endif
	strvar Ret num2
	GET_ITEM_NUM_INPAK 5300924 1 num2
	if num2 > 0
		TASK_STAT 3305 1 Ret
		if Ret = 2
			option 700 �����뻻����
		endif
	endif
	talk
endproc 
PROC 12
	TASKCYCLE_DO_OPTION 107 1
ENDPROC

PROC 103
	task_do_option 2248 1
ENDPROC


proc 500
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��û�뵽�Ȿ��Ѱ����ʮ��ġ�����Ұʷ����Ȼ��������У����һ��ɣ�����Ҫʲô��������
	add_talk #b#b�����ҵ����У���������ֻ�ܻ��һ�ܵ�ʹ��Ȩ����ô������ô��
	add_talk #b#b����Ұʷ��û������Ը���ղ�Ӵ���úÿ���һ�°ɣ�����������û���������
	option 501 #����
		
	talk
endproc
proc 501
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300781 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300781 1 1
			
		;	rand Rad 100
		;	if  @Rad < 95
		;		ADD_ITEM_NUM_INPAK 5300782 1 0 0
		;	endif
		;	if @Rad >= 95 
				ADD_ITEM_NUM_INPAK 5300785 1 0 0
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
;proc 2
;	get_trig_task 1 2
;	if 2 = 4438 
;		task_fini 4438 1
;	endif  
 ;
;endproc

proc 700
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��û�뵽�Ȿ��Ѱ����ʮ��ġ�����Ұʷ����Ȼ��������У����һ��ɣ�����Ҫʲô��������
	add_talk #b#b�����ҵ����У���������ֻ�ܻ��һ�ܵ�ʹ��Ȩ����ô������ô��
	add_talk #b#b����Ұʷ��û������Ը���ղ�Ӵ���úÿ���һ�°ɣ�����������û���������

		option 701 #����

	talk
	
endproc
proc 701
	
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300924 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300924 1 1
			
		;	rand Rad 100
		;	if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300935 1 0 0
		;	endif
		;	if @Rad >= 80 and @Rad < 95
		;		ADD_ITEM_NUM_INPAK 5300926 1 0 0
		;	endif
		;	if @Rad >= 95 and @Rad < 99
		;		ADD_ITEM_NUM_INPAK 5300927 1 0 0
		;	endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300928 1 0 0
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


