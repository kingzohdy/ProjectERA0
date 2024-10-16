; 阳光海滩经验翻倍

proc 1
        ADD_SYS_MSG_START
        ADD_SYS_MSG 好消息！18:00-24:00期间，沐浴阳光海滩获得的经验将提高一倍！
        SET_SYS_MSG_MODE 1 0 0
        SVR_MSG_INFO

        SET_SVR_STATUS 3085 2 21600 1
endproc

PROC 2
        ADD_SYS_MSG_START
        ADD_SYS_MSG 欢乐的时间总是过得特别快，转眼就24:00了，今天沐浴阳光海滩获得翻倍经验的活动已暂告一段落
        SET_SYS_MSG_MODE 1 0 0
        SVR_MSG_INFO
ENDPROC

proc 4
	strvar haitan
	GET_EVENT_STAT haitan 303
	
	if haitan = 1
		SET_SVR_STATUS 3085 2 21600 1
	endif
endproc
