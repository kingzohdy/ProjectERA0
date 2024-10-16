
PROC 1	
	;boss 是否死亡
	set_pworld_var 0 32 0 0
	;倒计时2分钟
	set_pworld_var 4 32 0 0
ENDPROC

;关闭事件
proc 2
	
endproc

;2秒一次
PROC 3
	get_pworld_var 0 32 10 11
	if @10 = 0 and @11 = 1
		get_pworld_var 4 32 12 13
		add 13 1
		set_pworld_var 4 32 @13 12
		
		DELETE_MON_BY_DEFID 207213 0
		DELETE_MON_BY_DEFID 207214 0
	endif
	
	get_pworld_var 4 32 14 15
	;2分钟关闭副本
	if @14 = 0 and @15 >= 60
		PWORLD_OVER	
		COMPEL_CLOSE_PWORLD 1
	endif
ENDPROC

PROC 4
	
ENDPROC
