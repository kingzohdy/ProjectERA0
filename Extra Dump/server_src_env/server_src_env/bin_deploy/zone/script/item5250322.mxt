;а���屦��

	;ADD_SYS_MSG_START
	;ADD_SYS_MSG 
	;SET_SYS_MSG_MODE 0 0 0
	;MSG_INFO
	

proc 1
	strvar itnum retitem
	GET_ITEM_NUM_INPAK 5060022 retitem itnum
	if retitem = 0 and itnum = 0
		ADD_SYS_MSG_START
		ADD_SYS_MSG ��û�й���֮Կ���޷��������䣡
		SET_SYS_MSG_MODE 1 0 0
		MSG_INFO
		return
	endif
	strvar Num
	GET_PACK_SPACE 0 Num
	if @Num < 1

		ADD_SYS_MSG_START
		ADD_SYS_MSG �����������������Ʒ����Ԥ��1�����ϱ����ո�λ���ٿ�����
		SET_SYS_MSG_MODE 1 0 0
		MSG_INFO
		return
	endif
		

;	GET_PLAYER_BASE_ATT 121 122 LV2 124
;	GET_PLAYER_BASE_ATT 121 122 LV3 124
	
;	strvar voi Exp_1
;	MUL LV3 @LV3
;	MUL LV3 2
;	VAR Exp_1 @LV3	
;	EXP_LIMIT 1 0 @Exp_1 voi	
	
;	if voi = 0
;		ADD_SYS_MSG_START
;		ADD_SYS_MSG  �㴢��ľ���̫���ˣ������п��ܻῪ�������飬���ֶ��������ٴο�����
;		SET_SYS_MSG_MODE 1 0 0
;		MSG_INFO
;		return
;	ENDIF	

;	ITEM_ADD_ONEDAY 5250322 1
	;ʹ�ô���+1
;	GET_ITEM_ONEDAY_INFO 5250322 1 800 901
	;���ʹ�ô���	
;	if 800 <= 20
	;ʹ��20������  
	;;;;;;;;;;;;;;;;;;;;;;���鲻���
		strvar RetNum retnum1
		DEC_ITEM_NUM_INPAK 5250322 1 RetNum
		DEC_ITEM_NUM_INPAK 5060022 1 retnum1
		
		if RetNum = -1 or retnum1 = -1
			return
		endif	
		
	;	
	;	strvar Ran_1 Ran_2 Ran_3 Ran_ExpMoney Exp Money
;
;		rand Ran_ExpMoney 100
;		if @Ran_ExpMoney < 99
;			MUL LV2 @LV2
;			MUL LV2 2
;			VAR Exp @LV2			
;			ADD_EXP 0 @Exp 1
;			ADD_SYS_MSG_START
;			ADD_SYS_MSG ������$@Exp$�㾭�飬�ٽ�������
;			SET_SYS_MSG_MODE 3 0 0
;			MSG_INFO
;			
;		else
;			MUL LV2 @LV2
;			DEV LV2 5
;			VAR Money @LV2
;			
;			strvar MonRet
;			ADD_MONEY 0 @Money 20 MonRet
;			if MonRet = 0
;				ADD_SYS_MSG_START
;				ADD_SYS_MSG ������$@Money$���ң��ٽ�������
;				SET_SYS_MSG_MODE 3 0 0
;				MSG_INFO
;			endif
;		endif
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;	;�ٷ�֮��ʮ���ʻ����Ʒ��1~2����
;			rand 200 2
;			add 200 100	
;			var 201 100
;				while 201 <= @200
				;����200��ֵ���ѭ��1�λ���2��
