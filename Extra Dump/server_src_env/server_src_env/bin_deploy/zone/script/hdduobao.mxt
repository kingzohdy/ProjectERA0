
proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#bһ���춼���л��㶼�������������Ҫ��Ҫ�����ѽ����ֳ������أ�
	add_talk #b#b#c186506���ּ��껪����ʱ�䣺
	add_talk #b#b#c0181f9 13:00-13:20��18:00-18:20
	
	task_add_option 3331 1 100
	
	OPTION 202 ���ּ��껪
	
	strvar RetTsk
	task_stat 3124 1 RetTsk
	if RetTsk = 2
		option 200 ����ʥ���Ϲ������������
	endif
	talk
	
endproc

PROC 100
	task_do_option 3331 1
ENDPROC

proc 202

	dialog 1
	add_talk $_npc$:
	add_talk #b#b��������кö౦��������Ҫ�Ļ��ͽ�ȥ���ưɡ� ������

	OPTION 300 #�μӻ��ּ��껪
	talk
endproc
proc 300 
;	GET_PLAYER_ID 1 50
;	ID_PLAYER_BASE_ATT @50 1 1 210 1 
;	if 210 >= 40
		GET_EVENT_STAT 401 8		
	
		IF 401 = 1
			enter_pworld 201 0 12
		ELSE
			add_sys_msg_start
			add_sys_msg ���û��ʼ���޷����룡
			set_sys_msg_mode 4 0 0
			msg_info
		ENDIF
;	else
;		add_sys_msg_start
;		add_sys_msg ��ĵȼ�С��40�������ܽ��룡��
;		set_sys_msg_mode 4 0 0
;		msg_info
;	
;	endif

endproc

proc 200 
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�ҵ�ʥ������ҵ�ʥ�����HOHOHO��������ҲҪ��ʥ�����ˣ�
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
			task_fini 3124 1
		endif
	endif
endproc 

	






