
proc 1
	strvar mid
	GET_MON_INFO 0 0 mid 0 0 0 0 0
	TARGET_ADD_STATUS 3 @mid 1095 1 25920000 1
endproc

;die
proc 2
	set_pworld_var 0 32 1 1
	DELETE_MON_BY_DEFID 207213 0
	DELETE_MON_BY_DEFID 207214 0
	strvar xpos ypos
	GET_MON_INFO 1 207212 0 0 0 0 xpos ypos 
	NEW_LIFE_NPC 1 307040 @xpos @ypos 0 300 npc307040.mac
endproc

proc 4
	
endproc
