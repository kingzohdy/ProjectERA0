
proc 2
	rand 50 1000
	ID_PLAYER_BASE_ATT 0 1 1 41 1
	mul 41 @41
	
	;29%
	if @50 >= 0 and @50 < 290
		dev 41 10
		ADD_MONEY 0 @41 20 123 ;单人加金钱
		if 123 < 0
			add_sys_msg_start
			add_sys_msg 您包裹中的银币已经达到上限
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		;获得银币系统有提示
	endif
	
	;29%
	if @50 >= 290 and @50 < 580			
		ADD_EXP 0 @41 123   ;单人加经验
		if 123 < 0
			add_sys_msg_start
			add_sys_msg 您当前存储的经验值已经达到上限，请提升等级，否则无法继续获得经验
			set_sys_msg_mode 3 0 0
			msg_info
		else
			add_sys_msg_start
			add_sys_msg 你获到了$@41$点经验
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif

	;29%
	if @50 >= 580 and @50 < 870
		dev 41 10
		ADD_MONEY 0 @41 21 123 ;单人加金钱
		if 123 < 0
			add_sys_msg_start
			add_sys_msg 您包裹中的绑银已经达到上限
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		;获得银币系统有提示
	endif
	
	;1%   大银币
	if @50 >= 870 and @50 < 880
		dev 41 10
		ADD_MONEY 0 @41 20 123 ;单人加金钱
		if 123 < 0
			add_sys_msg_start
			add_sys_msg 您包裹中的银币已经达到上限
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		;获得银币系统有提示
	endif
	
	;1%  大经验
	if @50 >= 880 and @50 < 890		
		ADD_EXP 0 @41 123   ;单人加经验
		if 123 < 0
			add_sys_msg_start
			add_sys_msg 您当前存储的经验值已经达到上限，请提升等级，否则无法继续获得经验
			set_sys_msg_mode 3 0 0
			msg_info
		else
			add_sys_msg_start
			add_sys_msg 你获到了$@41$点经验
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif

	;1%    大绑银
	if @50 >= 890 and @50 < 900
		dev 41 10
		ADD_MONEY 0 @41 21 123 ;单人加金钱
		if 123 < 0
			add_sys_msg_start
			add_sys_msg 您包裹中的绑银已经达到上限
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		;获得银币系统有提示
	endif
	
	if @50 >= 900
	
		GET_PACK_SPACE 0 11
		strvar ibind x1

		if 11 < 1
			add_sys_msg_start
			add_sys_msg 背包空间不足！
			set_sys_msg_mode 4 0 0
			msg_info
		else		
			;4%
			if @50 >= 900 and @50 < 940
				;5233000	幻化果实
				var x1 5233000
				ADD_BIND_ITEM_NUM_INPAK @x1 5 1 14
				add_sys_msg_start
				add_sys_msg $_player$早就听闻航海王的宝藏中藏着金银财宝无数，不料此番竟得到了幻化果实，真是鸿运当头，无人能挡！
				set_sys_msg_mode 18 0 0
				SVR_MSG_INFO 
			endif
					
			;3%
			if @50 >= 940 and @50 < 970
				;5052936	赏金宝匣
				var x1 5052936
				ADD_BIND_ITEM_NUM_INPAK @x1 5 1 14
				add_sys_msg_start
				add_sys_msg $_player$早就听闻航海王的宝藏中藏着金银财宝无数，不料此番竟得到了赏金宝匣，真是鸿运当头，无人能挡！
				set_sys_msg_mode 18 0 0
				SVR_MSG_INFO 
			endif
				
			;3%
			if @50 >= 970 and @50 < 1000
				;5012127	蓝色妖姬
				var x1 5012127
				ADD_BIND_ITEM_NUM_INPAK @x1 1 1 14
				 
				add_sys_msg_start
				add_sys_msg $_player$早就听闻航海王的宝藏中藏着金银财宝无数，不料此番竟得到了蓝色妖姬，真是鸿运当头，无人能挡！
				set_sys_msg_mode 18 0 0
				SVR_MSG_INFO 
			endif
		endif
	endif
endproc

