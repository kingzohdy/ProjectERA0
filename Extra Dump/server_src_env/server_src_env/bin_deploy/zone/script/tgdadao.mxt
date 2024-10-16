proc 1
	dialog 1
	add_talk 刀越磨越亮，人越练越壮！勇士，有机会咱们来切磋一下…
	task_add_option 4033 1 101
	task_add_option 4030 1 102
	task_add_option 4031 1 103

	task_add_option 4105 1 108
	talk
endproc

proc 101
    task_do_option 4033 1
endproc
proc 102
    task_do_option 4030 1
endproc
proc 103
    task_do_option 4031 1
endproc

proc 108
    task_do_option 4105 1
endproc
;proc 2
;	get_trig_task 1 600
;	if 600 = 4033 
;		task_fini 4033 1
;	endif
;ENDPROC
