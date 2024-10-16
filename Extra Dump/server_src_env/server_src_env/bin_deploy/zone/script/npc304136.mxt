;304136	马福

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b你马上就会发现，我们反调五人组比某些家伙更高等，无论是智商还是能力！
	
	; task_stat 4547 0 101 保留
	; task_stat 4578 0 102 待用
	; task_stat 4549 0 103 保留
	; task_stat 4550 0 104 保留
	task_stat 4579 0 105
	task_stat 4580 0 106
	task_stat 4581 0 107
	task_stat 4582 0 108
	task_stat 4583 0 109
	task_stat 4556 0 110
	task_stat 4557 0 111
	task_stat 4558 0 112
	task_stat 4559 0 113
	task_stat 4560 0 114
	task_stat 4584 0 115
	task_stat 4585 0 116
	task_stat 4586 0 117
	; task_stat 4587 0 118 待用
	task_stat 4588 0 119
	task_stat 4589 0 120
	task_stat 4590 0 121
	; task_stat 4591 0 122 待用
	; task_stat 4592 0 123 待用
	; task_stat 4593 0 124 待用
	; task_stat 4571 0 125 待用
	task_stat 4572 0 126
	
	if 101 = 2 or 102 = 2 or 103 = 2 or 104 = 2 or 105 = 2 or 106 = 2 or 107 = 2 or 108 = 2 or 109 = 2 or 110 = 2
		option 200 #少年帕克的烦恼-提交物品
	elseif 111 = 2 or 112 = 2 or 113 = 2 or 114 = 2 or 115 = 2 or 116 = 2 or 117 = 2 or 118 = 2 or 119 = 2 or 120 = 2
		option 200 #少年帕克的烦恼-提交物品
	else 
		if 121 = 2 or 122 = 2 or 123 = 2 or 124 = 2 or 125 = 2 or 126 = 2
			option 200 #少年帕克的烦恼-提交物品
		endif
	endif
	
	; task_add_option 4547 1 101
	; task_add_option 4578 1 102
	; task_add_option 4549 1 103
	; task_add_option 4550 1 104
	task_add_option 4579 1 105
	task_add_option 4580 1 106
	task_add_option 4581 1 107
	task_add_option 4582 1 108
	task_add_option 4583 1 109
	; task_add_option 4556 1 110
	; task_add_option 4557 1 111
	; task_add_option 4558 1 112
	; task_add_option 4559 1 113
	; task_add_option 4560 1 114
	task_add_option 4584 1 115
	task_add_option 4585 1 116
	task_add_option 4586 1 117
	; task_add_option 4587 1 118
	task_add_option 4588 1 119
	task_add_option 4589 1 120
	task_add_option 4590 1 121
	; task_add_option 4591 1 122
	; task_add_option 4592 1 123
	; task_add_option 4593 1 124
	; task_add_option 4571 1 125
	; task_add_option 4572 1 126
	
	talk
endproc 

; PROC 101
	; task_do_option 4547 0
; ENDPROC
; PROC 102
	; task_do_option 4578 0
; ENDPROC
; PROC 103
	; task_do_option 4549 0
; ENDPROC
; PROC 104
	; task_do_option 4550 0
; ENDPROC
PROC 105
	task_do_option 4579 0
ENDPROC
PROC 106
	task_do_option 4580 0
ENDPROC
PROC 107
	task_do_option 4581 0
ENDPROC
PROC 108
	task_do_option 4582 0
ENDPROC
PROC 109
	task_do_option 4583 0
ENDPROC
; PROC 110
	; task_do_option 4556 0
; ENDPROC
; PROC 111
	; task_do_option 4557 0
; ENDPROC
; PROC 112
	; task_do_option 4558 0
; ENDPROC
; PROC 113
	; task_do_option 4559 0
; ENDPROC
; PROC 114
	; task_do_option 4560 0
; ENDPROC
PROC 115
	task_do_option 4584 0
