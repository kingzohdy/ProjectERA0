	
proc 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b嗨，这位#Gender3#，想体验惊险刺激吗？想被人疯狂追求吗？想以一人之力感染整个世界吗？快来尖峰时刻吧，让你体验不同以往的刺激与激情！
	add_talk #b#b
	add_talk #c186506尖峰时刻开放时间：
	add_talk #b#b#c0181f9 10：00-10：15，15：00-15：15#n
	add_talk #b#b	
	add_talk 尖峰时刻小诀窍：
	add_talk #b#c186506如果你是人类：尽你所能的逃吧！
	add_talk #b人类胜利法则：坚持到底就是胜利！
	add_talk #b#b
	add_talk #b如果你是机械巨人、机械巨兽：尽可能地杀吧！
	add_talk #b机械巨人、机械巨兽胜利法则：消灭一切异族！
		
		OPTION 201 #进入副本
	
	talk
endproc 

proc 201
	
	GET_EVENT_STAT 10 85
	;	OPEN_PWORLD_UI 13
	if 10 = 1
		enter_pworld 200 0 12
	else
		add_sys_msg_start
		add_sys_msg 活动已关闭，请于每天10点和15点准时参加活动！
		set_sys_msg_mode 4 0 0
		msg_info
	endif
	
endproc 