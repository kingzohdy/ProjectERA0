

proc 1
	DIALOG 1
	;add_talk $_npc$:
	add_talk #b#b³Â¾ÉµÄÄ¾Ïä

	task_add_option 1661 1 101
	talk
endproc 


PROC 101
	task_do_option 1661 1
ENDPROC
