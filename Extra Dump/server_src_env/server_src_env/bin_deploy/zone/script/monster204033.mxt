proc 1

;	GET_MON_INFO 1 2 30 31 32 33 0 0
;	TARGET_ADD_STATUS 3 @30 1006 2 4 1
	
	GET_MON_VAR 0 8 1 10
	var 10 0
	SET_MON_VAR 0 8 @10 1
	
	GET_MON_VAR 1 8 1 11
	var 11 0
	SET_MON_VAR 1 8 @11 1
	strvar Mid Rad
	GET_MON_INFO 1 2 Mid 31 32 33 0 0
	rand Rad 3
	if Rad = 0
		MON_SPEECH @Mid 3 1 1 ����û��ս���ܸ����ҵ��ٶȣ�	
	endif
	if Rad = 1
		MON_SPEECH @Mid 3 1 1 �յ����ҵȴ�ʧ���ߵ���̨��
	endif
	if Rad = 2
		MON_SPEECH @Mid 3 1 1 �ҿ�����������ʧ�ܵ����飡
	endif	
	
endproc
proc 2

	MON_SPEECH @30 3 1 1 ��Ȼ������㣬�Ժ��һ�����������ġ���	
	add_sys_msg_start
	add_sys_msg ��Ӯ���˱�����ʤ����
	set_sys_msg_mode 3 0 0
	msg_info


endproc
proc 3 
;Ѳ�߽���

;	GET_MON_VAR 0 8 1 10
;	var 10 1
;	SET_MON_VAR 0 8 @10 1
	
;	GET_MON_INFO 1 2 30 31 32 33 0 0
;	TARGET_ADD_STATUS 3 @30 1006 2 10 1	

;			MON_SPEECH @30 3 1 1 ��Ȼ������㣬�Ժ��һ�����������ġ���		
;			DESTROY_MON 1	
			del_mon 1
endproc 
;PROC 4
;
;	GET_MON_VAR 0 8 1 10	
;	IF 10 = 1
;		GET_MON_VAR 1 8 1 11
;		ADD 11 1
;		SET_MON_VAR 1 8 @11 1
;		if 11 = 2
;			GET_MON_INFO 1 2 30 31 32 33 0 0	
;			
;		endif
;		if 11 = 3
;	
;		endif
;	ENDIF
;ENDPROC 
