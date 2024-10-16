;;公会巡逻卫兵

proc 1 

	strvar Ret Rad1 Rad2
	is_in_my_clan_city  Ret
	if Ret = 0 
		rand Rad1 2
		if Rad1 = 0
			mon_speech 0 0 0 1 这可不是你家，最好是老老实实的呆着！
		endif
		if Rad1 = 1
			mon_speech 0 0 0 1 你最好别找麻烦！
		endif
	endif	
	if Ret = 1
		rand Rad2 3
		if Rad2 = 0
			mon_speech 0 0 0 1 保卫城市是我们的责任！
		endif
		if Rad2 = 1
			mon_speech 0 0 0 1 如果有人欺负你，可以来告诉我！
		endif
		if Rad2 = 2
			mon_speech 0 0 0 1 注意那些外来人，他们随时可能会捣乱！
		endif
	endif
endproc 