; 直升机


proc 1
	dialog 1
	add_talk $_npc$:
	strvar Gend
	GET_PLAYER_BASE_ATT 0 Gend 0 0
	if Gend = 1
		
		add_talk #b#b帅哥，你想去观测台吗？免费哟！
	else
		
		add_talk #b#b美女，你想去观测台吗？免费哟！
	endif
	
		strvar Ret
		task_stat 2114 1 Ret
		if Ret = 3
			option 150 #乘坐直升机
		endif
		
	talk
endproc
proc 150
	AUTOFLY 124 10
endproc 
