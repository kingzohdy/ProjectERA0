;牧牛决赛,赶牛鞭
;赶牛鞭
proc 1
	strvar defid mid x y px py angle
	GET_MON_INFO 1 defid mid 0 0 0 x y
	if defid != 241015 and defid != 241016 and defid != 241017 and defid != 241018 and defid != 241019
		add_sys_msg_start
		add_sys_msg 请先选中您的牧宠
		set_sys_msg_mode 3 0 0
		msg_info	
		return
	endif
	is_player_owner_mon 1 0 10
	if 10 = 0
		add_sys_msg_start
		add_sys_msg 请先选中您的牧宠
		set_sys_msg_mode 3 0 0
		msg_info	
		return
	endif
	GET_PLAYER_MAPINFO  1 0 0 px py
	GET_POINT_DIST @x @y @px @py 1
	if 1 > 500
		add_sys_msg_start
		add_sys_msg 离目标太远了
		set_sys_msg_mode 3 0 0
		msg_info	
		return
	endif
	var angle 60
	if defid = 241015 
		var angle 40
	endif
	if defid = 241016
		var angle 30
	endif 
	if defid = 241017
		var angle 40
	endif
	if defid = 241018
		var angle 30
	endif
	if defid = 241019
		var angle 40
	endif
	MON_FORCED_MOVE 1 @mid @px @py @angle 500
	if 1 != 0
		add_sys_msg_start
		add_sys_msg 前方有障碍，请换个角度再鞭打它
		set_sys_msg_mode 3 0 0
		msg_info	
	endif
endproc
