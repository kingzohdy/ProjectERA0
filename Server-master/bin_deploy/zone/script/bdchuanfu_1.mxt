


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b您要去端了海盗的老巢，这真是太好了，我可以送您前去，最重要的是免费哟！
;		strvar Ret1
;		task_stat 1373 1 Ret1
;		if Ret1 = 2 
			option 150 #乘坐小船
;		endif
	talk
endproc 

proc 150
	AUTOFLY 122 1
;	transmit 2202 12100 36300 1
endproc
