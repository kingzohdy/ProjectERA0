proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b不要害怕，$_player$，这是我的意识世界，我的灵魂被束缚在冗长沉重的噩梦之中，所以至今无法清醒……
    add_talk #b#b#c676464<雷蒙盖顿指着地上躺着的自己>#n
    add_talk #b#b你是被钦点的勇士，来解救我脱离梦境，请按照我的指引来做，并且你只有一次机会，一旦失败，只能从头再来！
	
	task_add_option 3411 1 100
		option 120 #我还没做好准备，下次再来吧！
	talk
	
endproc
proc 100

	task_do_option 3411 1

endproc 
proc 120

	TRANSMIT 4402 18500 8300 1

endproc

