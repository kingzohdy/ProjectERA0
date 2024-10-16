
proc 3
	strvar ret patId
	GET_MON_INFO 11 0 12 0 0 0 0 0
	if @11 = 0
		GET_MON_PATROL_ID @12 ret patId
		if @ret = 0
			if @patId = 1130
				MACHINE_SPOUSE_CHG_MAP 1000 41125 33902 1131 13
			elseif @patId = 1131
				MACHINE_SPOUSE_CHG_MAP 4403 31656 9804 1132 13
			elseif @patId = 1132
				MACHINE_CLEAR 14
				set_wedding_sth 6 1 13   ;把婚礼蜜游设为结束标记
			else
			endif
		endif
	endif
endproc
