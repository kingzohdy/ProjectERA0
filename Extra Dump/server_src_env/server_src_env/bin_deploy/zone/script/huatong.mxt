
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��ð��������ʻ��͸��İ����˰�!
	
	;��һ�
	task_stat 4437 1 51
	if 51 = 2
		option 510 ����ο�ʡ�����������
	endif
	
	strvar Ret num1 num2
	GET_ITEM_NUM_INPAK 5300761 1 num1
	if num1 > 0
		TASK_STAT 3304 1 Ret
		if Ret = 2
			option 500 �����뻻����
		endif
	endif
	
	GET_ITEM_NUM_INPAK 5300911 1 num2
	if num2 > 0
		TASK_STAT 3305 1 Ret
		if Ret = 2
			option 700 �����뻻����
		endif
	endif
	
	strvar RetTsk
	task_stat 3125 1 RetTsk
	if RetTsk = 2
		option 200 ����ʥ���Ϲ������������
	endif
	
	option 800 #��Ҫ��һ�顰Ũ���ɿ�������
	option 900 #���λû�
	
	talk
endproc

proc 900
	OPEN_UI 93
endproc

proc 800
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�������Ũ���ɿ��������д���ȴ�����Ĳ���
	add_talk #b#b#cFF0080�������������ɿ������������ִ�ŵİ��⡣#n
	add_talk #b#b
	add_talk #b#b#c005ebe�ɹ���׺�˫���ɳ�Ϊ�����¡�#n
	add_talk #b#b�ۼۣ�#c18650610000����#n
	
	option 801 #��Ҫ��һ�顰Ũ���ɿ�����
	
	talk
endproc

proc 801
	PKG_IS_UNLOCK 10 20
	if 10 = 0 and 20 = 0
		return
	endif
	
	strvar spret mret
	GET_PACK_SPACE 0 spret
	if spret > 0
		dec_money 0 10000 20 mret
		if mret = -1
			add_sys_msg_start
			add_sys_msg ������Я�������Ҳ�������ʱ�޷������ɿ���
			set_sys_msg_mode 3 0 0
			msg_info
			
			BUY_HETONG 1
			; ���Ҳ����򿪿�ݹ���ʽ
			return
		else
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251131 1 1 0
			add_sys_msg_start
			add_sys_msg ���л������С������Ľӹ��ˡ�Ũ���ɿ�����
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg ��ı�����װ�����ˣ�������1�񱳰����Ӻ��ٹ���
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
endproc


proc 200 
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bлл�㣬ʥ���Ϲ��������������һ����úð�ϧ�ģ�
		option 210 #����ʥ������
	talk
endproc 
proc 210
	GET_ITEM_NUM_INPAK 5300844 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ��
		set_sys_msg_mode 1 0 0
		msg_info
	ENDIF
	if 11 >= 1
			
		dec_item_num_inpak 5300844 1 30
		if 30 != -1
			task_fini 3125 1
		endif
	endif
endproc 

proc 500
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���뻻��������Ȼ�������е�ĵ���������־���¾��ɣ�Ҳ�����ɹ��أ�
	add_talk #b#b������������û�У�ֻ��һ�����Ԫʷ�ǣ����ǿ˶��������͸��ҵ��أ��Ҿ���������㽻���ɡ�

		option 501 #����

	talk
	
endproc
proc 501
	
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300761 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300761 1 1
			
	;		rand Rad 100
	;		if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300762 1 0 0
	;		endif
	;		if @Rad >= 80 and @Rad < 95
	;			ADD_ITEM_NUM_INPAK 5300763 1 0 0
	;		endif
	;		if @Rad >= 95 and @Rad < 99
	;			ADD_ITEM_NUM_INPAK 5300764 1 0 0
	;		endif
	;		if @Rad = 99
	;			ADD_ITEM_NUM_INPAK 5300765 1 0 0
	;		endif
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
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
proc 700
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����һ�����ɳ��˴��̣����˱���֮����ö����ͻ��÷ǳ��м����ֵ���ţ��Һ��㻻�ɣ���Ȼ��û�б��������������ܰ���򿪳ɹ����ŵ�Կ�ף�
	add_talk #b#b���������������һ���ռ����Ԫʷ�ķ糱���ҾͳԵ���������Ԫʷ���㻻����Σ�
		option 701 #����

	talk
	
endproc
proc 701
	
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300911 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300911 1 1
			
		;	rand Rad 100
		;	if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300912 1 0 0
		;	endif
		;	if @Rad >= 80 and @Rad < 95
		;		ADD_ITEM_NUM_INPAK 5300913 1 0 0
		;	endif
		;	if @Rad >= 95 and @Rad < 99
		;		ADD_ITEM_NUM_INPAK 5300914 1 0 0
		;	endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300915 1 0 0
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

;��һ�
proc 510
	GET_ITEM_NUM_INPAK 5052308 1 10
	STRVAR flag
	var flag 0
	if 10 > 0
		DEC_ITEM_NUM_INPAK 5052308 1 11
		if 11 = 0
			var flag 1
			task_fini 4437 1
			
			DIALOG 1
			add_talk $_npc$:
			add_talk #b#bлл�㣬��#Gender1#���ҳ�����ҲҪ���ͽ������Ư���ܸɣ�
			option 511 #�ҿ�����Ӵ��
			talk
		endif
	endif
	
	if flag = 0
		add_sys_msg_start
		add_sys_msg ���ı�����û�С�ο�����ʡ���
		set_sys_msg_mode 1 0 0
		msg_info
		PLAYER_CLOSE_DIALOG1 0 0
	endif
endproc
