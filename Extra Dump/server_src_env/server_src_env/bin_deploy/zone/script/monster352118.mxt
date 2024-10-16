; ÀËÂþÑÌ»¨

Proc 2
	strvar x y num num1
	GET_MON_INFO 0 0 0 0 0 0 x y
	
	rand num 100
		if num < 50
			NEW_LIFE_MON 293800 1 @x @y 100 0 0 0 30 10 0
		endif
		if num >= 50
			NEW_LIFE_MON 293802 1 @x @y 100 0 0 0 30 10 0
		endif
	
	rand num1 100
		if num1 < 50
			NEW_LIFE_MON 293800 1 @x @y 100 0 0 0 30 10 0
		endif
		if num1 >= 50
			NEW_LIFE_MON 293802 1 @x @y 100 0 0 0 30 10 0
		endif

endproc
