;ÂÌµ° Áúµ°¶¨Ê±
proc 1
    var 10 0
	set_mon_var 0 32 @10 1
endproc
proc 4
	
	get_mon_var 0 32 1 10
	add 10 1
	set_mon_var 0 32 @10 1
	
	;Áúµ°ÆÆÁÑ£¬²úÉúÑ¸ÃÍÁú
	if 10 = 20
	DESTROY_MON 1
	get_mon_info 1 283002 0 0 0 0 100 101
	new_mon 283012 1 @100 @101 5 0 0 0 1
	endif
	
	
endproc

;proc 2
;	strvar ID1 
;	STRVAR ID2
;	STRVAR ID3
;	STRVAR ID4
;	STRVAR ID5
;	STRVAR ID6
;	strvar Num
;	
;	GET_TEAM_PLAYER_ID 50 0 Num ID1 ID2 ID3 ID4 ID5 ID6
;
;
;			
;	GET_PWORLD_INFO 1 10 11
;	if @10 = 1
;		if @50 = 0
;			
;				switch @Num
;				
;					case 2
;					
;						RUN_TASK_PROGRESS @ID1 3311 1 0
;						RUN_TASK_PROGRESS @ID2 3311 1 9
;		
;					endcase
;					case 3
;						RUN_TASK_PROGRESS @ID1 3311 1 0
;						RUN_TASK_PROGRESS @ID2 3311 1 0
;						RUN_TASK_PROGRESS @ID3 3311 1 0	
;					endcase
;					case 4
;						RUN_TASK_PROGRESS @ID1 3311 1 0
;						RUN_TASK_PROGRESS @ID2 3311 1 0
;						RUN_TASK_PROGRESS @ID3 3311 1 0
;						RUN_TASK_PROGRESS @ID4 3311 1 0
;					endcase
;					case 5
;						RUN_TASK_PROGRESS @ID1 3311 1 0
;						RUN_TASK_PROGRESS @ID2 3311 1 0
;						RUN_TASK_PROGRESS @ID3 3311 1 0
;						RUN_TASK_PROGRESS @ID4 3311 1 0
;						RUN_TASK_PROGRESS @ID5 3311 1 0
;					endcase
;					case 6
;						RUN_TASK_PROGRESS @ID1 3311 1 0
;						RUN_TASK_PROGRESS @ID2 3311 1 0
;						RUN_TASK_PROGRESS @ID3 3311 1 0
;						RUN_TASK_PROGRESS @ID4 3311 1 0
;						RUN_TASK_PROGRESS @ID5 3311 1 0
;						RUN_TASK_PROGRESS @ID6 3311 1 0
;					endcase
;				
;				endswitch
;
;		else	
;			RUN_TASK_PROGRESS 0 3311 1 0
;		endif
;	endif	
;			
;endproc
