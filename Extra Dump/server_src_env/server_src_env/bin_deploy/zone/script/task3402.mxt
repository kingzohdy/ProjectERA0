
proc 1
	var 30 0
	set_task_var 3402 0 8 @30 1
	var 31 0
	set_task_var 3402 1 8 @31 1 
	
	var 32 0
	set_task_var 3402 2 8 @32 1
endproc 

proc 5
	call 130
endproc 

proc 6

	strvar _Rac Map
	GET_ID_PLAYER_INFO 0 7 _Rac
	get_player_mapinfo 0 Map 2 0 0
	
	if _Rac = 1 and Map != 50900
		return 
	endif
	if _Rac = 2 and Map != 51000
		return
	endif
	if _Rac = 3 and Map != 51100
		return
	endif
	if _Rac = 4 and Map != 51200
		return
	endif
	
;;;;;;;;;;;;;;���������Աߺ����Ƿ��ڸ���
;	get_task_var 3402 1 8 1 @31 
;	if 31 = 1
;		strvar _Rac Map
;		GET_ID_PLAYER_INFO 0 7 _Rac
;		get_player_mapinfo 0 Map 2 0 0
;		
;		if _Rac = 1 and Map != 50900
;			TASK_FAIL 3402 1
;		endif
;		if _Rac = 2 and Map != 51000
;			
;			TASK_FAIL 3402 1
;		endif
;		if _Rac = 3 and Map != 51100
;			
;			TASK_FAIL 3402 1
;		endif
;		if _Rac = 4 and Map != 51200
;			
;			TASK_FAIL 3402 1
;		endif
;	endif

;;;;;;;;;;;;;;;;;;;;;
	get_task_var 3402 0 8 @30 1
	if 30 = 0
		strvar playerX playerY Dis Mid
		GET_PLAYER_MAPINFO 0 0 0 playerX playerY
		GET_POINT_DIST 5551 9508 @playerX @playerY Dis
		if Dis < 800
			GET_MAP_NPC_ID 282019 Mid 1
			mon_speech @Mid 2 0 1 ���������ˣ����������ˡ���
			var 30 10
			set_task_var 3402 0 8 @30 1
		endif
	endif
