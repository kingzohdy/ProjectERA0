




proc 1
	

	;DEC_ITEM_NUM_INPAK 5241520 1 100
	;if 100 = 0

		;GET_PLAYER_ID 1 Mid
		;EFFECT_ID 0 @Mid playMontion(%d,"352116@reset")
		;strvar x y
		; GET_PLAYER_MAPINFO 1 1 1 x y
		; effect 0 0 playEffct($@x$,$@y$,0,"effect\\entity\\352116.ent",15)
		
		;	get_task_var 4450 0 32 1 12
		
	strvar mid map
	get_role_var 45 32 1 mid 
	DEL_ID_MON @mid 1
		
	; add_sys_msg_start
	; add_sys_msg $@12$
	; set_sys_msg_mode 1 0 0
	; MSG_INFO
	PLAYER_CALL_BUB 302165 300 mid
	set_role_var 45 32 @mid 1

	; add_sys_msg_start
	; add_sys_msg ---$@12$
	; set_sys_msg_mode 1 0 0
	; MSG_INFO
;	if 12 = 0
	
		GET_PLAYER_MAPINFO 1 map 1 1 1 
	if map = 2000 or map = 2001 or map = 2002 or map = 2003 or map = 2005 or map = 2006 or map = 2007 or map = 2004
		
			rand 20 3
			if 20 = 0
				mon_speech @mid 10 2 1 �ۣ��Ҹղſ�����һ���ӣ���֪��������������
			endif
			if 20 = 1
				mon_speech @mid 10 2 1 ���ǻ᲻�᲻С�ĵ�������
			endif
			if 20 = 2
				mon_speech @mid 10 2 1 ������Ǵ�˵�е�������������һ�㶼���ף�
			endif
			
	endif
	
	if map = 2100 or map = 2101 or map = 2102 or map = 2103 or map = 2105 or map = 2106 or map = 2107 or map = 2108
		;����
		
		rand 20 3
		if 20 = 0
			mon_speech @mid 1 2 1 ��˵�����кܶ�Ұ�ˣ������Ұ����
		endif
		if 20 = 1
			mon_speech @mid 1 2 1 ��˵Ұ�˻��С���ӣ��������
		endif
		if 20 = 2
			mon_speech @mid 1 2 1 ���ǵ�����ȥ�����ɣ�˳�㲶���㡢Ϻʲô�ģ�
		endif
	endif
	
	if map = 2200
	;ѩԭ
		rand 20 4
		if 20 = 0
			mon_speech @mid 1 2 1 ��������䰡��
		endif
		if 20 = 1
			mon_speech @mid 1 2 1 �ö�ѩ����ʱ������겻��ѩ�أ�
		endif
		if 20 = 2
			mon_speech @mid 1 2 1 ������Ӧ�ô����������ģ�����ѩ����Ư������
		endif
		if 20 = 3
			mon_speech @mid 1 2 1 ����ȥ�ݷ���������Ѫ�����ɣ����ǻỶӭ���ǲ���
		endif
	endif
	
	if map = 2300 
	;ɳĮ
		rand 20 4
		if 20 = 0
			mon_speech @mid 1 2 1 �㿴�������ǻƽ��أ�
		endif
		if 20 = 1
			mon_speech @mid 1 2 1 ����ȥůȪ�򿴿��ɣ�˳��������Ȫ��
		endif
		if 20 = 2
			mon_speech @mid 1 2 1 ůȪ����ô��û����Ȫ�أ�
		endif
		if 20 = 3
			mon_speech @mid 1 2 1 ��Ŭ��˹�����Ǹ����ĺ��񹷵ľ�����
		endif
	endif
	
	if map = 2400
	;����
			
		rand 20 6
		if 20 = 0
			mon_speech @mid 1 2 1 ���ƺ���������Ĳ�̫һ������ô˵�أ��е������
		endif
		if 20 = 1
			mon_speech @mid 1 2 1 �㿴��Щ�ݣ����ǿݻƵģ����쵽����
		endif
		if 20 = 2
			mon_speech @mid 1 2 1 ʱ���һ�굽ͷ���Ǵ��죬û���ļ���û�뵽�����������ģ�
		endif
		if 20 = 3
			mon_speech @mid 1 2 1 ��Щ������������н����������ڿ������ɣ�
		endif
		if 20 = 4
			mon_speech @mid 1 2 1 ���ջ�Ұ��Ϧ�����᲻���ʱ��ǻ����أ����������������ɣ�
		endif
		if 20 = 5
			mon_speech @mid 1 2 1 �ƺ�������أ�������ȥ�����ɣ��㲻������¾�����ʲô��
		endif
	endif
	
	if map = 2500
	;Խ��
			rand 20 5
			if 20 = 0
				mon_speech @mid 1 2 1 �����������ʱ��ǲ�˶��١�
			endif
			if 20 = 1
				mon_speech @mid 1 2 1 �ۣ��öද������ǲ��ǿ��Ե��������ߣ�
			endif
			if 20 = 2
				mon_speech @mid 1 2 1 ������������ǲ����ȳԵ�ʲô�أ�
			endif
			if 20 = 3
				mon_speech @mid 1 2 1 �ף��Ҹղ��ƺ�����һ��С���ӡ����塭
			endif
			if 20 = 4
				mon_speech @mid 1 2 1 ��������������������Ϊʲô�Ұ�������겻�������أ�
			endif
	endif
	if map = 1000
		mon_speech @mid 1 2 1 �е�ʧ���أ�û�п���������Ҳû�п�������ˡ�
	endif
	
	
	
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		task_stat 4450 1 20
		task_stat 4451 1 21
		task_stat 4452 1 22
		task_stat 4453 1 23
		task_stat 4454 1 24
		task_stat 4455 1 25
		task_stat 4456 1 26
		if 20 = 2
			
			get_player_mapinfo 0 1 2 3 4
			get_point_dist @3 @4 16414 16404 10
			if 2 = 2000 or 2 = 2001 or 2 = 2002 or 2 = 2003 or 2 = 2005 or 2 = 2006 or 2 = 2007 or 2 = 2004
				if 10 <= 500  
					task_fini 4450 1
				endif
			endif
		
		endif
		
		if 21 = 2
			get_player_mapinfo 0 1 2 3 4
			get_point_dist @3 @4 15861 9965 10
			if 2 = 2100 or 2 = 2101 or 2 = 2102 or 2 = 2103 or 2 = 2105 or 2 = 2106 or 2 = 2107 or 2 = 2108
				if 10 <= 500  
					task_fini 4451 1
				endif
			endif
		endif
		if 22 = 2
			get_player_mapinfo 0 1 2 3 4
			get_point_dist @3 @4 21995 22804 10
			if 2 = 2200
				if 10 <= 500
					task_fini 4452 1
				endif
			endif
		endif
		if 23 = 2
			get_player_mapinfo 0 1 2 3 4
			get_point_dist @3 @4 6707 22577 10
			if 2 = 2300
				if 10 <= 500
					task_fini 4453 1
				endif
			endif
		endif
		if 24 = 2
			;����
			get_player_mapinfo 0 1 2 3 4
			get_point_dist @3 @4 21784 14086 10
			if 2 = 2400
				if 10 <= 500
					task_fini 4454 1
				endif
			endif
		endif
		if 25 = 2
			;Խ��
			get_player_mapinfo 0 1 2 3 4
			get_point_dist @3 @4 10096 27570 10
			if 2 = 2500
				if 10 <= 500
					task_fini 4455 1
				endif
			endif
		endif
		if 26 = 2
			;ʱ���
			get_player_mapinfo 0 1 2 3 4
			get_point_dist @3 @4 31000 19000 10
			if 2 = 1000
				if 10 <= 500
					task_fini 4456 1
				endif
			endif
		
		endif
		
;	endif
endproc 

