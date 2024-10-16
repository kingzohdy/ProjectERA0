

proc 3

	strvar num1 ret
	BCT_GET_CURR_LAYER num1 ret
	
	if ret = 0 and num1 = 1
		GET_MAP_NPC_ID 304047 50 0 
		mon_speech @50 0 0 1 既然大地之心的秘密已解开，我也暂时离开这里了。
		
		strvar num
		get_pworld_var 100 8 1 num
		var num 1
		set_pworld_var 100 8 @num 10
		
	endif
endproc 

