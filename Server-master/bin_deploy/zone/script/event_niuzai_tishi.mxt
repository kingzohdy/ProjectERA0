
;ţ�к�æ��ħӰ������ʾ


proc 1

	

endproc 

proc 3

	GET_NOW_TIME 0 0 0 20 30 40 50
	if 20 = 7 and 30 = 18 and 40 = 49 and 50 > 49
	;����17:00
		add_sys_msg_start
		add_sys_msg ��ħӰ���ء������10���Ӻ��������λ��ʿ�����ַ�׼����
		set_sys_msg_mode 6 0 0
		svr_msg_info	
	endif
	if 20 = 7 and 30 = 18 and 40 = 54 and 50 > 49
	;����17:00
		add_sys_msg_start
		add_sys_msg ��ħӰ���ء������5���Ӻ��������λ��ʿ�����ַ�׼����
		set_sys_msg_mode 6 0 0
		svr_msg_info	
	endif
	
	if 20 = 1 or 20 = 4
	;��һ ���� 8��20
		if 30 = 20 and 40 = 19 and 50 > 49
			add_sys_msg_start
			add_sys_msg �ʺ絺�ġ��ƶ�������10���Ӻ�򿪡�ţ�к�æ-�������Ĵ��ţ���ӵ�о����ʸ���������׼����
			set_sys_msg_mode 6 0 0
			svr_msg_info
			
		endif
		if 30 = 20 and 40 = 24 and 50 > 49
			add_sys_msg_start
			add_sys_msg �ʺ絺�ġ��ƶ�������5���Ӻ�򿪡�ţ�к�æ-�������Ĵ��ţ���ӵ�о����ʸ���������׼����
			set_sys_msg_mode 6 0 0
			svr_msg_info
			
		endif
	endif
	
	
;	strvar num
;	GET_MAP_MON_NUM 3200 200030 1 num 0
;	if num = 0
;		GET_NOW_TIME 0 0 0 20 30 40 50
;		if 30 = 18 and 40 = 54 and 50 > 49
;			add_sys_msg_start
;			add_sys_msg 5���Ӻ���˹�������#L�γ�@@(3200,15900,14500)#n�����ѣ����λ��ʿ�����ַ�׼����
;			set_sys_msg_mode 6 0 0
;			svr_msg_info	
;		endif
;	endif

endproc

