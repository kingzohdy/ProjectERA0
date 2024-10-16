;第6层通往7层，
;
proc 10
	get_task_status 265 100
	get_task_var 265 0 101
	if 100 = 2 and 101 >= 7
		GET_MAP_MONSTER_NUM 12
		if 12 = 0
		goto_map 30207 3305 2912 10 995
		if 995 = 0
			msg_player 3 2 进入九重天第七层
		else
			msg_player 3 2 进入九重天第七层失败！
		endif
		else
			msg_player 3 2 本层地图的怪物尚未清完，请先将怪物全部消灭！
		endif
	else
		msg_player 3 2 你尚未完成前面关卡的挑战，无法前往第七层！
	endif
endproc 