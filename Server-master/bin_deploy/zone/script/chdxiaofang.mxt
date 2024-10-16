
;小方
proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b5555，#060#060#060，这位#Gender3#，为什么打怪会这么难呀，为什么我总是死呀，为什么没人愿意组我呀，为什么#060#060#060，为什么#060#060#060，为什么呀 #060#060#060！
	add_talk #b#b我是零纪元新人，看你好像很厉害的样子，你愿意帮帮我么？
	
	strvar ret
	task_stat 30014 1 ret
	if ret = 2
		option 150 #举手之劳！
	endif
	talk
endproc
proc 150
	dialog 3
	add_talk $_npc$:
	
	get_task_var 30014 0 8 1 10 
	if 10 = 0
		add_talk #b#b听说，这世界上有种能够瞬间回复生命的药物，好像叫什么#c186506普通生命胶囊#n，你能给我一个吗？
	endif
	if 10 = 1
		add_talk #b#b听说这世界上有种能够瞬间回复魔法的药物，好像叫什么#c186506普通魔法药片#n，你能给我一个吗？
	endif
	if 10 = 2
		add_talk #b#b听说这世界上有种能让人#c186506回时间城的传送卷轴#n，你能给我一个吗？
	endif
	
	add_talk #b#b
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
	
	get_task_var 30014 0 8 1 10 
	if 10 = 0
		if itid = 5010101
			DEC_ITEM_NUM_BYIDX 5010101 1 @pos @ge 1
			task_fini 30014 1
			call 300
		else
			add_sys_msg_start
			add_sys_msg 前辈，不带这样的，人家想要普通生命胶囊
			set_sys_msg_mode 3 0 0
			msg_info
			
		endif
	endif
	if 10 = 1
		if itid = 5011101
			DEC_ITEM_NUM_BYIDX 5011101 1 @pos @ge 1
			task_fini 30014 1
			call 300
		else
			add_sys_msg_start
			add_sys_msg 前辈，不带这样的，人家想要普通魔法药片
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if 10 = 2
		if itid = 5041000
			DEC_ITEM_NUM_BYIDX 5041000 1 @pos @ge 1
			task_fini 30014 1
			call 300
		else
			add_sys_msg_start
			add_sys_msg 前辈，不带这样的，人家想要回时间城的传送卷轴
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif

endproc 

proc 300
	add_sys_msg_start
;	add_sys_msg 成功上交物品，任务完成
	set_sys_msg_mode 3 0 0
	msg_info
	PLAYER_CLOSE_DIALOG 0 0
endproc 












