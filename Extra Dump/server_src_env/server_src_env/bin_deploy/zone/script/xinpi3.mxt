;��ճǽ̹ٽű�
;39--����1019���ر�־ 119--
;40--����1020���ر�־ 120--����1020�¼� 70--����1020״̬
;0--����δ��ȡ 1--��������� 2--����δ���
;600--proc2trig 800--taskid

proc 1

	dialog 1
	
	add_talk ��ս���ϻ������������ı���,��ճ�,�ܹ����������˲Żᱻ��ͬ,������������ʿ��

	task_add_option 1019 39 119
	
	task_add_option 1020 40 120
	
	option 107 #���ø���
	
	talk
endproc

proc 119
	task_do_option 1019 39
endproc

proc 120
	task_do_option 1020 40
endproc

proc 107
	reset_pworld 502
	ADD_SYS_MSG_START
	ADD_SYS_MSG ����������
	SET_SYS_MSG_MODE 4 0 0
	MAP_MSG_INFO
endproc

proc 2
	GET_TRIG_TASK 600 800
	IF 800 = 1020
		ENTER_PWORLD 502 0 1		
	ENDIF
ENDPROC
