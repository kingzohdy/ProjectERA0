;副本中的智者
proc 1

	strvar m
	get_pworld_var 24 32 1 m
	if m != 10
		dialog 1
		add_talk $_npc$:
		add_talk #b#b当你一切准备就绪后，我们随时可以开始解码。
		
		option 150 #我们开始吧，我已准备好了
		option 151 #【文字密码】说明
		
		talk
	else
		dialog 1
		add_talk $_npc$:
		add_talk #b#b看来你已经顺利完成了10道解码。
		add_talk #b#b这是属于你完成解码的奖励。
		
		option 152 领取解码奖励
		
		talk
	endif
	
endproc

proc 150
	strvar startflag
	var startflag 1
	set_pworld_var 0 32 @startflag 1
	player_close_dialog1 302129 1
	
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
	talk
endproc 

proc 152
	; get award
	strvar m p
	strvar sex
	strvar level
	strvar job
	get_pworld_var 48 32 1 m
	get_player_base_att 1 sex level job

	; ((level-60)*50+500)*m
	sub @level 60
	mul @level 50
	add @level 500
	mul @level @m
	
	get_pworld_var 56 32 1 p
	if p = 0
		add_money 0 @level 20 1
		new_npc 1 333162 7668 8980 180 csm333162.mac
	endif
	del_npc 302129 1
	
	; flag already get the price
	var p 1
	set_pworld_var 56 32 @p 1
endproc 












