;腐皮豺狼


proc 2

	strvar Ret playmid
;	MON_GET_KILLER_ID 1 playmid
;	SET_PLAYER_CURR @playmid 1			
	MON_OWNER_TASK_CHECK 1411 Ret
;	TASK_STAT 1411 1 20	
;	if 20 = 2 
;		strvar playmid

	if Ret = 0
		rand 110 100
		if 110 < 35

			GET_MON_INFO 10 11 12 13 14 15 16 17 
			CALL_MON 1 303048 500 120 @16 @17 200	
			
				get_task_var 1316 1 32 1 11
				var 11 @200
				set_task_var 1316 1 32 @11 1		
			
				strvar Ret mid1 mid2 mid3 mid4 mid5 mid6 Map1 Map2 Map3 Map4 Map5 Map6
				GET_TEAM_PLAYER_ID Ret 0 0 mid1 mid2 mid3 mid4 mid5 mid6
				GET_ID_PLAYER_INFO @mid1 5 Map1
				GET_ID_PLAYER_INFO @mid2 5 Map2
				GET_ID_PLAYER_INFO @mid3 5 Map3
				GET_ID_PLAYER_INFO @mid4 5 Map4
				GET_ID_PLAYER_INFO @mid5 5 Map5
				GET_ID_PLAYER_INFO @mid6 5 Map6
				
				if Ret = 0
					if Map1 = 2200
						SET_PLAYER_CURR @mid1 1
						add_sys_msg_start
						add_sys_msg 小心！戴尔奈拉出现了，赶快消灭她！！
						set_sys_msg_mode 3 0 0
						MSG_INFO 
					endif
					if Map2 = 2200
						SET_PLAYER_CURR @mid2 1
						add_sys_msg_start
						add_sys_msg 小心！戴尔奈拉出现了，赶快消灭她！！
						set_sys_msg_mode 3 0 0
						MSG_INFO 
					endif
					if Map3 = 2200
						SET_PLAYER_CURR @mid3 1
						add_sys_msg_start
						add_sys_msg 小心！戴尔奈拉出现了，赶快消灭她！！
						set_sys_msg_mode 3 0 0
						MSG_INFO 
					endif
					if Map4 = 2200
						SET_PLAYER_CURR @mid4 1
						add_sys_msg_start
						add_sys_msg 小心！戴尔奈拉出现了，赶快消灭她！！
						set_sys_msg_mode 3 0 0
						MSG_INFO 
					endif
					if Map5 = 2200
						SET_PLAYER_CURR @mid5 1
						add_sys_msg_start
						add_sys_msg 小心！戴尔奈拉出现了，赶快消灭她！！
						set_sys_msg_mode 3 0 0
						MSG_INFO 
					endif
					if Map6 = 2200
						SET_PLAYER_CURR @mid6 1
						add_sys_msg_start
						add_sys_msg 小心！戴尔奈拉出现了，赶快消灭她！！
						set_sys_msg_mode 3 0 0
						MSG_INFO 
					endif
				else
					add_sys_msg_start
					add_sys_msg 小心！戴尔奈拉出现了，赶快消灭她！！
					set_sys_msg_mode 3 0 0
					MSG_INFO
				
				endif

		endif
	endif

endproc

