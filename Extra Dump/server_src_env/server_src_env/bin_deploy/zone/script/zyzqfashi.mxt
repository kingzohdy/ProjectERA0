;法师试炼师艾斯

proc 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b灵活运用你的大脑，它会让你更强大。
		
	get_task_var 3158 0 8 1 20
	task_stat 3158 1 25
	if 20 = 0 and 25 = 2
		option 150 #试炼一：击杀，阻止试炼幽魂
	endif
	get_pworld_var 4 8 1 31
	task_stat 3158 1 25
	
	if 31 = 3 and 25 = 2
	
		option 151 #试炼二：击杀，阻止试炼邪骸
	endif
	option 152 #回到时间城

	talk

endproc

proc 150
	
	get_task_var 3158 0 8 1 20
	var 20 1
	set_task_var 3158 0 8 @20 1
	
	GET_MAP_NPC_ID 302058 50 0 
	mon_speech @50 0 2 1 用一切方法去阻止试炼幽魂到达我这。

	EFFECT_ID 0 @50 playMontion(%d,"294005")
	
		add_sys_msg_start
		add_sys_msg 试着用“冰箭术”降低试炼幽魂的移动速度，在其到达艾斯位置前把其击杀。
		set_sys_msg_mode 1 0 0
		msg_info
endproc
 
proc 151
	
	get_task_var 3158 2 8 1 22
	var 22 1
	set_task_var 3158 2 8 @22 1	
	
	get_pworld_var 4 8 1 31
	var 31 4
	set_pworld_var 4 8 @31 1
	
	GET_MAP_NPC_ID 302058 50 0 
	mon_speech @50 0 2 1 与第一轮试炼一样，尽量保证三个以下的试炼邪骸到达我这，要成功请灵活运用你的头脑。
	EFFECT_ID 0 @50 playMontion(%d,"294005")
	
	
	add_sys_msg_start
	add_sys_msg 合理运用“冰霜之环”“天火流星”，在试炼邪骸到达艾斯位置前把其击杀。
	set_sys_msg_mode 1 0 0
	msg_info
endproc

proc 152
	transmit 1000 23800 27400 1
	get_pworld_var 0 32 1 30
	var 30 0
	set_pworld_var 0 32 @30 1
	
;	RESET_PWORLD 508
	
	task_stat 3158 1 30
	if 30 != 1
		TASK_FAIL 3158 1
	endif
	
	get_pworld_var 0 32 1 30
	get_pworld_var 4 8 1 31
	get_pworld_var 5 8 1 32
	get_pworld_var 7 8 1 34
	var 30 0
	set_pworld_var 0 32 @30 1
	var 31 0
	set_pworld_var 4 8 @31 1
	var 32 0
	set_pworld_var 5 8 @32 1
	var 34 0
	set_pworld_var 7 8 @34 1
	

endproc


