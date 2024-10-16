;;;
;l;
proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b听着，$_player$，我不能丢下我的战友，无论如何，我都不能丢下他们独自苟且。你明白我的意思，对吧？

		task_add_option 1856 1 100
		task_add_option 1857 1 101
		task_add_option 1835 1 102
		task_add_option 1836 1 103
		task_add_option 1837 1 104
		task_add_option 1838 1 105
		
		task_add_option 1830 1 106
	talk

endproc
PROC 100
	task_do_option 1856 1
ENDPROC
PROC 101
	task_do_option 1857 1
ENDPROC
PROC 102
	task_do_option 1835 1
ENDPROC
PROC 103
	task_do_option 1836 1
ENDPROC
PROC 104
	task_do_option 1837 1
ENDPROC
PROC 105
	task_do_option 1838 1
ENDPROC
PROC 106
	task_do_option 1830 1
ENDPROC

proc 2
	get_trig_task 1 2
;	if 2 = 1857 
;		add_item_num_inpak 5300542 1 1 0
;		add_item_num_inpak 5300567 1 1 0
;		add_item_num_inpak 5300568 1 1 0
;	endif
;
	if 2 = 1830
		task_fini 1830 1
	endif
endproc 