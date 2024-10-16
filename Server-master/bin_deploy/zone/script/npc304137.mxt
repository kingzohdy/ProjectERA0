;304137	纳西

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b都说物竞天择，优胜劣汰，但为什么这世界总是还有那么多歪瓜劣枣？

	option 120 #(买卖)打开商店	
	
	strvar tat_1 tat_2 tat_3 num_1 num_2
	task_stat 4502 0 tat_1
	task_stat 4503 0 tat_2
	task_stat 4504 0 tat_3
	GET_ITEM_NUM_INPAK 5300161 0 num_1
	GET_ITEM_NUM_INPAK 5301021 0 num_2
	
	;if @tat_1 = 2 or @tat_2 = 2 or @tat_3 = 2 and @num_1 <= 1 and @num_2 <= 1
	;	option 900 #购买染丝工具包（2万绑银1个）
	;endif
	
	; task_add_option 4502 1 101
	; task_add_option 4503 1 102
	; task_add_option 4504 1 103
	; task_add_option 4505 1 104
	; task_add_option 4506 1 105
	; task_add_option 4507 1 106
	; task_add_option 4508 1 107
	; task_add_option 4509 1 108
	; task_add_option 4510 1 109
	; task_add_option 4511 1 110
	; task_add_option 4512 1 111
	
	task_stat 4578 0 112
	task_stat 4587 0 113
	task_stat 4591 0 114
	task_stat 4592 0 115
	task_stat 4593 0 116
	
	if 112 = 2 or 113 = 2 or 114 = 2 or 115 = 2 or 116 = 2
		option 200 #少年帕克的烦恼-提交物品
	endif
	
	; task_add_option 4578 1 112
	; task_add_option 4587 1 113
	; task_add_option 4591 1 114
	; task_add_option 4592 1 115
	; task_add_option 4593 1 116
	
	talk
endproc 

; PROC 101	
	; task_do_option 4502 0
; ENDPROC
; PROC 102	
	; task_do_option 4503 0
; ENDPROC
; PROC 103	
	; task_do_option 4504 0
; ENDPROC
; PROC 104	
	; task_do_option 4505 0
; ENDPROC
; PROC 105	
	; task_do_option 4506 0
; ENDPROC
; PROC 106	
	; task_do_option 4507 0
; ENDPROC
; PROC 107	
	; task_do_option 4508 0
; ENDPROC
; PROC 108	
	; task_do_option 4509 0
; ENDPROC
; PROC 109	
	; task_do_option 4510 0
; ENDPROC
; PROC 110	
	; task_do_option 4511 0
; ENDPROC
; PROC 111	
	; task_do_option 4512 0
; ENDPROC

; PROC 112
	; task_do_option 4578 0
; ENDPROC
; PROC 113
	; task_do_option 4587 0
; ENDPROC
; PROC 114
	; task_do_option 4591 0
; ENDPROC
; PROC 115
	; task_do_option 4592 0
; ENDPROC
; PROC 116
	; task_do_option 4593 0
; ENDPROC

proc 120
	OPEN_NPC_SHOP 
endproc 


; PROC 900
	; strvar ret
	; get_pack_space 1 ret
	; if ret >= 1
		; DEC_MONEY_TEST 0 2000 21 1
		; if 1 = 0
			; ADD_ITEM_NUM_INPAK 5300161 1 0 10
			; if 1 = 0
				; DEC_MONEY 0 2000 21 1
			; endif
		; else
			; ADD_SYS_MSG_START
			; ADD_SYS_MSG 你好像没有那么多钱！
			; SET_SYS_MSG_MODE 4 0 0
			; MSG_INFO
		; endif
		; ADD_SYS_MSG_START
		; ADD_SYS_MSG 背包剩余空间不足！
		; SET_SYS_MSG_MODE 1 0 0
		; MSG_INFO
	; endif
; ENDPROC

