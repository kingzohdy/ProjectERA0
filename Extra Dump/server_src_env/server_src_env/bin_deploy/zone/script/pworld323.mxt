;��֮��
;������

;���������¼�
PROC 1
	;ʱ�� ÿ20s����
	set_pworld_var 0 32 0 0
	;�ڼ���
	set_pworld_var 4 32 0 0 
	;0��ʾ����ˢ���ι֣�1��ʾ����ˢ
	set_pworld_var 8 32 0 0
	;�ӳ�1�����˳�����
	set_pworld_var 12 32 0 0
	;1��ʾ��ʼ�����ӳٱ��
	set_pworld_var 16 32 0 0
	;monstate11,Ҫ������NPC״̬ 0δˢ 1��� 2����
	set_pworld_var 20 32 1 0
	
	NEW_LIFE_MON 212020 1 12771 12762 0 0 0 0 0 0 60
	;��ס���ɵ�212020��ʵ��ID
	set_pworld_var 24 32 @60 0
	
	;��˹��������������������½��븱��ʱ���ټӻ�buff
	set_pworld_var 28 32 0 0
ENDPROC

;�رո����¼�
proc 2
endproc

;������ʱ�¼���ÿ2�봥��һ�Σ�
proc 3
	IS_PWORLD_OVER 30
	if @30 != 1
		get_pworld_var 4 32 1 12
		if @12 < 6
			get_pworld_var 20 32 1 100
			if @100 = 2
				add_sys_msg_start
				add_sys_msg ˹���߲������������ص�������������֮����ɥʧ����ս��������
				set_sys_msg_mode 4 0 0
				map_msg_info
				
				DIANFENG_ADD_STATUS 2115 1 10800 90
				
				set_pworld_var 24 32 0 0
				set_pworld_var 20 32 0 0
				set_pworld_var 28 32 1 0
				return 
			endif
		endif
		
		get_pworld_var 8 32 1 60
		
		if @12 = 0 and @60 = 0
			set_pworld_var 8 32 1 1 
		elseif @12 = 1 and @60 = 0
			set_pworld_var 8 32 1 1 
			set_pworld_var 0 32 0 0
		elseif @12 = 2 and @60 = 0
			get_mon_num_by_defid 212110 20
			;����һ������û
			if @20 < 1
				set_pworld_var 8 32 1 1 
				set_pworld_var 0 32 0 0
			endif
		elseif @12 = 3 and @60 = 0
			get_mon_num_by_defid 212111 20
			;���ڶ�������û
			if @20 < 1
				set_pworld_var 8 32 1 1 
				set_pworld_var 0 32 0 0
			endif
		elseif @12 = 4 and @60 = 0
			get_mon_num_by_defid 212112 20
			;������������û
			if @20 < 1
				set_pworld_var 8 32 1 1 
				set_pworld_var 0 32 0 0
			endif
		elseif @12 = 5 and @60 = 0
			get_mon_num_by_defid 212113 20
			;�����Ĳ�����û
			if @20 < 1
				set_pworld_var 8 32 1 1 
				set_pworld_var 0 32 0 0
			endif
		elseif @12 = 6 and @60 = 0
			get_mon_num_by_defid 212114 20
			;�����岨����û
			if @20 < 1
				set_pworld_var 8 32 1 1 
				set_pworld_var 0 32 0 0
				
				get_pworld_var 24 32 70 60
				if @70 = 0 and @60 > 0
					add_sys_msg_start
					add_sys_msg ��˹������Ҫ�����ˣ����Ǽ��ͣ��������ˡ�
					set_sys_msg_mode 4 0 0
					MAP_MSG_INFO
					
					del_id_mon @60 1
				endif
			endif
		elseif @12 = 7 and @60 = 0
			get_mon_num_by_defid 212003 20
			;��boss����û
			if @20 < 1
				set_pworld_var 16 32 1 1
				set_pworld_var 8 32 1 1 
			endif
		else
		endif
		
		;��ʼ�ӳ�
		get_pworld_var 16 32 1 91
		if @91 = 1
			get_pworld_var 12 32 1 90
			;1���Ӻ�ȫ���˳�����
			if @90 >= 30
				pworld_over
				COMPEL_CLOSE_PWORLD 1
			else
				add 90 1
				set_pworld_var 12 32 @90 1
				
				if @90 = 1
					new_npc 1 333160 12889 12550 0 csm333160.mac			
					add_sys_msg_start
					add_sys_msg ��������ɣ���ͨ���������뿪
					set_sys_msg_mode 3 0 0
					map_msg_info
				endif
			endif
		else
			get_pworld_var 4 32 1 91
			if @91 >= 7
				return
			endif
			
			;ˢ��
			get_pworld_var 0 32 1 11
			if @11 >= 10
				get_pworld_var 8 32 1 50
				if @50 = 1
					call 100
				endif
				set_pworld_var 0 32 0 0
			else
				add 11 1
				set_pworld_var 0 32 @11 1
			endif
		endif
	endif	
