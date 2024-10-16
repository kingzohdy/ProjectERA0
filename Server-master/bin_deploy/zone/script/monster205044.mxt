;;;;;;军需官

proc 1
	strvar Co_1
	var Co_1 0
	set_mon_var 0 32 @Co_1 1
endproc 

proc 2

	strvar X Y Mid_1 HpRet
	GET_MON_INFO 1 1 Mid_1 31 32 33 X Y
	GET_MON_HP_PERCENT @Mid_1 HpRet
	if HpRet < 50
		NEW_LIFE_MON 205046 1 @X @Y 0 0 0 0 60 0 0
	endif
	NEW_MON 205044 1 19700 7900 0 138 0 0 0
endproc
proc 3
	DESTROY_MON 1
endproc 
proc 4

	strvar Fight Mid Mx My ReMid Co_1 StaRet

	IS_MON_FIGHTING 1 Fight
	GET_MON_INFO 1 1 Mid 31 32 33 Mx My
	
	GET_ID_MON_STATUS_INFO @Mid 1001 StaRet 0 0 0 0
	;;;;是否有某种状态
	if Fight = 1
		if StaRet = -1
			TARGET_ADD_STATUS 3 @Mid 1001 10 1000000 1
			NEW_LIFE_MON 205045 1 @Mx @My 0 0 0 0 0 0 ReMid
			get_mon_var 0 32 1 Co_1		
			var Co_1 @ReMid
			set_mon_var 0 32 @Co_1 1
		endif
	endif
	
	if Fight = 0
		if StaRet = 0
			TARGET_ADD_STATUS 3 @Mid 1001 10 0 1
			get_mon_var 0 32 1 Co_1	
			DEL_ID_MON @Co_1 1		
		endif
		
	endif

endproc 








