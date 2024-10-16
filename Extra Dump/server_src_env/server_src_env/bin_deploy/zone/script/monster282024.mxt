;勇士岛2号


proc 1

	strvar Mid
	GET_MON_INFO 0 0 Mid 0 0 0 0 0
	mon_speech @Mid 2 2 1 你们都将臣服在我的脚下！

	TARGET_ADD_STATUS 3 @Mid 1051 1 5 1
	TARGET_ADD_STATUS 3 @Mid 1095 1 5000000 1
endproc 

