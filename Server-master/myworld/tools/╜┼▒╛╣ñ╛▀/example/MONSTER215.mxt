;215的任务Monster狂暴的魔熊的死亡脚本
;事件3:死亡
;调整
PROC 3
    ;狂暴之熊【精英】任务
     GET_TASK_STATUS 215 212
    GET_TASK_VAR 215 1 213
    if 212 = 2 and 213 = 0
    	 is_inteam 6
	if 6 = 0
		GET_TEAM_INFO 1 215 0 990 220 221 222 223 224 225 226 227 228 229 230 231
		GET_TEAM_INFO 2 215 0 992 320 321 322 323 324 325 326 327 328 329 330 331
		GET_TEAM_INFO 2 215 1 993 340 341 342 343 344 345 346 347 348 349 350 351
		if 990 = 0 and 992 = 0 and 993 = 0
			trace_teamtask 215 2 1 0 3055 1 6
			;
			if 320 != 0
				if 221 = 2 and 321 < 6
					add 321 1
					SET_INFO_BYID 1 215 0 @321 @320 0 0 0 0 995					
				endif
				;
				if 221 = 2 and 321 = 6
					if 341 = 0
						SET_INFO_BYID 1 215 1 10 @320 0 0 0 0 996
						SET_INFO_BYID 8 215 0 10 @320 0 0 0 0 997
					endif
				endif
			endif
			;
			if 322 != 0
				if 223 = 2 and 323 < 6
					add 323 1
					SET_INFO_BYID 1 215 0 @323 @322 0 0 0 0 995				
				endif
				;
				if 223 = 2 and 323 = 6
					if 343 = 0
						SET_INFO_BYID 1 215 1 10 @322 0 0 0 0 996
						SET_INFO_BYID 8 215 0 10 @322 0 0 0 0 997
					endif
				endif
			endif
			;
			if 324 != 0 
				if 225 = 2 and 325 < 6
					add 325 1
					SET_INFO_BYID 1 215 0 @325 @324 0 0 0 0 995					
				endif
				;
				if 225 = 2 and 325 = 6 
					if 345 = 0
						SET_INFO_BYID 1 215 1 10 @324 0 0 0 0 996
						SET_INFO_BYID 8 215 0 10 @324 0 0 0 0 997
					endif
				endif
			endif
			;
			if 326 != 0
				if 227 = 2 and 327 < 6
					add 327 1
					SET_INFO_BYID 1 215 0 @327 @326 0 0 0 0 995				
				endif
				;
				if 227 = 2 and 327 = 6
					if 347 = 0
						SET_INFO_BYID 1 215 1 10 @326 0 0 0 0 996
						SET_INFO_BYID 8 215 0 10 @326 0 0 0 0 997
					endif
				endif
			endif
			;
			if 328 != 0
				if 229 = 2 and 329 < 6
					add 329 1
					SET_INFO_BYID 1 215 0 @329 @328 0 0 0 0 995				
				endif
				;
				if 229 = 2 and 329 = 6
					if 349 = 0
						SET_INFO_BYID 1 215 1 10 @328 0 0 0 0 996
						SET_INFO_BYID 8 215 0 10 @328 0 0 0 0 997
					endif
				endif
			endif
		endif
	else 
		get_task_var 215 0 998
    	 	IF 998 < 6
         		ADD 998 1
         		set_task_var 215 0 @998
          		TRACE_TASK 215 3055 @998 6        		
     	 	ENDIF
     	 	;
     	 	if 998 = 6
          		set_task_var 215 1 10
				add_task_hint 215 10
          	endif   
	endif
    endif
    ;GET_TASK_STATUS 215 251
  ;  GET_TASK_VAR 215 0 252
  ;  IF 251 = 2 and 252 < 6
   ;    ADD 252 1
   ;    SET_TASK_VAR 215 0 @252
    ;   TRACE_TASK 215 3055 @252 6
   ; ENDIF
ENDPROC