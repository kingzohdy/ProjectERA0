;Ìì¿Õ»ØÀÈ Âß¼­ÇøÓò4


PROC 1
	IS_IN_PWORLD 503 1 100 0
	if @100 = 1
	GET_PWORLD_VAR 4 32 1 11
	If 11 = 0
	
			EFFECT 0 0 playEffct(21594,43335,"effect\\entity\\5500002_15.ent")
			var 11 1
			set_pworld_var 4 32 @11 1

	endif
	endif
ENDPROC