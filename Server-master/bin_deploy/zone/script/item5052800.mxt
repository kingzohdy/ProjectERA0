;���ﱦ��

	;ADD_SYS_MSG_START
	;ADD_SYS_MSG 
	;SET_SYS_MSG_MODE 0 0 0
	;MSG_INFO
	

proc 1

	strvar Num RetX
	

	GET_PACK_SPACE 0 Num
	if @Num < 2

		ADD_SYS_MSG_START
		ADD_SYS_MSG �����������������Ʒ����Ԥ��2�������ո�λ���ٿ�����
		SET_SYS_MSG_MODE 1 0 0
		MSG_INFO
		return
	endif

	
;	strvar voi Exp_1
;	MUL LV3 @LV3
;	MUL LV3 2
;	VAR Exp_1 @LV3	
;	EXP_LIMIT 1 0 @Exp_1 voi	
	
;	if voi = 0
;		ADD_SYS_MSG_START
;		ADD_SYS_MSG  �㴢��ľ���̫���ˣ������п��ܻῪ��������Ŷ�����ֶ��������ٴο�����
;		SET_SYS_MSG_MODE 1 0 0
;		MSG_INFO
;		return
;	ENDIF	
		

	ITEM_ADD_ONEDAY 5052800 1
	;ʹ�ô���+1
	GET_ITEM_ONEDAY_INFO 5052800 1 800 901
	;���ʹ�ô���	
	if 800 <= 20
	;ʹ��20������  
		DEC_ITEM_NUM_INPAK 5052800 1 RetX
		if RetX = -1
			return
		endif
	
	;	strvar Ran_1 Ran_2 Ran_3 Ran_ExpMoney Exp Money
	;	rand Ran_ExpMoney 100
	;	if @Ran_ExpMoney < 99
	;		MUL LV2 @LV2
	;		MUL LV2 2
	;		VAR Exp @LV2			
	;		ADD_EXP 0 @Exp 1
	;		ADD_SYS_MSG_START
	;		ADD_SYS_MSG ������$@Exp$�㾭�飬�ٽ�������
	;		SET_SYS_MSG_MODE 3 0 0
	;		MSG_INFO
	;		
	;	else
	;		MUL LV2 @LV2
	;		DEV LV2 5
	;		VAR Money @LV2
	;		
	;		strvar MonRet
	;		ADD_MONEY 0 @Money 20 MonRet
	;		if MonRet = 0
	;			ADD_SYS_MSG_START
	;			ADD_SYS_MSG ������$@Money$���ң��ٽ�������
	;			SET_SYS_MSG_MODE 3 0 0
	;			MSG_INFO
	;		endif
	;	endif
		
		;�ٷ�֮��ʮ���ʻ����Ʒ��1~2����
