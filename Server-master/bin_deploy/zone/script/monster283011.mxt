;≥Ù—∏√Õ¡˙

proc 2
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
;		GET_PWORLD_INFO 1 10 11
;		if 10 = 1
;			if 50 = 0
;			;∆’Õ®
;				switch Num
;					case 2
;						RUN_TASK_PROGRESS @ID1 3312 1 0
;						RUN_TASK_PROGRESS @ID2 3312 1 0
;					endcase
;					case 3
;						RUN_TASK_PROGRESS @ID1 3312 1 0
;						RUN_TASK_PROGRESS @ID2 3312 1 0
;						RUN_TASK_PROGRESS @ID3 3312 1 0	
;					endcase
;					case 4
;						RUN_TASK_PROGRESS @ID1 3312 1 0
;						RUN_TASK_PROGRESS @ID2 3312 1 0
;						RUN_TASK_PROGRESS @ID3 3312 1 0
;						RUN_TASK_PROGRESS @ID4 3312 1 0
;					endcase
;					case 5
;						RUN_TASK_PROGRESS @ID1 3312 1 0
;						RUN_TASK_PROGRESS @ID2 3312 1 0
;						RUN_TASK_PROGRESS @ID3 3312 1 0
;						RUN_TASK_PROGRESS @ID4 3312 1 0
;						RUN_TASK_PROGRESS @ID5 3312 1 0
;					endcase
;					case 6
;						RUN_TASK_PROGRESS @ID1 3312 1 0
;						RUN_TASK_PROGRESS @ID2 3312 1 0
;						RUN_TASK_PROGRESS @ID3 3312 1 0
;						RUN_TASK_PROGRESS @ID4 3312 1 0
;						RUN_TASK_PROGRESS @ID5 3312 1 0
;						RUN_TASK_PROGRESS @ID6 3312 1 0
;					endcase
;				
;				endswitch
;			else
;				
;				RUN_TASK_PROGRESS 0 3312 1 0
;			
;			endif
;		
;		endif
;	endif

endproc
