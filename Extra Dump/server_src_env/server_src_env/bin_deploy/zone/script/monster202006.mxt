
proc 1
	strvar Mid
	GET_MON_INFO 1 1 Mid 1 1 1 1 1 
	TARGET_ADD_STATUS 3 @Mid 1009 3 5 1

endproc
proc 2

	strvar Num
	get_pworld_var 5 8 1 Num
	add Num 1
	set_pworld_var 5 8 @Num 1

endproc 