;;;;;;;;;;;;;;;;;;;;ѡ�����
	strvar Loc3 Tim3 Mid
	get_pworld_var 11 8 1 Loc3 
	if Loc3 = 1
		get_pworld_var 12 8 1 Tim3 
		add Tim3 1
		set_pworld_var 12 8 @Tim3 1
		strvar LeiMid2
		if Tim3 = 1
			new_npc LeiMid2 304060 5000 9000 0 zhleimeng.mac
			set_pworld_var 30 32 @LeiMid2 1
			TARGET_ADD_STATUS 2 @LeiMid2 1257 1 7200 1
		endif
		
		if Tim3 = 2
			GET_MAP_NPC_ID 282019 Mid 1
			mon_speech @Mid 2 0 1 �죡�������������ڰ�����צ�����Ͼ�Ҫ�������ˣ�
			
		endif
		if Tim3 = 3
			get_pworld_var 30 32 1 LeiMid2
			mon_speech @LeiMid2 2 0 1 ��Ȼ��ѡ��������ж����ˣ��Ǿ͸Ͻ������϶�Ϊһ�ɡ�
			
		endif
		if Tim3 = 4
			del_npc 282019 1
			NEW_TMP_ITEM 1460021 5551 9508 0 0 0 0
		endif
		if Tim3 = 7
			del_npc 304060 1
			
		endif
		;������û�����������
		strvar Loc_5
		get_pworld_var 19 8 1 Loc_5
		;������û��
		if Loc_5 = 1
		
			var Loc3 0
			set_pworld_var 11 8 @Loc3 1
		endif
		
	;	GET_ROLE_RIDE_ON_MACHINE 0 50
	;	GET_ROLE_RIDE_ON_MACHINE_MEM_ID 50 0
		GET_ROLE_TMP_MACHINE 0 50
		if 50 = -1 and Loc_5 != 1
			get_task_var 3402 2 8 1 32
			add 32 1
			set_task_var 3402 2 8 @32 1
			if 32 = 10
				
				add_sys_msg_start
				add_sys_msg ������������ж�����˻�е��ӭ����һ����ս��
				set_sys_msg_mode 3 0 0
				msg_info 
				
				var 32 0
				set_task_var 3402 2 8 @32 1
			endif
		endif
		if 50 = 1460021 and Loc_5 != 1
			get_task_var 3402 2 8 1 32
			add 32 1
			set_task_var 3402 2 8 @32 1
			if 32 = 10
				add_sys_msg_start
				add_sys_msg ��ǰ�ߣ�ȥӭ�����������ս������2��ʹ�û��ܼ��������˺�
				set_sys_msg_mode 3 0 0
				msg_info 
				
				var 32 0
				set_task_var 3402 2 8 @32 1
			endif
		endif
		; add_sys_msg_start
		; add_sys_msg $@50$ $@Loc_5$ $@32$ $@Loc3$
		; set_sys_msg_mode 1 0 0
		; msg_info 
	endif
	
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;�����ˣ���Ҫ��ɱ���ж�
	strvar Loc4 Tim4
	
	get_pworld_var 13 8 1 Loc4 
	if Loc4 = 1
		get_pworld_var 14 8 1 Tim4
		add Tim4 1
		set_pworld_var 14 8 @Tim4 1
		
		strvar LeiMid3
		if Tim4 = 1
			new_npc LeiMid3 304060 5000 9000 0 zhleimeng.mac
			set_pworld_var 34 32 @LeiMid3 1
			TARGET_ADD_STATUS 2 @LeiMid3 1257 1 7200 1
			mon_speech @LeiMid3 2 0 1 ����¸ң�#Name#�������ڣ������Ҫ��ɱ�������������Ŀ��ܻ����㣡
		endif
		if Tim4 = 2
			get_pworld_var 34 32 1 LeiMid3
			mon_speech @LeiMid3 2 0 1 ��Ȼѡ���ˣ�����ֻ�����޷��˵�����ȥ�����ģ��һ������ģ�
		endif
		if Tim4 = 3
			del_npc 282019 1
			NEW_MON 282026 1 5551 9508 0 0 0 0 0
		endif
		
		if Tim4 = 5
			var Tim4 4
			set_pworld_var 14 8 @Tim4 1	
		endif
		
		strvar Hp
		GET_ID_PLAYER_INFO 0 2 Hp
		if Hp < 4000
			get_pworld_var 34 32 1 LeiMid3
			mon_speech @LeiMid3 2 0 1 �������ֵ̫���ˣ����������㣡
		    ADD_PLAYER_HP 0 4000 1
			
			strvar Plmid
			get_player_id 0 Plmid
			effect_id 0 @Plmid playMontion(%d,"9505_16")
			
		endif
			
		strvar Loc_3
		get_pworld_var 17 8 1 Loc_3
		;���ж�û��
		if Loc_3 != 1
			get_task_var 3402 2 8 1 32
			add 32 1
			set_task_var 3402 2 8 @32 1
			if 32 = 10
				
				add_sys_msg_start
				add_sys_msg �Ͽ��ɱ���ж������ɸǶٵ���������һ��֮����
				set_sys_msg_mode 3 0 0
				msg_info 
				
				var 32 0
				set_task_var 3402 2 8 @32 1
			endif
		endif
	endif
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;	
	;;;ʹ��С����3��
		strvar Loc_1 Loc_2
		get_pworld_var 15 8 0 Loc_1
		if Loc_1 = 1
			get_pworld_var 16 8 1 Loc_2
			add Loc_2 1
			set_pworld_var 16 8 @Loc_2 1
			if Loc_2 = 3
				call 100
			endif
			
			strvar LeiMid1
			if Loc_2 = 4
				new_npc LeiMid1 304060 4800 10100 0 zhleimeng.mac
				set_pworld_var 26 32 @LeiMid1 1
				TARGET_ADD_STATUS 2 @LeiMid1 1257 1 7200 1
			endif
			if Loc_2 = 5
				get_pworld_var 26 32 1 LeiMid1
				mon_speech @LeiMid1 2 0 1 ʼ�ն��Ǳܲ����Ӳ�����ȥ�͹��ж�̸̸�ɣ�#Name#�������������
		
			
				; add_sys_msg_start
				; add_sys_msg ���㱳��Ĺ��ж��Ի�������ѡ�񣬽���ѡ��ǩ����Լ����
				; set_sys_msg_mode 3 0 0
				; msg_info 
			endif
			
			if Loc_2 = 8
				del_npc 304060 1
				
				var Loc_1 2
				set_pworld_var 15 8 @Loc_1 1
			endif
		endif
		
		;δ����ж�ѡ��֮ǰ5����ʾһ��
		get_pworld_var 15 8 1 Loc_1 
		if Loc_1 = 2
			get_pworld_var 0 8 1 50
			get_pworld_var 13 8 1 60 
			if 50 = 1 or 60 = 1
				var Loc_1 3
				set_pworld_var 15 8 @Loc_1 1
				
				get_task_var 3402 2 8 1 32
				var 32 0
				set_task_var 3402 2 8 @32 1
			else
				get_task_var 3402 2 8 1 32
				add 32 1
				set_task_var 3402 2 8 @32 1
				if 32 = 10
					
					add_sys_msg_start
					add_sys_msg ���㱳��Ĺ��ж��Ի�������ѡ�񣬽���ѡ��ǩ����Լ����
					set_sys_msg_mode 3 0 0
					msg_info 
					var 32 0
					set_task_var 3402 2 8 @32 1
				endif
			endif
		endif
