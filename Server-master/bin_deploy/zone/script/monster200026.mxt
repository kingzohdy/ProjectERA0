
;�ڳ��й�


proc 1
	strvar Mid Rad
	GET_MON_INFO 1 1 Mid 1 1 1 1 1
	TARGET_ADD_STATUS 3 @Mid 1262 1 5 1
	
	rand Rad 3
	if Rad = 0
		MON_SPEECH @Mid 1 1 1 ����Ŀ�꣬�����Ӷ�ģʽ
	endif
	if Rad = 1
		MON_SPEECH @Mid 1 1 1 Ŀ���ѽ���ƻ���Χ�������Ӷ�ģʽ
	endif
	if Rad = 2
		MON_SPEECH @Mid 1 1 1 ȷ��Ŀ�����������Ӷ�ģʽ
	endif
endproc


