;

proc 1
	strvar mid
	GET_MON_INFO 0 0 mid 0 0 0 0 0
	TARGET_ADD_STATUS 3 @mid 1095 1 25920000 0
endproc

proc 2
	strvar x y 
	GET_MON_INFO 0 0 0 0 0 0 x y
	NEW_LIFE_NPC 0 307031 @x @y 0 180 npc307031.mac
endproc
