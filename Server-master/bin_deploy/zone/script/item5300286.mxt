;合成甜品



proc 1

	get_item_num_inpak 5300286 0 10 
	get_item_num_inpak 5300332 0 11 
			if 11 != 0
				
				if 10 != 0
					
					dec_item_num_inpak 5300286 1 0
					dec_item_num_inpak 5300332 1 0

					add_item_num_inpak 5300333 1 1 0
					add_sys_msg_start
					add_sys_msg 获取了老鼠们爱吃的甜品！！
					set_sys_msg_mode 3 0 0
					msg_info
				
				else
					
					add_sys_msg_start
					add_sys_msg 合成需要卡萨布兰卡的汁液！！
					set_sys_msg_mode 3 0 0
					msg_info
				
				endif
			
			else
				
				add_sys_msg_start
				add_sys_msg 合成需要稻米！！
				set_sys_msg_mode 3 0 0
				
				map_msg_info
			
			endif

endproc


