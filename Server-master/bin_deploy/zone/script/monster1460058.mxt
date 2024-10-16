;70副本,飞碟
proc 2
	;ufoflag33,ufo重生标志,0重生,1不重生
	var 33 0
	get_pworld_var 92 32 1 33
	if 33 > 0
		return
	endif
	NEW_MON_MACHINE 1 1460058 0 1 19620 29736 0
endproc
