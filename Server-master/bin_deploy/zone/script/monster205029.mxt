




proc 1
	strvar Mid Rad_2
	GET_MON_INFO 10 1 Mid 1 1 1 1 1
	
	
	
	rand Rad_2 5

	
	switch Rad_2
		case 0 
			MON_SPEECH @Mid 0 1 1 ϣ���ܹ����ϴ�һ����ƽ���ؼҡ���

		endcase
		case 1 
			MON_SPEECH @Mid 0 1 1 ������䰡����������ů�ļҡ���

		endcase
		case 2 
			MON_SPEECH @Mid 0 1 1 ��Щ��ʲô�����������ϴ��ˣ��������ÿֲ�����

		endcase
		case 3 
			MON_SPEECH @Mid 0 1 1 ��ɱ�ң���ɱ�ң���֪��ʲô��Ԫ�ء���

		endcase
		case 4 
			MON_SPEECH @Mid 0 1 1 �쵰���Ҹ���ƴ�ˣ�

		endcase
	endswitch
	
endproc 
 