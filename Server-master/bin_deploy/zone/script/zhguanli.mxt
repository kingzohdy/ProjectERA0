;转职竞技场管理员

;


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b嗨，你好啊，#Race#，欢迎来到勇士竞技场，想要来比一场吗？这可是个一战成名的机会！
	
	
		option 100 #进入试炼场
	talk
	
endproc 
proc 100

	transmit 4402 12900 14000 1
endproc

