;���οռ���Ҫ�����Ĺ��� õ��
;ˢ��������¼�
proc 1
	strvar hp
	GET_MON_INFO 1 0 10 hp 0 0 0 0
	
	;����NPC��ʼѪ��
	;monsay10,NPC�����ܵ�����ǰѪ��
	set_mon_var 0 32 @hp 1
	
	set_mon_var 4 32 0 1
	
	;monmemid10,NPC�����ʵ��memid
	set_pworld_var 0 32 @10 1
	
	var 11 1
	;monstate11,NPC����״̬ 0δˢ 1��� 2����
	set_pworld_var 4 32 @11 1
	
	mon_speech 0 0 2 1 �����Ȱ˵���Ҳ����뿪�⣬ֻҪ��һ��������ǵĿ��гɹ��ͻ���֡�
endproc

;�����������¼�
proc 2
	var 11 2
	;monstate11,NPC����״̬ 0δˢ 1��� 2����
	set_pworld_var 4 32 @11 1
endproc

;���ﶨʱ��3��һ��
proc 4
	get_mon_var 4 32 0 10
	if 10 <= 3
		ADD 10 1
		set_mon_var 4 32 @10 1
	endif
	
	if 10 = 3
		mon_speech 0 0 2 1 �����Ŭ���յ��ջ�ʱ���Ҿ��Բ��ܷ�����������һ��������������
	endif
	
	strvar hp
	GET_MON_INFO 1 0 0 hp 0 0 0 0 
	get_mon_var 0 32 1 10
	
	;��ǰѪ��С��ǰһ�ν�Tickʱ��Ѫ��
	if 10 > @hp
		;��ʾ
		add_sys_msg_start
		add_sys_msg $_Monster$���ܵ����﹥�����뾡��֧Ԯ 
		set_sys_msg_mode 4 0 0
		map_msg_info

		;monhp11,��Ŧ���ϸ�tick��Ѫ��
		set_mon_var 0 32 @hp 1
	endif
	
endproc
