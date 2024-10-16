;雷蒙盖盾 转职处

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk  #b#b$_player$，与你一起奋斗，与你一起并肩作战是我的荣幸！为感谢你助我摆脱了梦境，将来你必定能在英雄的道路上战胜自己，拓展自己，成就自己！
		
		option 120 #请与我一起离开这里吧！
		
	talk
	
endproc
PROC 120
	TRANSMIT 4402 18500 8300 1
ENDPROC


