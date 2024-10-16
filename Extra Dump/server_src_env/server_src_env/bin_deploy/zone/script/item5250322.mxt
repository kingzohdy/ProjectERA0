;邪念体宝箱

	;ADD_SYS_MSG_START
	;ADD_SYS_MSG 
	;SET_SYS_MSG_MODE 0 0 0
	;MSG_INFO
	

proc 1
	strvar itnum retitem
	GET_ITEM_NUM_INPAK 5060022 retitem itnum
	if retitem = 0 and itnum = 0
		ADD_SYS_MSG_START
		ADD_SYS_MSG 你没有诡秘之钥，无法开启宝箱！
		SET_SYS_MSG_MODE 1 0 0
		MSG_INFO
		return
	endif
	strvar Num
	GET_PACK_SPACE 0 Num
	if @Num < 1

		ADD_SYS_MSG_START
		ADD_SYS_MSG 开启宝箱会随机获得物品，请预留1个以上背包空格位置再开启！
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
;		ADD_SYS_MSG  你储存的经验太多了，宝箱有可能会开启到经验，请手动升级后再次开启！
;		SET_SYS_MSG_MODE 1 0 0
;		MSG_INFO
;		return
;	ENDIF	

;	ITEM_ADD_ONEDAY 5250322 1
	;使用次数+1
;	GET_ITEM_ONEDAY_INFO 5250322 1 800 901
	;获得使用次数	
;	if 800 <= 20
	;使用20次以内  
	;;;;;;;;;;;;;;;;;;;;;;经验不获得
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
;			ADD_SYS_MSG 你获得了$@Exp$点经验，再接再厉！
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
;				ADD_SYS_MSG 你获得了$@Money$银币，再接再厉！
;				SET_SYS_MSG_MODE 3 0 0
;				MSG_INFO
;			endif
;		endif
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;	;百分之五十几率获得物品（1~2个）
;			rand 200 2
;			add 200 100	
;			var 201 100
;				while 201 <= @200
				;根据200的值随机循环1次或者2次
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
					;黑耀微粒*20
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5060200 20 1 100 0
				endif
			endif
			
			if @LV >= 20 and @LV < 40
			;20~40 级
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
					;黑耀微粒*20
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5060200 20 1 100 0
				endif
			endif
			
			if @LV >= 40 and @LV < 50
			;40~50 级	
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
					;炽铜微粒*20
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5060201 20 1 100 0
				endif
				if @Ran_2 >= 80 and @Ran_2 < 100
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5200050 1 1 100 0
				endif
			endif
			
			if @LV >= 50 and @LV < 60
			;50~60 级	
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
					;炽铜微粒*20
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5060201 20 1 100 0
				endif
				if @Ran_2 >= 80 and @Ran_2 < 100
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5200050 1 1 100 0
				endif
			endif
			
			if @LV >= 60 and @LV < 70
			;60~70 级
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
					;坚钢微粒*20
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5060202 20 1 100 0
				endif
				if @Ran_2 >= 80 and @Ran_2 < 100
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5200051 1 1 100 0
				endif
			endif
			
			if @LV >= 70 and @LV < 80
			;70~80 级
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
					;坚钢微粒*20
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5060202 20 1 100 0
				endif
				if @Ran_2 >= 80 and @Ran_2 < 100
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5200052 1 1 100 0
				endif
			endif

			if @LV >= 80 and @LV < 100
			;80~100 级
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
					;恒钻微粒*20
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5060203 20 1 100 0
				endif
				if @Ran_2 >= 80 and @Ran_2 < 100
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5200053 1 1 100 0
				endif
			endif
			if @LV >= 100
			;100~120 级
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
					;恒钻微粒*20
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5060203 20 1 100 0
				endif
				if @Ran_2 >= 80 and @Ran_2 < 100
					ADD_BIND_ITEM_NUM_INPAK_MONEY 5200053 1 1 100 0
				endif
			endif
			
;		endwhile

;	if 800 > 20
;	;使用20次以上
;		CALL 100
;	endif
endproc		

PROC 100

		strvar LV3
		GET_PLAYER_BASE_ATT 121 122 LV3 124
		rand 100 100
		add 100 100
		if 100 >= 100 and 100 < 199
			;获得经验值（固定数值）
			strvar Exp_20 
			mul LV3 5
			VAR Exp_20 @LV3
			
			ADD_EXP 0 @Exp_20 1
			ADD_SYS_MSG_START
			ADD_SYS_MSG 诡秘的箱子今天使用已经超过20次，只能获得$@Exp_20$经验。
			SET_SYS_MSG_MODE 1 0 0
			MSG_INFO
						
			DEC_ITEM_NUM_INPAK 5250322 1 1		
		endif
		if 100 >= 199 and 100 < 200
			;获得绑定货币（固定数值）
			strvar MonRet_1
			ADD_MONEY 0 @LV3 21 MonRet_1
			if MonRet_1 = -1
				ADD_SYS_MSG_START
				ADD_SYS_MSG 您这次开启箱子随即获得了绑银，但您的银币储存已达上限，开启失败！
				SET_SYS_MSG_MODE 1 0 0
				MSG_INFO
				
				return
			endif
			ADD_SYS_MSG_START
			ADD_SYS_MSG 诡秘的箱子今天已使用已经超过20次，只能获得$@LV3$绑银。
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
		ADD_SYS_MSG 你没有诡秘之钥，无法开启宝箱！
		SET_SYS_MSG_MODE 1 0 0
		MSG_INFO
		set_script_int_ret -1		
	endif
endproc 
	

	
	
	
	
	
	
	
	