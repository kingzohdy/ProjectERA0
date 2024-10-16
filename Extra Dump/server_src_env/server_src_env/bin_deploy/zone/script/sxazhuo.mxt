;;;
;l;
proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b这太恐怖了，$_player$，我怀疑他们全都死了……我们都会死在这里……那些机械怪兽无人能敌！

	
		task_add_option 1810 1 100
		task_add_option 1813 1 101
		task_add_option 1814 1 102
		task_add_option 1815 1 103
		task_add_option 1850 1 104
		task_add_option 1861 1 105
		task_add_option 1862 1 106
		task_add_option 1863 1 107
		task_add_option 1802 1 108
		
		task_add_option 1865 1 109
		
	talk

endproc
PROC 100
	task_do_option 1810 1
ENDPROC
PROC 101
	task_do_option 1813 1
ENDPROC
PROC 102
	task_do_option 1814 1
ENDPROC
PROC 103
	task_do_option 1815 1
ENDPROC
PROC 104
	task_do_option 1850 1
ENDPROC
PROC 105
	task_do_option 1861 1
ENDPROC
PROC 106
	task_do_option 1862 1
ENDPROC
PROC 107
	task_do_option 1863 1
ENDPROC
PROC 108
	task_do_option 1802 1
ENDPROC
PROC 109
	task_do_option 1865 1
ENDPROC

proc 2
	get_trig_task 1 2
	if 2 = 1810 
		task_fini 1810 1
	endif
	if 2 = 1815 
		task_fini 1815 1
	endif
endproc

