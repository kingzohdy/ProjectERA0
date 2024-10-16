;皮克希

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b从无到有，是件快乐的事。而从有到无，则是充满痛苦。
		task_add_option 1938 1 100
		task_add_option 1940 1 101
		task_add_option 1943 1 102

	talk
endproc

PROC 100
	task_do_option 1938 1
ENDPROC
PROC 101
	task_do_option 1940 1
ENDPROC
PROC 102
	task_do_option 1943 1
ENDPROC



;proc 2
;	get_trig_task 1 2
;	if 2 = 1943
;		
;	endif
;
;endproc
