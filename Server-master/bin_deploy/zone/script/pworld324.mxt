;��֮��
;���Ĳ�

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
	;ʱ�� 30s����
	set_pworld_var 20 32 0 0
	;ˢ��Ч�ֿ���
	set_pworld_var 24 32 0 0 
ENDPROC

;�رո����¼�
proc 2
endproc

;������ʱ�¼���ÿ2�봥��һ�Σ�
proc 3
	IS_PWORLD_OVER 30
	if @30 != 1
		get_pworld_var 4 32 1 12
		get_pworld_var 8 32 1 60
		
		if @12 = 0 and @60 = 0
			set_pworld_var 8 32 1 1 
		elseif @12 = 1 and @60 = 0
			set_pworld_var 8 32 1 1 
			set_pworld_var 0 32 0 0
			;��ʼ����ˢ��Ч����
			set_pworld_var 24 32 1 0 
		elseif @12 = 2 and @60 = 0
			get_mon_num_by_defid 212115 20
			;����һ������û
			if @20 < 1
				set_pworld_var 8 32 1 1 
				set_pworld_var 0 32 0 0
			endif
		elseif @12 = 3 and @60 = 0
			get_mon_num_by_defid 212116 20
			;���ڶ�������û
			if @20 < 1
				set_pworld_var 8 32 1 1 
				set_pworld_var 0 32 0 0
			endif
		elseif @12 = 4 and @60 = 0
			get_mon_num_by_defid 212117 20
			;������������û
			if @20 < 1
				set_pworld_var 8 32 1 1 
				set_pworld_var 0 32 0 0
			endif
		elseif @12 = 5 and @60 = 0
			get_mon_num_by_defid 212118 20
			;�����Ĳ�����û
			if @20 < 1
				set_pworld_var 8 32 1 1 
				set_pworld_var 0 32 0 0
			endif
		elseif @12 = 6 and @60 = 0
			get_mon_num_by_defid 212119 20
			;�����岨����û
			if @20 < 1
				set_pworld_var 8 32 1 1 
				set_pworld_var 0 32 0 0
			endif
		elseif @12 = 7 and @60 = 0
			get_mon_num_by_defid 212004 20
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
			
			;1 ��ʱ��ſ�ʼˢ��Ч��
			get_pworld_var 24 32 1 19
			if @19 = 1
				get_pworld_var 20 32 1 18
				if @18 >= 15
					call 101
					set_pworld_var 20 32 0 0
				else
					add 18 1
					set_pworld_var 20 32 @18 1
				endif
			endif
		endif
	endif	
endproc

;�����й��������¼�
proc 4

endproc

;�����н�ɫ�����¼�
proc 5

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
			add_sys_msg_start
			add_sys_msg �㿪���ϵĻ���
			set_sys_msg_mode 4 0 0
			MAP_MSG_INFO
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

;ˢ��Ч��
proc 101
	strvar PosX PosY	
	rand 14 20
	switch 14
		case 0
			var PosX 12931
			var PosY 13953
		endcase

		case 1
			var PosX 12954
			var PosY 14549
		endcase
		
		case 2
			var PosX 12496
			var PosY 14365
		endcase

		case 3
			var PosX 11776
			var PosY 13967
		endcase
		
		case 4
			var PosX 12475 
			var PosY 13054
		endcase

		case 5
			var PosX 10876
			var PosY 13076
		endcase
		
		case 6
			var PosX 12510 
			var PosY 12364
		endcase

		case 7
			var PosX 11117
			var PosY 12129
		endcase
		
		case 8
			var PosX 11829
			var PosY 12696
		endcase

		case 9
			var PosX 11484
			var PosY 13009
		endcase
		
		case 10
			var PosX 13371
			var PosY 12039
		endcase

		case 11
			var PosX 13420   
			var PosY 13092
		endcase
		
		case 12
			var PosX 12635   
			var PosY 10959
		endcase

		case 13
			var PosX 13666   
			var PosY 13995
		endcase
		
		case 14
			var PosX 14289
			var PosY 13084
		endcase

		case 15
			var PosX 14080
			var PosY 11645
		endcase
		
		case 16
			var PosX 12672
			var PosY 11747
		endcase

		case 17
			var PosX 11907
			var PosY 11153
		endcase
		
		case 18
			var PosX 11715
			var PosY 12000
		endcase

		case 19
			var PosX 13290
			var PosY 11060
		endcase
	endswitch
		
	NEW_LIFE_MON 212200 3 @PosX @PosY 300 0 0 0 30 0 0
