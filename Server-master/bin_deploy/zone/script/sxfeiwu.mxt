;;;
;l;
proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�Һܾ��ȣ�$_player$������������#Race#��ȻҲ���ֲ�����Ȥ�������Ը�����������µ���ӣ���Ҳ��ῼ��һ������Ϊͽ��

	task_add_option 1816 1 100
	task_add_option 1817 1 101
	task_add_option 1818 1 102
	task_add_option 1819 1 103
	task_add_option 1820 1 104
	task_add_option 1821 1 105
	task_add_option 1822 1 106
	task_add_option 1823 1 107
	task_add_option 1824 1 108
		
	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300928 1 num1
	if num1 > 0
		TASK_STAT 3305 1 Ret
		if Ret = 2
			option 700 �����뻻����
		endif
	endif
		
		
	talk

endproc
PROC 100
	task_do_option 1816 1
ENDPROC
PROC 101
	task_do_option 1817 1
ENDPROC
PROC 102
	task_do_option 1818 1
ENDPROC
PROC 103
	task_do_option 1819 1
ENDPROC
PROC 104
	task_do_option 1820 1
ENDPROC
PROC 105
	task_do_option 1821 1
ENDPROC
PROC 106
	task_do_option 1822 1
ENDPROC
PROC 107
	task_do_option 1823 1
ENDPROC
PROC 108
	task_do_option 1824 1
ENDPROC

proc 2
	get_trig_task 1 2
	if 2 = 1824 
		task_fini 1824 1
	endif
	if 2 = 1823
	;���ͷ���
		;204051 
		del_npc 304042 1
		strvar Ret
		CALL_MON_BY_PATROL 1 204051 0 0 6700 8700 106 0 Ret
		get_task_var 1823 0 32 1 20
		var 20 @Ret
		set_task_var 1823 0 32 @20 1
	endif
	

endproc

proc 700
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���������������
	add_talk #b#b֮ǰ�и��쵰�ø������������ң����������֪ʶ���Ǵ��ҵģ�
	add_talk #b#b����ô����������Ҳ����氡�����������߰���������ҲҪ�ˣ�

		option 701 #����

	talk
	
endproc
proc 701
	
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300928 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300928 1 1
			
		;	rand Rad 100
		;	if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300929 1 0 0
		;	endif
		;	if @Rad >= 80 and @Rad < 95
		;		ADD_ITEM_NUM_INPAK 5300930 1 0 0
		;	endif
		;	if @Rad >= 95 and @Rad < 99
		;		ADD_ITEM_NUM_INPAK 5300931 1 0 0
		;	endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300932 1 0 0
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



