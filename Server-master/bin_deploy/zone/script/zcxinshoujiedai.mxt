

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��ӭ����ʱ��ǣ�$_player$��
    add_talk #b#bΪ�˸�л�������ǵİ�������Ϊ��׼���˴�����������Ի������������֣���Ȼ���Ҳ���������Ǯ��һ��Ҳ�����գ���ֻ��Ҫ��������ȯ���������ˣ�
	add_talk #b#b�����ƽʱϲ��������Ļ�������Ӧ���ռ�������ȯ�˰ɣ�
	
;	OPTION 199 #(����)������ȯ�̵�
	
;	OPTION 300 #�������Ӷһ�΢�����ӣ�1��3��
;	OPTION 301 #Ч�����Ӷһ�΢�����ӣ�1��2��
;	OPTION 302 #̫����Դ�һ������Դ��1��3��
;	OPTION 303 #Զʼ��Դ�һ������Դ��1��2��
	
	
;	option 300 �и���
	task_add_option 2027 1 101
	task_add_option 2028 1 102
	
	strvar RetTsk
	task_stat 3126 1 RetTsk
	if RetTsk = 2
		option 200 ����ʥ���Ϲ������������
	endif
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;	
;	strvar month sun
;	GET_NOW_TIME month sun 0 0 0 0 0
;	if month = 5 and sun >= ?
	
;	OPTION 300 #��ȡ10����
;		
;	endif
;	if month > 5 and sun >= 1
;	
;	endif
	
	talk
endproc 

;proc 300
;		ADD_EXP 0 100000 0
;endproc 



PROC 199
	OPEN_NPC_SHOP
ENDPROC

PROC 101
	task_do_option 2027 1
ENDPROC

PROC 102
	task_do_option 2028 1
ENDPROC

proc 200 
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b Ү��ʥ�����������ˣ�лл�㣡$_player$��
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
			task_fini 3126 1
		endif
	endif
endproc 

proc 300
;;;;; �������Ӷһ�΢������
	strvar Num Num1 Ret Ret1 packNum zNum pNum BangNum
	var Num 0
	var Num1 0
	GET_ITEM_NUM_INPAK 5233512 1 Num
	GET_ITEM_NUM_INPAK 5233513 1 Num1

	var BangNum @Num
	add BangNum @Num1
	;�����ж��ٶһ���Ʒ����
	
	var zNum @BangNum
	mul zNum 3
	;�ó��ɶһ��ĳ������Ӹ���packNum	
	
	var packNum @zNum
	Dev packNum 99
	;��Ҫ���ٱ�������
	
	GET_PACK_SPACE 0 pNum
	if BangNum != 0
		if pNum >= @packNum
			dec_item_num_inpak 5233512 @Num Ret
			dec_item_num_inpak 5233513 @Num1 Ret1
			if Ret = 0 or Ret1 = 0
				ADD_ITEM_NUM_INPAK 5233500 @zNum 0 1
			endif
		else
			add_sys_msg_start
			add_sys_msg ��Ԥ��$@packNum$�����ϵı����ո��ٶһ���Ʒ��
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg ��Я���Ķһ����������㣬�޷��һ���
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc

proc 301
;;;;; Ч�����Ӷһ�΢������
	strvar Num Num1 Ret Ret1 packNum zNum pNum BangNum
	var Num 0
	var Num1 0
	GET_ITEM_NUM_INPAK 5233501 1 Num
	GET_ITEM_NUM_INPAK 5233511 1 Num1
	
	var BangNum @Num
	add BangNum @Num1
	;�����ж��ٶһ���Ʒ����
	
	var zNum @BangNum
	mul zNum 2
	;�ó��ɶһ��ĳ������Ӹ���packNum	
	
	var packNum @zNum
	Dev packNum 99
	;��Ҫ���ٱ�������
	
	GET_PACK_SPACE 0 pNum
	if BangNum != 0
		if pNum >= @packNum
			dec_item_num_inpak 5233501 @Num Ret
			dec_item_num_inpak 5233511 @Num1 Ret1
			if Ret = 0 or Ret1 = 0
				ADD_ITEM_NUM_INPAK 5233500 @zNum 0 1
			endif
		else
			add_sys_msg_start
			add_sys_msg ��Ԥ��$@packNum$�����ϵı����ո��ٶһ���Ʒ��
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg ��Я���Ķһ����������㣬�޷��һ���
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc


proc 302
;;;;; ̫����Դ������
	strvar Num Num1 Ret Ret1 packNum zNum pNum
	
	GET_ITEM_NUM_INPAK 5251117 1 Num

	;�����ж��ٶһ���Ʒ����
	
	var zNum @Num
	mul zNum 3
	;�ó��ɶһ��ĳ������Ӹ���packNum	
	
	var packNum @zNum
	Dev packNum 20
	;��Ҫ���ٱ�������.
	
	GET_PACK_SPACE 0 pNum
	if Num != 0
		if pNum >= @packNum
			dec_item_num_inpak 5251117 @Num Ret
			if Ret = 0
				ADD_ITEM_NUM_INPAK 5251115 @zNum 0 1
			endif
		else
			add_sys_msg_start
			add_sys_msg ��Ԥ��$@packNum$�����ϵı����ո��ٶһ���Ʒ��
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg ��Я���Ķһ����������㣬�޷��һ���
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc

proc 303
;;;;; Զʼ��Դ
	strvar Num Num1 Ret Ret1 packNum zNum pNum
	GET_ITEM_NUM_INPAK 5251116 1 Num
	;�����ж��ٶһ���Ʒ����
	
	var zNum @Num
	mul zNum 2

	;�ó��ɶһ��ĳ������Ӹ���packNum	
	
	var packNum @zNum
	Dev packNum 20
	
	;��Ҫ���ٱ�������.
	
	GET_PACK_SPACE 0 pNum
	if Num != 0
		if pNum >= @packNum
			dec_item_num_inpak 5251116 @Num Ret
			if Ret = 0
				ADD_ITEM_NUM_INPAK 5251115 @zNum 0 1
			endif
		else
			add_sys_msg_start
			add_sys_msg ��Ԥ��$@packNum$�����ϵı����ո��ٶһ���Ʒ��
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg ��Я���Ķһ����������㣬�޷��һ���
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc



proc 2
	get_trig_task 1 2
	if 2 = 2028 
		task_fini 2028 1
	endif

endproc
