

proc 1

	strvar Rflag Maxlv
	GET_CUR_MAX_LEVEL Rflag Maxlv
	if Rflag = 0 and Maxlv > 59
		
		add_sys_msg_start
		add_sys_msg 时间城上空的雾霾再次爆发，一大群【黑暗物质】突如袭来！
		set_sys_msg_mode 0 0 0
		SVR_MSG_INFO
	endif



	strvar Rflag Maxlv
	GET_CUR_MAX_LEVEL Rflag Maxlv
		if Rflag = 0 and Maxlv > 59
		strvar tim
		var tim 0
		set_event_var 0 32 @tim 1

		call 100
	endif
endproc 

proc 2
	strvar Rflag Maxlv
	GET_CUR_MAX_LEVEL Rflag Maxlv
	if Rflag = 0 and Maxlv > 59
		add_sys_msg_start
		add_sys_msg 【黑暗物质】终于被清扫出时间城，天空的雾霾开始散去
		set_sys_msg_mode 0 0 0
		SVR_MSG_INFO
	endif

endproc 
proc 3
	strvar Rflag Maxlv
	GET_CUR_MAX_LEVEL Rflag Maxlv
	if Rflag = 0 and Maxlv > 59
	
		get_event_var 0 32 1 tim
		add tim 1
		set_event_var 0 32 @tim 1
		
		if tim = 12
			call 100
			var tim 0
			set_event_var 0 32 @tim 1
		endif
	endif

endproc 

