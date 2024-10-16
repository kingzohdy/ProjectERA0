;304127	帕克


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b趁着暑假，我想举办一个焰火舞会，费了九牛二虎之力才把方案做好，正筹备地如火如荼的时候，突然冒出个“反调五人组”！
    ; add_talk #b#b
	add_talk #b#b不得不承认，他们这名字起的真是太贴切了！我敢保证，没有谁比他们更适合这个名字！
	add_talk #b#b
	add_talk #b#b#c186506你知道么，我已经发出了上万张请柬了！得想个办法，必须要让这舞会顺利举行！#n
	add_talk #b#b#cff0080不要轻易放弃任务哟，这会让你直接跳过本环重新开始新的环任务，每天最多可进行5环任务。#n
	
		TASKCYCLE_ADD_OPTION 142 1 140
	
	talk
endproc 

proc 140
	TASKCYCLE_DO_OPTION 142 1
endproc
