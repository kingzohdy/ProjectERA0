




proc 1
	strvar Mid Rad_2
	GET_MON_INFO 10 1 Mid 1 1 1 1 1
	
	
	
	rand Rad_2 5

	
	switch Rad_2
		case 0 
			MON_SPEECH @Mid 0 1 1 希望能够像上次一样，平安回家……

		endcase
		case 1 
			MON_SPEECH @Mid 0 1 1 这里好冷啊，好想念温暖的家……

		endcase
		case 2 
			MON_SPEECH @Mid 0 1 1 这些是什么东西？它们上船了，啊……好恐怖……

		endcase
		case 3 
			MON_SPEECH @Mid 0 1 1 别杀我，别杀我，不知道什么火元素……

		endcase
		case 4 
			MON_SPEECH @Mid 0 1 1 混蛋，我跟你拼了！

		endcase
	endswitch
	
endproc 
 