endproc

;�����й��������¼�
proc 4

endproc

;�����н�ɫ�����¼�
proc 5
	;˹���������󣬽�ɫ���½���fb���ӻ�buff
	get_pworld_var 28 32 71 61
	if @71 = 0 and @61 = 1
		strvar memvar
		get_player_id 10 memvar
		if 10 = 0
			TARGET_ADD_STATUS  1 @memvar 2115 1 10800 11
		endif	
	endif
endproc

;�����н�ɫ�����¼�
proc 6

endproc

;�����е���ʱ�����¼�
proc 7
	
endproc 

;�������ʼ�¼�
proc 8
endproc 

proc 100
	strvar flag
	var flag 0
	
	;���Ⲩ�����������ˢ��һ��
	set_pworld_var 8 32 0 0
	
	get_pworld_var 4 32 1 13
	if @13 = 0
		add_sys_msg_start
		add_sys_msg 20���ˢ����һ�������ʿ�ǣ�׼����ս���ɣ�
		set_sys_msg_mode 4 0 0
		MAP_MSG_INFO
	else
		if @13 < 6
			get_pworld_var 24 32 70 61
			if @70 = 0 and @61 > 0
				add_sys_msg_start
				add_sys_msg ������˹���ߣ���
				set_sys_msg_mode 4 0 0
				MAP_MSG_INFO
			endif
		endif
		
		if @13 = 1
			call 201
		elseif @13 = 2
			call 202
		elseif @13 = 3
			call 203
		elseif @13 = 4
			call 204
		elseif @13 = 5
			call 205
		elseif @13 = 6			
			;boss
			call 206
		else
			var flag 1 
		endif
	endif
	
	if @flag = 0
		add 13 1
		set_pworld_var 4 32 @13 1
	endif
endproc

proc 201
	NEW_LIFE_MON 212110 1 12931 13953 0 0 0 0 0 0 0
	NEW_LIFE_MON 212110 1 12954 14549 0 0 0 0 0 0 0
	NEW_LIFE_MON 212110 1 12496 14365 0 0 0 0 0 0 0
	NEW_LIFE_MON 212110 1 11776 13967 0 0 0 0 0 0 0
	NEW_LIFE_MON 212110 1 12475 13054 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212110 1 10876 13076 0 0 0 0 0 0 0
	NEW_LIFE_MON 212110 1 12510 12364 0 0 0 0 0 0 0
	NEW_LIFE_MON 212110 1 11117 12129 0 0 0 0 0 0 0
	NEW_LIFE_MON 212110 1 11829 12696 0 0 0 0 0 0 0
	NEW_LIFE_MON 212110 1 11484 13009 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212110 1 13371 12039 0 0 0 0 0 0 0
	NEW_LIFE_MON 212110 1 13420 13092 0 0 0 0 0 0 0
	NEW_LIFE_MON 212110 1 12635 10959 0 0 0 0 0 0 0
	NEW_LIFE_MON 212110 1 13666 13995 0 0 0 0 0 0 0
	NEW_LIFE_MON 212110 1 14289 13084 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212110 1 14080 11645 0 0 0 0 0 0 0
	NEW_LIFE_MON 212110 1 12672 11747 0 0 0 0 0 0 0
	NEW_LIFE_MON 212110 1 11907 11153 0 0 0 0 0 0 0
	NEW_LIFE_MON 212110 1 11715 12000 0 0 0 0 0 0 0
	NEW_LIFE_MON 212110 1 13290 11060 0 0 0 0 0 0 0
