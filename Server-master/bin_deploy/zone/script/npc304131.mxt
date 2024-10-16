;npc灯塔

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b如果你迷路了，我可以送你回岸上；
    add_talk #b#b
	add_talk #b#b如果你想继续航海比赛，请向前行！
	
	option 200 #回到岸上
		
	talk
endproc 


PROC 200
	;传送
	TRANSMIT 4403 24200 31800 0
	DEL_ROLE_TMP_ITEM 0 0
ENDPROC
