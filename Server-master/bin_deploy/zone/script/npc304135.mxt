;304135 里奇

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b世界上并没有绝对的善与恶，差别只在于强者和无法分清事实的弱者！
	task_stat 4513 1 0
	if 0 = 2
		option 201 #区区倒酒难不到我（开始倒酒）
	endif
	task_stat 4514 1 0
	if 0 = 2
		option 301 #区区倒酒难不到我（开始倒酒）
	endif
	task_stat 4515 1 0
	if 0 = 2
		option 401 #区区倒酒难不到我（开始倒酒）
	endif
	task_stat 4516 1 0
	if 0 = 2
		option 501 #区区倒酒难不到我（开始倒酒）
	endif
	task_stat 4517 1 0
	if 0 = 2
		option 601 #区区倒酒难不到我（开始倒酒）
	endif
	
	task_add_option 4513 1 101
	task_add_option 4514 1 102
	task_add_option 4515 1 103
	task_add_option 4516 1 104
	task_add_option 4517 1 105
	
	talk
ENDPROC

PROC 101	
	task_do_option 4513 0
ENDPROC

PROC 102	
	task_do_option 4514 0
ENDPROC

PROC 103	
	task_do_option 4515 0
ENDPROC

PROC 104	
	task_do_option 4516 0
ENDPROC

PROC 105	
	task_do_option 4517 0
ENDPROC

proc 201;2
	call 701
	DIALOG 1
	add_talk #b#b#b#b#b#b#c186506以最少的步骤把酒倒好
	add_talk #b#b#b#b#b倒错一步就只有重新开始喽#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9要求：金桶1升酒，银桶空#n
		option 704 #将金桶装满（5升酒）
		option 202 #将银桶装满（3升酒）
		; option 704 #将金桶里的酒倒入银桶
		; option 704 #将银桶里的酒倒入金桶
		; option 704 #将金桶倒空
		; option 704 #将银桶倒空
		; option 705 #我倒好了！
	talk
endproc
proc 202;4
	call 702
	DIALOG 1
	add_talk #b#b#b#b#b#c186506现在：金桶空，银桶3升酒#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9要求：金桶1升酒，银桶空#n
		option 704 #将金桶装满（5升酒）
		option 704 #将银桶装满（3升酒）
		option 704 #将金桶里的酒倒入银桶
		option 203 #将银桶里的酒倒入金桶
		; option 704 #将金桶倒空
		; option 704 #将银桶倒空
		; option 705 #我倒好了！
	talk
endproc
proc 203;2
	call 702
	DIALOG 1
	add_talk #b#b#b#b#b#c186506现在：金桶3升酒，银桶空#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9要求：金桶1升酒，银桶空#n
		option 704 #将金桶装满（5升酒）
		option 204 #将银桶装满（3升酒）
		option 704 #将金桶里的酒倒入银桶
		option 704 #将银桶里的酒倒入金桶
		option 704 #将金桶倒空
		option 704 #将银桶倒空
		; option 705 #我倒好了！
	talk
endproc
proc 204;4
	call 702
	DIALOG 1
	add_talk #b#b#b#b#b#c186506现在：金桶3升酒，银桶3升酒#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9要求：金桶1升酒，银桶空#n
		option 704 #将金桶装满（5升酒）
		option 704 #将银桶装满（3升酒）
		option 704 #将金桶里的酒倒入银桶
		option 205 #将银桶里的酒倒入金桶
		option 704 #将金桶倒空
		option 704 #将银桶倒空
		; option 705 #我倒好了！
	talk
endproc
proc 205;5
	call 702
	DIALOG 1
	add_talk #b#b#b#b#b#c186506现在：金桶5升酒，银桶1升酒#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9要求：金桶1升酒，银桶空#n
		option 704 #将金桶装满（5升酒）
		option 704 #将银桶装满（3升酒）
		option 704 #将金桶里的酒倒入银桶
		option 704 #将银桶里的酒倒入金桶
		option 206 #将金桶倒空
		option 704 #将银桶倒空
		; option 705 #我倒好了！
	talk
