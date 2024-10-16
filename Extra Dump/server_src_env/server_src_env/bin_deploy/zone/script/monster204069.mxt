;;;;;ÊÜÉËµÄÊ¿±ø

proc 1
	strvar Mid
	GET_MON_INFO 1 1 Mid 31 32 33 1 1
	TARGET_ADD_STATUS 3 @Mid 1001 10 1000 1
endproc 

proc 2
	TASK_FAIL 1966 100
endproc 

proc 4
	strvar Hp
	GET_MON_INFO 0 0 0 Hp 0 0 0 0 
	
	
	if Hp < 100
		del_mon 1
	endif
endproc 