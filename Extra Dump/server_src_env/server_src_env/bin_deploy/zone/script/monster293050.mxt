;拯救人质
proc 1

	strvar Mid

	var 10 0
	set_mon_var 0 8 @10 1

	
	
endproc 
proc 4
	strvar Mid	
	GET_MON_INFO 0 0 Mid 0 0 0 0 0
	
	get_mon_var 0 8 1 10
	rand 11 50
	add 10 @11
	set_mon_var 0 8 @10 1
	
	if 10 = 3
		rand 20 3
		switch 20
			case 0
	;			mon_speech @Mid 0 1 1 你是来救我们的吗？勇士！囚笼的钥匙在赫巴贡斥候身上！。
			endcase
			case 1
	;			mon_speech @Mid 0 1 1 好心人，一看你就非凡人，拜托你从赫巴贡斥候身上拿到钥匙，还我们自由好吗？
			
			endcase
			case 2
	;			mon_speech @Mid 0 1 1 救救我吧！勇士！我一秒钟也不想呆在这里！
			endcase
		endswitch
	endif
	if 10 > 250
		var 10 0
		set_mon_var 0 8 @10 1
	endif

endproc 