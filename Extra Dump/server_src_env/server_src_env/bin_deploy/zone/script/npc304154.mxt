;马里奥

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b面临各种邪恶势力的威胁，战车无疑成了热兵器的标杆战斗力。
	add_talk #b#b为了发挥战车100%的性能，自治联盟斥耗巨资开展实战模拟，#c186506开垦了专属的对抗场地，投入了几十批重型战车#n。
	add_talk #b#b在这里，你将体验到沙场上战车与炮火间的疯狂对抗。
	
	option 100 #轰炸大战
	
	talk

endproc

proc 100
	dialog 1
	add_talk $_npc$:
	add_talk #c186506活动时间：
	add_talk #b#b每天12:10-12:20，16:40-16:50#n
	add_talk #b#b
	add_talk #c005ebe活动玩法：
	add_talk #b#b尽可能击毁敌方阵营更多的战车#n
	add_talk #b#b
	add_talk #c6d00d2活动奖励：
	add_talk #b#b1. 只要参与即可获得大量经验！
	add_talk #b#b1. 只要参与即送【正义点数】！
	add_talk #b#b2. 击毁战车数排名前60%可获得超值银币！#n
	
	option 200 #参加【轰炸大战】
	
	talk

endproc 

proc 200 
		GET_EVENT_STAT 10 107		
	
		IF 10 = 1
			enter_pworld 302 0 0
		ELSE
			add_sys_msg_start
			add_sys_msg 维修员仍在检修战车中，暂时无法参加【轰炸大战】，请稍后再来
			set_sys_msg_mode 4 0 0
			msg_info
		ENDIF
endproc
