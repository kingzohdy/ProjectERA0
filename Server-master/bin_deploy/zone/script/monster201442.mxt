;������� ,�ڰ�ħ����3
proc 1
	var 10 0
	set_mon_var 0 32 @10 0
endproc
proc 2
	;boss4��ʵ��id
;	VAR 15 0
;	get_pworld_var 20 32 1 15
;	
;	;1��״̬�����
;	TARGET_ADD_STATUS 3 @15 3030 1 1 0	
;	;��ͼ��״̬
;	RANGE_ADD_STATUS 0 3033 1 7200 0
		;��������ؼ���
	get_pworld_var 8 32 1 12
	add 12 1
	set_pworld_var 8 32 @12 1
	strvar ret num
	var ret 0
	GET_MON_NUM_BY_DEFID 294028 num
	add ret @num
	GET_MON_NUM_BY_DEFID 294029 num
	add ret @num
	GET_MON_NUM_BY_DEFID 294030 num
	add ret @num
	
	if ret = 1
		add_sys_msg_start
		add_sys_msg ��Ӱ�����ѽ���������ǰ��ռ��ڰ���̳! 
		set_sys_msg_mode 1 0 0
		map_msg_info
	endif
	if ret = 2
		add_sys_msg_start
		add_sys_msg ��Ӱ�������������㣬��ʣ�µĽ���ƻ�����ȫ�����Ӱ���� 
		set_sys_msg_mode 1 0 0
		map_msg_info
	endif
	if ret = 3
		add_sys_msg_start
		add_sys_msg ��Ӱ����������һ�㣬��������������ƻ�����ȫ�����Ӱ���� 
		set_sys_msg_mode 1 0 0
		map_msg_info
	endif
	
	
endproc
proc 4
	get_mon_var 0 32 0 10
	if 10 > 2
		return
	endif
	add 10 1
	set_mon_var 0 32 @10 0
;	add_sys_msg_start
;	add_sys_msg ��Ӱ���İ�Ӱ��Ļ�ѱ���� 
;	set_sys_msg_mode 1 0 0
;	map_msg_info
endproc