;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;ɱ����ˢ������
	strvar Loc_3 Loc_4
	get_pworld_var 17 8 1 Loc_3
	if Loc_3 = 1 
		get_pworld_var 18 8 1 Loc_4 
		add Loc_4 1
		set_pworld_var 18 8 @Loc_4 1
		
		strvar Mid
		if Loc_4 = 1
			get_pworld_var 34 32 1 LeiMid3
			mon_speech @LeiMid3 2 0 1 ��л�㣬#Name#��һ����������ҡ�ڵ����ž������������ˡ�

		endif
		if Loc_4 = 5
			get_pworld_var 34 32 1 LeiMid3
			mon_speech @LeiMid3 2 0 1 ʱ�䲻���ˣ�#Name#�����Ѹо����ڰ�����Ȯ����Ϣ��

		endif
		if Loc_4 = 9
			get_pworld_var 34 32 1 LeiMid3
			mon_speech @LeiMid3 2 0 1 �����������죡�Ҷ��ε���Դ��ֻҪ�����������ξ��ͻ����飡
		endif
		if Loc_4 = 13
			del_npc 304060 1
			NEW_MON 282020 1 6000 6600 0 0 0 0 0
			var Loc_3 2
			set_pworld_var 17 8 @Loc_3 1
		endif
	endif
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	strvar Loc_5 Loc_6
;;;;;����������3�봫��
	get_pworld_var 19 8 1 Loc_5
	if Loc_5 = 1
		get_pworld_var 20 8 1 Loc_6
		add Loc_6 1
		set_pworld_var 20 8 @Loc_6 1
		if Loc_6 = 1
			
			DELETE_MON_BY_DEFID 261009 1
			DELETE_MON_BY_DEFID 261010 1
		endif
		if Loc_6 = 2
			strvar LeiMid5 X Y
			get_player_mapinfo 0 0 0 X Y
			ADD X 200
			ADD Y 200
			new_npc LeiMid5 304060 @X @Y 0 zhleimeng.mac
			TARGET_ADD_STATUS 2 @LeiMid5 1257 1 7200 1
			mon_speech @LeiMid5 2 0 1 ����������Ҫ���ٵģ��������Ŀ��飬��������û�а취���㣬�����Լ����һ����ɹ���
		endif
		
		if Loc_6 = 6
			call 101
			;�������ػ�����

;		get_task_var 3402 1 8 1 31
;		var 31 1
;		set_task_var 3402 1 8 @31 1
		endif
		strvar LeiMid4
		
		if Loc_6 = 9
			get_pworld_var 38 32 1 LeiMid4
			mon_speech @LeiMid4 2 0 1 ������ؼ���һ���ˣ�������ʲô�����������κζ����ӽ����
		
		endif
		if Loc_6 = 13
			get_pworld_var 38 32 1 LeiMid4
			mon_speech @LeiMid4 2 0 1 ֻҪ��ס�����ֻҪ�ҵ�����ָ����ҾͿ��԰���������ˣ�
		
		endif
		if Loc_6 = 15
			del_npc 304060 1
			var Loc_5 2
			set_pworld_var 19 8 @Loc_5 1
		endif
		
		
	endif
