
;��ȫ�˺�

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b$_player$������Ϊ�����Ԫ������Ҫ����ʲô��
	add_talk #b#b��Ȼ���˺Ű�ȫ�ˣ��������˺Ű�ȫ�ò������ϣ���ô�ȼ���װ�������ʲô�Ķ��Ǹ��ư���
	add_talk #b#b˵������Щ�������ӻ������������ȥ��ƭ��ĺ��ѣ�����������ʧ����

	task_add_option 2260 1 100
	task_add_option 2261 1 101
	
	
;	player_need_new_name 1
;	if 1 > 0
;		option 190 #�޸Ľ�ɫ��
;	endif
	clan_need_new_name 1
	if 1 > 0
		option 191 #�޸Ĺ�����
	endif
	
	talk
	
endproc 

proc 190
	OPEN_UI 17
endproc

proc 191
	open_ui 53
endproc


PROC 100
	task_do_option 2260 1
ENDPROC

PROC 101
	task_do_option 2261 1
ENDPROC

proc 2
	
	get_trig_task 1 2
	if 2 = 2260
		IS_UIN_FLAG_SET 30 0 64
		if 30 = 1
			task_fini 2260 1
		endif
	endif
	
	if 2 = 2261
		IS_UIN_FLAG_SET 30 0 32
		if 30 = 1
			task_fini 2261 1
		endif
	endif
endproc 















