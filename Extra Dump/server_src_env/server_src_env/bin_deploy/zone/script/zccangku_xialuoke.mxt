 

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��ӭ���١��������С������Ǹ���ֿ����Ļ�����$_npc$���ṩ����Ʒ��Ǯ����صĴ洢����
	add_talk #b#b#c186506Ϊ�������˺ŲƲ���ȫ�������ñ�������ݼ� B ����ȫ����#n

	;��һ�
	task_stat 4431 1 51
	if 51 = 2
		option 510 ����ο�ʡ�����������
	endif
	
	OPTION 104 #�򿪲ֿ�
;	option 200 #�����ҽ�����
	option 130 #�޸İ�ȫ������
	option 131 #ȡ����ȫ������
	talk
endproc 

PROC 104
	OPEN_UI 0
ENDPROC

proc 130
	OPEN_UI 55
endproc
proc 131
	open_ui 56
endproc

;��һ�
proc 510
	GET_ITEM_NUM_INPAK 5301004 1 10
	STRVAR flag
	var flag 0
	if 10 > 0
		DEC_ITEM_NUM_INPAK 5301004 1 11
		if 11 = 0
			var flag 1
			task_fini 4431 1
			
			DIALOG 1
			add_talk $_npc$:
			add_talk #b#bлл�㣬$_player$���뽫�ҵ�ף���������ͣ�˳������лл����
			add_talk #b#b��������̤�Ͽ��ֵ��ọ́�
			add_talk #b#b�����ۣ������续�ķ羰��
			add_talk #b#b���¼���������Զ���Ҹ���
			add_talk #b#bлл�㣬���ݿ��ֺ����㡣
			option 511 #û����
			talk
		endif
	endif
	
	if flag = 0
		add_sys_msg_start
		add_sys_msg ���ı�����û�С���Ʒ�С���
		set_sys_msg_mode 1 0 0
		msg_info
		PLAYER_CLOSE_DIALOG1 0 0
	endif
endproc