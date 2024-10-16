;商人 
; 打开商店

proc 1
	dialog 1
	add_talk 维奇说：来来来,走过路过机会莫错过!错过好机会回家要受罪!

	OPTION 110 #打开商店 

	talk
endproc
proc 110
    OPEN_NPC_SHOP
endproc
