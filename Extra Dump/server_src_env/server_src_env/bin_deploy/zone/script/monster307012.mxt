;;;�ɵ�1��



proc 1
	strvar mid
	GET_MON_INFO 1 1 mid 1 1 1 1 1
	TARGET_ADD_STATUS 3 @mid 1009 2 10 1
	MON_SPEECH @mid 1 1 1 ��ʼ̽�ⱦ�أ�
	
	strvar Loc val_2 val_3 val_4 Time
	var Loc 0
	set_mon_var 0 8 @Loc 1
	var Time 0
	set_mon_var 1 8 @Time 1
	
endproc 

proc 2
	;�õ�����ID
	strvar ClanID mhp mx my
	get_pworld_var 0 32 1 ClanID	
	GET_MON_INFO 1 1 1 mhp 1 1 mx my

	add_sys_msg_start
	if mhp != 0	
		add_sys_msg ����̽������Դ�ľ���Ѱ��������� 
	else
		add_sys_msg ����̽�����������أ��޷�������Ѱ��Ѱ���������
	endif
	set_sys_msg_mode 14 0 0
	CLAN_MSG_INFO 1 @ClanID
	
	;���Ϊ�ѽ���
	DYN_ACTIVE_STATUS_SET 1 19 2
	
	;�����ڱ�
	strvar Locw
	get_pworld_var 100 8 1 Locw
	var Locw 0
	set_pworld_var 100 8 @Locw 1
	
	
	;���ø�������
	
	set_pworld_var 64 8 0 1
	set_pworld_var 100 8 0 1 	
	set_pworld_var 101 8 0 1
	
	set_pworld_var 4 32 0 1 
	set_pworld_var 8 32 0 1  
	set_pworld_var 12 32 0 1
	set_pworld_var 16 32 0 1
	
endproc
proc 3
;Ѳ�ߵ����
endproc 

proc 4

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;�ڳ��������������ɵ�
	
	strvar LocMon px py
	get_pworld_var 101 8 1 LocMon
	if LocMon = 10
		get_pworld_var 12 32 1 px 
		get_pworld_var 16 32 1 py
		
		;NEW_LIFE_MON 284272 1 @px @py 0 0 0 0 60 0 0
		
		
;		MON_CALL_ENEMY_MON 284272 1 60 2 @px @py 0 0 0 0 60 1 0
	;	effect 0 0 playEffct($@px$,$@py$,"effect\\entity\\1000900_8.ent")

		var LocMon 0
		set_pworld_var 101 8 @LocMon 1
		
	endif
		
;;;;;;;;;;;;;;;;;;;;;;;;;
	strvar mid midx midy
	GET_MON_INFO 1 1 mid 1 1 1 1 1
	
;;;;;;;;;;;;;;;;;;;;;;;�����ɵ���ֹ1���ӵ�ʱ������timer
	get_mon_var 0 8 1 Loc 
	if Loc = 1
		get_mon_var 1 8 1 Time
		add Time 1
		set_mon_var 1 8 @Time 1
		if Time < 20
			if Time = 7
				MON_SPEECH @mid 1 1 1 �������ֱ��أ��������ֱ��أ�
			endif
			if Time = 14
				MON_SPEECH @mid 1 1 1 �������ֱ��أ��������ֱ��أ�
			endif
			return
		else
			get_mon_var 0 8 1 Loc 	
			var Loc 0
			set_mon_var 0 8 @Loc 1
			
			get_mon_var 1 8 1 Time
			var Time 0
			set_mon_var 1 8 @Time 1
			
			;�����ڱ����
			strvar Locw
			get_pworld_var 100 8 1 Locw
			var Locw 0
			set_pworld_var 100 8 @Locw 1
			
		endif
	endif
	
;;;;;;;;;;;;;;;;;;;;;;��ʱ 90�뵽180��֮���漴ͣ��
	get_mon_var 1 8 1 Time
	add Time 1
	set_mon_var 1 8 @Time 1
	if Time = 1
		
		get_mon_var 1 8 1 Time
		rand Time 30
		set_mon_var 1 8 @Time 1
	endif
	
	;add_sys_msg_start
	;add_sys_msg $@Time$
	;set_sys_msg_mode 1 0 0
	;map_msg_info
	
	if Time = 40

		TARGET_ADD_STATUS 3 @mid 1009 2 60 1
		
		
		strvar ClanID
		get_pworld_var 0 32 1 ClanID	
		
		add_sys_msg_start
		add_sys_msg ̽�������ֵ����̲صı��أ���ҿ�����̽�����������ھ��������ھ�
		set_sys_msg_mode 14 0 0
		CLAN_MSG_INFO 1 @ClanID
		
		MON_SPEECH @mid 1 1 1 �������ֱ��أ��������ֱ��أ�
		;��¼�ɵ����ֱ���ʱ������,�����ж�������ɵ��ľ���
		
		strvar midx midy
		GET_MON_INFO 1 1 1 1 1 1 midx midy
		set_pworld_var 4 32 @midx 1
		set_pworld_var 8 32 @midy 1
		;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		;�����ڱ����
		strvar Locw
		get_pworld_var 100 8 1 Locw
		var Locw 1
		set_pworld_var 100 8 @Locw 1
		
		;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		get_mon_var 1 8 1 Time
		var Time 0
		set_mon_var 1 8 @Time 1
		
		get_mon_var 0 8 1 Loc 	
		var Loc 1
		set_mon_var 0 8 @Loc 1
	endif

		
endproc  











