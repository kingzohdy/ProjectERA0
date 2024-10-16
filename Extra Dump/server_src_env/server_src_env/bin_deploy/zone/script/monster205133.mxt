;蝙蝠


proc 1

	call 100
	
	
endproc



proc 2

		;刷怪的标记
		strvar roc
		get_pworld_var 101 8 1 roc
		var roc 57
		set_pworld_var 101 8 @roc 1
		;计时器归零
		get_pworld_var 104 8 1 44
		var 44 0
		set_pworld_var 104 8 @44 1
	
endproc 

proc 100
	;计时器归零
		get_pworld_var 104 8 1 44
		var 44 0
		set_pworld_var 104 8 @44 1
	
endproc 


