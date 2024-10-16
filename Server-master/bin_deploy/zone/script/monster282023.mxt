;勇士岛1号


proc 1

	strvar Mid
	GET_MON_INFO 0 0 Mid 0 0 0 0 0
	mon_speech @Mid 2 2 1 没有人能战胜我！
	
	TARGET_ADD_STATUS 3 @Mid 1051 1 5 1
	TARGET_ADD_STATUS 3 @Mid 1095 1 5000000 1
endproc
proc 2

	DELETE_MON_BY_DEFID 205021 1
endproc
