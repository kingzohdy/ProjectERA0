;������ ��ͨ

proc 2

	TASK_STAT 3313 1 20 
	if 20 = 2
		is_in_pworld 211 0 0 10
		if 10 = 0
		;��ͨ
			RUN_TASK_PROGRESS 0 3313 1 0
		
		
		endif
	endif
	
	task_stat 3314 1 20
	if 20 = 2
		is_in_pworld 211 0 0 10
		if 10 = 1
		;���� 8����
			RUN_TASK_PROGRESS 0 3314 1 0
		
		
		endif		
	endif
	
endproc
