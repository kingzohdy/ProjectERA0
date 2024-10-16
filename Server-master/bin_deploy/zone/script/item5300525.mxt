;匕首
proc 1

	  strvar x y
	  get_player_mapinfo 0 1 2 x y

		get_point_dist @x @y 11800 9900 10
		  if 1 = 50900 or 1 = 51000 or 1 = 51100 or 1 = 51200
				
;				NEW_LIFE_MON 290003 1 @x @y 0 0 0 0 3 0 0
;				ADD_ITEM_NUM_INPAK 5300336 1 1 0
;				dec_item_num_inpak 5300514 1 0

				strvar Hp1 Num Hpm hpb ret
				
				GET_ID_PLAYER_INFO 0 2 Hp1
				GET_ID_PLAYER_INFO 0 9 Hpm
				
				if Hpm > 0
					mul Hp1 100
					dev Hp1 @Hpm
					var hpb @Hp1
					
					if hpb < 30
						add_sys_msg_start
						add_sys_msg 你现在太虚弱了，先补充一些血量吧！
						set_sys_msg_mode 1 0 0
						msg_info
						return
					endif

				;	ADD_PLAYER_HP 0 -1000 1
					strvar ret
					ADD_PLAYER_HP2 0 -25 ret
					if ret = 0
					
						strvar Hp2 Hpm2 hpb2
						GET_ID_PLAYER_INFO 0 2 Hp2
						GET_ID_PLAYER_INFO 0 9 Hpm2
						mul Hp2 100
						dev Hp2 @Hpm2
						var hpb2 @Hp2
						if hpb2 <= 30
					;		COMMIT_TASK 3411 1
							task_fini 3411 1
							DEL_NPC 304041 1
							strvar Loc_1
							get_pworld_var 15 8 0 Loc_1
							var Loc_1 1
							set_pworld_var 15 8 @Loc_1 1
							return
						endif
					endif	
		
					get_pworld_var 24 8 1 Num
					var Num 1
					set_pworld_var 24 8 @Num 1
					if Num < 3 
						add_sys_msg_start
						add_sys_msg 滴入雷蒙盖顿身体上的血液还不够多，再多使用几次匕首
						set_sys_msg_mode 1 0 0
						msg_info
					endif
					if Num >= 3
						add_sys_msg_start
						add_sys_msg 你感觉到眼前有点模糊，掌心的痛楚又让你清醒了些
						set_sys_msg_mode 1 0 0
						msg_info
					endif
				endif
		  else
			add_sys_msg_start
			add_sys_msg 请在指定地图使用！
			set_sys_msg_mode 3 0 0
			msg_info
		  
		  endif
		  
endproc 
