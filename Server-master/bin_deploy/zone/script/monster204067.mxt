;�ȳ��ľ���


proc 1

	strvar Mid
	GET_MON_INFO 0 0 Mid 0 0 0 0 0
	rand 10 3
	
	if 10 = 0
	
		mon_speech @Mid 0 2 1 ������������������Ϣ�ĵط��ˣ�
	endif
	if 10 = 1
		mon_speech @Mid 0 2 1 �޵پ��ڿ�������Ĳɼ����У�
	endif
	if 10 = 2
	
		mon_speech @Mid 0 2 1 һ��Ҫ�Ⱦȴ�ң�
	endif

endproc 
