;珈纳

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b你想和奥斯卡兑换礼品吗，他可不是你找到就可以了，你得完成我给你的任务，我才会给你可以和他兑换礼物的【万圣糖果礼券】。
	add_talk #b#b每天可做10次，每次需要5个下列任意科技生产材料，非常物超所值呢！
	add_talk #c18650矿石：原质矿、密英矿、热能矿
	add_talk 油料：原生油、低构油、光合油
	add_talk 精华：秘境精华、宁静精华、冷川精华#n
	add_talk #c0181f9奥斯卡每天08:00-24:00 每10分钟随机在某些地图中出现，可以通过时间城-格莱美处查询奥斯卡当先所在位置。
	
	task_stat 4640 0 101
	if 101 = 2
		OPTION 300 #上交5个科技生活材料
	endif

	TASK_ADD_OPTION 4640 0 200
	
	talk
endproc

PROC 200
	task_do_option 4640 0
ENDPROC

PROC 300
	dialog 3
	add_talk $_npc$:
	add_talk #b#b能让我瞧瞧你带来了什么吗？
	add_talk #b#b
	add_talk #c186506收集5个以下任意的科技生产材料
	add_talk 矿石：原质矿、密英矿、热能矿
	add_talk 油料：原生油、低构油、光合油
	add_talk 精华：秘境精华、宁静精华、冷川精华
	
		NPC_COMMIT_OPT 301
	
	talk
ENDPROC

proc 301

	strvar ID Pos_1 Pos_2 ret
	GET_COMMIT_ITEM_INFO 1 ID
	GET_COMMIT_ITEM_INFO 2 Pos_1
	GET_COMMIT_ITEM_INFO 3 Pos_2
	
	if @ID = -1
		add_sys_msg_start
		add_sys_msg 你还没有提交物品！
		set_sys_msg_mode 1 0 0
		msg_info		
		return 
	endif

	task_stat 4640 0 100
	if 100 = 2
		if @ID = 5101001 or @ID = 5101002 or @ID = 5101003 or @ID = 5102001 or @ID = 5102002 or @ID = 5102003 or @ID = 5103001 or @ID = 5103002 or @ID = 5103003
			switch @ID
				case 5101001
					DEC_ITEM_NUM_BYIDX 5101001 5 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 303
					else
						if @ret = 0
							task_fini 4640 0
							call 302
						ENDIF
					ENDIF
				endcase
				case 5101002
					DEC_ITEM_NUM_BYIDX 5101002 5 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 303
					else
						if @ret = 0
							task_fini 4640 0
							call 302
						ENDIF
					ENDIF
				endcase	
				case 5101003
					DEC_ITEM_NUM_BYIDX 5101003 5 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 303
					else
						if @ret = 0
							task_fini 4640 0
							call 302
						ENDIF
					ENDIF
				endcase
				case 5102001
					DEC_ITEM_NUM_BYIDX 5102001 5 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 303
					else
						if @ret = 0
							task_fini 4640 0
							call 302
						ENDIF
					ENDIF
				endcase
				case 5102002
					DEC_ITEM_NUM_BYIDX 5102002 5 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 303
					else
						if @ret = 0
							task_fini 4640 0
							call 302
						ENDIF
					ENDIF
				endcase
				case 5102003
					DEC_ITEM_NUM_BYIDX 5102003 5 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 303
					else
						if @ret = 0
							task_fini 4640 0
							call 302
						ENDIF
					ENDIF
				endcase
				case 5103001
					DEC_ITEM_NUM_BYIDX 5103001 5 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 303
					else
						if @ret = 0
							task_fini 4640 0
							call 302
						ENDIF
					ENDIF
				endcase
				case 5103002
					DEC_ITEM_NUM_BYIDX 5103002 5 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 303
					else
						if @ret = 0
							task_fini 4640 0
							call 302
						ENDIF
					ENDIF
				endcase
				case 5103003
					DEC_ITEM_NUM_BYIDX 5103003 5 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 303
					else
						if @ret = 0
							task_fini 4640 0
							call 302
						ENDIF
					ENDIF
				endcase
			endswitch
		else
			add_sys_msg_start
			add_sys_msg 物品不符任务要求，未能成功上交
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
endproc
	
proc 302
	add_sys_msg_start
	add_sys_msg 成功上交物品，任务完成
	set_sys_msg_mode 3 0 0
	msg_info
	PLAYER_CLOSE_DIALOG 0 0
endproc

proc 303
	add_sys_msg_start
	add_sys_msg 物品不足要求数量，未能成功上交
	set_sys_msg_mode 3 0 0
	msg_info
endproc
