;殿堂荣耀传送门

proc 1
	choose 0 100 101 离开本地图后将视为自动放弃比赛，是否确认离开？
endproc

proc 100
	strvar rFlag
	DIANFENG_CLEAR_BAOMING_INFO 11 rFlag
	if @rFlag = 1 and @11 = 0
	;程序中退出
	else
		TRANSMIT 1000 26600 21400 0
	endif
endproc
