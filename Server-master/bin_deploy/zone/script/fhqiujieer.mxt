; 丘杰尔


proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b 如果你要引人注目，就要使得自己成为一棵树，傲立于天地之间；而不是做一颗草，你见过谁踩了一颗草，还抱歉地说对不起？
	option 110 #打开商店
	talk
endproc

PROC 110
	OPEN_NPC_SHOP 
ENDPROC


;proc 2
;	get_trig_task 1 2
;	if 2 = 1910
;		task_fini 1910 1
;	endif
;
;endproc