;			rand 200 2
;			add 200 100	
;			var 201 100
;				while 201 <= @200
;				;����200��ֵ���ѭ��1�λ���2��
;				add 201 1
;			strvar diaoluo1 
;			var diaoluo1 0
;			rand diaoluo1 100
;			if diaoluo1 < 5
;				ADD_ITEM_NUM_INPAK 5233000 1 1 0
;			endif
;			if diaoluo1 >= 5 and diaoluo1 < 20
;				ADD_ITEM_NUM_INPAK 5233500 1 1 0
;			endif
;			if diaoluo1 >= 20 and diaoluo1 < 35
;				ADD_ITEM_NUM_INPAK 5200020 1 1 0
;			endif
;			if diaoluo1 >= 35 and diaoluo1 < 45
;				ADD_ITEM_NUM_INPAK 5234000 1 1 0
;			endif
;			if diaoluo1 >= 45 and diaoluo1 < 60
;				ADD_ITEM_NUM_INPAK 5230000 1 1 0
;			endif
;			if diaoluo1 >= 60 and diaoluo1 < 80
;				ADD_ITEM_NUM_INPAK 5050000 1 1 0
;			endif
;			if diaoluo1 >= 80 and diaoluo1 < 100
;				ADD_ITEM_NUM_INPAK 5055100 1 1 0
;			endif
					
	strvar LV LV2
	GET_PLAYER_BASE_ATT 121 122 LV 124
			strvar Ran_2
			if @LV < 30
			;20�� 30 ��
				rand Ran_2 2
				if @Ran_2 = 0
					ADD_BIND_ITEM_NUM_INPAK 5000001 1 1 0
				endif
				if @Ran_2 = 1
					ADD_BIND_ITEM_NUM_INPAK 5001001 1 1 0
				endif
			endif
			if @LV >= 30 and @LV < 40
			;����30��
				rand Ran_2 100
				if @Ran_2 < 30
					ADD_BIND_ITEM_NUM_INPAK 5109501 3 1 0
				endif
				if @Ran_2 >= 30 and @Ran_2 < 65
					ADD_BIND_ITEM_NUM_INPAK 5101001 3 1 0
				endif
				if @Ran_2 >= 65 and @Ran_2 < 80
					ADD_BIND_ITEM_NUM_INPAK 5000001 1 1 0
				endif
				if @Ran_2 >= 80 and @Ran_2 < 95
					ADD_BIND_ITEM_NUM_INPAK 5001001 1 1 0
				endif
				if @Ran_2 >= 95 and @Ran_2 < 100
					ADD_BIND_ITEM_NUM_INPAK 5200020 1 1 0
				endif
			endif
			if @LV >= 40 and @LV < 50
			;����40��
				rand Ran_2 100
				if @Ran_2 < 30
					ADD_BIND_ITEM_NUM_INPAK 5109502 3 1 0
				endif
				if @Ran_2 >= 30 and @Ran_2 < 75
					ADD_BIND_ITEM_NUM_INPAK 5101001 3 1 0
				endif
				if @Ran_2 >= 75 and @Ran_2 < 85
					ADD_BIND_ITEM_NUM_INPAK 5000002 1 1 0
				endif
				if @Ran_2 >= 85 and @Ran_2 < 95
					ADD_BIND_ITEM_NUM_INPAK 5001002 1 1 0
				endif
				if @Ran_2 >= 95 and @Ran_2 < 100
					ADD_BIND_ITEM_NUM_INPAK 5200020 1 1 0
				endif
			endif
			if @LV >= 50 and @LV < 60
			;����50��
				rand Ran_2 100
				if @Ran_2 < 30
					ADD_BIND_ITEM_NUM_INPAK 5109503 3 1 0
				endif
				if @Ran_2 >= 30 and @Ran_2 < 75
					ADD_BIND_ITEM_NUM_INPAK 5101002 5 1 0
				endif
				if @Ran_2 >= 75 and @Ran_2 < 85
					ADD_BIND_ITEM_NUM_INPAK 5000002 1 1 0
				endif
				if @Ran_2 >= 85 and @Ran_2 < 95
					ADD_BIND_ITEM_NUM_INPAK 5001002 1 1 0
				endif
				if @Ran_2 >= 95 and @Ran_2 < 100
					ADD_BIND_ITEM_NUM_INPAK 5200020 1 1 0
				endif
				
			endif
			
			if @LV >= 60 and @LV < 70
			;����60
				rand Ran_2 100
				if @Ran_2 < 30
					ADD_BIND_ITEM_NUM_INPAK 5109504 3 1 0
				endif
				if @Ran_2 >= 30 and @Ran_2 < 75
					ADD_BIND_ITEM_NUM_INPAK 5101002 5 1 0
				endif
				if @Ran_2 >= 75 and @Ran_2 < 85
					ADD_BIND_ITEM_NUM_INPAK 5000003 1 1 0
				endif
				if @Ran_2 >= 85 and @Ran_2 < 95
					ADD_BIND_ITEM_NUM_INPAK 5001003 1 1 0
				endif
				if @Ran_2 >= 95 and @Ran_2 < 100
					ADD_BIND_ITEM_NUM_INPAK 5200020 1 1 0
				endif
			endif
			if @LV >= 70 and @LV <= 80
			;����70��
				rand Ran_2 100
				if @Ran_2 < 30
					ADD_BIND_ITEM_NUM_INPAK 5109505 3 1 0
				endif
				if @Ran_2 >= 30 and @Ran_2 < 75
					ADD_BIND_ITEM_NUM_INPAK 5101003 5 1 0
				endif
				if @Ran_2 >= 75 and @Ran_2 < 85
					ADD_BIND_ITEM_NUM_INPAK 5000003 1 1 0
				endif
				if @Ran_2 >= 85 and @Ran_2 < 95
					ADD_BIND_ITEM_NUM_INPAK 5001003 1 1 0
				endif
				if @Ran_2 >= 95 and @Ran_2 < 100
					ADD_BIND_ITEM_NUM_INPAK 5200020 1 1 0
				endif
			endif
			if @LV > 80 and @LV <= 90
			;����80��
				rand Ran_2 100
				if @Ran_2 < 30
					ADD_BIND_ITEM_NUM_INPAK 5109506 3 1 0
				endif
				if @Ran_2 >= 30 and @Ran_2 < 75
					ADD_BIND_ITEM_NUM_INPAK 5101003 5 1 0
				endif
				if @Ran_2 >= 75 and @Ran_2 < 85
					ADD_BIND_ITEM_NUM_INPAK 5000004 1 1 0
				endif
				if @Ran_2 >= 85 and @Ran_2 < 95
					ADD_BIND_ITEM_NUM_INPAK 5001004 1 1 0
				endif
				if @Ran_2 >= 95 and @Ran_2 < 100
					ADD_BIND_ITEM_NUM_INPAK 5200020 1 1 0
				endif
			endif
			if @LV >= 90 and @LV < 100
			;����90��
				rand Ran_2 100
				if @Ran_2 < 30
					ADD_BIND_ITEM_NUM_INPAK 5109507 3 1 0
				endif
				if @Ran_2 >= 30 and @Ran_2 < 75
					ADD_BIND_ITEM_NUM_INPAK 5101004 5 1 0
				endif
				if @Ran_2 >= 75 and @Ran_2 < 85
					ADD_BIND_ITEM_NUM_INPAK 5000004 1 1 0
				endif
				if @Ran_2 >= 85 and @Ran_2 < 95
					ADD_BIND_ITEM_NUM_INPAK 5001004 1 1 0
				endif
				if @Ran_2 >= 95 and @Ran_2 < 100
					ADD_BIND_ITEM_NUM_INPAK 5200020 1 1 0
				endif
			endif
			if @LV >= 100 and @LV < 110
			;����100��
				rand Ran_2 100
				if @Ran_2 < 30
					ADD_BIND_ITEM_NUM_INPAK 5109508 3 1 0
				endif
				if @Ran_2 >= 30 and @Ran_2 < 75
					ADD_BIND_ITEM_NUM_INPAK 5101004 5 1 0
				endif
				if @Ran_2 >= 75 and @Ran_2 < 85
					ADD_BIND_ITEM_NUM_INPAK 5000005 1 1 0
				endif
				if @Ran_2 >= 85 and @Ran_2 < 95
					ADD_BIND_ITEM_NUM_INPAK 5001005 1 1 0
				endif
				if @Ran_2 >= 95 and @Ran_2 < 100
					ADD_BIND_ITEM_NUM_INPAK 5200020 1 1 0
				endif
			endif
			if @LV >= 110 and @LV <= 120
			;����110��
				rand Ran_2 100
				if @Ran_2 < 30
					ADD_BIND_ITEM_NUM_INPAK 5109509 3 1 0
				endif
				if @Ran_2 >= 30 and @Ran_2 < 75
					ADD_BIND_ITEM_NUM_INPAK 5101004 5 1 0
				endif
				if @Ran_2 >= 75 and @Ran_2 < 85
					ADD_BIND_ITEM_NUM_INPAK 5000005 1 1 0
				endif
				if @Ran_2 >= 85 and @Ran_2 < 95
					ADD_BIND_ITEM_NUM_INPAK 5001005 1 1 0
				endif
				if @Ran_2 >= 95 and @Ran_2 < 100
					ADD_BIND_ITEM_NUM_INPAK 5200020 1 1 0
				endif
			endif
		endif
	if 800 > 20
	;ʹ��20������
		CALL 100
	endif
