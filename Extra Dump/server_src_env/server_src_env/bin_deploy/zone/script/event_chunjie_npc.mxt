
proc 1

;	add_sys_msg_start
;	add_sys_msg ���ڻ��ʼ������ʱ��������ҵ�����ʹ�����죬�μӸ�����Ȥ�Ļ�ɣ�
;	set_sys_msg_mode 6 0 0
;	svr_msg_info

	NEW_MAP_LIFE_NPC 1 1000 307023 29400 33300 0 1814400 zcletian.mac
	
endproc 
proc 2
	GET_MAP_INST 1000 10
	DEL_NPC 307023 15
	add_sys_msg_start
	add_sys_msg ���ڻ�Ѿ�Բ���������ڴ�Ԥף�����2013������Ʒ������ROLL��100�����ض��飡��
	set_sys_msg_mode 6 0 0
	svr_msg_info
	
endproc
