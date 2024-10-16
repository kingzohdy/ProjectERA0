
;智者
proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b浩瀚的宇宙中，人类就如同那微弱的星尘。
	add_talk #b#b我们总希望探索更多的奥秘，然而能帮助我们的只有无尽的智慧。
	
	option 151 【文字密码】活动
	option 150 【智力竞赛】说明
	
	talk
endproc

proc 150

	dialog 1
	add_talk $_npc$:
	add_talk #b#b参加竞赛的时候，建议前往安全区域，以免出现意外损失！
	add_talk #b#b竞赛积分和答题速度的快慢有关，选择正确答案的速度越快，得分越高。
	add_talk #b#b灵感模式：在阅题期间，点击灵感之星，可以开启灵感模式，在答题时自动选择答案。
	add_talk #b#b竞赛奖励：奖励和自身等级有关，同时排名越高，奖励越高！ 
	talk
endproc 

proc 151
	dialog 1
	add_talk $_npc$:
	add_talk #c186506活动时间：
	add_talk #b#b每天任何时间#n
	add_talk #c005ebe活动玩法：
	add_talk #b#b时间内按词语正确的汉字顺序逐个击破汉字#n
	add_talk #b#b
	add_talk #c6d00d2活动奖励：
	add_talk #b#b1. 顺利完成10道题目，即可领取银币奖励
	add_talk #b#b2. 成功次数越多，奖励银币越多#n
	add_talk #b#b
	add_talk #c186506参加条件：
	add_talk #b#b每次参加活动需要交出3个“智者钥匙”#n
	
	option 154 参加【文字密码】活动
	option 155 哪里找到【智者钥匙】
	option 1 返回
	
	talk
endproc 

proc 152
	strvar num ret
	GET_ITEM_NUM_INPAK 5063100 0 num
	if num >= 3
		enter_pworld 250 1 ret
		if ret = 0
			DEC_ITEM_NUM_INPAK 5063100 3 0
		endif
	else
		add_sys_msg_start
		add_sys_msg 背包中找不到3个“智者钥匙”，暂时无法参加【文字密码】活动
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc

proc 154
	;enter_pworld 250 1 1
	choose 2 152 153 进入“解码秘境”需要交出3个“智者钥匙”\n是否确认前往“解码秘境”？
endproc 

proc 155
	dialog 1
	add_talk $_npc$:
	add_talk 钥匙用途：
	add_talk #b#b每次参加活动需要交出3个“智者钥匙”
	add_talk #b#b
	add_talk #c186506获取途径：
	add_talk #b#b1. 击杀60级以上专家副本的最终头目掉落
	add_talk #b#b2. 采集【欢乐嘉年华】活动的宝箱有几率掉落
	add_talk #b#b3. 完成公会环组任务奖励#n
	
	option 151 返回
	
	talk
endproc
