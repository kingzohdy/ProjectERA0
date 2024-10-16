
;;;;;胜利的喜悦

proc 6


	strvar X Y M Mid
	GET_PLAYER_MAPINFO 0 0 0 X Y
	GET_POINT_DIST 17700 9800 @X @Y M
	if M < 1
		
		GET_MAP_NPC_ID 350016 Mid 1
		mon_speech @Mid 2 0 1 尊敬的暖泉镇父老乡亲，阿拜多斯被击败了！我们胜利了！
		task_fini 1585 1
	endif

endproc 





