endproc
proc 206;4
	call 702
	DIALOG 1
	add_talk #b#b#b#b#b#c186506现在：金桶空，银桶1升酒#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9要求：金桶1升酒，银桶空#n
		option 704 #将金桶装满（5升酒）
		option 704 #将银桶装满（3升酒）
		option 704 #将金桶里的酒倒入银桶
		option 207 #将银桶里的酒倒入金桶
		option 704 #将金桶倒空
		option 704 #将银桶倒空
		; option 705 #我倒好了！
	talk
endproc
proc 207;+
	call 703
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b#b要求达成
	add_talk #b#b#b#b#b点击选项#c6d00d2我倒好了#n完成任务
	add_talk #b#b
	add_talk #b#b#b#b#b#c186506现在：金桶1升酒，银桶空#n
	add_talk #b#b#b#b#b#c0181f9要求：金桶1升酒，银桶空#n
	strvar rad
	rand rad 7
		switch rad 
			case 0
				option 208 #我倒好了！
				option 704 #将金桶装满（5升酒）
				option 704 #将银桶装满（3升酒）
				option 704 #将金桶里的酒倒入银桶
				option 704 #将银桶里的酒倒入金桶
				option 704 #将金桶倒空
				option 704 #将银桶倒空
			endcase
			case 1
				option 704 #将金桶装满（5升酒）
				option 208 #我倒好了！
				option 704 #将银桶装满（3升酒）
				option 704 #将金桶里的酒倒入银桶
				option 704 #将银桶里的酒倒入金桶
				option 704 #将金桶倒空
				option 704 #将银桶倒空
			endcase
			case 2
				option 704 #将金桶装满（5升酒）
				option 704 #将银桶装满（3升酒）
				option 208 #我倒好了！
				option 704 #将金桶里的酒倒入银桶
				option 704 #将银桶里的酒倒入金桶
				option 704 #将金桶倒空
				option 704 #将银桶倒空
			endcase
			case 3
				option 704 #将金桶装满（5升酒）
				option 704 #将银桶装满（3升酒）
				option 704 #将金桶里的酒倒入银桶
				option 208 #我倒好了！
				option 704 #将银桶里的酒倒入金桶
				option 704 #将金桶倒空
				option 704 #将银桶倒空
			endcase
			case 4
				option 704 #将金桶装满（5升酒）
				option 704 #将银桶装满（3升酒）
				option 704 #将金桶里的酒倒入银桶
				option 704 #将银桶里的酒倒入金桶
				option 208 #我倒好了！
				option 704 #将金桶倒空
				option 704 #将银桶倒空
			endcase
			case 5
				option 704 #将金桶装满（5升酒）
				option 704 #将银桶装满（3升酒）
				option 704 #将金桶里的酒倒入银桶
				option 704 #将银桶里的酒倒入金桶
				option 704 #将金桶倒空
				option 208 #我倒好了！
				option 704 #将银桶倒空
			endcase
			case 6
				option 704 #将金桶装满（5升酒）
				option 704 #将银桶装满（3升酒）
				option 704 #将金桶里的酒倒入银桶
				option 704 #将银桶里的酒倒入金桶
				option 704 #将金桶倒空
				option 704 #将银桶倒空
				option 208 #我倒好了！
			endcase
		endswitch
		
	talk
endproc
proc 208;o
	task_fini 4513 0
endproc

proc 301;2
	call 701
	DIALOG 1
	add_talk #b#b#b#b#b#b#c186506以最少的步骤把酒倒好
	add_talk #b#b#b#b#b倒错一步就只有重新开始喽#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9要求：金桶空，银桶1升酒#n
		option 704 #将金桶装满（5升酒）
		option 302 #将银桶装满（3升酒）
		; option 704 #将金桶里的酒倒入银桶
		; option 704 #将银桶里的酒倒入金桶
		; option 704 #将金桶倒空
		; option 704 #将银桶倒空
		; option 705 #我倒好了！
	talk
