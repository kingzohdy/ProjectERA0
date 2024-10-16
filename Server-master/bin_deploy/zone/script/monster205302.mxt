;¼À×æÉñµî ,ÃÅ¿ÚÉÚ±ø2
proc 2
	strvar ret defid memid hp mapdef map x y
	GET_MON_INFO ret defid memid hp mapdef map x y
	
	NEW_TMP_ITEM 1400020 @x @y 0 0 0 1
endproc
