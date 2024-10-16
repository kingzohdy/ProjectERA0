;哈利（副本）

proc 1
	dialog 1
	add_talk 整理诀窍：
	add_talk #b#b剪刀石头包袱是一个无限循环的过程，被打（杀死）之后，就会变成另外一个，也就是说，无论您怎样努力，它们的数量是不会减的！
	add_talk #b#b
	add_talk 衍变规则：
	add_talk #b#b#c186506石头 → 剪刀
	add_talk #b#b剪刀 → 包袱
	add_talk #b#b包袱 → 石头#n
	add_talk 胜利条件：
	add_talk #b#b当计时结束时，逆往之间的怪物#c186506归于同一类型#n
	
	get_task_var 4614 1 8 1 10
	get_task_var 4614 2 8 1 11
	get_task_var 4614 3 8 1 12
	if 10 = 0 and 11 = 0 and 12 = 0
		add_talk #b#b
		add_talk 整理要求：开始整理后，我将告诉你整理要求
	elseif 10 = 1
		add_talk #b#b
		add_talk 整理要求：请把所有的剪刀石头布，整理为#c005ebe剪刀#n
	elseif 11 = 1
		add_talk #b#b
		add_talk 整理要求：请把所有的剪刀石头布，都整理为#c005ebe石头#n
	elseif 12 = 1
		add_talk #b#b
		add_talk 整理要求：请把所有的剪刀石头布，都整理为#c005ebe包袱#n
	else
	endif
	
	strvar ret
	task_stat 4614 1 ret
	if ret = 2
		option 100 #开始整理
		option 200 #整理完毕
	endif
		option 300 #谢谢指导
	
	talk
endproc

proc 100
	strvar num ran
	GET_MON_NUM 0 num
	
	if num <= 0
		NEW_MON 230056 1 12822 12737 1500 0 0 0 0
		NEW_MON 230057 1 12822 12737 1500 0 0 0 0
		NEW_MON 230058 1 12822 12737 1500 0 0 0 0
		
		add_sys_msg_start
		add_sys_msg 四周出现了匪夷所思的石头剪刀布！
		set_sys_msg_mode 4 0 0
		msg_info
		
		rand ran 2
		switch @ran
			case 0
				get_task_var 4614 1 8 0 10
				VAR 10 1
				set_task_var 4614 1 8 @10 0
				call 500
			endcase
			case 1
				get_task_var 4614 2 8 0 11
				VAR 11 1
				set_task_var 4614 2 8 @11 0
				call 500
			endcase
			case 2
				get_task_var 4614 3 8 0 12
				VAR 12 1
				set_task_var 4614 3 8 @12 0
				call 500
			endcase
		endswitch
	else
		add_sys_msg_start
		add_sys_msg 请先把现在的剪刀石头布整理好
		set_sys_msg_mode 4 0 0
		msg_info
	endif
	call 1
endproc

proc 200
	call 502
endproc

proc 300
;关闭面板
endproc


proc 500
	get_task_var 4614 0 8 0 50
	VAR 50 1
	set_task_var 4614 0 8 @50 0
endproc

proc 502
	strvar nMon_1 nMon_2 nMon_3
	GET_MAP_MON_NUM 51400 230056 0 nMon_1 0
	GET_MAP_MON_NUM 51400 230057 0 nMon_2 0
	GET_MAP_MON_NUM 51400 230058 0 nMon_3 0
	
	GET_TASK_VAR 4614 1 8 0 10
	if @10 = 1 and @nMon_1 >= 30
		TASK_FINI 4614 0
	else
		add_sys_msg_start
		add_sys_msg 请把所有的剪刀石头布，整理为剪刀
		set_sys_msg_mode 3 0 0
		msg_info
	endif
	
	GET_TASK_VAR 4614 2 8 0 11
	if @11 = 1 and @nMon_2 >= 30
		TASK_FINI 4614 0
	else
		add_sys_msg_start
		add_sys_msg 请把所有的剪刀石头布，整理为石头
		set_sys_msg_mode 3 0 0
		msg_info
	endif
	
	GET_TASK_VAR 4614 3 8 0 12
	if @12 = 1 and @nMon_3 >= 30
		TASK_FINI 4614 0
	else
		add_sys_msg_start
		add_sys_msg 请把所有的剪刀石头布，整理为包袱
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc
