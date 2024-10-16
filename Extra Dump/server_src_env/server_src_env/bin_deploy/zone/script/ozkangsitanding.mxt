; 
; 携带任务\副本永夜内NPC

proc 1



	dialog 1
	add_talk $_npc$:
	add_talk #b#b这一场战斗，不管时空的转移、场景的变换，战斗的历史都将持续不断的延续下去，直到光明再次笼罩大地。
	task_add_option 2241 1 101
	task_add_option 2242 1 102
	task_add_option 2243 1 103
	task_add_option 2244 1 104
	task_add_option 2245 1 105

	get_pworld_var 0 32 1 10
	if 10 = 1
			add_talk #b#b准备好净化最后一个亡灵了么？
			option 500 是的，牧师
	endif
	talk
endproc
proc 101
    task_do_option 2241 1
endproc
proc 102
    task_do_option 2242 1
endproc

proc 103
    task_do_option 2243 1
endproc
proc 104
    task_do_option 2244 1
endproc
proc 105
    task_do_option 2245 1
endproc

proc 2
	
	get_trig_task 1 2
	get_pworld_var 0 32 1 10
	if 10 = 1
		dialog 1
		add_talk $_npc$:

		add_talk #b#b准备好净化最后一个亡灵了么？
		option 500 是的，牧师
		talk

		
	endif
endproc



proc 500
	dialog 1
	add_talk $_npc$:

	add_talk #b#b我在这个死亡之地呆太久了，我体内的黑暗力量经过不断积累已经无比强大，也许你该和队友商量一下再做决定。
	option 501 #放松，牧师，剩下的事就交给我们吧
	option 502 #请继续忍耐片刻，我的队友还没准备好
	talk

endproc
proc 501
	
	var 10 2
	set_pworld_var 0 32 @10 1
	add_sys_msg_start
	add_sys_msg 勇敢的$_player$开启了康斯坦丁净化仪式
	set_sys_msg_mode 4 0 0
	map_msg_info

endproc














