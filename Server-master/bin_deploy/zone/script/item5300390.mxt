;合成空白的面具


proc 1

	get_item_num_inpak 5300390 0 10 
	get_item_num_inpak 5300388 0 11 
			if 11 != 0
				
				if 10 != 0
					
					dec_item_num_inpak 5300390 1 0
					dec_item_num_inpak 5300388 1 0
;					GET_PLAYER_MAPINFO 0 20 21 22 23
;					effect 0 0 playEffct($@22$,$@23$,"effect\\entity\\9548_16.ent")	
					
					add_item_num_inpak 5300392 1 1 0
					
					add_sys_msg_start
					add_sys_msg 获得空白的面具！！
					set_sys_msg_mode 3 0 0
					msg_info
				
				else
					
					add_sys_msg_start
					add_sys_msg 合成需要宣纸！！
					set_sys_msg_mode 3 0 0
					msg_info
				
				endif
			
			else
				
				add_sys_msg_start
				add_sys_msg 合成需要竹签！！
				set_sys_msg_mode 3 0 0
				
				map_msg_info
			
			endif

endproc


