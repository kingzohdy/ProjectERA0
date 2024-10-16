;勇士岛3号

proc 1
	strvar Mid
	GET_MON_INFO 0 0 Mid 0 0 0 0 0
	mon_speech @Mid 2 2 1 PK之王，万王之王，就是我——伊索！
	
	TARGET_ADD_STATUS 3 @Mid 1051 1 5 1
	
	TARGET_ADD_STATUS 3 @Mid 1095 1 5000000 1

endproc 
