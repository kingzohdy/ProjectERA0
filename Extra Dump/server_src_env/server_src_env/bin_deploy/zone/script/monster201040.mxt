;1�Ž��BOSS 

proc 2
	strvar hp mid
	GET_MON_INFO 0 0 mid hp 0 0 0 0
	if hp = 0

		get_pworld_var 0 32 1 10
		DEL_ID_MON @10 1
		
		MON_SPEECH @mid 1 2 1 �ҡ������С����ˣ����ǡ���
		
	;	add_sys_msg_start
	;	add_sys_msg ��������ӥ��Ա���������ν�磬��ǿ��������Ƽ�������ƽ�磬�ȳ����ǣ�
	;	set_sys_msg_mode 4 0 0
	;	map_msg_info
	
	endif
endproc 

proc 9
	DESTROY_MON 1
	strvar momid
	NEW_MON_BY_DIR 201040 1 19200 29800 0 0 0 30 1 momid
	TARGET_ADD_STATUS 3 @momid 1095 1 25920000 0 

endproc 





