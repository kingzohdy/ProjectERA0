;70副本,第5个警报器
proc 4
	;逻辑区域boss1触发区,0未触发过,1第一排报警,2第一排报警,3第三排报警
	get_pworld_var 80 32 1 30
	if 30 < 3
		return
	endif
	strvar x y
	GET_MON_INFO 1 0 0 0 0 0 x y
	del_mon 1
	new_mon 282049 1 @x @y 0 0 0 1 0
endproc
