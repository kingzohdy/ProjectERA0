

proc 1
	strvar ret x y
	GET_TARGET_INFO 3 3 x
	GET_TARGET_INFO 3 4 y	
	
	del_mon ret
	if ret = 0

		NEW_LIFE_MON 293238 1 @x @y 0 0 0 0 360 10 0
	endif
	
endproc 