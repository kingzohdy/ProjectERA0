;������˹�ű�
;35--����1015���ر�־ 115--
;36--����1016���ر�־ 116--����1016�¼� 66--����1016״̬
;37--����1017���ر�־ 117--����1017�¼� 67--����1016״̬
;38--����1018���ر�־ 118--����1018�¼� 68--����1018״̬
;0--����δ��ȡ 1--��������� 2--����δ���
;600--proc2trig 800--taskid

proc 1
	dialog 1
	
	add_talk ���汲�Ϳ�ʼ��ս��,�����ڲ��ϵ�ս��,���Ѿ�����,Ҳ������ʤ������ĵ�����

	task_add_option 1015 35 115
	
	task_add_option 1016 36 116

	task_add_option 1017 37 117

	task_add_option 1018 38 118

	talk
endproc 

proc 115
	task_do_option 1015 35
endproc

proc 116
	task_do_option 1016 36
endproc

proc 117
	task_do_option 1017 37
endproc

proc 118
	task_do_option 1018 38
endproc

proc 2
	get_trig_task 600 800
	if 800 = 1018
		task_fini 1018 68
	endif
endproc
