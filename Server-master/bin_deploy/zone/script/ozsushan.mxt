;������ɯ
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��ò��һ��Ů�ף�������ħ���£��ҳ��ǻ����������ܽ�ġ�

	
;20~30
	
	task_add_option 1427 1 104
	task_add_option 1441 1 107
	
	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300767 1 num1
	if num1 > 0
		TASK_STAT 3304 1 Ret
		if Ret = 2
			option 500 �����뻻����
		endif	
	endif
	talk

endproc 

PROC 104
	task_do_option 1427 1
ENDPROC

PROC 107
	task_do_option 1441 1
ENDPROC

proc 500
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�Ǹ���ŷ�գ����һ����������˼Ұ��Լ���������������Ķ������㿴�
	option 501 #����

	talk
endproc

proc 501
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300767 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300767 1 1
			
		;	rand Rad 100
		;	if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300768 1 0 0
		;	endif
		;	if @Rad >= 80 and @Rad < 95
		;		ADD_ITEM_NUM_INPAK 5300769 1 0 0
		;	endif
		;	if @Rad >= 95 and @Rad < 99
		;		ADD_ITEM_NUM_INPAK 5300770 1 0 0
		;	endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300771 1 0 0
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
;	
;	if 2 = 1438
;		task_fini 1438 1
;	endif
;	
;endproc 