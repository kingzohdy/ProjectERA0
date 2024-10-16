
;初级记忆扫描器
proc 1
	strvar ret play player1 player2 player3 player4 player5 player6 num
	strvar px py m mx my pnum lev
	strvar pinjunlev lev1 lev2 lev3 lev4 lev5 lev6 templev maxlev slev
	strvar defilev
	var pnum 0
	var pinjunlev 0
	var templev 0
	var maxlev 0
	var slev 0
	var defilev 5
	GET_TEAM_PLAYER_ID ret 0 num player1 player2 player3 player4 player5 player6
	if ret = 0
		GET_MON_INFO 10 0 0 0 0 0 px py 
		if @10 = 0
			if @num <= 0
				return
			endif
			
			var 20 0
			while 20 < @num
				add 20 1
				if @20 = 1
					GET_ID_PLAYER_INFO @player1 3 mx
					GET_ID_PLAYER_INFO @player1 4 my
					GET_ID_PLAYER_INFO @player1 8 lev1
					;在这个坐标的20米内
					GET_POINT_DIST @px @py @mx @my m
					if m < 2000
						add pnum 1
						add templev @lev1
						if @lev1 >= @maxlev
							var maxlev @lev1
						endif
					endif
				elseif @20 = 2
					GET_ID_PLAYER_INFO @player2 3 mx
					GET_ID_PLAYER_INFO @player2 4 my
					GET_ID_PLAYER_INFO @player1 8 lev2
					;在这个坐标的20米内
					GET_POINT_DIST @px @py @mx @my m
					if m < 2000
						add pnum 1
						add templev @lev2
						if @lev2 >= @maxlev
							var maxlev @lev2
						endif
					endif
				elseif @20 = 3
					GET_ID_PLAYER_INFO @player3 3 mx
					GET_ID_PLAYER_INFO @player3 4 my
					GET_ID_PLAYER_INFO @player1 8 lev3
					;在这个坐标的20米内
					GET_POINT_DIST @px @py @mx @my m
					if m < 2000
						add pnum 1
						add templev @lev3
						if @lev3 >= @maxlev
							var maxlev @lev3
						endif
					endif
				elseif @20 = 4
					GET_ID_PLAYER_INFO @player4 3 mx
					GET_ID_PLAYER_INFO @player4 4 my
					GET_ID_PLAYER_INFO @player1 8 lev4
					;在这个坐标的20米内
					GET_POINT_DIST @px @py @mx @my m
					if m < 2000
						add pnum 1
						add templev @lev4
						if @lev4 >= @maxlev
							var maxlev @lev4
						endif
					endif
				elseif @20 = 5
					GET_ID_PLAYER_INFO @player5 3 mx
					GET_ID_PLAYER_INFO @player5 4 my
					GET_ID_PLAYER_INFO @player1 8 lev5
					;在这个坐标的20米内
					GET_POINT_DIST @px @py @mx @my m
					if m < 2000
						add pnum 1
						add templev @lev5
						if @lev5 >= @maxlev
							var maxlev @lev5
						endif
					endif
				elseif @20 = 6
					GET_ID_PLAYER_INFO @player6 3 mx
					GET_ID_PLAYER_INFO @player6 4 my
					GET_ID_PLAYER_INFO @player1 8 lev6
					;在这个坐标的20米内
					GET_POINT_DIST @px @py @mx @my m
					if m < 2000
						add pnum 1
						add templev @lev6
						if @lev6 >= @maxlev
							var maxlev @lev6
						endif
					endif
				else
					return
				endif
			endwhile
			 
			if @templev != 0
				var pinjunlev @templev
				dev pinjunlev @pnum
			endif
			
			var slev @maxlev
			sub slev @pinjunlev
			if @slev > @defilev
				var pinjunlev @maxlev
				sub pinjunlev @defilev
			endif
			
			if @pnum >= 4 and @pnum <= 6
				;有队伍。且包括自己之内有4到6人在可疑的人15米之内
				strvar LV MonMid px py	
				DEC_ITEM_NUM_INPAK 5300006 1 90
				if @90 = 0
					DESTROY_MON 1
					GET_PLAYER_MAPINFO  0 0 0 px py
					NEW_LIFE_DYN_MON 1 254027 1 @px @py 500 0 0 MonMid 0 @pinjunlev 0 600
					SET_MON_OWNER 0 @MonMid
					
					GET_TASK_VAR 4635 0 32 124 16
					if @124 = 0 
						add @16 1
						SET_TASK_VAR 4635 0 32 @16 123
					endif
					
					if @16 = 1
						SET_TASK_VAR 4635 4 32 @MonMid 125
					elseif @16 = 2
						SET_TASK_VAR 4635 8 32 @MonMid 125
					elseif @16 = 3
						SET_TASK_VAR 4635 12 32 @MonMid 125
					elseif @16 = 4
						SET_TASK_VAR 4635 16 32 @MonMid 125
					else
						SET_TASK_VAR 4635 0 32 0 125
					endif
					
					add_sys_msg_start
					add_sys_msg 咔嚓一声响，可疑的人露出真貌，竟是高级间谍
					set_sys_msg_mode 3 0 0
					TEAM_MSG_INFO
				endif
			elseif @pnum >= 2 and @pnum <= 3
				;有队伍。且包括自己之内有2到3人在可疑的人15米之内
				strvar LV MonMid px py	
				DEC_ITEM_NUM_INPAK 5300006 1 90
				if @90 = 0
					DESTROY_MON 1
					GET_PLAYER_MAPINFO  0 0 0 px py
					NEW_LIFE_DYN_MON 1 254026 1 @px @py 500 0 0 MonMid 0 @pinjunlev 0 600
					SET_MON_OWNER 0 @MonMid
					
					GET_TASK_VAR 4635 0 32 124 16
					if @124 = 0 
						add @16 1
						SET_TASK_VAR 4635 0 32 @16 123
					endif
					
					if @16 = 1
						SET_TASK_VAR 4635 4 32 @MonMid 125
					elseif @16 = 2
						SET_TASK_VAR 4635 8 32 @MonMid 125
					elseif @16 = 3
						SET_TASK_VAR 4635 12 32 @MonMid 125
					elseif @16 = 4
						SET_TASK_VAR 4635 16 32 @MonMid 125
					else
						SET_TASK_VAR 4635 0 32 0 125
					endif
					
					add_sys_msg_start
					add_sys_msg 咔嚓一声响，可疑的人现出原形，竟是初级间谍
					set_sys_msg_mode 3 0 0
					TEAM_MSG_INFO
				endif
			elseif @pnum = 1
				;有队伍。但是只有自己在线
				strvar LV MonMid px py	
				DEC_ITEM_NUM_INPAK 5300006 1 90
				if @90 = 0
					DESTROY_MON 1
					GET_PLAYER_MAPINFO  0 0 0 px py
					GET_PLAYER_BASE_ATT 0 0 LV 0
					NEW_LIFE_DYN_MON 1 254026 1 @px @py 500 0 0 MonMid 0 @LV 0 600
					SET_MON_OWNER 0 @MonMid
					
					GET_TASK_VAR 4635 0 32 124 16
					if @124 = 0 
						add @16 1
						SET_TASK_VAR 4635 0 32 @16 123
					endif
					
					if @16 = 1
						SET_TASK_VAR 4635 4 32 @MonMid 125
					elseif @16 = 2
						SET_TASK_VAR 4635 8 32 @MonMid 125
					elseif @16 = 3
						SET_TASK_VAR 4635 12 32 @MonMid 125
					elseif @16 = 4
						SET_TASK_VAR 4635 16 32 @MonMid 125
					else
						SET_TASK_VAR 4635 0 32 0 125
					endif
					
					add_sys_msg_start
					add_sys_msg 咔嚓一声响，可疑的人现出原形，竟是初级间谍
					set_sys_msg_mode 3 0 0
					TEAM_MSG_INFO
				endif
			else
				return
			endif
		endif
	else
		;没队伍
		strvar LV MonMid px py	
		DEC_ITEM_NUM_INPAK 5300006 1 90
		if @90 = 0
			DESTROY_MON 1
			GET_PLAYER_MAPINFO  0 0 0 px py
			GET_PLAYER_BASE_ATT 0 0 LV 0
			NEW_LIFE_DYN_MON 1 254026 1 @px @py 500 0 0 MonMid 0 @LV 0 600
			SET_MON_OWNER 0 @MonMid
			
			GET_TASK_VAR 4635 0 32 124 16
			if @124 = 0 
				add @16 1
				SET_TASK_VAR 4635 0 32 @16 123
			endif
			
			if @16 = 1
				SET_TASK_VAR 4635 4 32 @MonMid 125
			elseif @16 = 2
				SET_TASK_VAR 4635 8 32 @MonMid 125
			elseif @16 = 3
				SET_TASK_VAR 4635 12 32 @MonMid 125
			elseif @16 = 4
				SET_TASK_VAR 4635 16 32 @MonMid 125
			else
				SET_TASK_VAR 4635 0 32 0 125
			endif
					
			add_sys_msg_start
			add_sys_msg 咔嚓一声响，可疑的人现出原形，竟是初级间谍
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
endproc 

