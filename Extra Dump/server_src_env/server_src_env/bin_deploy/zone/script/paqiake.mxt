

proc 1
	DIALOG 1
	;STRVAR id
	;GET_PLAYER_ID 1 id
	;HAS_STATUS  1219 1
	;IF 1 = 0
	;	TARGET_ADD_STATUS 1 @id 1219 1 1 1
	;ENDIF
	add_talk $_npc$:
	add_talk  #b#b������ʵ�ڲ�֪�������������ĸı䣬�����¸�����ڰ���սʱ���ͻᷢ�ֺڰ����Ѳ��ڡ�  


	task_add_option 2240 1 113
	
	task_add_option 1719 1 11
	task_add_option 1720 1 12
	task_add_option 1721 1 13
	task_add_option 1722 1 14
	task_add_option 1723 1 15

	
	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300925 1 num1
	if num1 > 0
		TASK_STAT 3305 1 Ret
		if Ret = 2
			option 700 �����뻻����
		endif
	endif
	talk
endproc 
PROC 11
	task_do_option 1719 1
ENDPROC
PROC 12
	task_do_option 1720 1
ENDPROC
PROC 13
	task_do_option 1721 1
ENDPROC
PROC 14
	task_do_option 1722 1
ENDPROC
PROC 15
	task_do_option 1723 1
ENDPROC

PROC 113
	task_do_option 2240 1
ENDPROC

PROC 150
    	DIALOG 1
   	ADD_TALK $_npc$       
    	ADD_TALK  �����Ѿ����ٵ���Ƭ���أ��ֲ�������һ������һ��������

	OPTION 250 #(����)֪���ˣ�
	TALK
ENDPROC
PROC 250
	GET_ITEM_NUM_INPAK 5300509 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 5300509 1 0
	task_fini 12062 1
	endif
ENDPROC

PROC 160
    	DIALOG 1
   	ADD_TALK $_npc$       
    	ADD_TALK  ��Σ�յĴ���֮�����ߣ�һֱ������ȥע����ʲô���������������������ʦҪ��Ļ��������һῼ��һ�¡�

	OPTION 260 #(����)лл���ˣ�
	TALK
ENDPROC
PROC 260
	GET_ITEM_NUM_INPAK 5300519 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 5300519 1 0
	task_fini 12262 1
	endif
ENDPROC


proc 700
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���ڽ���Ʈ������ʲô���˽���ط��׸��õ��أ�
	add_talk #b#b��������ۣ���ƣ�����־���ֶ�����������ˮ����ض��ǡ�

		option 701 #����

	talk
	
endproc
proc 701
	
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300925 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300925 1 1
			
		;	rand Rad 100
		;	if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300924 1 0 0
		;	endif
		;	if @Rad >= 80 and @Rad < 95
		;		ADD_ITEM_NUM_INPAK 5300927 1 0 0
		;	endif
		;	if @Rad >= 95 and @Rad < 99
		;		ADD_ITEM_NUM_INPAK 5300928 1 0 0
		;	endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300929 1 0 0
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