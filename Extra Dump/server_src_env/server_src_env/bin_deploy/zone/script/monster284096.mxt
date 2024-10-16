;70副本,蓝密码光环2脚本
proc 1
	var 10 0
	set_mon_var 0 32 @10 1
endproc
;死亡脚本
proc 2
	;删红环
	delete_mon_by_defid 284049 1
	delete_mon_by_defid 284088 1
	delete_mon_by_defid 284089 1
	delete_mon_by_defid 284090 1
	delete_mon_by_defid 284091 1
	delete_mon_by_defid 284092 1
	delete_mon_by_defid 284093 1
	delete_mon_by_defid 284094 1
	delete_mon_by_defid 284095 1
	
	;区域内有无玩家
	strvar num all
	mon_get_player_num_in_rect 1 400 400 num
	GET_PWORLD_ROLE_NUM all 1		
	;密码光环站对次数
	VAR 14 0
	get_pworld_var 16 32 1 14
	if num = @all
		add 14 1
	else
		var 14 0
		;副本阶段,主要用于tick时要触发的proc
		var 13 0
		set_pworld_var 12 32 @13 1
		mon_skill 10352 1
		effect 0 0 playEffct(19620,29736,"effect\\entity\\1000900_8.ent")
		add_sys_msg_start
		add_sys_msg 很遗憾，你们失败了，不过还能选择重新再来！
		set_sys_msg_mode 3 0 0
		map_msg_info
	endif
	set_pworld_var 16 32 @14 1 
	;死亡后计时器清空
	;10计时数
	var 10 0
	set_pworld_var 0 32 @10 1
	
endproc
proc 4
	;区域内有无玩家
	strvar num all
	mon_get_player_num_in_rect 1 400 400 num
	GET_PWORLD_ROLE_NUM all 1
	if num = @all
		get_mon_var 0 32 1 10
		if 10 = 0
			add_sys_msg_start
			add_sys_msg 恭喜你，答对了，请继续保持，直至密码破译成功！
			set_sys_msg_mode 3 0 0
			map_msg_info
		else
			if 10 = 1
				add_sys_msg_start
				add_sys_msg 在这条通往未来的道路上，做出正确的选择很重要！
				set_sys_msg_mode 3 0 0
				map_msg_info
			else
				add_sys_msg_start
				add_sys_msg 更重要的是，在做出正确的选择后，能坚持走下去！
				set_sys_msg_mode 3 0 0
				map_msg_info
			endif
		endif
		
		add 10 1
		set_mon_var 0 32 @10 1
	else
		add_sys_msg_start
		add_sys_msg 你们必须要同心协力，才能通过此关！
		set_sys_msg_mode 3 0 0
		map_msg_info
	endif
endproc
