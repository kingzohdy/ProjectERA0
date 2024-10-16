;Ìì¿Õ»ØÀÈ Âß¼­ÇøÓò6


PROC 1
	IS_IN_PWORLD 503 1 100 0
	if @100 = 1
	GET_PWORLD_VAR 28 32 1 17
	If 17 = 0
			EFFECT 0 0 playEffct(25298,43422,"effect\\entity\\5500002_15.ent")
			var 17 1
			set_pworld_var 28 32 @17 1

	endif
	endif
ENDPROC