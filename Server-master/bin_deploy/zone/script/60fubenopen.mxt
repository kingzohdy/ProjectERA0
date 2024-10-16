

proc 1
	dialog 1
	add_talk $_npc$:
	strvar lv
	GET_ID_PLAYER_INFO 0 8 lv
	if lv < 60
		add_talk #b#b你还太小，$_player$，我不能让你拿生命开玩笑，森林结界很危险，等你转职后60级再来吧！
	
	else
		add_talk #b#b $_player$，您能来真是太好了，我早就听说了您的英雄事迹，这次救人行动有您参与必然是马到功成！
		add_talk #b#b我们的战友已经在里面等您了，他会向您介绍结界的详细情况。
	
		option 150 #进入森林结界
	endif
	
	task_add_option 1901 1 110
	task_add_option 1830 1 111
	task_add_option 1834 1 112
	task_add_option 1840 1 113
	task_add_option 1835 1 114
	
	
	
	talk
	
endproc 
proc 150
	open_pworld_ui 4

endproc

PROC 110
	task_do_option 1901 1
ENDPROC
PROC 111
	task_do_option 1830 1
ENDPROC
PROC 112
	task_do_option 1834 1
ENDPROC
PROC 113
	task_do_option 1840 1
ENDPROC
PROC 114
	task_do_option 1835 1
ENDPROC

proc 2
	get_trig_task 1 2
	if 2 = 1901
		task_fini 1901 1
	endif

endproc








