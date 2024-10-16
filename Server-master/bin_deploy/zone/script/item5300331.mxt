;合成甜品


proc 1

	get_item_num_inpak 5300338 0 10 
	get_item_num_inpak 5300331 0 11 
			if 11 != 0
				
				if 10 != 0
					GET_PLAYER_MAPINFO 0 20 21 22 23
					effect 0 0 playEffct($@22$,$@23$,"effect\\entity\\9548_16.ent")	

					dec_item_num_inpak 5300338 1 0

					add_item_num_inpak 5300286 1 1 0
;				add_sys_msg_start
;					add_sys_msg 获取了卡萨布兰卡的汁液！！
;					set_sys_msg_mode 3 0 0
;					msg_info

				else 

					add_sys_msg_start
					add_sys_msg 合成需要卡萨布兰卡！！
					set_sys_msg_mode 3 0 0
					msg_info
				
				endif

			else
				
				add_sys_msg_start
				add_sys_msg 合成需要磨石！！
				set_sys_msg_mode 3 0 0
				
				map_msg_info
			
			endif

endproc


