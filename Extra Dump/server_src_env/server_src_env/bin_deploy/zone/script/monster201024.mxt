;祭祖神殿 ,大祭祀—德斯顿.阿萨曼
;出生
proc 1
	strvar memid
	GET_MON_INFO 1 0 memid 0 0 0 0 0
	;TARGET_ADD_STATUS 3 @memid 3028 1 25920000 0 
	;TARGET_ADD_STATUS 3 @memid 3029 1 25920000 0 
	;TARGET_ADD_STATUS 3 @memid 3030 1 25920000 0 
	TARGET_ADD_STATUS 3 @memid 1095 1 25920000 0
	
	strvar mid
	GET_MON_INFO 1 0 mid 0 0 0 0 0
	;boss4的实例id
	VAR 15 @mid
	set_pworld_var 20 32 @15 1
	var 10 0
	set_mon_var 0 32 @10 1
endproc
proc 2
	strvar mid
	GET_MON_INFO 1 0 mid 0 0 0 0 0
;	mon_speech @mid 2 2 1 伟大的神灵啊，以我之灵魂来祭祀，召唤影.杰克.耐罗！
	;boss4的实例id
	VAR 15 0
	set_pworld_var 20 32 @15 1
	;灵魂流开关计数
	get_pworld_var 8 32 1 12
	
	if 12 > 0 
		return
	endif	
	return
	call 10
endproc
proc 4
	return
	;灵魂流开关计数
	get_pworld_var 8 32 1 12
	
	if 12 > 0 
		return
	endif
	get_mon_var 0 32 1 10
	if 10 > 0
		return
	endif
	strvar percent
	GET_MON_HP_PERCENT percent 1
	if percent < 50
		var 10 1
		set_mon_var 0 32 @10 1
		call 10
	endif
endproc
proc 10
	add 12 3
	set_pworld_var 8 32 @12 1
	strvar mid
	GET_MON_INFO 1 0 mid 0 0 0 0 0
	mon_speech @mid 2 2 1 该死的自治联盟，这样下去不是办法，得加快祭祀速度；子民们，伟大的影.杰克.耐罗就快降临，是时候贡献出你们信仰的灵魂了，耐罗大人会拯救你们的。
endproc