endproc		

PROC 100
	strvar rad
	rand rad 10
	if rad < 6
	;������
		strvar LV3 Ret voi
		GET_PLAYER_BASE_ATT 121 122 LV3 124
		mul LV3 @LV3
		
		EXP_LIMIT 1 0 @LV3 voi	
		if voi = 1
			DEC_ITEM_NUM_INPAK 5052800 1 Ret	
			if Ret != -1
				ADD_EXP 0 @LV3 1
				ADD_SYS_MSG_START
				ADD_SYS_MSG ���ﱦ��ʹ���Ѿ�����20�Σ�ֻ�ܻ��$@LV3$���顣
				SET_SYS_MSG_MODE 4 0 0
				MSG_INFO
			endif
		else
			ADD_SYS_MSG_START
			ADD_SYS_MSG  �㴢��ľ���̫���ˣ������п��ܻῪ��������Ŷ�����ֶ��������ٴο�����
			SET_SYS_MSG_MODE 4 0 0
			MSG_INFO
			return
		
		endif
	endif
	if rad >= 6 and rad < 10
	;��Ǯ
		strvar lv MonRet_1 ret
		GET_PLAYER_BASE_ATT 0 0 lv 0
		mul lv @lv
		dev lv 10
			DEC_ITEM_NUM_INPAK 5052800 1 ret
			if ret = 0
				ADD_MONEY 0 @lv 21 MonRet_1
				if MonRet_1 = -1
					ADD_SYS_MSG_START
					ADD_SYS_MSG ����ο��������������˰����������İ��������Ѵ����ޣ�����ʧ�ܣ�
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
					return
				endif
			endif
	endif
		
		
		
		
		
		
