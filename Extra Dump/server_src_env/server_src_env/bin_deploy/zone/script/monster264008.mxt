;�ڶ������� 55-60����������
 
proc 1
;����ˢ��
	var 10 0
	SET_MON_VAR 0 8 @10 1
	var 11 0
	SET_MON_VAR 4 8 @11 1
	var 12 0
	SET_MON_VAR 7 8 @12 1	
	var 13 0
	SET_MON_VAR 1 8 @13 1
	var 200 100
	STRVAR MonX MonY MonID

	GET_MON_INFO 1 MonID 30 31 32 33 MonX MonY
	TARGET_ADD_STATUS 3 @30 1009 2 10 1
endproc
proc 2
;��������
		SET_MON_TASK_STAT 30003 2 1
endproc

proc 3
;����Ѳ�߽����¼�
	STRVAR PatrolID Mon_Ret 
	
	STRVAR NumRand_1 NumRand_2 NumRand_3	
	
	GET_MON_INFO 1 40 41 42 43 44 45 46
	;�õ��������ڵ�ͼ/��Ϣ
	GET_MON_PATROL_ID @41 Mon_Ret PatrolID
	;�õ�����·��ID
	
	if Mon_Ret = 0
		if PatrolID = 20 or PatrolID = 33 or PatrolID = 34 or PatrolID = 23 or PatrolID = 35 or PatrolID = 36 or PatrolID = 26 or PatrolID = 37 or PatrolID = 38 or PatrolID = 22 or PatrolID = 25 or PatrolID = 28
				SET_MON_TASK_STAT 30003 1 1
				DEL_MON 1
		endif

		if PatrolID = 15 
			rand NumRand_1 4
			add NumRand_1 100
			switch NumRand_1
				case 100
					MON_CHG_MAP 2300 16190 20698 20 10 1
				endcase
				case 101
					MON_CHG_MAP 2300 16190 20698 33 10 1
				endcase
				case 102
					MON_CHG_MAP 2300 16190 20698 34 10 1
				endcase
				case 103
					MON_CHG_MAP 2300 16190 20698 21 10 1
				endcase
			endswitch
		endif
		if PatrolID = 16 
			rand NumRand_2 4
			add NumRand_2 100
			switch NumRand_2
				case 100
					MON_CHG_MAP 2400 3320 3550 23 10 1
				endcase
				case 101
					MON_CHG_MAP 2400 3320 3550 35 10 1
				endcase
				case 102
					MON_CHG_MAP 2400 3320 3550 36 10 1
				endcase
				case 103
					MON_CHG_MAP 2400 3320 3550 24 10 1
				endcase
			endswitch
		endif
		if PatrolID = 17 
			rand NumRand_3 4
			add NumRand_3 100
			switch NumRand_3
				case 100
					MON_CHG_MAP 2500 31067 22561 26 10 1
				endcase
				case 101
					MON_CHG_MAP 2500 31067 22561 37 10 1
				endcase
				case 102
					MON_CHG_MAP 2500 31067 22561 38 10 1
				endcase
				case 103
					MON_CHG_MAP 2500 31067 22561 27 10 1
				endcase
				
			endswitch
		endif
		if PatrolID = 21
			MON_CHG_MAP 3300 1005 7374 22 10 1
		endif
		if PatrolID = 24
			MON_CHG_MAP 3400 2855 3153 25 10 1
		endif
		if PatrolID = 27
			MON_CHG_MAP 3500 5327 12796 28 10 1
		endif
	endif
endproc

proc 4
;3��ִ��һ��


;	GET_MON_VAR 0 8 1 10
;	GET_MON_VAR 4 8 1 11	
	GET_MON_VAR 7 8 1 12	
	GET_MON_VAR 1 8 1 13	

	GET_MON_INFO 1 MonID 30 31 32 33 MonX MonY
		;�����ҵȼ�
	
;	if 11 = 0 
		MON_TASK_AREA_CHECK 30003 3000 300 200		
		;�����Ч���
;	endif
			
;	add 11 1
;	SET_MON_VAR 4 8 @11 1	
;	if 11 = 7 
;		MON_TASK_AREA_CHECK 30003 3000 300 200 
;		;�����Ч���
;		SET_MON_VAR 4 8 0 1	
;	endif
	
	if 200 = 0
	;ʧ�ܺ�
			
			GET_MON_VAR 7 8 1 12
			add 12 1
			SET_MON_VAR 7 8 @12 1			
			if 12 = 1
				DEL_MON 1			
			endif
	else

	
		if 32 != 1000

			add 13 1
			SET_MON_VAR 1 8 @13 1	
			
			if 13 = 20 
				;60��ʱ���ڳ�ͣ��30���ٻ�����
				TARGET_ADD_STATUS 3 @30 1009 2 20 1
				if 32 = 2300 or 32 = 3300
					MON_CALL_ENEMY_MON 200024 2 35 1 @MonX @MonY 600 0 0 0 30 1 0
					MON_CALL_ENEMY_MON 200025 2 35 1 @MonX @MonY 600 0 0 0 30 1 0
				endif
				if 32 = 2400 or 32 = 3400
					MON_CALL_ENEMY_MON 200024 2 45 1 @MonX @MonY 600 0 0 0 30 1 0
					MON_CALL_ENEMY_MON 200025 2 45 1 @MonX @MonY 600 0 0 0 30 1 0
				endif
				if 32 = 2500 or 32 = 3500
					MON_CALL_ENEMY_MON 200024 2 55 1 @MonX @MonY 600 0 0 0 30 1 0
					MON_CALL_ENEMY_MON 200025 2 55 1 @MonX @MonY 600 0 0 0 30 1 0
				endif
				var 13 0
				SET_MON_VAR 1 8 @13 1		
			endif
		endif

	endif

endproc
