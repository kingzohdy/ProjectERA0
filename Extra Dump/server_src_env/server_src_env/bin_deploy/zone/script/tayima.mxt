;������ű�
;20--����1000���ر�־ 100--
;21--����1001���ر�ʾ 101--����1001�¼� 51--����1001״̬
;27--����1007���ر�־ 107--
;28--����1008���ر�־ 108--����1008�¼� 58--����1008״̬
;29--����1009���ر�־ 109--����1009�¼� 59--����1009״̬
;30--����1010���ر�־ 110--����1010�¼�	60--����1010״̬
;31--����1011���ر�־ 111--����1011�¼�
;0--����δ��ȡ 1--��������� 2--����δ���
;600--proc2trig 800--taskid

proc 1

	dialog 1
	
	add_talk ��������������,�ҿɲ��뱻��Ⱥ��֪�ĵ������ƻ�,�ҵ���ʿ,���Ը������ң�

	task_add_option 1000 20 100
	
	task_add_option 1001 21 101

	task_add_option 1007 27 107
	
	task_add_option 1008 28	108

	task_add_option 1009 29 109

	task_add_option 1010 30 110

	task_add_option 1011 31 111

	talk
endproc

proc 100
	task_do_option 1000 20
endproc

proc 101
	task_do_option 1001 21
endproc

proc 107
	task_do_option 1007 27
endproc

proc 108
	task_do_option 1008 28
endproc

proc 109
	task_do_option 1009 29
endproc

proc 110
	task_do_option 1010 30
endproc

proc 111
	task_do_option 1011 31
endproc

proc 2
	get_trig_task 600 800
	if 800 = 1001
		task_fini 1001 51
	endif
	if 800 = 1011 
		task_fini 1011 61
	endif
endproc
