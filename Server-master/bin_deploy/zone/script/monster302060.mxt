;������ά��
proc 1
	strvar Mid
	GET_MON_INFO 1 1 Mid 1 1 1 1 1
	MON_SPEECH @Mid 1 2 1 ��Щ̰����͵���߻����ڸ��������㻤����ȥ�������ǡ�
endproc
proc 2
	TASK_FAIL 3148 1
endproc 
proc 3
;Ѳ�߽���
	DESTROY_MON 0
	new_npc 1 302056 26300 46700 0 zyzqjisi.mac
	
	get_pworld_var 0 32 1 23
	var 23 1
	set_pworld_var 0 32 @23 1
	
endproc
proc 4
	strvar HP
	GET_MON_INFO 1 1 1 HP 1 1 1 1
	if HP < 800
		strvar Mid
		GET_MON_INFO 1 1 Mid 1 1 1 1 1
		MON_SPEECH @Mid 1 2 1 �����ң�����̫ǿ�ˣ�
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
	
	if Num = 3
		MON_CALL_ENEMY_MON 205020 1 50 4 20200 43600 0 0 0 0 0 0 0
		MON_CALL_ENEMY_MON 205020 1 50 4 20200 42900 0 0 0 0 0 0 0
		rand 90 3
		switch 90
			case 0
				MON_SPEECH @Mid 1 2 1 ����Э���ң�
			endcase
			case 1
				MON_SPEECH @Mid 1 2 1 ������Щ͵���ߣ�
			endcase
			case 2
				MON_SPEECH @Mid 1 2 1 �����Ƕ��ϳ�ȥ��
			endcase
								
		endswitch
	endif
	if Num = 4

		MON_CALL_ENEMY_MON 205020 1 50 4 26200 43800 0 0 0 0 0 0 0
		MON_CALL_ENEMY_MON 205020 1 50 4 26200 43000 0 0 0 0 0 0 0
		MON_CALL_ENEMY_MON 205020 1 50 4 26700 43500 0 0 0 0 0 0 0
		rand 90 3
		switch 90
			case 0
				MON_SPEECH @Mid 1 2 1 ����Э���ң�
			endcase
			case 1
				MON_SPEECH @Mid 1 2 1 ������Щ͵���ߣ�
			endcase
			case 2
				MON_SPEECH @Mid 1 2 1 �����Ƕ��ϳ�ȥ��
			endcase
								
		endswitch

	endif
	if Num = 5
		MON_CALL_ENEMY_MON 205020 1 50 4 26200 47100 0 0 0 0 0 0 0
		MON_CALL_ENEMY_MON 205020 1 50 4 25800 46600 0 0 0 0 0 0 0
		MON_CALL_ENEMY_MON 205020 1 50 4 26600 46600 0 0 0 0 0 0 0
		rand 90 3
		switch 90
			case 0
				MON_SPEECH @Mid 1 2 1 ����Э���ң�
			endcase
			case 1
				MON_SPEECH @Mid 1 2 1 ������Щ͵���ߣ�
			endcase
			case 2
				MON_SPEECH @Mid 1 2 1 �����Ƕ��ϳ�ȥ��
			endcase
								
		endswitch

	
	endif
endproc