ENDPROC
PROC 116
	task_do_option 4585 0
ENDPROC
PROC 117
	task_do_option 4586 0
ENDPROC
; PROC 118
	; task_do_option 4587 0
; ENDPROC
PROC 119
	task_do_option 4588 0
ENDPROC
PROC 120
	task_do_option 4589 0
ENDPROC
PROC 121
	task_do_option 4590 0
ENDPROC
; PROC 122
	; task_do_option 4591 0
; ENDPROC
; PROC 123
	; task_do_option 4592 0
; ENDPROC
; PROC 124
	; task_do_option 4593 0
; ENDPROC
; PROC 125
	; task_do_option 4571 0
; ENDPROC
; PROC 126
	; task_do_option 4572 0
; ENDPROC

proc 200
	dialog 3
	add_talk $_npc$:
	add_talk #b#b能让我瞧瞧你带来了什么吗？
	add_talk #b#b
	
	; task_stat 4547 0 101 保留
	; task_stat 4578 0 102 待用
	; task_stat 4549 0 103 保留
	; task_stat 4550 0 104 保留
	task_stat 4579 0 105
	task_stat 4580 0 106
	task_stat 4581 0 107
	task_stat 4582 0 108
	task_stat 4583 0 109
	task_stat 4556 0 110
	task_stat 4557 0 111
	task_stat 4558 0 112
	task_stat 4559 0 113
	task_stat 4560 0 114
	task_stat 4584 0 115
	task_stat 4585 0 116
	task_stat 4586 0 117
	; task_stat 4587 0 118 待用
	task_stat 4588 0 119
	task_stat 4589 0 120
	task_stat 4590 0 121
	; task_stat 4591 0 122 待用
	; task_stat 4592 0 123 待用
	; task_stat 4593 0 124 待用
	; task_stat 4571 0 125 待用
	task_stat 4572 0 126
	
	; if 101 = 2
		; add_talk #b#b#c186506收集1张藏宝图#n
	; endif
	; if 102 = 2
		; add_talk #b#b#c186506收集1个炼金制造的鉴定仪#n
	; endif
	; if 103 = 2
		; add_talk #b#b#c186506收集1个人物经验果子#n
	; endif
	; if 104 = 2
		; add_talk #b#b#c186506收集1个宠物经验果#n
	; endif
	if 105 = 2
		add_talk #b#b#c186506收集1个武器或防具进阶的碎块材料
		add_talk #b#b项链、戒指、护符进阶的碎块材料除外#n
	endif
	if 106 = 2
		add_talk #b#b#c186506收集1张（绿色品质）头盔部位的附魔卷轴#n
	endif
	if 107 = 2
		add_talk #b#b#c186506收集1张（绿色品质）胸甲部位的附魔卷轴#n
	endif
	if 108 = 2
		add_talk #b#b#c186506收集1张（绿色品质）主手部位的附魔卷轴#n
	endif
	if 109 = 2
		add_talk #b#b#c186506收集1张（绿色品质）肩甲部位的附魔卷轴#n
	endif
	if 110 = 2
		add_talk #b#b#c186506收集10个相同的装备进阶微粒
		add_talk #b#b项链、戒指、护符、面饰进阶材料除外#n
	endif
	if 111 = 2
		add_talk #b#b#c186506收集1个公会宝藏挖掘器#n
	endif
	if 112 = 2
		add_talk #b#b#c186506收集3张相同的附魔制作的符咒#n
	endif
	if 113 = 2
		add_talk #b#b#c186506收集3块相同的铸造制造的金属#n
	endif
	if 114 = 2
		add_talk #b#b#c186506收集3块相同的炼金提炼的合成油#n
	endif
	if 115 = 2
		add_talk #b#b#c186506收集1张（绿色品质）面饰部位的附魔卷轴#n
	endif
	if 116 = 2
		add_talk #b#b#c186506收集1张（绿色品质）靴子部位的附魔卷轴#n
	endif
	if 117 = 2
		add_talk #b#b#c186506收集1张（绿色品质）副手部位的附魔卷轴#n
	endif
	; if 118 = 2
		; add_talk #b#b#c186506收集10瓶炼金制作的恢复类药水#n
	; endif
	if 119 = 2
		add_talk #b#b#c186506收集1个精良的（绿色品质）戒指#n
	endif
	if 120 = 2
		add_talk #b#b#c186506收集1个精良的（绿色品质）护符#n
	endif
	if 121 = 2
		add_talk #b#b#c186506收集1个精良的（绿色品质）项链#n
	endif
	; if 122 = 2
		; add_talk #b#b#c186506收集10份炼金制作的药片或胶囊#n
	; endif
	; if 123 = 2
		; add_talk #b#b#c186506收集5瓶炼金制作的合剂#n
	; endif
	; if 124 = 2
		; add_talk #b#b#c186506收集3瓶炼金制作的属性药水#n
		; add_talk #b#b例如：微效灵巧药水、微效致命药水、微效精准药水、微效战斗药水等#n
	; endif
	; if 125 = 2
		; add_talk #b#b
	; endif
	if 126 = 2
		add_talk #b#b#c186506收集10份相同的科技生产的辅助材料
		add_talk #b#b例如：漏时之沙、地热石心等#n
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

	task_stat 4547 0 101
	if 101 = 2
		if @ID = 5062000 or @ID = 5062003
			switch @ID
				case 5062000
					DEC_ITEM_NUM_BYIDX 5062000 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4547 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5062003
					DEC_ITEM_NUM_BYIDX 5062003 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4547 0
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
	
	task_stat 4549 0 103
	if 103 = 2
		if @ID = 5250101 or @ID = 5250102
			switch @ID
				case 5250101
					DEC_ITEM_NUM_BYIDX 5250101 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4549 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5250102
					DEC_ITEM_NUM_BYIDX 5250102 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4549 0
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
	
	task_stat 4550 0 104
	if 104 = 2
		if @ID = 5230001 or @ID = 5230002 or @ID = 5230003 or @ID = 5230004 or @ID = 5230005
			switch @ID
				case 5230001
					DEC_ITEM_NUM_BYIDX 5230001 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4550 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5230002
					DEC_ITEM_NUM_BYIDX 5230002 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4550 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5230003
					DEC_ITEM_NUM_BYIDX 5230003 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4550 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5230004
					DEC_ITEM_NUM_BYIDX 5230004 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4550 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5230005
					DEC_ITEM_NUM_BYIDX 5230005 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4550 0
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
	
	task_stat 4556 0 110
	if 110 = 2
		if @ID = 5060200 or @ID = 5060201 or @ID = 5060202 or @ID = 5060203
			switch @ID
				case 5060200
					DEC_ITEM_NUM_BYIDX 5060200 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4556 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5060201
					DEC_ITEM_NUM_BYIDX 5060201 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4556 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5060202
					DEC_ITEM_NUM_BYIDX 5060202 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4556 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5060203
					DEC_ITEM_NUM_BYIDX 5060203 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4556 0
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
	
	task_stat 4557 0 111
	if 111 = 2
		if @ID = 5052101 or @ID = 5052102 or @ID = 5052103
			switch @ID
				case 5052101
					DEC_ITEM_NUM_BYIDX 5052101 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4557 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5052102
					DEC_ITEM_NUM_BYIDX 5052102 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4557 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5052103
					DEC_ITEM_NUM_BYIDX 5052103 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4557 0
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
	
	task_stat 4558 0 112
	if 112 = 2
		if @ID = 5109905 or @ID = 5109910 or @ID = 5109917 or @ID = 5109918 or @ID = 5109919 or @ID = 5109920
			switch @ID
				case 5109905
					DEC_ITEM_NUM_BYIDX 5109905 3 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4558 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5109910
					DEC_ITEM_NUM_BYIDX 5109910 3 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4558 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5109917
					DEC_ITEM_NUM_BYIDX 5109917 3 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4558 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5109918
					DEC_ITEM_NUM_BYIDX 5109918 3 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4558 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5109919
					DEC_ITEM_NUM_BYIDX 5109919 3 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4558 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5109920
					DEC_ITEM_NUM_BYIDX 5109920 3 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4558 0
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
	
	task_stat 4559 0 113
	if 113 = 2
		if @ID = 5109903 or @ID = 5109906 or @ID = 5109907 or @ID = 5109909 or @ID = 5109915 or @ID = 5109916
			switch @ID
				case 5109903
					DEC_ITEM_NUM_BYIDX 5109903 3 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4559 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5109906
					DEC_ITEM_NUM_BYIDX 5109906 3 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4559 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5109907
					DEC_ITEM_NUM_BYIDX 5109907 3 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4559 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5109909
					DEC_ITEM_NUM_BYIDX 5109909 3 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4559 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5109915
					DEC_ITEM_NUM_BYIDX 5109915 3 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4559 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5109916
					DEC_ITEM_NUM_BYIDX 5109916 3 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4559 0
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
	
	task_stat 4560 0 114
	if 114 = 2
		if @ID = 5109904 or @ID = 5109908 or @ID = 5109911 or @ID = 5109912 or @ID = 5109913 or @ID = 5109914
			switch @ID
				case 5109904
					DEC_ITEM_NUM_BYIDX 5109904 3 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4560 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5109908
					DEC_ITEM_NUM_BYIDX 5109908 3 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4560 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5109911
					DEC_ITEM_NUM_BYIDX 5109911 3 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4560 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5109912
					DEC_ITEM_NUM_BYIDX 5109912 3 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4560 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5109913
					DEC_ITEM_NUM_BYIDX 5109913 3 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4560 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5109914
					DEC_ITEM_NUM_BYIDX 5109914 3 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4560 0
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
	
	task_stat 4572 0 126
	if 126 = 2
		if @ID = 5109501 or @ID = 5109502 or @ID = 5109503 or @ID = 5109504 or @ID = 5109505 or @ID = 5109506
			switch @ID
				case 5109501
					DEC_ITEM_NUM_BYIDX 5109501 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4572 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5109502
					DEC_ITEM_NUM_BYIDX 5109502 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4572 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5109503
					DEC_ITEM_NUM_BYIDX 5109503 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4572 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5109504
					DEC_ITEM_NUM_BYIDX 5109504 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4572 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5109505
					DEC_ITEM_NUM_BYIDX 5109505 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4572 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5109506
					DEC_ITEM_NUM_BYIDX 5109506 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4572 0
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
	
	task_stat 4579 0 105
	if 105 = 2
		if @ID = 5060100 or @ID = 5060101 or @ID = 5060102 or @ID = 5060103
			switch @ID
				case 5060100
					DEC_ITEM_NUM_BYIDX 5060100 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4579 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5060101
					DEC_ITEM_NUM_BYIDX 5060101 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4579 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5060102
					DEC_ITEM_NUM_BYIDX 5060102 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4579 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5060103
					DEC_ITEM_NUM_BYIDX 5060103 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4579 0
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
	
	task_stat 4580 0 106
	if 106 = 2
		if @ID = 5240301 or @ID = 5240311 or @ID = 5240321 or @ID = 5240331
			switch @ID
				case 5240301
					DEC_ITEM_NUM_BYIDX 5240301 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4580 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5240311
					DEC_ITEM_NUM_BYIDX 5240311 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4580 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5240321
					DEC_ITEM_NUM_BYIDX 5240321 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4580 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5240331
					DEC_ITEM_NUM_BYIDX 5240331 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4580 0
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
	
	task_stat 4581 0 107
	if 107 = 2
		if @ID = 5241001 or @ID = 5241101 or @ID = 5241011 or @ID = 5241111 or @ID = 5241021 or @ID = 5241121 or @ID = 5241131 or @ID = 5241031
			switch @ID
				case 5241001
					DEC_ITEM_NUM_BYIDX 5241001 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4581 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5241101
					DEC_ITEM_NUM_BYIDX 5241101 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4581 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5241011
					DEC_ITEM_NUM_BYIDX 5241011 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4581 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5241111
					DEC_ITEM_NUM_BYIDX 5241111 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4581 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5241021
					DEC_ITEM_NUM_BYIDX 5241021 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4581 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5241121
					DEC_ITEM_NUM_BYIDX 5241121 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4581 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5241131
					DEC_ITEM_NUM_BYIDX 5241131 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4581 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5241031
					DEC_ITEM_NUM_BYIDX 5241031 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4581 0
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
	task_stat 4582 0 108
	if 108 = 2
		if @ID = 5240001 or @ID = 5240101 or @ID = 5240011 or @ID = 5240111 or @ID = 5240021 or @ID = 5240121 or @ID = 5240131 or @ID = 5240031
			switch @ID
				case 5240001
					DEC_ITEM_NUM_BYIDX 5240001 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4582 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5240101
					DEC_ITEM_NUM_BYIDX 5240101 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4582 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5240011
					DEC_ITEM_NUM_BYIDX 5240011 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4582 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5240111
					DEC_ITEM_NUM_BYIDX 5240111 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4582 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5240021
					DEC_ITEM_NUM_BYIDX 5240021 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4582 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5240121
					DEC_ITEM_NUM_BYIDX 5240121 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4582 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5240131
					DEC_ITEM_NUM_BYIDX 5240131 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4582 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5240031
					DEC_ITEM_NUM_BYIDX 5240031 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4582 0
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
	task_stat 4583 0 109
	if 109 = 2
		if @ID = 5240701 or @ID = 5240711 or @ID = 5240721 or @ID = 5240931
			switch @ID
				case 5240701
					DEC_ITEM_NUM_BYIDX 5240701 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4583 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5240711
					DEC_ITEM_NUM_BYIDX 5240711 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4583 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5240721
					DEC_ITEM_NUM_BYIDX 5240721 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4583 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5240931
					DEC_ITEM_NUM_BYIDX 5240931 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4583 0
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
	
	task_stat 4584 0 115
	if 115 = 2
		if @ID = 5240901 or @ID = 5240911 or @ID = 5240921 or @ID = 5240731
			switch @ID		
				case 5240901
					DEC_ITEM_NUM_BYIDX 5240901 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4584 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5240911
					DEC_ITEM_NUM_BYIDX 5240911 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4584 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5240921
					DEC_ITEM_NUM_BYIDX 5240921 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4584 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5240731
					DEC_ITEM_NUM_BYIDX 5240731 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4584 0
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
	
	task_stat 4585 0 116
	if 116 = 2
		if @ID = 5240501 or @ID = 5240511 or @ID = 5240521 or @ID = 5240531
			switch @ID
				case 5240501
					DEC_ITEM_NUM_BYIDX 5240501 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4585 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5240511
					DEC_ITEM_NUM_BYIDX 5240511 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4585 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5240521
					DEC_ITEM_NUM_BYIDX 5240521 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4585 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5240531
					DEC_ITEM_NUM_BYIDX 5240531 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4585 0
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
	
	task_stat 4586 0 117
	if 117 = 2
		if @ID = 5241301 or @ID = 5241201 or @ID = 5241311 or @ID = 5241211 or @ID = 5241321 or @ID = 5241221 or @ID = 5241231 or @ID = 5241331
			switch @ID
				case 5241301
					DEC_ITEM_NUM_BYIDX 5241301 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4586 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5241201
					DEC_ITEM_NUM_BYIDX 5241201 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4586 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5241311
					DEC_ITEM_NUM_BYIDX 5241311 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4586 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5241211
					DEC_ITEM_NUM_BYIDX 5241211 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4586 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5241321
					DEC_ITEM_NUM_BYIDX 5241321 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4586 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5241221
					DEC_ITEM_NUM_BYIDX 5241221 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4586 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5241231
					DEC_ITEM_NUM_BYIDX 5241231 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4586 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5241331
					DEC_ITEM_NUM_BYIDX 5241331 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4586 0
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
	
	task_stat 4588 0 119
	if 119 = 2
		if @ID = 2913003 or @ID = 2923003 or @ID = 2913004 or @ID = 2923004 or @ID = 2913005 or @ID = 2923005 or @ID = 2913006 or @ID = 2923006 or @ID = 2913007 or @ID = 2923007 or @ID = 2913008 or @ID = 2923008 or @ID = 2913009 or @ID = 2923009
			switch @ID
				case 2913003
					DEC_ITEM_NUM_BYIDX 2913003 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4588 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 2923003
					DEC_ITEM_NUM_BYIDX 2923003 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4588 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 2913004
					DEC_ITEM_NUM_BYIDX 2913004 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4588 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 2923004
					DEC_ITEM_NUM_BYIDX 2923004 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4588 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 2913005
					DEC_ITEM_NUM_BYIDX 2913005 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4588 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 2923005
					DEC_ITEM_NUM_BYIDX 2923005 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4588 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 2913006
					DEC_ITEM_NUM_BYIDX 2913006 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4588 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 2923006
					DEC_ITEM_NUM_BYIDX 2923006 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4588 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 2913007
					DEC_ITEM_NUM_BYIDX 2913007 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4588 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 2923007
					DEC_ITEM_NUM_BYIDX 2923007 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4588 0
							call 202
						ENDIF
					ENDIF
				endcase		
				case 2913008
					DEC_ITEM_NUM_BYIDX 2913008 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4588 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 2923008
					DEC_ITEM_NUM_BYIDX 2923008 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4588 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 2913009
					DEC_ITEM_NUM_BYIDX 2913009 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4588 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 2923009
					DEC_ITEM_NUM_BYIDX 2923009 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4588 0
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

	task_stat 4589 0 120
	if 120 = 2
		if @ID = 2713006 or @ID = 2713007 or @ID = 2713008 or @ID = 2713009 or @ID = 2723005 or @ID = 2723007 or @ID = 2723008 or @ID = 2723009
			switch @ID
				case 2713006
					DEC_ITEM_NUM_BYIDX 2713006 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4589 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 2713007
					DEC_ITEM_NUM_BYIDX 2713007 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4589 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 2713008
					DEC_ITEM_NUM_BYIDX 2713008 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4589 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 2713009
					DEC_ITEM_NUM_BYIDX 2713009 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4589 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 2723005
					DEC_ITEM_NUM_BYIDX 2723005 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4589 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 2723007
					DEC_ITEM_NUM_BYIDX 2723007 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4589 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 2723008
					DEC_ITEM_NUM_BYIDX 2723008 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4589 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 2723009
					DEC_ITEM_NUM_BYIDX 2723009 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4589 0
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

	task_stat 4590 0 121
	if 121 = 2
		if @ID = 2813004 or @ID = 2813005 or @ID = 2813006 or @ID = 2813007 or @ID = 2813008 or @ID = 2813009
			switch @ID
				case 2813004
					DEC_ITEM_NUM_BYIDX 2813004 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4590 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 2813005
					DEC_ITEM_NUM_BYIDX 2813005 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4590 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 2813006
					DEC_ITEM_NUM_BYIDX 2813006 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4590 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 2813007
					DEC_ITEM_NUM_BYIDX 2813007 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4590 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 2813008
					DEC_ITEM_NUM_BYIDX 2813008 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4590 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 2813009
					DEC_ITEM_NUM_BYIDX 2813009 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4590 0
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
