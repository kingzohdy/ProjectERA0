;304133	福吉


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b要挺身而出对抗敌人需要很大的勇气，要挺身而出反抗朋友却需要更大的勇气，但这些都什么了不起，为了帮助朋友而散尽家财，才真正让人刮目！
	
	task_stat 4535 0 100
	; task_stat 4536 0 101
	; task_stat 4537 0 102
	task_stat 4538 0 103
	task_stat 4539 0 104
	task_stat 4540 0 105
	task_stat 4541 0 106
	task_stat 4542 0 107
	task_stat 4543 0 108
	task_stat 4544 0 109
	task_stat 4545 0 110
	task_stat 4546 0 111
	
	if 100 = 2 or 103 = 2 or 104 = 2 or 105 = 2 or 106 = 2 or 107 = 2 or 108 = 2 or 109 = 2 or 110 = 2 or 111 = 2
		option 200 #少年帕克的烦恼-提交物品
	endif
	
	task_add_option 4535 1 101
	task_add_option 4536 1 102
	task_add_option 4537 1 103
	task_add_option 4538 1 104
	; task_add_option 4539 1 105
	task_add_option 4540 1 106
	; task_add_option 4541 1 107
	; task_add_option 4542 1 108
	; task_add_option 4543 1 109
	; task_add_option 4544 1 110
	; task_add_option 4545 1 111
	; task_add_option 4546 1 112
	
	talk
endproc 

PROC 101	
	task_do_option 4535 0
ENDPROC
PROC 102	
	task_do_option 4536 0
ENDPROC
PROC 103	
	task_do_option 4537 0
ENDPROC
PROC 104	
	task_do_option 4538 0
ENDPROC
; PROC 105	
	; task_do_option 4539 0
; ENDPROC
PROC 106	
	task_do_option 4540 0
ENDPROC
; PROC 107	
	; task_do_option 4541 0
; ENDPROC
; PROC 108	
	; task_do_option 4542 0
; ENDPROC
; PROC 109	
	; task_do_option 4543 0
; ENDPROC
; PROC 110	
	; task_do_option 4544 0
; ENDPROC
; PROC 111	
	; task_do_option 4545 0
; ENDPROC
; PROC 112	
	; task_do_option 4546 0
; ENDPROC

