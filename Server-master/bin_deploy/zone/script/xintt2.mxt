proc 1
	dialog 1
	add_talk 这个东西出现的那么突然,你完全无法理解它为什么会存在在这里.
	option 101 #给我一个BOSS! 
	talk
endproc 

proc 101
	new_mon 282008 1 26191 7396 0 0 0 0 23
	new_mon 290000 1 24099 7391 0 0 0 0 22
	del_npc 0 24
endproc 
