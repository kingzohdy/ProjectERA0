


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bվס������������ʲô����˵���أ�
	add_talk #b#b��ʲô����ס�ģ������ˣ����Ｘ���ˣ���ʵ������
	add_talk #b#b���п������롣��������
	add_talk #b#bʲô��ʱ���������Э���ߣ�����ʧ���ˣ�#Name#���ˣ�������֣���������ۺ����ǳ�����������Ҫ��ÿһ��İ����׽����Ų飡

		task_add_option 1366 1 100
		task_add_option 1367 1 101
		task_add_option 1368 1 102

	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300914 1 num1
	if num1 > 0
		TASK_STAT 3305 1 Ret
		if Ret = 2
			option 700 �����뻻����
		endif
	endif
	talk
	
endproc 

PROC 100
	task_do_option 1366 1
ENDPROC
PROC 101
	task_do_option 1367 1
ENDPROC
PROC 102
	task_do_option 1368 1
ENDPROC

proc 2

	get_trig_task 1 2
	if 2 = 1368
		task_fini 1368 1
	endif

endproc 
	
proc 700
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���������������������׽���������㿴��Щ��������ͷ����ʹʲô��������Ҳ��Խ��Խ������һ��û�䣬��һ��������ʰ��ֱ�������˼���
	add_talk #b#b�������ǽ��������ף���ϧ�Ҿ�û����ô����������ֻ����������ķϵ���ͷ���������ű�ʱ��ǻ�Ҫ���ֵ����Ρ�
	add_talk #b#bͬ�����Σ����в���ȫ֮��
		option 701 #����

	talk
	
endproc
proc 701
	
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300914 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300914 1 1
			
		;	rand Rad 100
		;	if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300915 1 0 0
		;	endif
		;	if @Rad >= 80 and @Rad < 95
		;		ADD_ITEM_NUM_INPAK 5300916 1 0 0
		;	endif
		;	if @Rad >= 95 and @Rad < 99
		;		ADD_ITEM_NUM_INPAK 5300917 1 0 0
		;	endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300918 1 0 0
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

