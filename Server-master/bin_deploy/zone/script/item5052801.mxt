



proc 1
	strvar ret pid
	HAS_STATUS 1005 ret
	get_player_id 1 pid
	
	if ret = 0
		TARGET_ADD_STATUS 1 @pid 1005 4 0 0
		TARGET_ADD_STATUS 1 @pid 1005 1 0 0
		
	else
		TARGET_ADD_STATUS 1 @pid 1005 1 25920000 0
	endif	

endproc 


