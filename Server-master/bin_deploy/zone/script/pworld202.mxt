
proc 1

	effect 0 0 playEffct(6700,4400,"effect\\entity\\9570_16.ent")
;	effect 0 0 playEffct(6900,5100,"effect\\entity\\9570_16.ent")

	
	strvar Time
	var Time 0 
	set_pworld_var 32 32 @Time 1
endproc 


proc 3
	
	get_pworld_var 32 32 1 Time 
	add Time 1 
	set_pworld_var 32 32 @Time 1

	if Time = 3
		effect 0 0 playEffct(6700,4400,"effect\\entity\\9570_16.ent")
	;	effect 0 0 playEffct(6900,5100,"effect\\entity\\9570_16.ent")
	endif
	if Time = 6
		effect 0 0 playEffct(6700,4400,"effect\\entity\\9570_16.ent")
	;	effect 0 0 playEffct(6900,5100,"effect\\entity\\9570_16.ent")
	endif
	if Time = 9
		effect 0 0 playEffct(6700,4400,"effect\\entity\\9570_16.ent")
	;	effect 0 0 playEffct(6900,5100,"effect\\entity\\9570_16.ent")
	endif
	if Time = 12
		effect 0 0 playEffct(6700,4400,"effect\\entity\\9570_16.ent")
	;	effect 0 0 playEffct(6900,5100,"effect\\entity\\9570_16.ent")
	endif
	if Time > 12
		get_pworld_var 32 32 1 Time 
		var Time 13 
		set_pworld_var 32 32 @Time 1
		return
	endif
endproc 