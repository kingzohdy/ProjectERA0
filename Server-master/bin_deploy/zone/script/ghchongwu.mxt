

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b这里是把凶兽驯养成公会宠物的地方，不想受伤就远离这里。
	
;		option 120 #建设/升级生物中心

	talk
endproc 

proc 120
	open_ui 24

endproc