endproc

proc 202
	NEW_LIFE_MON 212111 1 12931 13953 0 0 0 0 0 0 0
	NEW_LIFE_MON 212111 1 12954 14549 0 0 0 0 0 0 0
	NEW_LIFE_MON 212111 1 12496 14365 0 0 0 0 0 0 0
	NEW_LIFE_MON 212111 1 11776 13967 0 0 0 0 0 0 0
	NEW_LIFE_MON 212111 1 12475 13054 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212111 1 10876 13076 0 0 0 0 0 0 0
	NEW_LIFE_MON 212111 1 12510 12364 0 0 0 0 0 0 0
	NEW_LIFE_MON 212111 1 11117 12129 0 0 0 0 0 0 0
	NEW_LIFE_MON 212111 1 11829 12696 0 0 0 0 0 0 0
	NEW_LIFE_MON 212111 1 11484 13009 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212111 1 13371 12039 0 0 0 0 0 0 0
	NEW_LIFE_MON 212111 1 13420 13092 0 0 0 0 0 0 0
	NEW_LIFE_MON 212111 1 12635 10959 0 0 0 0 0 0 0
	NEW_LIFE_MON 212111 1 13666 13995 0 0 0 0 0 0 0
	NEW_LIFE_MON 212111 1 14289 13084 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212111 1 14080 11645 0 0 0 0 0 0 0
	NEW_LIFE_MON 212111 1 12672 11747 0 0 0 0 0 0 0
	NEW_LIFE_MON 212111 1 11907 11153 0 0 0 0 0 0 0
	NEW_LIFE_MON 212111 1 11715 12000 0 0 0 0 0 0 0
	NEW_LIFE_MON 212111 1 13290 11060 0 0 0 0 0 0 0
endproc

proc 203
	NEW_LIFE_MON 212112 1 12931 13953 0 0 0 0 0 0 0
	NEW_LIFE_MON 212112 1 12954 14549 0 0 0 0 0 0 0
	NEW_LIFE_MON 212112 1 12496 14365 0 0 0 0 0 0 0
	NEW_LIFE_MON 212112 1 11776 13967 0 0 0 0 0 0 0
	NEW_LIFE_MON 212112 1 12475 13054 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212112 1 10876 13076 0 0 0 0 0 0 0
	NEW_LIFE_MON 212112 1 12510 12364 0 0 0 0 0 0 0
	NEW_LIFE_MON 212112 1 11117 12129 0 0 0 0 0 0 0
	NEW_LIFE_MON 212112 1 11829 12696 0 0 0 0 0 0 0
	NEW_LIFE_MON 212112 1 11484 13009 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212112 1 13371 12039 0 0 0 0 0 0 0
	NEW_LIFE_MON 212112 1 13420 13092 0 0 0 0 0 0 0
	NEW_LIFE_MON 212112 1 12635 10959 0 0 0 0 0 0 0
	NEW_LIFE_MON 212112 1 13666 13995 0 0 0 0 0 0 0
	NEW_LIFE_MON 212112 1 14289 13084 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212112 1 14080 11645 0 0 0 0 0 0 0
	NEW_LIFE_MON 212112 1 12672 11747 0 0 0 0 0 0 0
	NEW_LIFE_MON 212112 1 11907 11153 0 0 0 0 0 0 0
	NEW_LIFE_MON 212112 1 11715 12000 0 0 0 0 0 0 0
	NEW_LIFE_MON 212112 1 13290 11060 0 0 0 0 0 0 0
