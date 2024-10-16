
proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#bHEY，$_player$，锻炼的机会来了，想要变得更强吗？还在不停的弄装备吗？来我这里只要你参加活动，你就可以变得非常强大，不过再强大也是有死亡的危险哦，还有丰富的奖励呢！
	add_talk #b#b#c186506光影之战开放时间：
	add_talk #b#b#c0181f912:00-12:10，16:30-16:40
	
	OPTION 202 光影之战

	talk
	
endproc


proc 202

	dialog 1
	add_talk $_npc$:
	add_talk #b#b里面可是有好多经验的奖励呀，而且简单粗暴，赶紧进来参加吧！

	OPTION 300 #参加光影之战
	talk
endproc
proc 300 
		GET_EVENT_STAT 401 80		
	
		IF 401 = 1
			enter_pworld 221 0 12
		ELSE
			add_sys_msg_start
			add_sys_msg 活动还没开始，无法进入！
			set_sys_msg_mode 4 0 0
			msg_info
		ENDIF
endproc