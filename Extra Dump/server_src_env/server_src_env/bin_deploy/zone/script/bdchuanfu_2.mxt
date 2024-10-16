


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b我这艘船不出海，只开往阳帆港，您要坐吗？免费哟！
		strvar Ret2

;		task_stat 1374 1 Ret2
;		if Ret2 = 2
			option 150 #回到阳帆港
;		endif
	talk
endproc 

proc 150
;	AUTOFLY 19999 1

	transmit 2202 16500 17000 1
endproc