endproc
proc 302;4
	call 702
	DIALOG 1
	add_talk #b#b#b#b#b#c186506现在：金桶空，银桶3升酒#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9要求：金桶空，银桶1升酒#n
		option 704 #将金桶装满（5升酒）
		option 704 #将银桶装满（3升酒）
		option 704 #将金桶里的酒倒入银桶
		option 303 #将银桶里的酒倒入金桶
		; option 704 #将金桶倒空
		; option 704 #将银桶倒空
		; option 705 #我倒好了！
	talk
endproc
proc 303;2
	call 702
	DIALOG 1
	add_talk #b#b#b#b#b#c186506现在：金桶3升酒，银桶空#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9要求：金桶空，银桶1升酒#n
		option 704 #将金桶装满（5升酒）
		option 304 #将银桶装满（3升酒）
		option 704 #将金桶里的酒倒入银桶
		option 704 #将银桶里的酒倒入金桶
		option 704 #将金桶倒空
		option 704 #将银桶倒空
		; option 705 #我倒好了！
	talk
endproc
proc 304;4
	call 702
	DIALOG 1
		add_talk #b#b#b#b#b#c186506现在：金桶3升酒，银桶3升酒#n
		add_talk #b#b
		add_talk #b#b#b#b#b#c0181f9要求：金桶空，银桶1升酒#n
		option 704 #将金桶装满（5升酒）
		option 704 #将银桶装满（3升酒）
		option 704 #将金桶里的酒倒入银桶
		option 305 #将银桶里的酒倒入金桶
		option 704 #将金桶倒空
		option 704 #将银桶倒空
		; option 705 #我倒好了！
	talk
endproc
proc 305;5
	call 702
	DIALOG 1
	add_talk #b#b#b#b#b#c186506现在：金桶5升酒，银桶1升酒#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9要求：金桶空，银桶1升酒#n
		option 704 #将金桶装满（5升酒）
		option 704 #将银桶装满（3升酒）
		option 704 #将金桶里的酒倒入银桶
		option 704 #将银桶里的酒倒入金桶
		option 306 #将金桶倒空
		option 704 #将银桶倒空
		; option 705 #我倒好了！
	talk
endproc
proc 306;+
	call 703
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b#b要求达成
	add_talk #b#b#b#b#b点击选项#c6d00d2我倒好了#n完成任务
	add_talk #b#b
	add_talk #b#b#b#b#b#c186506现在：金桶空，银桶1升酒#n
	add_talk #b#b#b#b#b#c0181f9要求：金桶空，银桶1升酒#n
	strvar rad
	rand rad 7
		switch rad 
			case 0
				option 307 #我倒好了！
				option 704 #将金桶装满（5升酒）
				option 704 #将银桶装满（3升酒）
				option 704 #将金桶里的酒倒入银桶
				option 704 #将银桶里的酒倒入金桶
				option 704 #将金桶倒空
				option 704 #将银桶倒空
			endcase
			case 1
				option 704 #将金桶装满（5升酒）
				option 307 #我倒好了！
				option 704 #将银桶装满（3升酒）
				option 704 #将金桶里的酒倒入银桶
				option 704 #将银桶里的酒倒入金桶
				option 704 #将金桶倒空
				option 704 #将银桶倒空
			endcase
			case 2
				option 704 #将金桶装满（5升酒）
				option 704 #将银桶装满（3升酒）
				option 307 #我倒好了！
				option 704 #将金桶里的酒倒入银桶
				option 704 #将银桶里的酒倒入金桶
				option 704 #将金桶倒空
				option 704 #将银桶倒空
			endcase
			case 3
				option 704 #将金桶装满（5升酒）
				option 704 #将银桶装满（3升酒）
				option 704 #将金桶里的酒倒入银桶
				option 307 #我倒好了！
				option 704 #将银桶里的酒倒入金桶
				option 704 #将金桶倒空
				option 704 #将银桶倒空
			endcase
			case 4
				option 704 #将金桶装满（5升酒）
				option 704 #将银桶装满（3升酒）
				option 704 #将金桶里的酒倒入银桶
				option 704 #将银桶里的酒倒入金桶
				option 307 #我倒好了！
				option 704 #将金桶倒空
				option 704 #将银桶倒空
			endcase
			case 5
				option 704 #将金桶装满（5升酒）
				option 704 #将银桶装满（3升酒）
				option 704 #将金桶里的酒倒入银桶
				option 704 #将银桶里的酒倒入金桶
				option 704 #将金桶倒空
				option 307 #我倒好了！
				option 704 #将银桶倒空
			endcase
			case 6
				option 704 #将金桶装满（5升酒）
				option 704 #将银桶装满（3升酒）
				option 704 #将金桶里的酒倒入银桶
				option 704 #将银桶里的酒倒入金桶
				option 704 #将金桶倒空
				option 704 #将银桶倒空
				option 307 #我倒好了！
			endcase
		endswitch
			
	talk
