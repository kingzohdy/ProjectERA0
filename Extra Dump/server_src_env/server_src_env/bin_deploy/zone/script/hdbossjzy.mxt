

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b一整天都在闲晃你都不会觉得无聊吗？要不要跟朋友来参加活动啊？

		task_add_option 3201 1 100
		task_add_option 3202 1 101
		task_add_option 3203 1 102
		task_add_option 3204 1 103
		task_add_option 3205 1 104
		task_add_option 3206 1 105
		task_add_option 3207 1 106
		task_add_option 3208 1 107
		task_add_option 3209 1 108
		task_add_option 3210 1 109
		task_add_option 3211 1 110
		task_add_option 3212 1 111
		task_add_option 3213 1 112
		task_add_option 3214 1 113
		task_add_option 3215 1 114
		task_add_option 3216 1 115
		task_add_option 3217 1 116
		task_add_option 3218 1 117
		task_add_option 3219 1 118
		task_add_option 3220 1 119
	
	talk
endproc 

proc 100
    task_do_option 3201 1
endproc
proc 101
    task_do_option 3202 1
endproc
proc 102
    task_do_option 3203 1
endproc
proc 103
    task_do_option 3204 1
endproc
proc 104
    task_do_option 3205 1
endproc
proc 105
    task_do_option 3206 1
endproc
proc 106
    task_do_option 3207 1
endproc
proc 107
    task_do_option 3208 1
endproc
proc 108
    task_do_option 3209 1
endproc
proc 109
    task_do_option 3210 1
endproc
proc 110
    task_do_option 3211 1
endproc
proc 111
    task_do_option 3212 1
endproc
proc 112
    task_do_option 3213 1
endproc
proc 113
    task_do_option 3214 1
endproc
proc 114
    task_do_option 3215 1
endproc
proc 115
    task_do_option 3216 1
endproc
proc 116
    task_do_option 3217 1
endproc
proc 117
    task_do_option 3218 1
endproc
proc 118
    task_do_option 3219 1
endproc
proc 119
    task_do_option 3220 1
endproc






