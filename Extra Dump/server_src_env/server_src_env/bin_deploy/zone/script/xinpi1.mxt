;�������̹ٽű�
;22--����1002���ر�־ 102--             52--����1002״̬
;23--����1003���ر�־ 103--����1003�¼� 53--����1003״̬
;24--����1004���ر�־ 104--����1004�¼� 54--����1004״̬

;0--����δ��ȡ 1--��������� 2--����δ���
;600--proc2trig 800--taskid
;501--���븱��1 502--�뿪 

proc 1

	dialog 1
	
	add_talk ��������Ѫ��ζ,�����Ⱥ��ν�ġ���ʿ����ܲ�ϲ�������������Զ����ս���Ĳпᡣ

	task_add_option 1002 22 102
	
	task_add_option 1003 23 103

	task_add_option 1004 24 104
	
	task_stat 1003 23 53
	if 53 = 3
		option 501 #���븱��
	endif
	
	if 53 = 2
		option 503 #���븱��
	endif
	
	option 502 #�뿪

	option 107 #���ø���
	talk
endproc

proc 102
	task_do_option 1002 22
endproc

proc 103
	task_do_option 1003 23
endproc

proc 104
	task_do_option 1004 24
endproc

proc 107
	reset_pworld 500
	ADD_SYS_MSG_START
	ADD_SYS_MSG ����������
	SET_SYS_MSG_MODE 4 0 0
	MAP_MSG_INFO
endproc

proc 2	
	get_trig_task 600 800
	if 800 = 1004
		task_fini 1004 24
	endif
	if 800 = 1003
		enter_pworld 500 0 1
	endif
endproc

proc 501 
	enter_pworld 500 0 1
endproc

proc 502
	return
endproc

