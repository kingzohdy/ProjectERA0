;���ִ�1����Ա�ű�
;50--��ͼ����ID 60--��ͼʵ��ID 70--����X 80--����Y

proc 1
	GET_PLAYER_MAPINFO  1 2 50 10 11
			
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�����￴����������ѣ�����Ӧ������̫�ࣿ��ȥ����λ�����յ�������ѡ������ȥ�ĵط��ɣ��ҿ�����������ȥ��
	if 50 = 2000
		option 11 #��յ�����
		option 12 #��յ�����
		option 13 #��յ�����
		option 14 #�ټ�
	endif
	if 50 = 2001
		option 10 #��յ�һ��
		option 12 #��յ�����
		option 13 #��յ�����
		option 14 #�ټ�
	endif
	if 50 = 2002
		option 10 #��յ�һ��
		option 11 #��յ�����
		option 13 #��յ�����
		option 14 #�ټ�
	endif
	if 50 = 2003
		option 10 #��յ�һ��
		option 11 #��յ�����
		option 12 #��յ�����
		option 14 #�ټ�
	endif
	
	;option 15 #���͵�ͭ�Ĵ�
	talk
endproc
	
proc 10
	transmit 2000 17037 22834 1
endproc


proc 11
	transmit 2001 17037 22834 1
endproc
	
proc 12
	transmit 2002 17037 22834 1
endproc
	
proc 13
	transmit 2003 17037 22834 1
endproc
	
proc 14
	return
endproc

;proc 15
;	get_player_base_att 1 2 3 4
;	if 3 > 9
;		transmit 2100 4181 3890 1
;	else
;		add_sys_msg_start
;		add_sys_msg �㻹̫����,���������������Ұ�!
;		set_sys_msg_mode 4 0 0
;		msg_info
;	endif
;endproc