endproc

proc 201
	NEW_LIFE_MON 212115 1 12931 13953 0 0 0 0 0 0 0
	NEW_LIFE_MON 212115 1 12954 14549 0 0 0 0 0 0 0
	NEW_LIFE_MON 212115 1 12496 14365 0 0 0 0 0 0 0
	NEW_LIFE_MON 212115 1 11776 13967 0 0 0 0 0 0 0
	NEW_LIFE_MON 212115 1 12475 13054 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212115 1 10876 13076 0 0 0 0 0 0 0
	NEW_LIFE_MON 212115 1 12510 12364 0 0 0 0 0 0 0
	NEW_LIFE_MON 212115 1 11117 12129 0 0 0 0 0 0 0
	NEW_LIFE_MON 212115 1 11829 12696 0 0 0 0 0 0 0
	NEW_LIFE_MON 212115 1 11484 13009 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212115 1 13371 12039 0 0 0 0 0 0 0
	NEW_LIFE_MON 212115 1 13420 13092 0 0 0 0 0 0 0
	NEW_LIFE_MON 212115 1 12635 10959 0 0 0 0 0 0 0
	NEW_LIFE_MON 212115 1 13666 13995 0 0 0 0 0 0 0
	NEW_LIFE_MON 212115 1 14289 13084 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212115 1 14080 11645 0 0 0 0 0 0 0
	NEW_LIFE_MON 212115 1 12672 11747 0 0 0 0 0 0 0
	NEW_LIFE_MON 212115 1 11907 11153 0 0 0 0 0 0 0
	NEW_LIFE_MON 212115 1 11715 12000 0 0 0 0 0 0 0
	NEW_LIFE_MON 212115 1 13290 11060 0 0 0 0 0 0 0
endproc

proc 202
	NEW_LIFE_MON 212116 1 12931 13953 0 0 0 0 0 0 0
	NEW_LIFE_MON 212116 1 12954 14549 0 0 0 0 0 0 0
	NEW_LIFE_MON 212116 1 12496 14365 0 0 0 0 0 0 0
	NEW_LIFE_MON 212116 1 11776 13967 0 0 0 0 0 0 0
	NEW_LIFE_MON 212116 1 12475 13054 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212116 1 10876 13076 0 0 0 0 0 0 0
	NEW_LIFE_MON 212116 1 12510 12364 0 0 0 0 0 0 0
	NEW_LIFE_MON 212116 1 11117 12129 0 0 0 0 0 0 0
	NEW_LIFE_MON 212116 1 11829 12696 0 0 0 0 0 0 0
	NEW_LIFE_MON 212116 1 11484 13009 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212116 1 13371 12039 0 0 0 0 0 0 0
	NEW_LIFE_MON 212116 1 13420 13092 0 0 0 0 0 0 0
	NEW_LIFE_MON 212116 1 12635 10959 0 0 0 0 0 0 0
	NEW_LIFE_MON 212116 1 13666 13995 0 0 0 0 0 0 0
	NEW_LIFE_MON 212116 1 14289 13084 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212116 1 14080 11645 0 0 0 0 0 0 0
	NEW_LIFE_MON 212116 1 12672 11747 0 0 0 0 0 0 0
	NEW_LIFE_MON 212116 1 11907 11153 0 0 0 0 0 0 0
	NEW_LIFE_MON 212116 1 11715 12000 0 0 0 0 0 0 0
	NEW_LIFE_MON 212116 1 13290 11060 0 0 0 0 0 0 0
endproc

proc 203
	NEW_LIFE_MON 212117 1 12931 13953 0 0 0 0 0 0 0
	NEW_LIFE_MON 212117 1 12954 14549 0 0 0 0 0 0 0
	NEW_LIFE_MON 212117 1 12496 14365 0 0 0 0 0 0 0
	NEW_LIFE_MON 212117 1 11776 13967 0 0 0 0 0 0 0
	NEW_LIFE_MON 212117 1 12475 13054 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212117 1 10876 13076 0 0 0 0 0 0 0
	NEW_LIFE_MON 212117 1 12510 12364 0 0 0 0 0 0 0
	NEW_LIFE_MON 212117 1 11117 12129 0 0 0 0 0 0 0
	NEW_LIFE_MON 212117 1 11829 12696 0 0 0 0 0 0 0
	NEW_LIFE_MON 212117 1 11484 13009 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212117 1 13371 12039 0 0 0 0 0 0 0
	NEW_LIFE_MON 212117 1 13420 13092 0 0 0 0 0 0 0
	NEW_LIFE_MON 212117 1 12635 10959 0 0 0 0 0 0 0
	NEW_LIFE_MON 212117 1 13666 13995 0 0 0 0 0 0 0
	NEW_LIFE_MON 212117 1 14289 13084 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212117 1 14080 11645 0 0 0 0 0 0 0
	NEW_LIFE_MON 212117 1 12672 11747 0 0 0 0 0 0 0
	NEW_LIFE_MON 212117 1 11907 11153 0 0 0 0 0 0 0
	NEW_LIFE_MON 212117 1 11715 12000 0 0 0 0 0 0 0
	NEW_LIFE_MON 212117 1 13290 11060 0 0 0 0 0 0 0