endproc
proc 307;o
	task_fini 4514 0
endproc

proc 401;1
	call 701
	DIALOG 1
	add_talk #b#b#b#b#b#b#c186506以最少的步骤把酒倒好
	add_talk #b#b#b#b#b倒错一步就只有重新开始喽#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9要求：金桶2升酒，银桶空#n
		option 402 #将金桶装满（5升酒）
		option 704 #将银桶装满（3升酒）
		; option 704 #将金桶里的酒倒入银桶
		; option 704 #将银桶里的酒倒入金桶
		; option 704 #将金桶倒空
		; option 704 #将银桶倒空
		; option 705 #我倒好了！
	talk
endproc
proc 402;3
	call 702
	DIALOG 1
	add_talk #b#b#b#b#b#c186506现在：金桶5升酒，银桶空#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9要求：金桶2升酒，银桶空#n
		option 704 #将金桶装满（5升酒）
		option 704 #将银桶装满（3升酒）
		option 403 #将金桶里的酒倒入银桶
		option 704 #将银桶里的酒倒入金桶
		; option 704 #将金桶倒空
		; option 704 #将银桶倒空
		; option 705 #我倒好了！
	talk
endproc
proc 403;6
	call 702
	DIALOG 1
	add_talk #b#b#b#b#b#c186506现在：金桶2升酒，银桶3升酒#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9要求：金桶2升酒，银桶空#n
		option 704 #将金桶装满（5升酒）
		option 704 #将银桶装满（3升酒）
		option 704 #将金桶里的酒倒入银桶
		option 704 #将银桶里的酒倒入金桶
		option 704 #将金桶倒空
		option 404 #将银桶倒空
		; option 705 #我倒好了！
	talk
endproc
proc 404;+
	call 703
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b#b要求达成
	add_talk #b#b#b#b#b点击选项#c6d00d2我倒好了#n完成任务
	add_talk #b#b
	add_talk #b#b#b#b#b#c186506现在：金桶2升酒，银桶空#n
	add_talk #b#b#b#b#b#c0181f9要求：金桶2升酒，银桶空#n
	strvar rad
	rand rad 7
		switch rad 
			case 0
				option 405 #我倒好了！
				option 704 #将金桶装满（5升酒）
				option 704 #将银桶装满（3升酒）
				option 704 #将金桶里的酒倒入银桶
				option 704 #将银桶里的酒倒入金桶
				option 704 #将金桶倒空
				option 704 #将银桶倒空
			endcase
			case 1
				option 704 #将金桶装满（5升酒）
				option 405 #我倒好了！
				option 704 #将银桶装满（3升酒）
				option 704 #将金桶里的酒倒入银桶
				option 704 #将银桶里的酒倒入金桶
				option 704 #将金桶倒空
				option 704 #将银桶倒空
			endcase
			case 2
				option 704 #将金桶装满（5升酒）
				option 704 #将银桶装满（3升酒）
				option 405 #我倒好了！
				option 704 #将金桶里的酒倒入银桶
				option 704 #将银桶里的酒倒入金桶
				option 704 #将金桶倒空
				option 704 #将银桶倒空
			endcase
			case 3
				option 704 #将金桶装满（5升酒）
				option 704 #将银桶装满（3升酒）
				option 704 #将金桶里的酒倒入银桶
				option 405 #我倒好了！
				option 704 #将银桶里的酒倒入金桶
				option 704 #将金桶倒空
				option 704 #将银桶倒空
			endcase
			case 4
				option 704 #将金桶装满（5升酒）
				option 704 #将银桶装满（3升酒）
				option 704 #将金桶里的酒倒入银桶
				option 704 #将银桶里的酒倒入金桶
				option 405 #我倒好了！
				option 704 #将金桶倒空
				option 704 #将银桶倒空
			endcase
			case 5
				option 704 #将金桶装满（5升酒）
				option 704 #将银桶装满（3升酒）
				option 704 #将金桶里的酒倒入银桶
				option 704 #将银桶里的酒倒入金桶
				option 704 #将金桶倒空
				option 405 #我倒好了！
				option 704 #将银桶倒空
			endcase
			case 6
				option 704 #将金桶装满（5升酒）
				option 704 #将银桶装满（3升酒）
				option 704 #将金桶里的酒倒入银桶
				option 704 #将银桶里的酒倒入金桶
				option 704 #将金桶倒空
				option 704 #将银桶倒空
				option 405 #我倒好了！
			endcase
		endswitch
	talk
	
