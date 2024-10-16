
;信徒
proc 1
	DIALOG 1
	add_talk $_npc$:
	strvar randtalk
	rand randtalk 6
	switch randtalk
		case 0
			add_talk #b#b世界末日大降临，只有信奉阿拜多斯神才能得救！
		endcase
		case 1
			add_talk #b#b焚我残躯，熊熊圣火。生亦何欢，死亦何苦。
		endcase
		case 2
			add_talk #b#b阿拜多斯神请救救我，我是您忠诚的信徒。
		endcase
		case 3
			add_talk #b#b我的生命、我的灵魂献给您，尊敬的阿拜多斯神！
		endcase
		case 4
			add_talk #b#b鲜血，才是洗涤你灵魂最好的药剂。
		endcase
		case 5
			add_talk #b#b阿拜多斯神能斩断你的一切苦闷和不幸的根源，加入我们吧！
		endcase
	endswitch	
	
	
	strvar Ret
	TASK_STAT 1561 1 Ret
	get_task_var 1561 4 32 1 lock 
	if Ret = 2 and lock != 10
	
		option 99 你好，忠诚的信徒。

	endif
	strvar Ret1
	TASK_STAT 1569 1 Ret1
	get_task_var 1569 0 8 1 55
	if Ret1 = 2 and 55 = 0
		option 500 #伙计们，跟我走，干大事去！
	endif
	
	talk
endproc 
proc 99
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b找我有什么事吗？

		option 100 有件事想请您帮忙。
		option 200 梅利亚女士让我来找你的。
	talk
endproc 

proc 100
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b有什么事就在这儿说吧，我还要继续听梅利亚女士的教诲。
		option 101 这是机密，能否借一步说话。
		option 102 那边有个漂亮美眉找你。
	talk
endproc
proc 101
;;;;;;;;;;;;;return
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b机密？我的灵魂已经献给阿拜多斯神，我所知道的秘密都会无条件无隐瞒的告诉梅利亚女士，我去叫她，你等等……
		option 300 #……
	talk
endproc
proc 102 
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b哦？漂亮美眉？
		option 103 是的，说是仰慕你已久
		
	talk
endproc
proc 103
;;;;;;;;;;;;;return
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b我的灵魂已经献给阿拜多斯神，我的生命已经献给了阿拜多斯神，我所有的一切都只属于阿拜多斯神！请不要打断我对神表现的忠诚！
		option 300 #……
		
	talk
endproc
 

proc 300
	return
endproc


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
proc 200
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b梅利亚女士不是正在讲课吗？怎么又会让你来找我？
		option 201 由于你出色的表现，女士决定给你个机会
		option 210 梅利亚女士的心思，你不需要知道
		
	talk

endproc

proc 201
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b什么机会？出色的表现？

		option 202 对呀，女士想让你去执行秘密任务
	talk
endproc
proc 202
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b可是刚才梅利亚女士才说过，我们是一群渣滓，需要净化才能够格为阿拜多斯神效命。
		option 300 #那个……我找错人了……
	talk
endproc
proc 210
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b长官，请问梅利亚女士有什么吩咐？

		option 211 #废话少说，跟我走！
	talk
endproc


proc 211
	strvar Mid lock 
	PLAYER_CALL_BUB 213033 180 Mid
	set_task_var 1561 0 32 @Mid 1
	
	var lock 10
	set_task_var 1561 4 32 @lock 1
endproc


proc 500
	strvar Mid1 Mid2 Mid3 
	PLAYER_CALL_BUB 213034 240 Mid1
	PLAYER_CALL_BUB 213034 240 Mid2
	PLAYER_CALL_BUB 213034 240 Mid3
	
	set_task_var 1569 0 32 @Mid1 10
	set_task_var 1569 4 32 @Mid2 11
	set_task_var 1569 8 32 @Mid3 12
	
	if Mid1 != -1 and Mid2 != -1 and Mid3 != -1
		get_task_var 1569 0 8 1 10
		var 10 1
		set_task_var 1569 0 8 @10 1
		add_sys_msg_start
		add_sys_msg 信徒，展现你们忠心的时刻到了！
		set_sys_msg_mode 3 0 0
		msg_info
		
		
	endif
	
endproc 