proc 200
	dialog 3
	add_talk $_npc$:
	add_talk #b#b能让我瞧瞧你带来了什么吗？
	add_talk #b#b
	
	task_stat 4578 0 112
	task_stat 4587 0 113
	task_stat 4591 0 114
	task_stat 4592 0 115
	task_stat 4593 0 116
	
	if 112 = 2
		add_talk #b#b#c186506收集1个炼金制造的鉴定仪#n
	endif
	if 113 = 2
		add_talk #b#b#c186506收集5瓶炼金制作的恢复类药水#n
	endif
	if 114 = 2
		add_talk #b#b#c186506收集10份炼金制作的药片或胶囊#n
	endif
	if 115 = 2
		add_talk #b#b#c186506收集1瓶炼金制作的合剂#n
	endif
	if 116 = 2
		add_talk #b#b#c186506收集1瓶炼金制作的属性药水#n
		add_talk #b#b例如：微效灵巧药水、微效致命药水、微效精准药水、微效战斗药水等#n
	endif
	
	talk
	
	NPC_COMMIT_OPT 201
	
endproc

proc 201

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
	
	task_stat 4578 0 112
	if 112 = 2
		if @ID = 5241510 or @ID = 5241511 or @ID = 5241512 or @ID = 5241513 or @ID = 5241514 or @ID = 5241515 or @ID = 5241516
			switch @ID
				case 5241510
					DEC_ITEM_NUM_BYIDX 5241510 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4578 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5241511
					DEC_ITEM_NUM_BYIDX 5241511 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4578 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5241512
					DEC_ITEM_NUM_BYIDX 5241512 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4578 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5241513
					DEC_ITEM_NUM_BYIDX 5241513 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4578 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5241514
					DEC_ITEM_NUM_BYIDX 5241514 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4578 0
							call 202
						ENDIF
					ENDIF
				endcase
								case 5241515
					DEC_ITEM_NUM_BYIDX 5241515 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4578 0
							call 202
						ENDIF
					ENDIF
				endcase
								case 5241516
					DEC_ITEM_NUM_BYIDX 5241516 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4578 0
							call 202
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
	
	task_stat 4587 0 113
	if 113 = 2
		if @ID = 5001002 or @ID = 5000002 or @ID = 5001003 or @ID = 5000003 or @ID = 5001004 or @ID = 5000004 or @ID = 5001001 or @ID = 5000001
			switch @ID
				case 5001002
					DEC_ITEM_NUM_BYIDX 5001002 5 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4587 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5000002
					DEC_ITEM_NUM_BYIDX 5000002 5 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4587 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5001003
					DEC_ITEM_NUM_BYIDX 5001003 5 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4587 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5000003
					DEC_ITEM_NUM_BYIDX 5000003 5 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4587 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5001004
					DEC_ITEM_NUM_BYIDX 5001004 5 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4587 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5000004
					DEC_ITEM_NUM_BYIDX 5000004 5 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4587 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5001001
					DEC_ITEM_NUM_BYIDX 5001001 5 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4587 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5000001
					DEC_ITEM_NUM_BYIDX 5000001 5 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4587 0
							call 202
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
	
	task_stat 4591 0 114
	if 114 = 2
		if @ID = 5011101 or @ID = 5010101 or @ID = 5011102 or @ID = 5010102 or @ID = 5010106 or @ID = 5011103 or @ID = 5010103 or @ID = 5011106
			switch @ID
				case 5011101
					DEC_ITEM_NUM_BYIDX 5011101 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4591 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5010101
					DEC_ITEM_NUM_BYIDX 5010101 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4591 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5011102
					DEC_ITEM_NUM_BYIDX 5011102 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4591 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5010102
					DEC_ITEM_NUM_BYIDX 5010102 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4591 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5010106
					DEC_ITEM_NUM_BYIDX 5010106 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4591 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5011103
					DEC_ITEM_NUM_BYIDX 5011103 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4591 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5010103
					DEC_ITEM_NUM_BYIDX 5010103 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4591 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5011106
					DEC_ITEM_NUM_BYIDX 5011106 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4591 0
							call 202
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
	
	task_stat 4592 0 115
	if 115 = 2
		if @ID = 5005126 or @ID = 5005124 or @ID = 5005125 or @ID = 5005127 or @ID = 5005128 or @ID = 5005129 or @ID = 5005226 or @ID = 5005224 or @ID = 5005225
			switch @ID
				case 5005126
					DEC_ITEM_NUM_BYIDX 5005126 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4592 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5005124
					DEC_ITEM_NUM_BYIDX 5005124 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4592 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5005125
					DEC_ITEM_NUM_BYIDX 5005125 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4592 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5005127
					DEC_ITEM_NUM_BYIDX 5005127 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4592 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5005128
					DEC_ITEM_NUM_BYIDX 5005128 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4592 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5005129
					DEC_ITEM_NUM_BYIDX 5005129 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4592 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5005226
					DEC_ITEM_NUM_BYIDX 5005226 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4592 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5005224
					DEC_ITEM_NUM_BYIDX 5005224 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4592 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5005225
					DEC_ITEM_NUM_BYIDX 5005225 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4592 0
							call 202
						ENDIF
					ENDIF
				endcase
			endswitch
		elseif @ID = 5005227 or @ID = 5005228 or @ID = 5005229 or @ID = 5005326 or @ID = 5005324 or @ID = 5005325 or @ID = 5005327 or @ID = 5005328 or @ID = 5005329
			switch @ID
				case 5005227
					DEC_ITEM_NUM_BYIDX 5005227 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4592 0
							call 202
						ENDIF
					ENDIF
				endcase		
				case 5005228
					DEC_ITEM_NUM_BYIDX 5005228 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4592 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5005229
					DEC_ITEM_NUM_BYIDX 5005229 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4592 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5005326
					DEC_ITEM_NUM_BYIDX 5005326 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4592 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5005324
					DEC_ITEM_NUM_BYIDX 5005324 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4592 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5005325
					DEC_ITEM_NUM_BYIDX 5005325 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4592 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5005327
					DEC_ITEM_NUM_BYIDX 5005327 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4592 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5005328
					DEC_ITEM_NUM_BYIDX 5005328 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4592 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5005329
					DEC_ITEM_NUM_BYIDX 5005329 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4592 0
							call 202
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
	
	task_stat 4593 0 116
	if 116 = 2
		if @ID = 5004125 or @ID = 5004126 or @ID = 5004124 or @ID = 5006310 or @ID = 5004225 or @ID = 5004226 or @ID = 5004224 or @ID = 5006311 or @ID = 5004325 or @ID = 5004326 or @ID = 5004324 or @ID = 5006312
			switch @ID 											
				case 5004125
					DEC_ITEM_NUM_BYIDX 5004125 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4593 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5004126
					DEC_ITEM_NUM_BYIDX 5004126 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4593 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5004124
					DEC_ITEM_NUM_BYIDX 5004124 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4593 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5006310
					DEC_ITEM_NUM_BYIDX 5006310 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4593 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5004225
					DEC_ITEM_NUM_BYIDX 5004225 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4593 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5004226
					DEC_ITEM_NUM_BYIDX 5004226 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4593 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5004224
					DEC_ITEM_NUM_BYIDX 5004224 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4593 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5006311
					DEC_ITEM_NUM_BYIDX 5006311 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4593 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5004325
					DEC_ITEM_NUM_BYIDX 5004325 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4593 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5004326
					DEC_ITEM_NUM_BYIDX 5004326 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4593 0
							call 202
						ENDIF
					ENDIF
				endcase		
				case 5004324
					DEC_ITEM_NUM_BYIDX 5004324 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4593 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5006312
					DEC_ITEM_NUM_BYIDX 5006312 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4593 0
							call 202
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

proc 202
	add_sys_msg_start
	add_sys_msg 成功上交物品，任务完成
	set_sys_msg_mode 3 0 0
	msg_info
	PLAYER_CLOSE_DIALOG 0 0
endproc

proc 203
	add_sys_msg_start
	add_sys_msg 物品不足要求数量，未能成功上交
	set_sys_msg_mode 3 0 0
	msg_info
endproc