endproc
proc 405;o
	task_fini 4515 0
endproc

proc 501;1
	call 701
	DIALOG 1
	add_talk #b#b#b#b#b#b#c186506以最少的步骤把酒倒好
	add_talk #b#b#b#b#b倒错一步就只有重新开始喽#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9要求：金桶空，银桶2升酒#n
		option 502 #将金桶装满（5升酒）
		option 704 #将银桶装满（3升酒）
		; option 704 #将金桶里的酒倒入银桶
		; option 704 #将银桶里的酒倒入金桶
		; option 704 #将金桶倒空
		; option 704 #将银桶倒空
		; option 705 #我倒好了！
	talk
endproc
proc 502;3
	call 702
	DIALOG 1
	add_talk #b#b#b#b#b#c186506现在：金桶5升酒，银桶空#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9要求：金桶空，银桶2升酒#n
		option 704 #将金桶装满（5升酒）
		option 704 #将银桶装满（3升酒）
		option 503 #将金桶里的酒倒入银桶
		option 704 #将银桶里的酒倒入金桶
		; option 704 #将金桶倒空
		; option 704 #将银桶倒空
		; option 705 #我倒好了！
	talk
endproc
proc 503;6
	call 702
	DIALOG 1
	add_talk #b#b#b#b#b#c186506现在：金桶2升酒，银桶3升酒#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9要求：金桶空，银桶2升酒#n
		option 704 #将金桶装满（5升酒）
		option 704 #将银桶装满（3升酒）
		option 704 #将金桶里的酒倒入银桶
		option 704 #将银桶里的酒倒入金桶
		option 704 #将金桶倒空
		option 504 #将银桶倒空
		; option 705 #我倒好了！
	talk
endproc
proc 504;3
	call 702
	DIALOG 1
	add_talk #b#b#b#b#b#c186506现在：金桶2升酒，银桶空
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9要求：金桶空，银桶2升酒#n
		option 704 #将金桶装满（5升酒）
		option 704 #将银桶装满（3升酒）
		option 505 #将金桶里的酒倒入银桶
		option 704 #将银桶里的酒倒入金桶
		option 704 #将金桶倒空
		option 704 #将银桶倒空
		; option 705 #我倒好了！
	talk
