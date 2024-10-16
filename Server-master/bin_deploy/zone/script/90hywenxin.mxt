; 文馨

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b仅仅是一夜之间，世界怎么就变了呢？你永远也无法想象，昨日无限慈爱的父母突然狰狞地向你扑来，不把你撕成碎片誓不罢休的神情，你也无法理解我心中的感受！
	
		task_add_option 2105 1 100
		task_add_option 2106 1 101
		task_add_option 2107 1 102
		task_add_option 2108 1 103
		task_add_option 2110 1 104
		task_add_option 2111 1 105

		strvar Ret
		TASK_STAT 2107 1 Ret
		if Ret = 2
			option 150 我觉得我们有必要聊聊
		endif
		
		strvar Ret1
		TASK_STAT 2111 1 Ret1
		get_task_var 2111 4 8 1 20
		if Ret1 = 2 and 20 = 0
			option 160 #我们出发吧！
		endif
	talk
endproc

PROC 100
	task_do_option 2105 1
ENDPROC
PROC 101
	task_do_option 2106 1
ENDPROC
PROC 102
	task_do_option 2107 1
ENDPROC
PROC 103
	task_do_option 2108 1
ENDPROC
PROC 104
	task_do_option 2110 1
ENDPROC
PROC 105
	task_do_option 2111 1
ENDPROC


proc 2
	get_trig_task 1 2
	if 2 = 2107
		dialog 1
		add_talk $_npc$:
		add_talk #b#b仅仅是一夜之间，世界怎么就变了呢？你永远也无法想象，昨日无限慈爱的父母突然狰狞地向你扑来，不把你撕成碎片誓不罢休的神情，你也无法理解我心中的感受！	
			option 150 我觉得我们有必要聊聊
		talk
	endif
	if 2 = 2111
		dialog 1
		add_talk $_npc$:
		add_talk #b#b仅仅是一夜之间，世界怎么就变了呢？你永远也无法想象，昨日无限慈爱的父母突然狰狞地向你扑来，不把你撕成碎片誓不罢休的神情，你也无法理解我心中的感受！	
			option 160 #我们出发吧！
		talk
	endif
endproc
proc 160
	;护送NPC
	strvar Mid
	CALL_MON_BY_PATROL 1 254023 0 0 11300 20700 503 0 Mid
	if Mid >= 0
		get_task_var 2111 0 32 1 10
		var 10 @Mid 
		set_task_var 2111 0 32 @10 1
		
		get_task_var 2111 4 8 1 20
		var 20 1
		set_task_var 2111 4 8 @20 1
	endif
endproc


proc 150
	dialog 1
	add_talk $_npc$:
	add_talk #b#b我…请你一定要答应我，$_player$，我看得出来，你是一个果断勇敢、敢作敢为、有担当的人，你就将我当成是丧尸，杀了吧，只需要你动一下手指头，就可以了，拜托了，$_player$！
		
		option 151 丧尸？你……
	talk
endproc 
proc 151
	dialog 1
	add_talk $_npc$:
	add_talk #b#b我…我刚刚被丧尸咬了…所有被丧尸咬过的人，几天后都会变成丧尸，我不想变得那么恶心，更不忍心伤害我的弟弟，请在我造成更多的伤害之前，杀了我！
	
		option 152 被咬过就会变成丧尸？
	talk
endproc 
proc 152
	dialog 1
	add_talk $_npc$:
	add_talk #b#b是啊，原本我们村庄的猎户出去打猎，被丧尸袭击，虽然丧尸被猎犬咬死了，但是几天后，猎犬发狂咬了猎户，然后一传十，十传百……我和弟弟是孤儿，由于我们住的比较偏远，所以幸免于难。 
	
		option 153 那你知道究竟是什么袭击了猎户吗？
	talk
endproc 
proc 153
	dialog 1
	add_talk $_npc$:
	add_talk #b#b不是很清楚，只是听说过一个“啃脸男”出现，在光天化日之下扑倒行人啃食人脸，不过那个“啃脸男”已经被警察击毙了！
	
		option 154 那个被啃的呢？
	talk
endproc 
proc 154
	dialog 1
	add_talk $_npc$:
	add_talk #b#b那就不太清楚了，据说是被送到医院抢救了……#Name#，我不想变成丧尸，不想变成吃人的怪物，更不想伤害我的弟弟，请你答应我吧，在我没有变异之前，杀了我！

		option 155 #很抱歉，我不能答应你！
	talk
endproc 
proc 155
	task_fini 2107 1
	return
endproc 













