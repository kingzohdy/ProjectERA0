proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk 我在马路边，看到一个箱！
	OPTION 104 #打开仓库
	talk
endproc

PROC 104
	OPEN_UI 0
ENDPROC 