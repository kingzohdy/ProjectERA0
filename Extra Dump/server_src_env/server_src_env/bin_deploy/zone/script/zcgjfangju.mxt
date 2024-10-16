;主城高级防具商人 317000

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk 光靠自己身上的肉就想承担战斗中的伤害吗？那你很快就会变成一根钢管的，来挑选适合你的防具吧。

	OPTION 101 #敬请期待
	talk
ENDPROC


PROC 101
	;OPEN_NPC_SHOP
	RETURN
ENDPROC