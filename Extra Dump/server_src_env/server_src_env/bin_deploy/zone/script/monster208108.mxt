

proc 2

	GET_MON_INFO 0 0 0 50 0 0 0 0
	if 50 = 0
	
		CLAN_BABY_DIE 1

	endif
	
	strvar x y 
	GET_MON_INFO 0 0 0 0 0 0 x y
	NEW_LIFE_NPC 0 307037 @x @y 0 21600 npc307037.mac
endproc 