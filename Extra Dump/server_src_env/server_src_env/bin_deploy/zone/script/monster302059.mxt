
proc 1
	var 10 1
	set_mon_var 0 8 @10 1
	strvar Mid
	GET_MON_INFO 1 1 Mid 1 1 1 1 1
	MON_SPEECH @Mid 1 2 1 ��������������ǻ�����ǰ������̨��С��;�е�����ͻϮ�������ұ���������ζ������ʧ�ܡ�
endproc
proc 2
		TASK_FAIL 3143 1
endproc 
proc 3
;Ѳ�߽���
	DESTROY_MON 0
	new_npc 1 302055 26300 46400 0 zyzqzhanshi.mac
	strvar Mid
	GET_MON_INFO 1 1 Mid 1 1 1 1 1
	MON_SPEECH @Mid 1 2 1 ���ǵ���Ŀ�ĵ��ˣ����ֲ�������������Ҫ��Ը�ǿ����谭�ˣ�׼���ú�������Ұɡ�

endproc
proc 4
	strvar HP
	GET_MON_INFO 1 1 1 HP 1 1 1 1
	if HP < 500
		strvar Mid
		GET_MON_INFO 1 1 Mid 1 1 1 1 1
		MON_SPEECH @Mid 1 2 1 �ҿ�Ҫ��ֲ�ס�ˣ�
	endif
endproc
	
proc 5
;Ѳ�ߵ�ص�
;	get_mon_var 0 8 1 10
;	add 10 1
;	set_mon_var 0 8 @10 1
	
	strvar Num Mid
	GET_MON_INFO 0 0 Mid 0 0 0 0 0
	GET_MON_PATROLIDX @Mid Num 0
	
	if Num = 2
		MON_CALL_ENEMY_MON 205019 1 50 4 16300 43000 0 0 0 0 0 0 0
		rand 90 3
		switch 90
			case 0
				MON_SPEECH @Mid 1 2 1 �����ң�
			endcase
			case 1
				MON_SPEECH @Mid 1 2 1 ����ͻϮ�����ˣ�
			endcase
			case 2
				MON_SPEECH @Mid 1 2 1 ����׼����
			endcase
								
		endswitch
	endif
	if Num = 3
		MON_CALL_ENEMY_MON 205019 1 50 4 20200 43600 0 0 0 0 0 0 0
		MON_CALL_ENEMY_MON 205019 1 50 4 20200 42900 0 0 0 0 0 0 0
		rand 90 3
		switch 90
			case 0
				MON_SPEECH @Mid 1 2 1 �����ң�
			endcase
			case 1
				MON_SPEECH @Mid 1 2 1 ����ͻϮ�����ˣ�
			endcase
			case 2
				MON_SPEECH @Mid 1 2 1 ����׼����
			endcase
								
		endswitch		
		
	endif
	if Num = 4
		MON_CALL_ENEMY_MON 205019 1 50 4 26200 43800 0 0 0 0 0 0 0
		MON_CALL_ENEMY_MON 205019 1 50 4 26200 43000 0 0 0 0 0 0 0
		MON_CALL_ENEMY_MON 205019 1 50 4 26700 43500 0 0 0 0 0 0 0
		rand 90 3
		switch 90
			case 0
				MON_SPEECH @Mid 1 2 1 �����ң�
			endcase
			case 1
				MON_SPEECH @Mid 1 2 1 ����ͻϮ�����ˣ�
			endcase
			case 2
				MON_SPEECH @Mid 1 2 1 ����׼����
			endcase
								
		endswitch
	endif
endproc
