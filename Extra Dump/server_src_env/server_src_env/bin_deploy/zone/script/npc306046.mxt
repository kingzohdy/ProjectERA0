;306046 希伯特·乔治


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b这是自由联盟颁布的红色击杀令，无论是谁，都需要配合击杀凶林霸主；凶林对于联盟的潜在威胁实在是太大了。
	add_talk #b#b你不知道它何时会对你发起进攻，它的存在等于在联盟背后有一把无形的刀一直时刻准备在你背后捅上一刀。
	add_talk #b#b
	add_talk #c005ebe#b#b报名后将随机将个人（队伍）分配到副本内！
		option 100 绿野凶林【单人报名】
		option 200 绿野凶林【队伍报名】
		
	TASK_ADD_OPTION 4641 0 300
	
	talk
endproc 

proc 100
	choose 2 101 102 是否确定需要绿野凶林【单人报名】？
endproc

proc 101
	apply_pworld 516 0 0
endproc

proc 200
	choose 2 201 102 是否确定需要绿野凶林【队伍报名】？
endproc

proc 201
	apply_pworld 516 1 0
endproc

proc 300
	task_do_option 4641 1
endproc
