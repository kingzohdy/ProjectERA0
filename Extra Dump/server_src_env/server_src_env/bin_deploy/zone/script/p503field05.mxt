;Ìì¿Õ»ØÀÈ Âß¼­ÇøÓò5


PROC 1
	IS_IN_PWORLD 503 1 100 0
	if @100 = 1
	GET_PWORLD_VAR 24 32 1 16
	If 16 = 0
		EFFECT 0 0 playEffct(23674,43363,"effect\\entity\\5500002_15.ent")
		var 16 1
		set_pworld_var 24 32 @16 1

	endif
	endif
ENDPROC