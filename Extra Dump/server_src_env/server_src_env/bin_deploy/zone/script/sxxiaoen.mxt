;;;
;l;
proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b很高兴你选择帮助加入我们，相信有了你的加入，我们必定能够战胜一切困难，消灭所有的敌人，胜利与荣誉终将属于我们！

		task_add_option 1825 1 100
		task_add_option 1826 1 101
		task_add_option 1827 1 102
		task_add_option 1828 1 103
		task_add_option 1829 1 104
		task_add_option 1830 1 105
		task_add_option 1831 1 106
		task_add_option 1832 1 107
		task_add_option 1833 1 108
		task_add_option 1835 1 109
		
;;;;;70+		
	;	task_add_option 1901 1 110
		
		
		
	talk

endproc
PROC 100
	task_do_option 1825 1
ENDPROC
PROC 101
	task_do_option 1826 1
ENDPROC
PROC 102
	task_do_option 1827 1
ENDPROC
PROC 103
	task_do_option 1828 1
ENDPROC
PROC 104
	task_do_option 1829 1
ENDPROC
PROC 105
	task_do_option 1830 1
ENDPROC
PROC 106
	task_do_option 1831 1
ENDPROC
PROC 107
	task_do_option 1832 1
ENDPROC
PROC 108
	task_do_option 1833 1
ENDPROC
PROC 109
	task_do_option 1835 1
ENDPROC
PROC 110
	task_do_option 1901 1
ENDPROC

proc 2
	get_trig_task 1 2
	if 2 = 1835
		task_fini 1835 1
	endif

endproc



