
;ѩԭ���ͷ��
proc 1
	strvar Lv
	GET_ID_PLAYER_INFO  0 8 Lv
	if Lv >= 70
		transmit 2201 32700 4400 1
	else
		add_sys_msg_start
		add_sys_msg �˵�ͼ̫Σ���ˣ�Ϊ������������������ǵ�70���Ժ���ȥ�ɣ�
		set_sys_msg_mode 3 0 0
		msg_info
	endif	
endproc