;;;;;;;;;;;;;;;;;;;;;;;;
	strvar Ret
	get_pworld_var 0 8 1 50
	if 50 = 1
		GET_ROLE_TMP_MACHINE 0 Ret
		;�����ʱ��е
		if Ret = 1460021
			strvar TimLei
			get_pworld_var 25 8 1 TimLei
			add TimLei 1
			set_pworld_var 25 8 @TimLei 1
			if TimLei = 1
				get_pworld_var 30 32 1 LeiMid2
				mon_speech @LeiMid2 2 0 1 ʱ�䲻���ˣ�#Name#�����Ѹо����ڰ�����Ȯ����Ϣ��
			endif
			if TimLei = 2
				get_pworld_var 30 32 1 LeiMid2
				mon_speech @LeiMid2 2 0 1 �����������죡�����ε���Դ��ֻҪ�����������ξ��ͻ����飡
			endif
			if TimLei = 3
				NEW_MON 282020 1 6000 6600 0 0 0 0 0
				var 50 0
				set_pworld_var 0 8 @50 1
			endif
		endif
		
	endif
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;;;;��������
	strvar MapLoc LosTim
	get_pworld_var 22 8 1 MapLoc
	
	
	if MapLoc = 1
		get_pworld_var 23 8 1 LosTim
		add LosTim 1
		set_pworld_var 23 8 @LosTim 1
		if LosTim = 1
			add_sys_msg_start
			add_sys_msg �����������𣬴˴�Ӫ���ж�ʧ�ܣ��㽫��3�����뿪�˵ء�
			set_sys_msg_mode 3 0 0
			msg_info 
		endif
	
		DELETE_MON_BY_DEFID 284018 1
		DELETE_MON_BY_DEFID 284026 1
		DELETE_MON_BY_DEFID 284027 1
		DELETE_MON_BY_DEFID 284028 1
		DELETE_MON_BY_DEFID 284019 1
		DELETE_MON_BY_DEFID 284020 1
		DELETE_MON_BY_DEFID 284025 1
		DELETE_MON_BY_DEFID 284024 1
		DELETE_MON_BY_DEFID 284021 1
		DELETE_MON_BY_DEFID 284022 1
		DELETE_MON_BY_DEFID 284023 1
		
		if LosTim = 3
			call 130
			var MapLoc 2
			set_pworld_var 22 8 @MapLoc 1
		endif
	endif
	
endproc 

proc 100
;ʹ��С����
	strvar Rac
	GET_ID_PLAYER_INFO 0 7 Rac
	if Rac = 1
		GET_PLAYER_ID 1 100
		TRANSMIT_CAMERA @100 90 50900 5042 10350 0 5
	endif 
	if Rac = 2 
		GET_PLAYER_ID 1 100
		TRANSMIT_CAMERA @100 90 51000 5042 10350 0 5
	
	endif
	if Rac = 3
		GET_PLAYER_ID 1 100
		TRANSMIT_CAMERA @100 90 51100 5042 10350 0 5
	endif
	if Rac = 4
		GET_PLAYER_ID 1 100
		TRANSMIT_CAMERA @100 90 51200 5042 10350 0 5
	endif

endproc 

proc 101
;;;;����
	strvar Rac LeiMid4
	GET_ID_PLAYER_INFO 0 7 Rac

		if Rac = 1
			GET_PLAYER_ID 1 100
			TRANSMIT_CAMERA @100 0 50900 11200 13700 0 5
			DEL_ROLE_TMP_ITEM 0 0
			
			NEW_MON 284018 1 11462 14381 0 0 0 0 0
			
			new_npc LeiMid4 304060 11700 13900 0 zhleimeng.mac
			set_pworld_var 38 32 @LeiMid4 1
			TARGET_ADD_STATUS 2 @LeiMid4 1257 1 7200 1
			;����
		endif
		if Rac = 2
			GET_PLAYER_ID 1 100
			TRANSMIT_CAMERA @100 0 51000 20000 9500 0 5
			DEL_ROLE_TMP_ITEM 0 20
			
			NEW_MON 284026 1 20003 8388 0 0 0 0 0
			
			new_npc LeiMid4 304060 20000 9000 0 zhleimeng.mac
			set_pworld_var 38 32 @LeiMid4 1
			TARGET_ADD_STATUS 2 @LeiMid4 1257 1 7200 1
			;����
		endif
		if Rac = 3
		
			GET_PLAYER_ID 1 100
			TRANSMIT_CAMERA @100 0 51100 19180 9641 0 5
			DEL_ROLE_TMP_ITEM 0 20
			
			NEW_MON 284027 1 20003 8388 0 0 0 0 0
			
			new_npc LeiMid4 304060 20000 9000 0 zhleimeng.mac
			set_pworld_var 38 32 @LeiMid4 1
			TARGET_ADD_STATUS 2 @LeiMid4 1257 1 7200 1
			;����
		endif
		if Rac = 4
			GET_PLAYER_ID 1 100
			TRANSMIT_CAMERA @100 0 51200 19180 9641 0 5
			DEL_ROLE_TMP_ITEM 0 20
			
			NEW_MON 284028 1 20003 8388 0 0 0 0 0
			
			new_npc LeiMid4 304060 20000 9000 0 zhleimeng.mac
			set_pworld_var 38 32 @LeiMid4 1
			TARGET_ADD_STATUS 2 @LeiMid4 1257 1 7200 1
			;����
		endif