endproc
proc 505;+
	call 703
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b#b要求达成
	add_talk #b#b#b#b#b点击选项#c6d00d2我倒好了#n完成任务
	add_talk #b#b
	add_talk #b#b#b#b#b#c186506现在：金桶空，银桶2升酒#n
	add_talk #b#b#b#b#b#c0181f9要求：金桶空，银桶2升酒#n
	strvar rad
	rand rad 7
		switch rad 
			case 0
				option 506 #我倒好了！
				option 704 #将金桶装满（5升酒）
				option 704 #将银桶装满（3升酒）
				option 704 #将金桶里的酒倒入银桶
				option 704 #将银桶里的酒倒入金桶
				option 704 #将金桶倒空
				option 704 #将银桶倒空
			endcase
			case 1
				option 704 #将金桶装满（5升酒）
				option 506 #我倒好了！
				option 704 #将银桶装满（3升酒）
				option 704 #将金桶里的酒倒入银桶
				option 704 #将银桶里的酒倒入金桶
				option 704 #将金桶倒空
				option 704 #将银桶倒空
			endcase
			case 2
				option 704 #将金桶装满（5升酒）
				option 704 #将银桶装满（3升酒）
				option 506 #我倒好了！
				option 704 #将金桶里的酒倒入银桶
				option 704 #将银桶里的酒倒入金桶
				option 704 #将金桶倒空
				option 704 #将银桶倒空
			endcase
			case 3
				option 704 #将金桶装满（5升酒）
				option 704 #将银桶装满（3升酒）
				option 704 #将金桶里的酒倒入银桶
				option 506 #我倒好了！
				option 704 #将银桶里的酒倒入金桶
				option 704 #将金桶倒空
				option 704 #将银桶倒空
			endcase
			case 4
				option 704 #将金桶装满（5升酒）
				option 704 #将银桶装满（3升酒）
				option 704 #将金桶里的酒倒入银桶
				option 704 #将银桶里的酒倒入金桶
				option 506 #我倒好了！
				option 704 #将金桶倒空
				option 704 #将银桶倒空
			endcase
			case 5
				option 704 #将金桶装满（5升酒）
				option 704 #将银桶装满（3升酒）
				option 704 #将金桶里的酒倒入银桶
				option 704 #将银桶里的酒倒入金桶
				option 704 #将金桶倒空
				option 506 #我倒好了！
				option 704 #将银桶倒空
			endcase
			case 6
				option 704 #将金桶装满（5升酒）
				option 704 #将银桶装满（3升酒）
				option 704 #将金桶里的酒倒入银桶
				option 704 #将银桶里的酒倒入金桶
				option 704 #将金桶倒空
				option 704 #将银桶倒空
				option 506 #我倒好了！
			endcase
		endswitch
	talk
	
endproc
proc 506;o
	task_fini 4516 0
endproc

proc 601;1
	call 701
	DIALOG 1
	add_talk #b#b#b#b#b#b#c186506以最少的步骤把酒倒好
	add_talk #b#b#b#b#b倒错一步就只有重新开始喽#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9要求：金桶4升酒，银桶空#n
		option 602 #将金桶装满（5升酒）
		option 704 #将银桶装满（3升酒）
		; option 704 #将金桶里的酒倒入银桶
		; option 704 #将银桶里的酒倒入金桶
		; option 704 #将金桶倒空
		; option 704 #将银桶倒空
		; option 705 #我倒好了！
	talk
endproc
proc 602;3
	call 702
	DIALOG 1
	add_talk #b#b#b#b#b#c186506现在：金桶5升酒，银桶空#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9要求：金桶4升酒，银桶空#n
		option 704 #将金桶装满（5升酒）
		option 704 #将银桶装满（3升酒）
		option 603 #将金桶里的酒倒入银桶
		option 704 #将银桶里的酒倒入金桶
		; option 704 #将金桶倒空
		; option 704 #将银桶倒空
		; option 705 #我倒好了！
	talk
endproc
proc 603;6
	call 702
	DIALOG 1
	add_talk #b#b#b#b#b#c186506现在：金桶2升酒，银桶3升酒#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9要求：金桶4升酒，银桶空#n
		option 704 #将金桶装满（5升酒）
		option 704 #将银桶装满（3升酒）
		option 704 #将金桶里的酒倒入银桶
		option 704 #将银桶里的酒倒入金桶
		option 704 #将金桶倒空
		option 604 #将银桶倒空
		; option 705 #我倒好了！
	talk
endproc
proc 604;3
	call 702
	DIALOG 1
	add_talk #b#b#b#b#b#c186506现在：金桶2升酒，银桶空#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9要求：金桶4升酒，银桶空#n
		option 704 #将金桶装满（5升酒）
		option 704 #将银桶装满（3升酒）
		option 605 #将金桶里的酒倒入银桶
		option 704 #将银桶里的酒倒入金桶
		option 704 #将金桶倒空
		option 704 #将银桶倒空
		; option 705 #我倒好了！
	talk