endproc

proc 204
	NEW_LIFE_MON 212118 1 12931 13953 0 0 0 0 0 0 0
	NEW_LIFE_MON 212118 1 12954 14549 0 0 0 0 0 0 0
	NEW_LIFE_MON 212118 1 12496 14365 0 0 0 0 0 0 0
	NEW_LIFE_MON 212118 1 11776 13967 0 0 0 0 0 0 0
	NEW_LIFE_MON 212118 1 12475 13054 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212118 1 10876 13076 0 0 0 0 0 0 0
	NEW_LIFE_MON 212118 1 12510 12364 0 0 0 0 0 0 0
	NEW_LIFE_MON 212118 1 11117 12129 0 0 0 0 0 0 0
	NEW_LIFE_MON 212118 1 11829 12696 0 0 0 0 0 0 0
	NEW_LIFE_MON 212118 1 11484 13009 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212118 1 13371 12039 0 0 0 0 0 0 0
	NEW_LIFE_MON 212118 1 13420 13092 0 0 0 0 0 0 0
	NEW_LIFE_MON 212118 1 12635 10959 0 0 0 0 0 0 0
	NEW_LIFE_MON 212118 1 13666 13995 0 0 0 0 0 0 0
	NEW_LIFE_MON 212118 1 14289 13084 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212118 1 14080 11645 0 0 0 0 0 0 0
	NEW_LIFE_MON 212118 1 12672 11747 0 0 0 0 0 0 0
	NEW_LIFE_MON 212118 1 11907 11153 0 0 0 0 0 0 0
	NEW_LIFE_MON 212118 1 11715 12000 0 0 0 0 0 0 0
	NEW_LIFE_MON 212118 1 13290 11060 0 0 0 0 0 0 0
endproc

proc 205
	NEW_LIFE_MON 212119 1 12931 13953 0 0 0 0 0 0 0
	NEW_LIFE_MON 212119 1 12954 14549 0 0 0 0 0 0 0
	NEW_LIFE_MON 212119 1 12496 14365 0 0 0 0 0 0 0
	NEW_LIFE_MON 212119 1 11776 13967 0 0 0 0 0 0 0
	NEW_LIFE_MON 212119 1 12475 13054 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212119 1 10876 13076 0 0 0 0 0 0 0
	NEW_LIFE_MON 212119 1 12510 12364 0 0 0 0 0 0 0
	NEW_LIFE_MON 212119 1 11117 12129 0 0 0 0 0 0 0
	NEW_LIFE_MON 212119 1 11829 12696 0 0 0 0 0 0 0
	NEW_LIFE_MON 212119 1 11484 13009 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212119 1 13371 12039 0 0 0 0 0 0 0
	NEW_LIFE_MON 212119 1 13420 13092 0 0 0 0 0 0 0
	NEW_LIFE_MON 212119 1 12635 10959 0 0 0 0 0 0 0
	NEW_LIFE_MON 212119 1 13666 13995 0 0 0 0 0 0 0
	NEW_LIFE_MON 212119 1 14289 13084 0 0 0 0 0 0 0
	
	NEW_LIFE_MON 212119 1 14080 11645 0 0 0 0 0 0 0
	NEW_LIFE_MON 212119 1 12672 11747 0 0 0 0 0 0 0
	NEW_LIFE_MON 212119 1 11907 11153 0 0 0 0 0 0 0
	NEW_LIFE_MON 212119 1 11715 12000 0 0 0 0 0 0 0
	NEW_LIFE_MON 212119 1 13290 11060 0 0 0 0 0 0 0
endproc

proc 206
	NEW_LIFE_MON 212004 1 12889 12550 0 0 0 0 0 0 0
endproc
