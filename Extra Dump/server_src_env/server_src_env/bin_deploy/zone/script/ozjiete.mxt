

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b������̣�$_player$����ûʲô��Ҫ�����飬�벻Ҫ������Ѱ�һƽ��ż������������ṩ���ڻƽ��ż����������һ����ǳ����ı��ꡣ
	
	
;20~30 �޸�
	
;	task_add_option 1426 1 108
;	task_add_option 1427 1 109


	
;	TASK_STAT 1409 1 20
;	if 1 = 0 and 20 = 2
;		option 179 ��������նԻ�
;	endif

	TASK_STAT 1424 1 20 
	if 1 = 0 and 20 = 2
		option 400 #��ȡ��ָ

	endif
	task_stat 1426 1 21
	if 21 = 2
		option 410 #��������նԻ�
	endif
	
	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300915 1 num1
	if num1 > 0
		TASK_STAT 3305 1 Ret
		if Ret = 2
			option 700 �����뻻����
		endif
	endif

	talk
endproc 
proc 410
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b�¡��������㡭�㲻���Ѿ���
		add_talk #b#b<������տ�����ĳ��֣����ŵĺ����˼��ˣ��漴���ƺ�������ʲô����������һ˿����>
		
			option 411 #�þò����ˣ�
		talk
endproc 
;proc 411
;	task_fini 1426 1
;endproc

proc 179
	
;		GET_PLAYER_ID 0 79
;		GET_ID_PLAYER_STATUS_INFO @79 1099 80 1 0 0 0
		DIALOG 1
		add_talk $_npc$:
;		if 80 = 0
		
			add_talk #b#b������ԭҰ�����������ɧ����
			add_talk #b#b<����������ŵķ��˷���ñ�������˵>
			add_talk #b#b�����������������׵�һԱ����ΪĳЩ�޴�����������������֯����ûȥ׷�����������Ѿ���������һ���ˣ������û���������飬�벻Ҫ�����ҵ�ʱ�䡣

			option 180 #֪����
;		else
;			add_talk �㡭����˭��
;		
;		endif
		
		talk

endproc 

;PROC 108
;	task_do_option 1426 1
;ENDPROC
;PROC 109
;	task_do_option 1427 1
;ENDPROC

;proc 180
;	task_fini 1409 1
;	return
;endproc


proc 400
	GET_PLAYER_ID 1 54
	GET_ID_PLAYER_STATUS_INFO @54 1099 60 3 0 0 0

	if 60 = 0

		TARGET_ADD_STATUS 1 @54 1099 2 0 53
		add_item_num_inpak 5300294 1 1 0

		GET_PLAYER_MAPINFO 1 2 3 56 57
		call_mon 1 204008 500 10 @56 @57 1

	else
	
		add_sys_msg_start
		add_sys_msg �����Ŀ�ͽ�����컯��֮�»������ٲ��ɣ�
		set_sys_msg_mode 4 0 0
		msg_info
	
	endif

endproc

proc 2
	get_trig_task 1 2

;	if 2 = 1427
;
;		task_fini 1427 1
;		add_sys_msg_start
;		add_sys_msg ������գ���Ȼ���Ʋ��ԳԷ��ƣ������б�Ҫ�úý�ѵ��ѵ�㡣
;		set_sys_msg_mode 4 0 0
;		msg_info
;		CALL_MON 1 204090 500 30 3400 2231 1
;		del_npc 304008 1
		
;	endif 
;	if 2 = 1439
;		add_sys_msg_start
;		add_sys_msg �������ͻȻ��Զ����ȥ����׷�����������ˣ�
;		set_sys_msg_mode 3 0 0
;		msg_info
;		
;		call_mon 1 304037 0 0 3300 2200 1 
;		CALL_MON_BY_PATROL 1 204091 0 60 3300 2200 72 0 30
;
;		del_npc 304008 1
;	endif
endproc



proc 700
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���˶�˵�ƽ�֮���Ǵ�˵����ȴ����ΪȻ�����еĴ�˵��Ȼ���������գ���Ȼ�������ⱾС˵�Ƕ�׫�ģ��������ܹ��е�ʲô��������ʵ���ڵġ�
	add_talk #b#b��ô����Ҳ��������ҵ��ƽ�֮�ǣ����������˴�뱲�ӵ�ʱ�䶼û�ҵ�����ƾ�㣿
		option 701 #����

	talk
	
endproc
proc 701
	
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300915 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300915 1 1
			
		;	rand Rad 100
		;	if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300930 1 0 0
		;	endif
		;	if @Rad >= 80 and @Rad < 95
		;		ADD_ITEM_NUM_INPAK 5300917 1 0 0
		;	endif
		;	if @Rad >= 95 and @Rad < 99
		;		ADD_ITEM_NUM_INPAK 5300918 1 0 0
		;	endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300919 1 0 0
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