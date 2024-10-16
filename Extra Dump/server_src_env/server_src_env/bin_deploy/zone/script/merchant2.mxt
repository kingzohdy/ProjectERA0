proc 1
	dialog 1
	add_talk 被哥布林阻隔的商路，不知道什么时候才能够重新打通。
;	task_add_option 4021 1 101
	task_add_option 4039 1 102
	task_add_option 4040 1 103	
	talk
endproc

;proc 101
;	task_do_option 4021 1
;endproc
proc 102
	task_do_option 4039 1
endproc
proc 103
    task_do_option 4040 1
endproc

