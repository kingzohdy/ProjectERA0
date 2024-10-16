;–°≈Û”— -3
proc 2
	strvar sta1 sta hpvar
	GET_MON_INFO 0 0 0 hpvar 0 0 0 0
	if @hpvar = 0
		TASK_STAT 4615 12 sta
		if @sta = 2
			GET_PWORLD_VAR 8 32 20 sta1
			sub sta1 3
			if @sta1 < 0
				var sta1 0
			endif
			set_pworld_var 8 32 @sta1 1
			DAQIANGDAO_FENSHU @sta1 0
			
			if @sta1 >= 100
				TASK_FINI 4615 0
				set_pworld_var 8 32 0 1
				set_pworld_var 0 32 0 1
				set_pworld_var 4 32 0 1
			endif
		endif
	endif
endproc