;				add 201 1
		strvar LV LV2
		GET_PLAYER_BASE_ATT 121 122 LV 124
		
		
		strvar Ran_2 Ran_3 ret

			if @LV < 20
				rand Ran_2 100		
				if @Ran_2 < 25
					
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5200030 1 1 100 ret
					if ret = -2
						ADD_BIND_ITEM_NUM_INPAK_MONEY 5060200 20 1 100 0
					endif
				endif
				if @Ran_2 >= 25 and @Ran_2 < 50
				
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5200040 1 1 100 ret
					if ret = -2
						ADD_BIND_ITEM_NUM_INPAK_MONEY 5060200 20 1 100 0
					endif
				endif
				if @Ran_2 >= 50 and @Ran_2 < 65
				
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5251001 1 1 100 0
				endif
				if @Ran_2 >= 65 and @Ran_2 < 80
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5200020 1 1 100 ret
					if ret = -2
						ADD_BIND_ITEM_NUM_INPAK_MONEY 5060200 20 1 100 0
					endif
				endif
				if @Ran_2 >= 80
					;��ҫ΢��*20
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5060200 20 1 100 0
				endif
			endif
			
			if @LV >= 20 and @LV < 40
			;20~40 ��
				rand Ran_2 100		
				if @Ran_2 < 25
					
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5200030 1 1 100 ret
					if ret = -2
						ADD_BIND_ITEM_NUM_INPAK_MONEY 5060200 20 1 100 0
					endif
				endif
				if @Ran_2 >= 25 and @Ran_2 < 50
				
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5200040 1 1 100 ret
					if ret = -2
						ADD_BIND_ITEM_NUM_INPAK_MONEY 5060200 20 1 100 0
					endif
				endif
				if @Ran_2 >= 50 and @Ran_2 < 65
				
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5251001 1 1 100 0
				endif
				if @Ran_2 >= 65 and @Ran_2 < 80
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5200020 1 1 100 ret
					if ret = -2
						ADD_BIND_ITEM_NUM_INPAK_MONEY 5060200 20 1 100 0
					endif
				endif
				if @Ran_2 >= 80
					;��ҫ΢��*20
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5060200 20 1 100 0
				endif
			endif
			
			if @LV >= 40 and @LV < 50
			;40~50 ��	
				rand Ran_2 100		
				if @Ran_2 < 12
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5200031 1 1 100 ret
					if ret = -2
						ADD_BIND_ITEM_NUM_INPAK_MONEY 5060201 20 1 100 0
					endif
				endif
				if @Ran_2 >= 12 and @Ran_2 < 24
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5200041 1 1 100 ret
					if ret = -2
						ADD_BIND_ITEM_NUM_INPAK_MONEY 5060201 20 1 100 0
					endif
				endif
				if @Ran_2 >= 24 and @Ran_2 < 37
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5251001 1 1 100 0
				endif
				if @Ran_2 >= 37 and @Ran_2 < 52
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5200020 1 1 100 ret
					if ret = -2
						ADD_BIND_ITEM_NUM_INPAK_MONEY 5060201 20 1 100 0
					endif
				endif
				if @Ran_2 >= 52 and @Ran_2 < 80
					;��ͭ΢��*20
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5060201 20 1 100 0
				endif
				if @Ran_2 >= 80 and @Ran_2 < 100
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5200050 1 1 100 0
				endif
			endif
			
			if @LV >= 50 and @LV < 60
			;50~60 ��	
				rand Ran_2 100		
				if @Ran_2 < 11
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5200031 1 1 100 ret
					if ret = -2
						ADD_BIND_ITEM_NUM_INPAK_MONEY 5060201 20 1 100 0
					endif
				endif
				if @Ran_2 >= 11 and @Ran_2 < 22
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5200041 1 1 100 ret
					if ret = -2
						ADD_BIND_ITEM_NUM_INPAK_MONEY 5060201 20 1 100 0
					endif
				endif
				if @Ran_2 >= 22 and @Ran_2 < 33
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5251001 1 1 100 0
				endif
				if @Ran_2 >= 33 and @Ran_2 < 48
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5200020 1 1 100 ret
					if ret = -2
						ADD_BIND_ITEM_NUM_INPAK_MONEY 5060201 20 1 100 0
					endif
				endif
				if @Ran_2 >= 48 and @Ran_2 < 80
					;��ͭ΢��*20
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5060201 20 1 100 0
				endif
				if @Ran_2 >= 80 and @Ran_2 < 100
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5200050 1 1 100 0
				endif
			endif
			
			if @LV >= 60 and @LV < 70
			;60~70 ��
				rand Ran_2 100		
				if @Ran_2 < 10
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5200032 1 1 100 ret
					if ret = -2
						ADD_BIND_ITEM_NUM_INPAK_MONEY 5060202 20 1 100 0
					endif
				endif
				if @Ran_2 >= 10 and @Ran_2 < 20
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5200042 1 1 100 ret
					if ret = -2
						ADD_BIND_ITEM_NUM_INPAK_MONEY 5060202 20 1 100 0
					endif
				endif
				if @Ran_2 >= 20 and @Ran_2 < 29
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5251001 1 1 100 0
				endif
				if @Ran_2 >= 29 and @Ran_2 < 44
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5200020 1 1 100 ret
					if ret = -2
						ADD_BIND_ITEM_NUM_INPAK_MONEY 5060202 20 1 100 0
					endif
				endif
				if @Ran_2 >= 44 and @Ran_2 < 80
					;���΢��*20
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5060202 20 1 100 0
				endif
				if @Ran_2 >= 80 and @Ran_2 < 100
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5200051 1 1 100 0
				endif
			endif
			
			if @LV >= 70 and @LV < 80
			;70~80 ��
				rand Ran_2 100		
				if @Ran_2 < 10
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5200032 1 1 100 ret
					if ret = -2
						ADD_BIND_ITEM_NUM_INPAK_MONEY 5060202 20 1 100 0
					endif
				endif
				if @Ran_2 >= 10 and @Ran_2 < 20
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5200042 1 1 100 ret
					if ret = -2
						ADD_BIND_ITEM_NUM_INPAK_MONEY 5060202 20 1 100 0
					endif
				endif
				if @Ran_2 >= 20 and @Ran_2 < 27
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5251001 1 1 100 0
				endif
				if @Ran_2 >= 27 and @Ran_2 < 42
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5200020 1 1 100 ret
					if ret = -2
						ADD_BIND_ITEM_NUM_INPAK_MONEY 5060202 20 1 100 0
					endif
				endif
				if @Ran_2 >= 42 and @Ran_2 < 80
					;���΢��*20
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5060202 20 1 100 0
				endif
				if @Ran_2 >= 80 and @Ran_2 < 100
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5200052 1 1 100 0
				endif
			endif

			if @LV >= 80 and @LV < 100
			;80~100 ��
				rand Ran_2 100		
				if @Ran_2 < 15
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5200033 1 1 100 ret
					if ret = -2
						ADD_BIND_ITEM_NUM_INPAK_MONEY 5060203 20 1 100 0
					endif
				endif
				if @Ran_2 >= 15 and @Ran_2 < 30
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5200043 1 1 100 ret
					if ret = -2
						ADD_BIND_ITEM_NUM_INPAK_MONEY 5060203 20 1 100 0
					endif
				endif
				if @Ran_2 >= 30 and @Ran_2 < 35
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5251001 1 1 100 0
				endif
				if @Ran_2 >= 35 and @Ran_2 < 50
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5200020 1 1 100 ret
					if ret = -2
						ADD_BIND_ITEM_NUM_INPAK_MONEY 5060203 20 1 100 0
					endif
				endif
				if @Ran_2 >= 50 and @Ran_2 < 80
					;����΢��*20
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5060203 20 1 100 0
				endif
				if @Ran_2 >= 80 and @Ran_2 < 100
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5200053 1 1 100 0
				endif
			endif
			if @LV >= 100
			;100~120 ��
				rand Ran_2 100		
				if @Ran_2 < 15
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5200033 1 1 100 ret
					if ret = -2
						ADD_BIND_ITEM_NUM_INPAK_MONEY 5060203 20 1 100 0
					endif
				endif
				if @Ran_2 >= 15 and @Ran_2 < 30
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5200043 1 1 100 ret
					if ret = -2
						ADD_BIND_ITEM_NUM_INPAK_MONEY 5060203 20 1 100 0
					endif
				endif
				if @Ran_2 >= 30 and @Ran_2 < 35
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5251001 1 1 100 0
				endif
				if @Ran_2 >= 35 and @Ran_2 < 50
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5200020 1 1 100 ret
					if ret = -2
						ADD_BIND_ITEM_NUM_INPAK_MONEY 5060203 20 1 100 0
					endif
				endif
				if @Ran_2 >= 50 and @Ran_2 < 80
					;����΢��*20
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5060203 20 1 100 0
				endif
				if @Ran_2 >= 80 and @Ran_2 < 100
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5200053 1 1 100 0
				endif
			endif
			