proc 100

	NEW_MAP_MON 200029 1 1000 23778 34326 0 0 0 0 600 0 0
	NEW_MAP_MON 200029 1 1000 28730 34343 0 0 0 0 600 0 0
	NEW_MAP_MON 200029 1 1000 22094 30393 0 0 0 0 600 0 0
	NEW_MAP_MON 200029 1 1000 25227 25874 0 0 0 0 600 0 0
	NEW_MAP_MON 200029 1 1000 29202 28607 0 0 0 0 600 0 0
	NEW_MAP_MON 200029 1 1000 30785 32583 0 0 0 0 600 0 0
	NEW_MAP_MON 200029 1 1000 21909 27275 0 0 0 0 600 0 0
	NEW_MAP_MON 200029 1 1000 24014 25473 0 0 0 0 600 0 0
	NEW_MAP_MON 200029 1 1000 23458 23367 0 0 0 0 600 0 0
	NEW_MAP_MON 200029 1 1000 26642 26163 0 0 0 0 600 0 0
	NEW_MAP_MON 200029 1 1000 27787 23334 0 0 0 0 600 0 0
	NEW_MAP_MON 200029 1 1000 30297 25405 0 0 0 0 600 0 0
	NEW_MAP_MON 200029 1 1000 36617 31031 0 0 0 0 600 0 0
	NEW_MAP_MON 200029 1 1000 37931 35408 0 0 0 0 600 0 0
	NEW_MAP_MON 200029 1 1000 38436 42315 0 0 0 0 600 0 0
	NEW_MAP_MON 200029 1 1000 43153 34313 0 0 0 0 600 0 0
	NEW_MAP_MON 200029 1 1000 48039 32426 0 0 0 0 600 0 0
	NEW_MAP_MON 200029 1 1000 45950 28130 0 0 0 0 600 0 0
	NEW_MAP_MON 200029 1 1000 41251 30573 0 0 0 0 600 0 0
	NEW_MAP_MON 200029 1 1000 39145 28130 0 0 0 0 600 0 0
	NEW_MAP_MON 200029 1 1000 26628 18524 0 0 0 0 600 0 0
	NEW_MAP_MON 200029 1 1000 21861 13689 0 0 0 0 600 0 0
	NEW_MAP_MON 200029 1 1000 25483 11213 0 0 0 0 600 0 0
	NEW_MAP_MON 200029 1 1000 28869 13739 0 0 0 0 600 0 0
	NEW_MAP_MON 200029 1 1000 20734 33047 0 0 0 0 600 0 0
	NEW_MAP_MON 200029 1 1000 17684 34677 0 0 0 0 600 0 0
	NEW_MAP_MON 200029 1 1000 16488 29219 0 0 0 0 600 0 0
	NEW_MAP_MON 200029 1 1000 13639 28322 0 0 0 0 600 0 0
	NEW_MAP_MON 200029 1 1000 13804 34886 0 0 0 0 600 0 0
	NEW_MAP_MON 200029 1 1000 21713 46436 0 0 0 0 600 0 0
	NEW_MAP_MON 200029 1 1000 20517 37577 0 0 0 0 600 0 0
	NEW_MAP_MON 200029 1 1000 29996 45069 0 0 0 0 600 0 0
	NEW_MAP_MON 200029 1 1000 23776 36157 0 0 0 0 600 0 0
	NEW_MAP_MON 200029 1 1000 22460 41794 0 0 0 0 600 0 0
	NEW_MAP_MON 200029 1 1000 29413 43140 0 0 0 0 600 0 0
	NEW_MAP_MON 200029 1 1000 30340 37413 0 0 0 0 600 0 0
	NEW_MAP_MON 200029 1 1000 28052 38609 0 0 0 0 600 0 0
	NEW_MAP_MON 200029 1 1000 24494 40718 0 0 0 0 600 0 0
	NEW_MAP_MON 200029 1 1000 20247 43080 0 0 0 0 600 0 0
	NEW_MAP_MON 200029 1 1000 32583 42183 0 0 0 0 600 0 0
	
	; effect 0 0 playEffct(23778,34326,"effect\\entity\\700100_4.ent")
	; effect 0 0 playEffct(28730,34343,"effect\\entity\\700100_4.ent")
	; effect 0 0 playEffct(22094,30393,"effect\\entity\\700100_4.ent")
	; effect 0 0 playEffct(25227,25874,"effect\\entity\\700100_4.ent")
	; effect 0 0 playEffct(29202,28607,"effect\\entity\\700100_4.ent")
	; effect 0 0 playEffct(30785,32583,"effect\\entity\\700100_4.ent")
	; effect 0 0 playEffct(21909,27275,"effect\\entity\\700100_4.ent")
	; effect 0 0 playEffct(24014,25473,"effect\\entity\\700100_4.ent")
	; effect 0 0 playEffct(23458,23367,"effect\\entity\\700100_4.ent")
	; effect 0 0 playEffct(26642,26163,"effect\\entity\\700100_4.ent")
	; effect 0 0 playEffct(27787,23334,"effect\\entity\\700100_4.ent")
	; effect 0 0 playEffct(30297,25405,"effect\\entity\\700100_4.ent")
	; effect 0 0 playEffct(36617,31031,"effect\\entity\\700100_4.ent")
	; effect 0 0 playEffct(37931,35408,"effect\\entity\\700100_4.ent")
	; effect 0 0 playEffct(38436,42315,"effect\\entity\\700100_4.ent")
	; effect 0 0 playEffct(43153,34313,"effect\\entity\\700100_4.ent")
	; effect 0 0 playEffct(48039,32426,"effect\\entity\\700100_4.ent")
	; effect 0 0 playEffct(45950,28130,"effect\\entity\\700100_4.ent")
	; effect 0 0 playEffct(41251,30573,"effect\\entity\\700100_4.ent")
	; effect 0 0 playEffct(39145,28130,"effect\\entity\\700100_4.ent")
	; effect 0 0 playEffct(26628,18524,"effect\\entity\\700100_4.ent")
	; effect 0 0 playEffct(21861,13689,"effect\\entity\\700100_4.ent")
	; effect 0 0 playEffct(25483,11213,"effect\\entity\\700100_4.ent")
	; effect 0 0 playEffct(28869,13739,"effect\\entity\\700100_4.ent")
	; effect 0 0 playEffct(20734,33047,"effect\\entity\\700100_4.ent")
	; effect 0 0 playEffct(17684,34677,"effect\\entity\\700100_4.ent")
	; effect 0 0 playEffct(16488,29219,"effect\\entity\\700100_4.ent")
	; effect 0 0 playEffct(13639,28322,"effect\\entity\\700100_4.ent")
	; effect 0 0 playEffct(13804,34886,"effect\\entity\\700100_4.ent")
	; effect 0 0 playEffct(21713,46436,"effect\\entity\\700100_4.ent")
	; effect 0 0 playEffct(20517,37577,"effect\\entity\\700100_4.ent")
	; effect 0 0 playEffct(29996,45069,"effect\\entity\\700100_4.ent")
	; effect 0 0 playEffct(23776,36157,"effect\\entity\\700100_4.ent")
	; effect 0 0 playEffct(22460,41794,"effect\\entity\\700100_4.ent")
	; effect 0 0 playEffct(29413,43140,"effect\\entity\\700100_4.ent")
	; effect 0 0 playEffct(30340,37413,"effect\\entity\\700100_4.ent")
	; effect 0 0 playEffct(28052,38609,"effect\\entity\\700100_4.ent")
	; effect 0 0 playEffct(24494,40718,"effect\\entity\\700100_4.ent")
	; effect 0 0 playEffct(20247,43080,"effect\\entity\\700100_4.ent")
	; effect 0 0 playEffct(32583,42183,"effect\\entity\\700100_4.ent")
	
endproc 