endproc 


proc 7

	strvar _Rac Map
	GET_ID_PLAYER_INFO 0 7 _Rac
	get_player_mapinfo 0 Map 2 0 0
	
	if _Rac = 1 and Map != 50900
		return 
	endif
	if _Rac = 2 and Map != 51000
		return
	endif
	if _Rac = 3 and Map != 51100
		return
	endif
	if _Rac = 4 and Map != 51200
		return
	endif
	
	
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;�����������
	
	strvar Com Loc_7 
	get_pworld_var 6 8 1 Com	
	if Com = 1
		get_pworld_var 21 8 1 Loc_7
		add Loc_7 1
		set_pworld_var 21 8 @Loc_7 1
		if Loc_7 = 1
			strvar LeiMid5 X Y
			get_player_mapinfo 0 0 0 X Y
			ADD X 100
			ADD Y 100
			new_npc LeiMid5 304060 @X @Y 0 zhleimeng.mac
			set_pworld_var 42 32 @LeiMid5 1
		endif
		if Loc_7 = 3
			get_pworld_var 42 32 1 LeiMid5
			mon_speech @LeiMid5 2 0 1 #Name#������յ���Ϊע���㽫��Ϊһ�����˾�����Ӣ�ۣ�
		endif
		if Loc_7 = 5
			strvar Hp Rac m_PlayerID
			GET_ID_PLAYER_INFO 0 7 Rac
			GET_PLAYER_ID 1 m_PlayerID
			if Rac = 1
				TRANSMIT_CAMERA @m_PlayerID 0 50900 19684 19531 0 5		
				task_fini 3402 1
			endif
			if Rac = 2
				TRANSMIT_CAMERA @m_PlayerID 0 51000 19684 19531 0 5	
				task_fini 3402 1
			endif
			if Rac = 3
				TRANSMIT_CAMERA @m_PlayerID 0 51100 19684 19531 0 5	
				task_fini 3402 1
			endif
			if Rac = 4
				TRANSMIT_CAMERA @m_PlayerID 0 51200 19684 19531 0 5	
				task_fini 3402 1
			endif
			var Com 2
			set_pworld_var 6 8 @Com 1
			
		endif
	endif
	if Com = 2
		
		get_player_mapinfo 0 Map 2 0 0
		if Map = 50900 or Map = 51000 or Map = 51100 or Map = 51200
			
			get_task_var 3402 2 8 1 32
			add 32 1
			set_task_var 3402 2 8 @32 1
			if 32 = 10
				add_sys_msg_start
				add_sys_msg �����ɸǶٶԻ����뿪�þ����ص���ʿ����
				set_sys_msg_mode 3 0 0
				msg_info 
				var 32 0
				set_task_var 3402 2 8 @32 1
			endif
		endif
	endif
endproc 
proc 130
	strvar Map Rac
	GET_PLAYER_MAPINFO 1 Map 0 0 0
	GET_ID_PLAYER_INFO 0 7 Rac

	if Map = 50900 and Rac = 1
		TRANSMIT 4402 18500 8300 1
	endif
	if Map = 51000 and Rac = 2
		TRANSMIT 4402 18500 8300 1
	endif
	if Map = 51100 and Rac = 3
		TRANSMIT 4402 18500 8300 1
	endif
	if Map = 51200 and Rac = 4
		TRANSMIT 4402 18500 8300 1
	endif

endproc


