;未打磨的无瑕的宝石



proc 1

		strvar Rad Ret
		rand Rad 11
		switch Rad
			case 0	
				add_item_num_inpak 5210000 1 2 Ret
				if Ret = 0 
					dec_item_num_inpak 5012110 1 10
					if 10 = 0
						add_item_num_inpak 5210000 1 1 0
					endif
				else
					call 100
				endif
			endcase
			case 1	
				add_item_num_inpak 5210100 1 2 Ret
				if Ret = 0 
					dec_item_num_inpak 5012110 1 10
					if 10 = 0
						add_item_num_inpak 5210100 1 1 0
					endif
				else
					call 100
				endif
			endcase
			case 2	
				add_item_num_inpak 5211000 1 2 Ret
				if Ret = 0 
					dec_item_num_inpak 5012110 1 10
					if 10 = 0
						add_item_num_inpak 5211000 1 1 0
					endif
				else
					call 100
				endif
			endcase
			case 3	
				add_item_num_inpak 5211200 1 2 Ret
				if Ret = 0 
					dec_item_num_inpak 5012110 1 10
					if 10 = 0
						add_item_num_inpak 5211200 1 1 0
					endif
				else
					call 100
				endif
			endcase
			case 4	
				add_item_num_inpak 5211100 1 2 Ret
				if Ret = 0 
					dec_item_num_inpak 5012110 1 10
					if 10 = 0
						add_item_num_inpak 5211100 1 1 0
					endif
				else
					call 100
				endif
			endcase
			case 5	
				add_item_num_inpak 5212000 1 2 Ret
				if Ret = 0 
					dec_item_num_inpak 5012110 1 10
					if 10 = 0
						add_item_num_inpak 5212000 1 1 0
					endif
				else
					call 100
				endif
			endcase
			case 6	
				add_item_num_inpak 5212100 1 2 Ret
				if Ret = 0 
					dec_item_num_inpak 5012110 1 10
					if 10 = 0
						add_item_num_inpak 5212100 1 1 0
					endif
				else
					call 100
				endif
			endcase
			case 7	
				add_item_num_inpak 5212200 1 2 Ret
				if Ret = 0 
					dec_item_num_inpak 5012110 1 10
					if 10 = 0
						add_item_num_inpak 5212200 1 1 0
					endif
				else
					call 100
				endif
			endcase
			case 8	
				add_item_num_inpak 5212400 1 2 Ret
				if Ret = 0 
					dec_item_num_inpak 5012110 1 10
					if 10 = 0
						add_item_num_inpak 5212400 1 1 0
					endif
				else
					call 100
				endif
			endcase
			case 9	
				add_item_num_inpak 5213300 1 2 Ret
				if Ret = 0 
					dec_item_num_inpak 5012110 1 10
					if 10 = 0
						add_item_num_inpak 5213300 1 1 0
					endif
				else
					call 100
				endif
			endcase
			case 10	
				add_item_num_inpak 5213400 1 2 Ret
				if Ret = 0 
					dec_item_num_inpak 5012110 1 10
					if 10 = 0
						add_item_num_inpak 5213400 1 1 0
					endif
				else
					call 100
				endif
			endcase
			
		endswitch
	
endproc
proc 100
	add_sys_msg_start
	add_sys_msg 背包空间不足，无法使用该物品。
	set_sys_msg_mode 1 0 0
	msg_info
endproc 
