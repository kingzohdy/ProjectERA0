;护送废物
proc 1
	var 20 0
	set_mon_var 0 8 @20 1
	var 21 0
	set_mon_var 1 8 @21 1
	var 22 0
	set_mon_var 2 8 @22 1
	
	strvar Mid
	GET_MON_INFO 1 1 Mid 1 1 1 1 1	
	TARGET_ADD_STATUS 3 @Mid 1009 2 4 1

endproc 

proc 2
	get_mon_var 0 8 1 20
	if 20 = 1
		task_fini 1823 1			
	else
		TASK_FAIL 1823 1
	endif
endproc 

proc 3

	get_mon_var 0 8 1 20
	var 20 1
	set_mon_var 0 8 @20 1
	
	strvar Mid
	GET_MON_INFO 1 1 Mid 1 1 1 1 1
	MON_SPEECH @Mid 1 2 1 我想你应该还有很多事情要忙，就到这里吧，其他的自己来就可以了。
	
	del_mon 1		
endproc 

proc 4
	get_mon_var 1 8 1 21
	if 21 = 0
		
		get_mon_var 2 8 1 22
		add 22 1
		set_mon_var 2 8 @22 1
		if 22 = 2
			strvar Mid
			GET_MON_INFO 1 1 Mid 1 1 1 1 1
			MON_SPEECH @Mid 1 2 1 这一段路程虽然不算太远，但充满危险，你一定要紧紧跟着我！
			var 21 1
			set_mon_var 1 8 @21 1
		endif
	endif

endproc 

proc 5
	strvar Num Mid X Y
	GET_MON_INFO 0 0 Mid 0 0 0 X Y
	GET_MON_PATROLIDX @Mid Num 0
	if Num = 2
		MON_SPEECH @Mid 1 2 1 我似乎感觉到了危险的气息，提高警惕！
	endif
	if Num = 3
		MON_CALL_ENEMY_MON 261030 2 60 0 @X @Y 200 0 0 0 40 0 0		
	endif
	if Num = 5
		MON_SPEECH @Mid 1 2 1 我似乎感觉到了危险的气息，提高警惕！
	endif
	if Num = 6
		MON_CALL_ENEMY_MON 261030 2 60 0 @X @Y 200 0 0 0 40 0 0		
	endif
	
endproc






