
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b航海大亨比赛时间已经结束了，现在你可以前往哥伦布那里查看排行榜，如果你榜上有名的话，别忘了领取排名奖励哟。
    add_talk #b#b
	add_talk #b#b 希望下一届的航海大亨你能够榜上有名，亲爱的$_player$。

	  option 200 #回到岸上
	talk
endproc 

PROC 200
	;传送
	TRANSMIT 4403 24200 31800 0
	DEL_ROLE_TMP_ITEM 0 0
ENDPROC

