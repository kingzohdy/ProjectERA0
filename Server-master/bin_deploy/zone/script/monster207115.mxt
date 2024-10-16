; »€—“æﬁ ﬁ

proc 1
	strvar memid
	GET_MON_INFO 1 0 memid 0 0 0 0 0
	TARGET_ADD_STATUS 3 @memid 1095 1 25920000 0 
endproc

proc 2
	strvar x y 
	GET_MON_INFO 0 0 0 0 0 0 x y
	NEW_LIFE_NPC 0 307039 @x @y 0 180 npc307039.mac
endproc