endproc

proc 204
    NEW_LIFE_MON 212113 1 12931 13953 0 0 0 0 0 0 0
	NEW_LIFE_MON 212113 1 12954 14549 0 0 0 0 0 0 0
	NEW_LIFE_MON 212113 1 12496 14365 0 0 0 0 0 0 0
	NEW_LIFE_MON 212113 1 11776 13967 0 0 0 0 0 0 0
	NEW_LIFE_MON 212113 1 12475 13054 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212113 1 10876 13076 0 0 0 0 0 0 0
	NEW_LIFE_MON 212113 1 12510 12364 0 0 0 0 0 0 0
	NEW_LIFE_MON 212113 1 11117 12129 0 0 0 0 0 0 0
	NEW_LIFE_MON 212113 1 11829 12696 0 0 0 0 0 0 0
	NEW_LIFE_MON 212113 1 11484 13009 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212113 1 13371 12039 0 0 0 0 0 0 0
	NEW_LIFE_MON 212113 1 13420 13092 0 0 0 0 0 0 0
	NEW_LIFE_MON 212113 1 12635 10959 0 0 0 0 0 0 0
	NEW_LIFE_MON 212113 1 13666 13995 0 0 0 0 0 0 0
	NEW_LIFE_MON 212113 1 14289 13084 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212113 1 14080 11645 0 0 0 0 0 0 0
	NEW_LIFE_MON 212113 1 12672 11747 0 0 0 0 0 0 0
	NEW_LIFE_MON 212113 1 11907 11153 0 0 0 0 0 0 0
	NEW_LIFE_MON 212113 1 11715 12000 0 0 0 0 0 0 0
	NEW_LIFE_MON 212113 1 13290 11060 0 0 0 0 0 0 0
endproc

proc 205
	NEW_LIFE_MON 212114 1 12931 13953 0 0 0 0 0 0 0
	NEW_LIFE_MON 212114 1 12954 14549 0 0 0 0 0 0 0
	NEW_LIFE_MON 212114 1 12496 14365 0 0 0 0 0 0 0
	NEW_LIFE_MON 212114 1 11776 13967 0 0 0 0 0 0 0
	NEW_LIFE_MON 212114 1 12475 13054 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212114 1 10876 13076 0 0 0 0 0 0 0
	NEW_LIFE_MON 212114 1 12510 12364 0 0 0 0 0 0 0
	NEW_LIFE_MON 212114 1 11117 12129 0 0 0 0 0 0 0
	NEW_LIFE_MON 212114 1 11829 12696 0 0 0 0 0 0 0
	NEW_LIFE_MON 212114 1 11484 13009 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212114 1 13371 12039 0 0 0 0 0 0 0
	NEW_LIFE_MON 212114 1 13420 13092 0 0 0 0 0 0 0
	NEW_LIFE_MON 212114 1 12635 10959 0 0 0 0 0 0 0
	NEW_LIFE_MON 212114 1 13666 13995 0 0 0 0 0 0 0
	NEW_LIFE_MON 212114 1 14289 13084 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212114 1 14080 11645 0 0 0 0 0 0 0
	NEW_LIFE_MON 212114 1 12672 11747 0 0 0 0 0 0 0
	NEW_LIFE_MON 212114 1 11907 11153 0 0 0 0 0 0 0
	NEW_LIFE_MON 212114 1 11715 12000 0 0 0 0 0 0 0
	NEW_LIFE_MON 212114 1 13290 11060 0 0 0 0 0 0 0
endproc

proc 206		
	NEW_LIFE_MON 212003 1 12889 12550 0 0 0 0 0 0 0
endproc

