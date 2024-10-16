proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b只要我还有一口气在，野兽就休想靠近村子半步。


	task_add_option 1301 51 152

	task_add_option 1302 51 153
	task_add_option 1304 51 155

	talk
endproc


proc 152
    task_do_option 1301 51
endproc
proc 153
    task_do_option 1302 51
endproc

proc 155
    task_do_option 1304 51
endproc
proc 2
	get_trig_task 500 600
	if 600 = 1304
		task_fini 1304 500
	endif  
endproc
