
;小志
proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b昨天和小白他们斗宠，竟然输了！
	add_talk #b#b#046#046#046#046#046
	add_talk #b#b今天我一定要找到一只彪悍的宠物，赢得比赛！你愿意帮我吗？
	add_talk #b#b当然我不会让你当我的宠物，去参加比赛啦，只是希望你能送个宠物蛋给我，可以吗？
		
	strvar ret
	task_stat 30015 1 ret
	if ret = 2
		option 150 #你想要什么宠物蛋呢？
	endif
	talk
endproc

proc 150
	dialog 3
	add_talk $_npc$:
	get_task_var 30015 0 8 1 10	
	if 10 = 0
		add_talk #b#b我想要一只#c186506嘟嘟鸟宠物蛋#n，可以不？
	endif
	if 10 = 1
		add_talk #b#b我想要一只#c186506野猪宠物蛋#n，可以不？
	endif
	if 10 = 2
		add_talk #b#b我想要一只#c186506孢子人宠物蛋#n，可以不？
	endif
	if 10 = 3
		add_talk #b#b我想要一只#c186506沼泽巨鳄宠物蛋#n，可以不？
	endif
	if 10 = 4
		add_talk #b#b我想要一只#c186506秃鹫宠物蛋#n，可以不？
	endif
	if 10 = 5
		add_talk #b#b我想要一只#c186506幽光幼龙宠物蛋#n，可以不？
	endif
	if 10 = 6
		add_talk #b#b我想要一只#c186506剑齿虎宠物蛋#n，可以不？
	endif
	
		add_talk #b#b将需要提交的物品拖至下方的框内
		
		NPC_COMMIT_OPT 160

	talk
endproc  

proc 160
	strvar itid pos ge
	GET_COMMIT_ITEM_INFO 1 itid
	
	GET_COMMIT_ITEM_INFO 2 pos
	;位置
	GET_COMMIT_ITEM_INFO 3 ge
	;格子
	
	get_task_var 30015 0 8 1 10 
	if 10 = 0
		if itid = 5610100
			DEC_ITEM_NUM_BYIDX 5610100 1 @pos @ge 1
			task_fini 30015 1
			call 300
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是嘟嘟鸟宠物蛋？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if 10 = 1
		if itid = 5610101
			DEC_ITEM_NUM_BYIDX 5610101 1 @pos @ge 1
			task_fini 30015 1
			call 300
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是野猪宠物蛋？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if 10 = 2
		if itid = 5610102
			DEC_ITEM_NUM_BYIDX 5610102 1 @pos @ge 1
			task_fini 30015 1
			call 300
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是孢子人宠物蛋？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if 10 = 3
		if itid = 5611116
			DEC_ITEM_NUM_BYIDX 5611116 1 @pos @ge 1
			task_fini 30015 1
			call 300
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是沼泽巨鳄宠物蛋？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if 10 = 4
		if itid = 5610104
			DEC_ITEM_NUM_BYIDX 5610104 1 @pos @ge 1
			task_fini 30015 1
			call 300
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是秃鹫宠物蛋？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if 10 = 5
		if itid = 5610106
			DEC_ITEM_NUM_BYIDX 5610106 1 @pos @ge 1
			task_fini 30015 1
			call 300
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是幽光幼龙宠物蛋？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if 10 = 6
		if itid = 5610107
			DEC_ITEM_NUM_BYIDX 5610107 1 @pos @ge 1
			task_fini 30015 1
			call 300
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是剑齿虎宠物蛋？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
endproc  

proc 300
	add_sys_msg_start
;	add_sys_msg 成功上交物品，任务完成
	set_sys_msg_mode 1 0 0
	msg_info
	PLAYER_CLOSE_DIALOG 0 0
endproc 












