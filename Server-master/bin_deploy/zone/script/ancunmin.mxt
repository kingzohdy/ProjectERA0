

proc 1
	DIALOG 1
	add_talk $_npc$:
	
	strvar randtalk
	rand randtalk 9
	switch randtalk
		case 0
			add_talk #b#b$_player$，你是来保护我们的吗？我还不想死，我真的不想死！
		endcase
		case 1
			add_talk #b#b一开始我还反抗，可是那次差点丧命在血色追随者之手，我就害怕了……
		endcase
		case 2
			add_talk #b#b我不知道为什么，即使只是听他人说起，我也会莫名的害怕，我想离开这里，到一个安全的地方去……$_player$，你带我离开这儿好吗？
		endcase
		case 3
			add_talk #b#b没用的，没用的，阿拜多斯拥有无穷无尽的法力，我们斗不过他的……
		endcase
		case 4
			add_talk #b#b为什么受伤害的是我们？我们什么都没做，真的，我什么都没做……
		endcase
		case 5
			add_talk #b#b我们要相信艾斯尤特，他会保护我们的！
		endcase
		case 6
			add_talk #b#b梅里耶特拉镇长正在寻找杀死阿拜多斯的办法，她一定会成功的！
		endcase
		case 7
			add_talk #b#b梅里耶特拉只是个女人，我们怎么能相信一个女人？
		endcase
		case 8
			add_talk #b#b如果豪斯镇长还在就好了！
		endcase
	endswitch
	strvar Rad Ret
	task_stat 1544 1 Ret
	if Ret = 2
	
	
		get_task_var 1544 16 8 1 val
		if val < 1
			rand Rad 3
			switch Rad
;			case 0
;				option 110 #迫不得已之举
;			endcase
				case 0
					option 110 #拿起武器，你的命运只能自己掌控！
				endcase
				case 1
					option 110 #为了胜利，为了生存，起来战斗吧！
				endcase
				case 2
					option 110 #起来，不愿做奴隶的人民！
				endcase
			endswitch
		endif
	endif
	talk
endproc 
proc 110
	strvar Mid1 val Pmid Retm
	PLAYER_CALL_BUB 204107 40 Mid1
	set_task_var 1544 0 32 @Mid1 1
	GET_PLAYER_ID 1 Pmid
	SET_MON_CURR @Mid1 Retm
	
	if Retm = 0
		set_mon_var 0 32 @Pmid 1     
		strvar Loc
		get_task_var 1544 17 8 1 Loc
		var Loc 10
		set_task_var 1544 17 8 @Loc 1
		add_sys_msg_start
		add_sys_msg 有一名村民响应了你的号召，快将他带回去见艾斯尤特吧！
		set_sys_msg_mode 3 0 0
		msg_info	
	endif
	
	if Mid1 >= 0
		get_task_var 1544 16 8 1 val
		add val 1
		set_task_var 1544 16 8 @val 1
;		DEL_NPC 0 0
	endif
endproc

proc 111
	strvar Mid2 val Pmid Retm
	PLAYER_CALL_BUB 204107 40 Mid2
	set_task_var 1544 4 32 @Mid2 1
	GET_PLAYER_ID 1 Pmid
	SET_MON_CURR @Mid2 Retm
	if Retm = 0
		set_mon_var 0 32 @Pmid 1     
	endif
	if Mid2 >= 0
		get_task_var 1544 16 8 1 val
		add val 1
		set_task_var 1544 16 8 @val 1
;		DEL_NPC 0 0
	endif
endproc 
proc 112
	strvar Mid3 val Pmid Retm
	PLAYER_CALL_BUB 204107 40 Mid3
	set_task_var 1544 8 32 @Mid3 1
	GET_PLAYER_ID 1 Pmid
	SET_MON_CURR @Mid3 Retm
	if Retm = 0
		set_mon_var 0 32 @Pmid 1     
	endif   
	
	if Mid3 >= 0
		get_task_var 1544 16 8 1 val
		add val 1
		set_task_var 1544 16 8 @val 1
;		DEL_NPC 0 0
	endif
endproc 
;proc 113
;	strvar Mid4 val
;	PLAYER_CALL_BUB 204107 40 Mid4
;	set_task_var 1544 12 32 @Mid4 1
;	
;	if Mid4 >= 0
;		get_task_var 1544 16 8 1 val
;		add val 1
;		set_task_var 1544 16 8 @val 1
;	endif
;endproc 