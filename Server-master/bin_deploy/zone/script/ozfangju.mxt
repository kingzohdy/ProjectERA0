;商人 
; 打开商店

proc 1
	dialog 1
	add_talk 大熊说过：在我这绝对不会少你的秤

	OPTION 110 #打开商店 

	talk
endproc
proc 110
    OPEN_NPC_SHOP
endproc
