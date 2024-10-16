;ÀÇ


proc 2

		strvar roc
		get_pworld_var 101 8 1 roc
		var roc 2
		set_pworld_var 101 8 @roc 1
		;¼ÆÊ±Æ÷¹éÁã
		get_pworld_var 104 8 1 44
		var 44 0
		set_pworld_var 104 8 @44 1
		
endproc 