endproc
proc 605;1
	call 702
	DIALOG 1
	add_talk #b#b#b#b#b#c186506现在：金桶空，银桶2升酒#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9要求：金桶4升酒，银桶空#n
		option 606 #将金桶装满（5升酒）
		option 704 #将银桶装满（3升酒）
		option 704 #将金桶里的酒倒入银桶
		option 704 #将银桶里的酒倒入金桶
		option 704 #将金桶倒空
		option 704 #将银桶倒空
		; option 705 #我倒好了！
	talk
endproc
proc 606;3
	call 702
	DIALOG 1
	add_talk #b#b#b#b#b#c186506现在：金桶5升酒，银桶2升酒#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9要求：金桶4升酒，银桶空#n
		option 704 #将金桶装满（5升酒）
		option 704 #将银桶装满（3升酒）
		option 607 #将金桶里的酒倒入银桶
		option 704 #将银桶里的酒倒入金桶
		option 704 #将金桶倒空
		option 704 #将银桶倒空
		; option 705 #我倒好了！
	talk
endproc
proc 607;6
	call 702
	DIALOG 1
	add_talk #b#b#b#b#b#c186506现在：金桶4升酒，银桶3升酒
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9要求：金桶4升酒，银桶空#n
		option 704 #将金桶装满（5升酒）
		option 704 #将银桶装满（3升酒）
		option 704 #将金桶里的酒倒入银桶
		option 704 #将银桶里的酒倒入金桶
		option 704 #将金桶倒空
		option 608 #将银桶倒空
		; option 705 #我倒好了！
	talk
endproc
proc 608;+
	call 703
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b#b要求达成
	add_talk #b#b#b#b#b点击选项#c6d00d2我倒好了#n完成任务
	add_talk #b#b
	add_talk #b#b#b#b#b#c186506现在：金桶4升酒，银桶空#n
	add_talk #b#b#b#b#b#c0181f9要求：金桶4升酒，银桶空#n
	strvar rad
	rand rad 7
		switch rad 
			case 0
				option 609 #我倒好了！
				option 704 #将金桶装满（5升酒）
				option 704 #将银桶装满（3升酒）
				option 704 #将金桶里的酒倒入银桶
				option 704 #将银桶里的酒倒入金桶
				option 704 #将金桶倒空
				option 704 #将银桶倒空
			endcase
			case 1
				option 704 #将金桶装满（5升酒）
				option 609 #我倒好了！
				option 704 #将银桶装满（3升酒）
				option 704 #将金桶里的酒倒入银桶
				option 704 #将银桶里的酒倒入金桶
				option 704 #将金桶倒空
				option 704 #将银桶倒空
			endcase
			case 2
				option 704 #将金桶装满（5升酒）
				option 704 #将银桶装满（3升酒）
				option 609 #我倒好了！
				option 704 #将金桶里的酒倒入银桶
				option 704 #将银桶里的酒倒入金桶
				option 704 #将金桶倒空
				option 704 #将银桶倒空
			endcase
			case 3
				option 704 #将金桶装满（5升酒）
				option 704 #将银桶装满（3升酒）
				option 704 #将金桶里的酒倒入银桶
				option 609 #我倒好了！
				option 704 #将银桶里的酒倒入金桶
				option 704 #将金桶倒空
				option 704 #将银桶倒空
			endcase
			case 4
				option 704 #将金桶装满（5升酒）
				option 704 #将银桶装满（3升酒）
				option 704 #将金桶里的酒倒入银桶
				option 704 #将银桶里的酒倒入金桶
				option 609 #我倒好了！
				option 704 #将金桶倒空
				option 704 #将银桶倒空
			endcase
			case 5
				option 704 #将金桶装满（5升酒）
				option 704 #将银桶装满（3升酒）
				option 704 #将金桶里的酒倒入银桶
				option 704 #将银桶里的酒倒入金桶
				option 704 #将金桶倒空
				option 609 #我倒好了！
				option 704 #将银桶倒空
			endcase
			case 6
				option 704 #将金桶装满（5升酒）
				option 704 #将银桶装满（3升酒）
				option 704 #将金桶里的酒倒入银桶
				option 704 #将银桶里的酒倒入金桶
				option 704 #将金桶倒空
				option 704 #将银桶倒空
				option 609 #我倒好了！
			endcase
		endswitch
	talk
