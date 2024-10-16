

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b很多时候，人生和生意一样，就靠一个“赌”字，而且无非是赌运气、赌头脑和赌胆量。而我之所以滞留在这里，就是因为时运不济，遇到了阿拜多斯苏醒。
	
	task_add_option 1593 1 101
	task_add_option 1594 1 102
	task_add_option 1595 1 103
	task_add_option 1596 1 104
	task_add_option 1597 1 105
	
	task_add_option 1555 1 106

	strvar Ret
	task_stat 1503 1 Ret
	if Ret = 2
		option 150 斯塔里先生，听说您找我？
	endif

	talk
endproc 


PROC 101
	task_do_option 1593 1
ENDPROC
PROC 102
	task_do_option 1594 1
ENDPROC
PROC 103
	task_do_option 1595 1
ENDPROC
PROC 104
	task_do_option 1596 1
ENDPROC
PROC 105
	task_do_option 1597 1
ENDPROC
PROC 106
	task_do_option 1555 1
ENDPROC
proc 150
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b$_player$，见到你真是太好了，我现在有很多很多事情要做，急需一位像你这样机智、冷静、勇敢、犀利的好帮手……真是太让人兴奋了，兴奋的我都忘了应该先把哪件事交给你来做，让我想想，让我想一想，让我好好想一想……	
	task_fini 1503 1
		option 151 #呃，您慢慢想，我不着急！
	talk
endproc 
proc 151
	return
endproc 
proc 2
	get_trig_task 1 2
	if 2 = 1597
;		GET_MAP_MON_NUM 2300 293067 0 90 0

	endif 
endproc
