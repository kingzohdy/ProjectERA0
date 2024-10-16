
proc 1
	var 10 0
	set_task_var 1316 0 8 @10 1
;	var 11 0
;	set_task_var 1316 1 32 @11 1
	var 12 0
	set_task_var 1316 2 8 @12 1


endproc
proc 2
	GET_PLAYER_ID 1 41
	TARGET_ADD_STATUS 1 @41 1219 1 0 1
endproc

proc 4
	GET_ITEM_NUM_INPAK 5300385 0 20
	GET_ITEM_NUM_INPAK 5300386 0 21
	if 20 > 0 
		DEC_ITEM_NUM_INPAK 5300385 @20 0
	endif
	if 21 > 0
		
		DEC_ITEM_NUM_INPAK 5300386 @21 0
	endif

endproc
proc 5
;放弃任务

;	get_task_var 1316 1 32 1 11
;	DEL_ID_MON @11 0
	HAS_STATUS 1219 70
	if 70 = 0
		GET_PLAYER_ID 1 30
		TARGET_ADD_STATUS 1 @30 1219 1 1 1	
	
	endif
	
	GET_ITEM_NUM_INPAK 5300385 0 20
	GET_ITEM_NUM_INPAK 5300386 0 21
	if 20 > 0 
		DEC_ITEM_NUM_INPAK 5300385 @20 0
	endif
	if 21 > 0
		
		DEC_ITEM_NUM_INPAK 5300386 @21 0
	endif
endproc

proc 6

		get_task_var 1316 2 8 1 12
		if 12 = 0 
			get_task_var 1316 0 8 1 10
			add 10 1
			set_task_var 1316 0 8 @10 1
			if 10 = 5 
			;刷出笼子
;				call_mon 1 293088 1000 300 9400 15700 30			
;				get_task_var 1316 1 32 1 11
;				var 11 @30
;				set_task_var 1316 1 32 @11 1
				
				var 12 1
				get_task_var 1316 2 8 1 12

			endif
		endif

endproc

