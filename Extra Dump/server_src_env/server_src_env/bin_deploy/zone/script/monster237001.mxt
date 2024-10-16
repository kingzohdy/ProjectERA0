;鲁莽

proc 2


	
	STRVAR ID
	
			GET_MON_INFO 0 0 0 0 0 0 10 11
			new_mon 293078 1 @10 @11 0 0 0 0 ID
			
			get_pworld_var 16 32 1 32
			var 32 293078
			set_pworld_var 16 32 @32 1
			
			
			MON_SET_HP -5 @ID 0 
	
			
endproc  
