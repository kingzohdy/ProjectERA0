;怒火任务
proc 1

	strvar Mid

	var 10 0
	set_mon_var 0 8 @10 1

	GET_MON_INFO 0 0 Mid 0 0 0 0 0
	
	mon_speech @Mid 0 1 1 发生了什么？
	
	
endproc 