proc 200
	dialog 3
	add_talk $_npc$:
	add_talk #b#b能让我瞧瞧你带来了什么吗？
	add_talk #b#b
	
	task_stat 4535 0 100
	; task_stat 4536 0 101
	; task_stat 4537 0 102
	task_stat 4538 0 103
	task_stat 4539 0 104
	task_stat 4540 0 105
	task_stat 4541 0 106
	task_stat 4542 0 107
	task_stat 4543 0 108
	task_stat 4544 0 109
	task_stat 4545 0 110
	task_stat 4546 0 111
	
	if 100 = 2
		add_talk #b#b#c186506收集2个相同的蒸汽科技制造的炸弹或火箭筒#n
	endif
	; if 101 = 2
		; add_talk #b#b#c186506收集1辆蒸汽科技制造的采集车#n
	; endif
	; if 102 = 2
		; add_talk #b#b#c186506收集1个蒸汽科技制造的采集场#n
	; endif
	if 103 = 2
		add_talk #b#b#c186506收集5个相同的蒸汽科技制造的机械补给品#n
	endif
	if 104 = 2
		add_talk #b#b#c186506收集1架蒸汽科技制造的蒸汽机械#n
	endif
	if 105 = 2
		add_talk #b#b#c186506收集2瓶相同的变身药水#n
	endif
	if 106 = 2
		add_talk #b#b#c186506收集3个相同的传送卷轴#n
	endif
	if 107 = 2
		add_talk #b#b#c186506收集10份相同的附魔生产材料#n
	endif
	if 108 = 2
		add_talk #b#b#c186506收集10份相同的炼金的油料#n
	endif
	if 109 = 2
		add_talk #b#b#c186506收集10块相同的铸造的矿石#n
	endif
	if 110 = 2
		add_talk #b#b#c186506收集1个装备镶嵌的一级宝石#n
	endif
	if 111 = 2
		add_talk #b#b#c186506收集1个装备镶嵌的二级宝石#n
	endif
	
		NPC_COMMIT_OPT 201
	
	talk
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

	task_stat 4535 0 100
	if 100 = 2
		if @ID = 5051006 or @ID = 5051007 or @ID = 5051012 or @ID = 5051048 or @ID = 5051049 or @ID = 5051114 or @ID = 5051108 or @ID = 5051109 or @ID = 5051110 or @ID = 5051111 or @ID = 5051015 or @ID = 5051000 or @ID = 5051001
			switch @ID
				case 5051006
					DEC_ITEM_NUM_BYIDX 5051006 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4535 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5051007
					DEC_ITEM_NUM_BYIDX 5051007 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4535 0
							call 202
						ENDIF
					ENDIF
				endcase	
				case 5051012
					DEC_ITEM_NUM_BYIDX 5051012 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4535 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5051048
					DEC_ITEM_NUM_BYIDX 5051048 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4535 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5051049
					DEC_ITEM_NUM_BYIDX 5051049 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4535 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5051114
					DEC_ITEM_NUM_BYIDX 5051114 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4535 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5051108
					DEC_ITEM_NUM_BYIDX 5051108 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4535 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5051109
					DEC_ITEM_NUM_BYIDX 5051109 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4535 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5051110
					DEC_ITEM_NUM_BYIDX 5051110 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4535 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5051111
					DEC_ITEM_NUM_BYIDX 5051111 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4535 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5051015
					DEC_ITEM_NUM_BYIDX 5051015 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4535 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5051000
					DEC_ITEM_NUM_BYIDX 5051000 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4535 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5051001
					DEC_ITEM_NUM_BYIDX 5051001 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4535 0
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
	
	; task_stat 4536 0 101
	; if 101 = 2
		; if @ID = 5051004 or @ID = 5051005 or @ID = 5051018 or @ID = 5051019 or @ID = 5051017 or @ID = 5051023 or @ID = 5051103 or @ID = 5051104
			; switch @ID
				; case 5051004
					; DEC_ITEM_NUM_BYIDX 5051004 1 @Pos_1 @Pos_2 ret
					; if @ret = -1
						; call 203
					; else
						; if @ret = 0
							; task_fini 4536 0
							; call 202
						; ENDIF
					; ENDIF
				; endcase
				; case 5051005
					; DEC_ITEM_NUM_BYIDX 5051005 1 @Pos_1 @Pos_2 ret
					; if @ret = -1
						; call 203
					; else
						; if @ret = 0
							; task_fini 4536 0
							; call 202
						; ENDIF
					; ENDIF
				; endcase
				; case 5051018
					; DEC_ITEM_NUM_BYIDX 5051018 1 @Pos_1 @Pos_2 ret
					; if @ret = -1
						; call 203
					; else
						; if @ret = 0
							; task_fini 4536 0
							; call 202
						; ENDIF
					; ENDIF
				; endcase
				; case 5051019
					; DEC_ITEM_NUM_BYIDX 5051019 1 @Pos_1 @Pos_2 ret
					; if @ret = -1
						; call 203
					; else
						; if @ret = 0
							; task_fini 4536 0
							; call 202
						; ENDIF
					; ENDIF
				; endcase
				; case 5051017
					; DEC_ITEM_NUM_BYIDX 5051017 1 @Pos_1 @Pos_2 ret
					; if @ret = -1
						; call 203
					; else
						; if @ret = 0
							; task_fini 4536 0
							; call 202
						; ENDIF
					; ENDIF
				; endcase
				; case 5051023
					; DEC_ITEM_NUM_BYIDX 5051023 1 @Pos_1 @Pos_2 ret
					; if @ret = -1
						; call 203
					; else
						; if @ret = 0
							; task_fini 4536 0
							; call 202
						; ENDIF
					; ENDIF
				; endcase
				; case 5051103
					; DEC_ITEM_NUM_BYIDX 5051103 1 @Pos_1 @Pos_2 ret
					; if @ret = -1
						; call 203
					; else
						; if @ret = 0
							; task_fini 4536 0
							; call 202
						; ENDIF
					; ENDIF
				; endcase
				; case 5051104
					; DEC_ITEM_NUM_BYIDX 5051104 1 @Pos_1 @Pos_2 ret
					; if @ret = -1
						; call 203
					; else
						; if @ret = 0
							; task_fini 4536 0
							; call 202
						; ENDIF
					; ENDIF
				; endcase
			; endswitch
		; else
			; add_sys_msg_start
			; add_sys_msg 物品不符任务要求，未能成功上交
			; set_sys_msg_mode 3 0 0
			; msg_info
		; endif
	; endif

	; task_stat 4537 0 102
	; if 102 = 2
		; if @ID = 5051010 or @ID = 5051011 or @ID = 5051051 or @ID = 5051052
			; switch @ID
				; case 5051010
					; DEC_ITEM_NUM_BYIDX 5051010 1 @Pos_1 @Pos_2 ret
					; if @ret = -1
						; call 203
					; else
						; if @ret = 0
							; task_fini 4537 0
							; call 202
						; ENDIF
					; ENDIF
				; endcase
				; case 5051011
					; DEC_ITEM_NUM_BYIDX 5051011 1 @Pos_1 @Pos_2 ret
					; if @ret = -1
						; call 203
					; else
						; if @ret = 0
							; task_fini 4537 0
							; call 202
						; ENDIF
					; ENDIF
				; endcase
				; case 5051051
					; DEC_ITEM_NUM_BYIDX 5051051 1 @Pos_1 @Pos_2 ret
					; if @ret = -1
						; call 203
					; else
						; if @ret = 0
							; task_fini 4537 0
							; call 202
						; ENDIF
					; ENDIF
				; endcase
				; case 5051052
					; DEC_ITEM_NUM_BYIDX 5051052 1 @Pos_1 @Pos_2 ret
					; if @ret = -1
						; call 203
					; else
						; if @ret = 0
							; task_fini 4537 0
							; call 202
						; ENDIF
					; ENDIF
				; endcase
			; endswitch
		; else
			; add_sys_msg_start
			; add_sys_msg 物品不符任务要求，未能成功上交
			; set_sys_msg_mode 3 0 0
			; msg_info
		; endif
	; endif
	
	task_stat 4538 0 103
	if 103 = 2
		if @ID = 5051008 or @ID = 5051009 or @ID = 5051098 or @ID = 5051101 or @ID = 5051002 or @ID = 5051003
			switch @ID
				case 5051008
					DEC_ITEM_NUM_BYIDX 5051008 5 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4538 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5051009
					DEC_ITEM_NUM_BYIDX 5051009 5 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4538 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5051098
					DEC_ITEM_NUM_BYIDX 5051098 5 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4538 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5051101
					DEC_ITEM_NUM_BYIDX 5051101 5 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4538 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5051002
					DEC_ITEM_NUM_BYIDX 5051002 5 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4538 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5051003
					DEC_ITEM_NUM_BYIDX 5051003 5 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4538 0
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

	task_stat 4539 0 104
	if 104 = 2
		if @ID = 5051036 or @ID = 5051037 or @ID = 5051039 or @ID = 5051040 or @ID = 5051041 or @ID = 5051046 or @ID = 5051047 or @ID = 5051105
			switch @ID
				case 5051036
					DEC_ITEM_NUM_BYIDX 5051036 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4539 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5051037
					DEC_ITEM_NUM_BYIDX 5051037 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4539 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5051039
					DEC_ITEM_NUM_BYIDX 5051039 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4539 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5051040
					DEC_ITEM_NUM_BYIDX 5051040 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4539 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5051041
					DEC_ITEM_NUM_BYIDX 5051041 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4539 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5051046
					DEC_ITEM_NUM_BYIDX 5051046 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4539 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5051047
					DEC_ITEM_NUM_BYIDX 5051047 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4539 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5051105
					DEC_ITEM_NUM_BYIDX 5051105 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4539 0
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

	task_stat 4540 0 105
	if 105 = 2
		if @ID = 5006100 or @ID = 5006101 or @ID = 5006102 or @ID = 5006103 or @ID = 5006106 or @ID = 5006108 or @ID = 5006109 or @ID = 5006105 or @ID = 5006104 or @ID = 5006107 or @ID = 5006110 or @ID = 5006111 or @ID = 5006112 or @ID = 5006113
			switch @ID
				case 5006100
					DEC_ITEM_NUM_BYIDX 5006100 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4540 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5006101
					DEC_ITEM_NUM_BYIDX 5006101 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4540 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5006102
					DEC_ITEM_NUM_BYIDX 5006102 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4540 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5006103
					DEC_ITEM_NUM_BYIDX 5006103 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4540 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5006106
					DEC_ITEM_NUM_BYIDX 5006106 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4540 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5006108
					DEC_ITEM_NUM_BYIDX 5006108 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4540 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5006109
					DEC_ITEM_NUM_BYIDX 5006109 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4540 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5006105
					DEC_ITEM_NUM_BYIDX 5006105 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4540 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5006104
					DEC_ITEM_NUM_BYIDX 5006104 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4540 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5006107
					DEC_ITEM_NUM_BYIDX 5006107 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4540 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5006110
					DEC_ITEM_NUM_BYIDX 5006110 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4540 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5006111
					DEC_ITEM_NUM_BYIDX 5006111 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4540 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5006112
					DEC_ITEM_NUM_BYIDX 5006112 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4540 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5006113
					DEC_ITEM_NUM_BYIDX 5006113 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4540 0
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
	
	task_stat 4541 0 106
	if 106 = 2
		if @ID = 5041000 or @ID = 5041002 or @ID = 5041003 or @ID = 5041023
			switch @ID
				case 5041000
					DEC_ITEM_NUM_BYIDX 5041000 3 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4541 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5041002
					DEC_ITEM_NUM_BYIDX 5041002 3 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4541 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5041003
					DEC_ITEM_NUM_BYIDX 5041003 3 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4541 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5041023
					DEC_ITEM_NUM_BYIDX 5041023 3 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4541 0
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
	
	task_stat 4542 0 107
	if 107 = 2
		if @ID = 5103041 or @ID = 5103042 or @ID = 5103043 or @ID = 5103001 or @ID = 5103002 or @ID = 5103003 or @ID = 5103021 or @ID = 5103022 or @ID = 5103023
			switch @ID
				case 5103041
					DEC_ITEM_NUM_BYIDX 5103041 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4542 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5103042
					DEC_ITEM_NUM_BYIDX 5103042 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4542 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5103043
					DEC_ITEM_NUM_BYIDX 5103043 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4542 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5103001
					DEC_ITEM_NUM_BYIDX 5103001 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4542 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5103002
					DEC_ITEM_NUM_BYIDX 5103002 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4542 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5103003
					DEC_ITEM_NUM_BYIDX 5103003 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4542 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5103021
					DEC_ITEM_NUM_BYIDX 5103021 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4542 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5103022
					DEC_ITEM_NUM_BYIDX 5103022 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4542 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5103023
					DEC_ITEM_NUM_BYIDX 5103023 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4542 0
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
	
	task_stat 4543 0 108
	if 108 = 2
		if @ID = 5102001 or @ID = 5102002 or @ID = 5102003 or @ID = 5102004
			switch @ID
				case 5102001
					DEC_ITEM_NUM_BYIDX 5102001 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4543 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5102002
					DEC_ITEM_NUM_BYIDX 5102002 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4543 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5102003
					DEC_ITEM_NUM_BYIDX 5102003 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4543 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5102004
					DEC_ITEM_NUM_BYIDX 5102004 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4543 0
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
	
	task_stat 4544 0 109
	if 109 = 2
		if @ID = 5101001 or @ID = 5101002 or @ID = 5101003 or @ID = 5101004
			switch @ID
				case 5101001
					DEC_ITEM_NUM_BYIDX 5101001 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4544 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5101002
					DEC_ITEM_NUM_BYIDX 5101002 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4544 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5101003
					DEC_ITEM_NUM_BYIDX 5101003 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4544 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5101004
					DEC_ITEM_NUM_BYIDX 5101004 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4544 0
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
	
	task_stat 4545 0 110
	if 110 = 2
		if @ID = 5210000 or @ID = 5210100 or @ID = 5211000 or @ID = 5211100 or @ID = 5211200 or @ID = 5212000 or @ID = 5212100 or @ID = 5212200 or @ID = 5212400 or @ID = 5213200 or @ID = 5213300 or @ID = 5213400
			switch @ID
				case 5210000
					DEC_ITEM_NUM_BYIDX 5210000 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4545 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5210100
					DEC_ITEM_NUM_BYIDX 5210100 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4545 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5211000
					DEC_ITEM_NUM_BYIDX 5211000 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4545 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5211100
					DEC_ITEM_NUM_BYIDX 5211100 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4545 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5211200
					DEC_ITEM_NUM_BYIDX 5211200 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4545 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5212000
					DEC_ITEM_NUM_BYIDX 5212000 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4545 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5212100
					DEC_ITEM_NUM_BYIDX 5212100 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4545 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5212200
					DEC_ITEM_NUM_BYIDX 5212200 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4545 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5212400
					DEC_ITEM_NUM_BYIDX 5212400 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4545 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5213200
					DEC_ITEM_NUM_BYIDX 5213200 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4545 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5213300
					DEC_ITEM_NUM_BYIDX 5213300 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4545 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5213400
					DEC_ITEM_NUM_BYIDX 5213400 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4545 0
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
	
	task_stat 4546 0 111
	if 111 = 2
		if @ID = 5210001 or @ID = 5210101 or @ID = 5211001 or @ID = 5211101 or @ID = 5211201 or @ID = 5212001 or @ID = 5212101 or @ID = 5212201 or @ID = 5212401 or @ID = 5213201 or @ID = 5213301 or @ID = 5213401
			switch @ID
				case 5210001
					DEC_ITEM_NUM_BYIDX 5210001 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4546 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5210101
					DEC_ITEM_NUM_BYIDX 5210101 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4546 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5211001
					DEC_ITEM_NUM_BYIDX 5211001 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4546 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5211101
					DEC_ITEM_NUM_BYIDX 5211101 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4546 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5211201
					DEC_ITEM_NUM_BYIDX 5211201 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4546 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5212001
					DEC_ITEM_NUM_BYIDX 5212001 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4546 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5212101
					DEC_ITEM_NUM_BYIDX 5212101 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4546 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5212201
					DEC_ITEM_NUM_BYIDX 5212201 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4546 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5212401
					DEC_ITEM_NUM_BYIDX 5212401 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4546 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5213201
					DEC_ITEM_NUM_BYIDX 5213201 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4546 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5213301
					DEC_ITEM_NUM_BYIDX 5213301 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4546 0
							call 202
						ENDIF
					ENDIF
				endcase
				case 5213401
					DEC_ITEM_NUM_BYIDX 5213401 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 203
					else
						if @ret = 0
							task_fini 4546 0
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