;		endwhile

;	if 800 > 20
;	;ʹ��20������
;		CALL 100
;	endif
endproc		

PROC 100

		strvar LV3
		GET_PLAYER_BASE_ATT 121 122 LV3 124
		rand 100 100
		add 100 100
		if 100 >= 100 and 100 < 199
			;��þ���ֵ���̶���ֵ��
			strvar Exp_20 
			mul LV3 5
			VAR Exp_20 @LV3
			
			ADD_EXP 0 @Exp_20 1
			ADD_SYS_MSG_START
			ADD_SYS_MSG ���ص����ӽ���ʹ���Ѿ�����20�Σ�ֻ�ܻ��$@Exp_20$���顣
			SET_SYS_MSG_MODE 1 0 0
			MSG_INFO
						
			DEC_ITEM_NUM_INPAK 5250322 1 1		
		endif
		if 100 >= 199 and 100 < 200
			;��ð󶨻��ң��̶���ֵ��
			strvar MonRet_1
			ADD_MONEY 0 @LV3 21 MonRet_1
			if MonRet_1 = -1
				ADD_SYS_MSG_START
				ADD_SYS_MSG ����ο��������漴����˰��������������Ҵ����Ѵ����ޣ�����ʧ�ܣ�
				SET_SYS_MSG_MODE 1 0 0
				MSG_INFO
				
				return
			endif
			ADD_SYS_MSG_START
			ADD_SYS_MSG ���ص����ӽ�����ʹ���Ѿ�����20�Σ�ֻ�ܻ��$@LV3$������
			SET_SYS_MSG_MODE 1 0 0
			MSG_INFO
					
			DEC_ITEM_NUM_INPAK 5250322 1 1		
		endif
		
ENDPROC

proc 2
	strvar itnum retitem
	GET_ITEM_NUM_INPAK 5060022 retitem itnum
	if retitem = 0 and itnum = 0

		ADD_SYS_MSG_START
		ADD_SYS_MSG ��û�й���֮Կ���޷��������䣡
		SET_SYS_MSG_MODE 1 0 0
		MSG_INFO
		set_script_int_ret -1		
	endif
endproc 
	

	
	
	
	
	
	
	
	