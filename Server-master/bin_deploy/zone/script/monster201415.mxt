;祭祖神殿 ,招采集物开关2
;死亡事件
proc 2
	;灵魂流开关计数
	get_pworld_var 8 32 1 12
	add 12 1
	set_pworld_var 8 32 @12 1
	if 12 = 3
		PLAYER_MOVIE_START 1 0
	endif
endproc
