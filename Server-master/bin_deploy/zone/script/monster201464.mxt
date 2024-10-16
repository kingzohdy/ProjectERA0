;¼À×æÉñµî ,Ñ²Âß±ø1
proc 2
	var 10 0
	rand 10 100
	if 10 < 10
		strvar ret defid memid hp mapdef map x y
		GET_MON_INFO ret defid memid hp mapdef map x y
		
		NEW_TMP_ITEM 1400004 @x @y 0 0 0 1
	endif
endproc
