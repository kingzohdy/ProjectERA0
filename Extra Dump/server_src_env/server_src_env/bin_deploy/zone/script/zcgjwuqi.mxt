;主城高级武器商人 310001

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk 要战斗，不可能离开你的武器，年轻人，要像爱护自己的眼睛一样爱护自己的武器。
	
	OPTION 101 #敬请期待
	talk
endproc 


PROC 101
	;OPEN_NPC_SHOP
	RETURN
ENDPROC

