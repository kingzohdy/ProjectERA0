
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ļ����Ѿ�ӿ�ֵ������ͱڣ�ֻҪ��ǰ��һ�����ͻ���׳�����ٲ��������������е������������#Race#��������ս���ϴ�����Ҳ���м�ǿ�����ǣ�
	
		TASKCYCLE_ADD_OPTION 109 1 140
	
	task_add_option	2249 1 103
	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300929 1 num1
	if num1 > 0
		TASK_STAT 3305 1 Ret
		if Ret = 2
			option 700 �����뻻����
		endif
	endif
;		strvar Ret1 Ret2
;		GET_ROLE_TMP_MACHINE 0 Ret1
;		task_stat 1885 1 Ret2
;		if Ret2 = 2 and Ret1 = -1
;			option 141 #��ʻ�׹�ħ��
;		endif
	talk
	
endproc 
proc 140
	TASKCYCLE_DO_OPTION 109 1
endproc
PROC 103
	task_do_option 2249 1
ENDPROC
proc 141
;		strvar X Y
;		GET_PLAYER_MAPINFO 0 0 0 X Y
;		NEW_OWNER_TMP_ITEM 1460022 @X @Y 0 0 20 0 0 0 0 0
;		APPLY_TMP_ITEM 1460022 1 1
endproc

proc 2
	
	get_trig_task 1 2
	if 2 = 1885
;		strvar X Y
;		GET_PLAYER_MAPINFO 0 0 0 X Y
;		NEW_OWNER_TMP_ITEM 1460022 @X @Y 0 0 20 0 0 0 0 0
;		APPLY_TMP_ITEM 1460022 1 1
	endif
endproc 



proc 700
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���뻻����������ĺ�������������������û�б�����ֻ��һЩ���ڱȽ������½ڣ������Ը�����ҽ������һ���һ����õĸ��㣬ף�����մ����Ը��

		option 701 #����

	talk
	
endproc
proc 701
	
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300929 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300929 1 1
			
		;	rand Rad 100
		;	if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300914 1 0 0
		;	endif
		;	if @Rad >= 80 and @Rad < 95
		;		ADD_ITEM_NUM_INPAK 5300931 1 0 0
		;	endif
		;	if @Rad >= 95 and @Rad < 99
		;		ADD_ITEM_NUM_INPAK 5300932 1 0 0
		;	endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300933 1 0 0
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



