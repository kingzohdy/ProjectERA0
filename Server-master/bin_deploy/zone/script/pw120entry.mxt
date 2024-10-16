;80副本,入口npc,乌索尔
proc 1
	strvar l
	GET_PLAYER_BASE_ATT 1 0 l 0
	if l < 80
		dialog 1
		ADD_TALK $_npc$:
		ADD_TALK #b#b你好，#Name#,能到我这里，看来你已经经过了不少的历练。这将会是一个更艰难的挑战。
		talk
		
	else
		dialog 1
		ADD_TALK $_npc$:
		ADD_TALK #b#b你好，#Name#,能到我这里，看来你已经经过了不少的历练。这将会是一个更艰难的挑战。
		ADD_TALK #b#b远古要塞的势力已经在这里驻扎了许久，这里病疫蔓延，民不聊生，再加上这些邪恶势力的侵害，我们已经苦不堪言了。
		option 100 #进入副本
		talk
		
	endif
	
endproc
proc 100
;	enter_pworld 120 0 1
	
	OPEN_PWORLD_UI 5
endproc
