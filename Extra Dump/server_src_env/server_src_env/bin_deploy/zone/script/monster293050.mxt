;��������
proc 1

	strvar Mid

	var 10 0
	set_mon_var 0 8 @10 1

	
	
endproc 
proc 4
	strvar Mid	
	GET_MON_INFO 0 0 Mid 0 0 0 0 0
	
	get_mon_var 0 8 1 10
	rand 11 50
	add 10 @11
	set_mon_var 0 8 @10 1
	
	if 10 = 3
		rand 20 3
		switch 20
			case 0
	;			mon_speech @Mid 0 1 1 �����������ǵ�����ʿ��������Կ���ںհ͹�������ϣ���
			endcase
			case 1
	;			mon_speech @Mid 0 1 1 �����ˣ�һ����ͷǷ��ˣ�������Ӻհ͹���������õ�Կ�ף����������ɺ���
			
			endcase
			case 2
	;			mon_speech @Mid 0 1 1 �Ⱦ��Ұɣ���ʿ����һ����Ҳ����������
			endcase
		endswitch
	endif
	if 10 > 250
		var 10 0
		set_mon_var 0 8 @10 1
	endif

endproc 