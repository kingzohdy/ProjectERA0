;�ػ����� 1746
 
proc 1
;����ˢ��
	STRVAR a
endproc
proc 2
;��������
		get_task_var 1746 1 8 1 a
		var a 1
		set_task_var 1746 1 8 @a 1
			add_sys_msg_start
			add_sys_msg ���������ˣ�����ʧ����
			set_sys_msg_mode 3 0 0
			msg_info
endproc

