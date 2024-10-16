;残余的能量
proc 1

	  strvar x y
	  get_player_mapinfo 0 1 2 x y

		  if 1 = 3100 

				GET_TARGET_INFO 3 1 40
				GET_MON_HP_PERCENT @40 30
				if @30 < 20
					strvar X Y Mid
					GET_TARGET_INFO 3 3 X
					GET_TARGET_INFO 3 4 Y
					GET_TARGET_INFO 3 1 Mid
					
					effect 0 0 playEffct($@X$,$@Y$,"effect\\entity\\9548_16.ent")
					RUN_TASK_PROGRESS 0 1878 1 1
					TARGET_ADD_STATUS 3 @Mid 1260 1 10 1
					
				else
					add_sys_msg_start
					add_sys_msg 你需要先削弱圣地守护者，才能使用!
					set_sys_msg_mode 3 0 0
					msg_info
				endif
				
		  else
			add_sys_msg_start
			add_sys_msg 请在指定地图使用！
			set_sys_msg_mode 3 0 0
			msg_info
		  
		  endif
		  
endproc 
