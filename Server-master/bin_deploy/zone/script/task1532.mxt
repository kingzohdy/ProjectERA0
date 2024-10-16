
proc 1

	var 20 0
	set_task_var 1532 4 8 @20 1
	var 21 0
	set_task_var 1532 5 8 @21 1


endproc 
proc 3

	GET_ITEM_NUM_INPAK 5300376 0 52
	dec_item_num_inpak 5300376 @52 0
endproc
proc 4
	GET_ITEM_NUM_INPAK 5300376 0 52
	dec_item_num_inpak 5300376 @52 0
endproc 
proc 5

	GET_ITEM_NUM_INPAK 5300376 0 52
	dec_item_num_inpak 5300376 @52 0
	
endproc 
proc 6
	
	get_task_var 1532 4 8 1 20
	if 20 = 1 
		
		get_task_var 1532 5 8 1 21
		add 21 1
		set_task_var 1532 5 8 @21 1
		if 21 = 63
			TASK_FAIL 1532 1
			
			var 20 0
			set_task_var 1532 4 8 @20 1
		endif
	
	endif
endproc 

















