proc 1


	DIALOG 1
	ADD_TALK $_npc$:       
	ADD_TALK #b#b…无情的丹泽尔！竟然连他婶婶的表哥的老婆的弟弟的女儿的朋友我也不放过！
	ADD_TALK #b#b那些钱本来够我在酒吧玩上一周的！结果却输得要做赛马裁判来抵债！我美好的一周啊…你什么时候才能回到我身边啊…
	task_add_option 3136 1 100
	task_add_option 3135 1 101
;	option 110 #播放
	TALK
	
endproc

proc 110

			effect 2 0 playEffct(20900,44900,"effect\\entity\\9572_14.ent")
endproc

proc 100
    task_do_option 3136 1
endproc
proc 101
    task_do_option 3135 1
endproc
proc 2
	get_trig_task 1 2
	if 2 = 3136
		task_fini 3136 1
	endif

endproc
