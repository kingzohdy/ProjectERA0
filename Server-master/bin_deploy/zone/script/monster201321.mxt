

proc 2
	strvar x y
	GET_MON_INFO 0 0 0 0 0 0 x y
	NEW_MAP_LIFE_NPC_EX 1 3211 304122 @x @y 0 60 72 npc_222.mac
	
endproc 