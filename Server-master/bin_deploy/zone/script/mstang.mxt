;唐教授


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b啊，$_player$，听说你最近在进行一项秘密的工作？你看起来还是老样子，哎，有没有打算和我这个老朋友分享你的工作体验啊？
	add_talk #b#b对了，那天我的讲座上谢谢你送的花啊，什么，你没有来听我的讲座？你不要否认啦，这没有什么可害羞的，长的丑也不是你的错，放松心情，看开点，世界还是很美好的！
	
	task_add_option 2001 1 100
	task_add_option 2002 1 101
	task_add_option 2003 1 102
	task_add_option 2004 1 103
	task_add_option 2009 1 104
	task_add_option 2010 1 105
	task_add_option 2017 1 106
	
	task_add_option 2040 1 107
	task_add_option 2050 1 108
	
	task_add_option 2005 1 109
	task_add_option 2101 1 110
	
	
	
	talk
	
endproc 

PROC 100
	task_do_option 2001 1
ENDPROC
PROC 101
	task_do_option 2002 1
ENDPROC
PROC 102
	task_do_option 2003 1
ENDPROC
PROC 103
	task_do_option 2004 1
ENDPROC
PROC 104
	task_do_option 2009 1
ENDPROC
PROC 105
	task_do_option 2010 1
ENDPROC
PROC 106
	task_do_option 2017 1
ENDPROC
PROC 107
	task_do_option 2040 1
ENDPROC
PROC 108
	task_do_option 2050 1
ENDPROC
PROC 109
	task_do_option 2005 1
ENDPROC
PROC 110
	task_do_option 2101 1
ENDPROC

proc 2
	
	get_trig_task 1 2 
	if 2 = 2101
		task_fini 2101 1
	endif
	if 2 = 2005
		task_fini 2005 1
	endif
	if 2 = 2010
		task_fini 2010 1
	endif
	if 2 = 2040
		task_fini 2040 1
	endif
endproc