;		rand 100 100
;		add 100 100
;		if 100 >= 100 and 100 < 199
;			;��þ���ֵ���̶���ֵ��
;			strvar Exp_20 
;			mul LV3 5
;			VAR Exp_20 @LV3
;			
;			ADD_EXP 0 @Exp_20 1
;			ADD_SYS_MSG_START
;			ADD_SYS_MSG ���ﱦ��ʹ���Ѿ�����20�Σ�ֻ�ܻ��$@Exp_20$���顣
;			SET_SYS_MSG_MODE 1 0 0
;			MSG_INFO
;			
;			DEC_ITEM_NUM_INPAK 5052800 1 1
;								
;		endif
;		if 100 >= 199 and 100 < 200
;			;��ð󶨻��ң��̶���ֵ��
;			strvar MonRet_1
;			ADD_MONEY 0 @LV3 20 MonRet_1
;			if MonRet_1 = -1
;				ADD_SYS_MSG_START
;				ADD_SYS_MSG ����ο������������������ң����������Ҵ����Ѵ����ޣ�����ʧ�ܣ�
;				SET_SYS_MSG_MODE 1 0 0
;				MSG_INFO
;				return
;			endif
;			ADD_SYS_MSG_START
;			ADD_SYS_MSG ���ﱦ��ʹ���Ѿ�����20�Σ�ֻ�ܻ��$@LV3$���ҡ�
;			SET_SYS_MSG_MODE 1 0 0
;			MSG_INFO
;				
;			DEC_ITEM_NUM_INPAK 5052800 1 1	
;		endif
		
ENDPROC


	