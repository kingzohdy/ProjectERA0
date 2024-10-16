;牧牛预赛,杰轮
proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b曾经有人问我，哪条路才是通往成功人生的康庄大道，我想了想，便举办了这个活动，我告诉他可以从这个活动中找到答案。
;	add_talk #c186506牛仔很忙活动快速到达终点诀窍：
	task_add_option 30022 1 90
	task_stat 30022 1 2
	if 2 != 0 and 2 != 3
		option 100 选择牧宠
	endif
	get_id_player_info 0 8 10
	task_stat 30024 1 2
	task_stat 30025 1 3
	task_stat 30026 1 4
	task_stat 30027 1 5
	if 2 = 3 and 3 = 3 and 4 = 3 and 5 = 3 and 10 >= 45
	else
		option 88 激活牧宠选项的任务列表
	endif
	
	option 89 牛仔很忙活动快速到达终点诀窍
	talk
endproc
proc 2
	get_trig_task 1 2 
	if 2 = 30022 
		call 100 
	endif
endproc
proc 88
	dialog 1
	add_talk $_npc$:
;	add_talk #b#b曾经有人问我，哪条路才是通往成功人生的康庄大道，我想了想，便举办了这个活动，我告诉他可以从这个活动中找到答案。
	add_talk #b#b想要在初赛中比别人更胜一筹吗？想要拥有速度至上的剑齿虎和猎豹、贴心听话的野猪和沙漠白狼牧宠吗？
	add_talk #b#b只要完成下列任务，就会激活初赛、决赛中的牧宠选项，让你在比赛中拥有致胜优先权！
	add_talk #b#b#c186506特别声明：牧宠选项为永久激活哟！
	
	task_add_option 30024 1 91
	task_add_option 30025 1 92
	task_add_option 30026 1 93
	task_add_option 30027 1 94
	option 1 返回
	talk
endproc
proc 89
	dialog 1
	add_talk $_npc$:
;	add_talk #b#b曾经有人问我，哪条路才是通往成功人生的康庄大道，我想了想，便举办了这个活动，我告诉他可以从这个活动中找到答案。
	add_talk #b#b#c1865061、接取任务，获得牧鞭
	add_talk #b#b#c1865062、将牧鞭拖至快捷栏里顺手的位置
	add_talk #b#b#c1865063、屏蔽非队友玩家，快捷键F11、F12
	add_talk #b#b#c1865064、与我对话，选择牧宠
	add_talk #b#b#c1865065、选中你的牧宠（就在我附近）
	add_talk #b#b#c1865066、面向终点而站，即你|牧宠|终点
	add_talk #b#b#c1865067、对牧宠使用牧鞭
	add_talk #b#b#c1865068、不断调整方向#n
	
	option 1 返回
	talk
endproc
	
proc 90
	task_do_option 30022  1
endproc
proc 91
	task_do_option 30024  1
endproc
proc 92
	task_do_option 30025  1
endproc
proc 93
	task_do_option 30026  1
endproc
proc 94
	task_do_option 30027  1
endproc
proc 100
	dialog 1
	add_talk #b#b由于资源有限，我可以提供你一头可爱的小羊来帮助你进行牛仔很忙活动。（免费哟）
	add_talk #b#b如果你激活了其它牧宠选项，我建议你不要选择我提供你的小羊，它虽然可爱，却不若野猪、沙漠白狼听话，也不如剑齿虎、丛林猎豹速度快。
	add_talk #b#b特别声明：选项为永久激活哟。
	add_talk #b#b注：不要在此浪费太多时间哟，活动已经开始计时了哟。
	
	task_stat 30022 1 2
	if 2 != 0 and 2 != 3
		option 110 #给我一头可爱的小羊
	endif
	task_stat 30024 1 2
	if 2 = 3 
		option 120 #来头贴心又听话的野猪
	endif
	task_stat 30025 1 2
	if 2 = 3 	
		option 130 #剑齿虎一出谁敢竞速
	endif
	task_stat 30026 1 2
	if 2 = 3 
		option 140 #来头少走弯路的沙漠白狼
	endif
	task_stat 30027 1 2
	if 2 = 3 
		option 150 #丛林猎豹速度至上
	endif
	task_stat 30024 1 2
	task_stat 30025 1 3
	task_stat 30026 1 4
	task_stat 30027 1 5
	if 2 = 3 and 3 = 3 and 4 = 3 and 5 = 3
	else
		option 88 激活牧宠选项的任务列表
	endif
	option 1 返回
	talk
endproc
proc 110
	GET_TASK_VAR 30022 0 32 1 2
	if 2 != 0
		add_sys_msg_start
		add_sys_msg 如同人生一样，你不能后悔，既然选择了，就坚持到底吧！  
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	NEW_CONVOY_MON 241010 20600 31800 200 0 0 0 1800 30022 1 2
	SET_TASK_VAR 30022 0 32 @2 1	
	call 111	
endproc
proc 111
	add_sys_msg_start
	add_sys_msg 对牧宠使用物品鞭挞之，使其移动
	set_sys_msg_mode 3 0 0
	msg_info
	var 11 0
	get_time_sec 11
	;timebegin11,开始赶牛时间
	
	set_task_var 30022 4 32 @11 1
endproc
proc 120
	GET_TASK_VAR 30022 0 32 1 2
	if 2 != 0
		add_sys_msg_start
		add_sys_msg 如同人生一样，你不能后悔，既然选择了，就坚持到底吧！  
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	NEW_CONVOY_MON 241011 20600 31800 200 0 0 0 1800 30022 1 2
	SET_TASK_VAR 30022 0 32 @2 1	
	call 111	
endproc
proc 130
	GET_TASK_VAR 30022 0 32 1 2
	if 2 != 0
		add_sys_msg_start
		add_sys_msg 如同人生一样，你不能后悔，既然选择了，就坚持到底吧！  
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	NEW_CONVOY_MON 241012 20600 31800 200 0 0 0 1800 30022 1 2	
	SET_TASK_VAR 30022 0 32 @2 1
	
	call 111	
endproc
proc 140
	GET_TASK_VAR 30022 0 32 1 2
	if 2 != 0
		add_sys_msg_start
		add_sys_msg 如同人生一样，你不能后悔，既然选择了，就坚持到底吧！  
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	NEW_CONVOY_MON 241013 20600 31800 200 0 0 0 1800 30022 1 2	
	SET_TASK_VAR 30022 0 32 @2 1
	call 111
endproc
proc 150
	GET_TASK_VAR 30022 0 32 1 2
	if 2 != 0
		add_sys_msg_start
		add_sys_msg 如同人生一样，你不能后悔，既然选择了，就坚持到底吧！  
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	NEW_CONVOY_MON 241014 20600 31800 200 0 0 0 1800 30022 1 2
	SET_TASK_VAR 30022 0 32 @2 1	
	call 111	
endproc
