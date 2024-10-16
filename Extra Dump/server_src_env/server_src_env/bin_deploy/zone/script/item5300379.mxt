;锋利的尖刀

proc 1
		GET_ITEM_NUM_INPAK 5300380 0 10
		;新鲜
		GET_ITEM_NUM_INPAK 5300381 0 11
		;粗糙
		GET_ITEM_NUM_INPAK 5300382 0 12
		;略粗
	
		
		if 10 < 1 and 11 < 1 and 12 < 1
			add_sys_msg_start
			add_sys_msg 请先采集一些新鲜的竹节
			set_sys_msg_mode 3 0 0
			msg_info

		endif
		if 10 >= 1
			dec_item_num_inpak 5300380 1 0
			rand 50 4
			add 50 100
			if 50 = 100
			;粗糙
				ADD_ITEM_NUM_INPAK 5300381 1 1 0
				
				add_sys_msg_start
				add_sys_msg 这个竹节片距离粗细适宜竹签还很遥远，还要再多削几下！
				set_sys_msg_mode 3 0 0
				msg_info
				return
			endif
			if 50 = 101
			;略粗
				ADD_ITEM_NUM_INPAK 5300382 1 1 0
				add_sys_msg_start
				add_sys_msg 距离粗细适宜的竹签已经很近了，再多削几下吧！
				set_sys_msg_mode 3 0 0
				msg_info
				return
			endif
			if 50 = 102
			;适宜
				ADD_ITEM_NUM_INPAK 5300383 1 1 0
				dec_item_num_inpak 5300379 1 0
				return
			endif
			if 50 = 103
			;纤细
				ADD_ITEM_NUM_INPAK 5300384 1 1 0
				add_sys_msg_start
				add_sys_msg 你用的力气过大，竹签被削的过细，再去采些新鲜的竹节吧！
				set_sys_msg_mode 3 0 0
				msg_info
				return
				
			endif
		endif
		if 11 >= 1
			dec_item_num_inpak 5300381 1 0
			rand 51 3
			add 51 100
			switch 51
				case 100
					ADD_ITEM_NUM_INPAK 5300382 1 1 0
					add_sys_msg_start
					add_sys_msg 距离粗细适宜的竹签已经很近了，再多削几下吧！
					set_sys_msg_mode 3 0 0
					msg_info
					return
				endcase
				case 101
					ADD_ITEM_NUM_INPAK 5300383 1 1 0
					dec_item_num_inpak 5300379 1 0
					return
				endcase
				case 102
					ADD_ITEM_NUM_INPAK 5300384 1 1 0
					add_sys_msg_start
					add_sys_msg 你用的力气过大，竹签被削的过细，再去采些新鲜的竹节吧！
					set_sys_msg_mode 3 0 0
					msg_info
					return
				endcase
			
			endswitch
		
		endif
		if 12 >= 1
		
			dec_item_num_inpak 5300382 1 0
			rand 52 2
			add 52 100
			switch 52 
				case 100
					ADD_ITEM_NUM_INPAK 5300383 1 1 0
					dec_item_num_inpak 5300379 1 0
					return
				endcase 
				case 101
					ADD_ITEM_NUM_INPAK 5300384 1 1 0
					add_sys_msg_start
					add_sys_msg 你用的力气过大，竹签被削的过细，再去采些新鲜的竹节吧！
					set_sys_msg_mode 3 0 0
					msg_info
				endcase
			
			endswitch
		endif


endproc 
















