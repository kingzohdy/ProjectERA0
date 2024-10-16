;使用木炭


proc 1


	get_player_mapinfo 0 1 2 3 4
	if 2 = 2200
			dec_item_num_inpak 5300328 1 0
			GET_MON_INFO 10 11 12 13 14 15 16 17 
			effect 0 0 playEffct($@16$,$@17$,"effect\\entity\\9546_16.ent")	

			rand 100 3
			add 100 100
			switch 100
				case 100
					call_mon 2 236000 0 80 @16 @17 1
				endcase
				case 101
					call_mon 3 236000 0 80 @16 @17 1
				endcase
				case 102
					call_mon 4 236000 0 80 @16 @17 1
				endcase
			
			endswitch
			del_mon 1

	  else
			add_sys_msg_start
			add_sys_msg 目标不在本地图中！
			set_sys_msg_mode 3 0 0
			msg_info
	  endif

endproc


