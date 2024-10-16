
proc 2
	strvar n
	get_pworld_var 52 32 1 n

	if n = 0
		strvar x y
		get_mon_info 1 1 1 1 1 1 x y
		new_mon 200204 1 @x @y 0 0 0 0 0
		
		strvar curKillId
		get_word_pwd_mon_info 1 curKillId
		
		strvar correctKillId
		get_pworld_var 20 32 1 correctKillId
		add @correctKillId 1

		if @correctKillId = @curKillId
			set_pworld_var 20 32 @correctKillId 1
		else
			strvar f g l
			var f 0
			set_pworld_var 20 32 @f 1
			
			get_pworld_var 24 32 1 g
			add @g 1
			set_pworld_var 24 32 @g 1
			
			var l 0
			set_pworld_var 44 32 @l 1
			
			add_sys_msg_start
			add_sys_msg 很遗憾这道题你答错了
			set_sys_msg_mode 4 0 0
			map_msg_info
			
			; clean mon
			strvar cleanflag
			var cleanflag 1
			set_pworld_var 52 32 @cleanflag 1
			destroy_mon_by_defid 1 200203
			destroy_mon_by_defid 1 200204
		endif
	endif
	
endproc