endproc
proc 609;o
	task_fini 4517 0
endproc

proc 701;x
	ADD_SYS_MSG_START
	ADD_SYS_MSG 准备开始倒酒！以最少的步骤把酒倒好
	SET_SYS_MSG_MODE 3 0 0
	MSG_INFO
endproc
proc 702;x
	ADD_SYS_MSG_START
	ADD_SYS_MSG 倒酒步骤正确，但还没达到要求，请继续倒酒
	SET_SYS_MSG_MODE 3 0 0
	MSG_INFO
endproc
proc 703;x
	ADD_SYS_MSG_START
	ADD_SYS_MSG 已经达成要求，点击“我倒好了”汇报结果！
	SET_SYS_MSG_MODE 3 0 0
	MSG_INFO
endproc
proc 704;!
	DIALOG 1
	task_stat 4513 1 0
	if 0 = 2
		add_talk #b#b#b#b#b#b#b#b#b#c186506重新挑战
		add_talk #b#b#b#b 由于步骤错误，只好重新倒酒#n
		add_talk #b#b
		add_talk #b#b#b#b#b#c0181f9要求：金桶1升酒，银桶空#n
			option 704 #将金桶装满（5升酒）
			option 202 #将银桶装满（3升酒）
		talk
	endif
	task_stat 4514 1 0
	if 0 = 2
		add_talk #b#b#b#b#b#b#b#b#b#c186506重新挑战
		add_talk #b#b#b#b 由于步骤错误，只好重新倒酒#n
		add_talk #b#b
		add_talk #b#b#b#b#b#c0181f9要求：金桶空，银桶1升酒#n
			option 704 #将金桶装满（5升酒）
			option 302 #将银桶装满（3升酒）
		talk
	endif
	task_stat 4515 1 0
	if 0 = 2
		add_talk #b#b#b#b#b#b#b#b#b#c186506重新挑战
		add_talk #b#b#b#b 由于步骤错误，只好重新倒酒#n
		add_talk #b#b
		add_talk #b#b#b#b#b#c0181f9要求：金桶2升酒，银桶空#n
			option 402 #将金桶装满（5升酒）
			option 704 #将银桶装满（3升酒）
		talk
	endif
	task_stat 4516 1 0
	if 0 = 2
		add_talk #b#b#b#b#b#b#b#b#b#c186506重新挑战
		add_talk #b#b#b#b 由于步骤错误，只好重新倒酒#n
		add_talk #b#b
		add_talk #b#b#b#b#b#c0181f9要求：金桶空，银桶2升酒#n
			option 502 #将金桶装满（5升酒）
			option 704 #将银桶装满（3升酒）
		talk
	endif
	task_stat 4517 1 0
	if 0 = 2
		add_talk #b#b#b#b#b#b#b#b#b#c186506重新挑战
		add_talk #b#b#b#b 由于步骤错误，只好重新倒酒#n
		add_talk #b#b
		add_talk #b#b#b#b#b#c0181f9要求：金桶4升酒，银桶空#n
			option 602 #将金桶装满（5升酒）
			option 704 #将银桶装满（3升酒）
		talk
	endif
	
	ADD_SYS_MSG_START
	ADD_SYS_MSG 由于倒酒步骤错误，只好回到第一步重新倒酒
	SET_SYS_MSG_MODE 3 0 0
	MSG_INFO

endproc
proc 705;x
	ADD_SYS_MSG_START
	ADD_SYS_MSG 很遗憾，你未能达到要求，请重新开始倒酒！
	SET_SYS_MSG_MODE 4 0 0
	MSG_INFO
endproc
