; 皮克希

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b$_player$，你看到我爸爸妈妈了吗？
	add_talk #b#b<一个稚嫩的声音在你耳边响起>
	add_talk #b#b我在玩躲猫猫的时候睡着了，醒来后发现他们都不见了，还有我的布娃娃，也不见了……
		task_add_option 1936 1 100
		task_add_option 1937 1 101

	talk
endproc

PROC 100
	task_do_option 1936 1
ENDPROC
PROC 101
	task_do_option 1937 1
ENDPROC



;proc 2
;	get_trig_task 1 2
;	if 2 = 1910
;		task_fini 1910 1
;	endif
;
;endproc
