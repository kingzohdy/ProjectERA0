;ºã×êËéÆ¬

proc 1
		strvar ret x
		GET_PACK_SPACE 0 ret
		if ret >= 1
			DEC_ITEM_NUM_INPAK 5052115 1 10
			if 10 = 0
				GET_PLAYER_BASE_ATT 1 x 1 1
				if x = 1
					rand 20 5
					if 20 = 0
						ADD_ITEM_NUM_INPAK 3050030 1 1 0
					endif
					if 20 = 1
						ADD_ITEM_NUM_INPAK 3450030 1 1 0
					endif
					if 20 = 2
						ADD_ITEM_NUM_INPAK 3550030 1 1 0
					endif
					if 20 = 3
						ADD_ITEM_NUM_INPAK 3750030 1 1 0
					endif
					if 20 = 4
						ADD_ITEM_NUM_INPAK 3850030 1 1 0
					endif
				else
				rand 30 5
					if 30 = 0
						ADD_ITEM_NUM_INPAK 3050031 1 1 0
					endif
					if 30 = 1
						ADD_ITEM_NUM_INPAK 3450031 1 1 0
					endif
					if 30 = 2
						ADD_ITEM_NUM_INPAK 3550031 1 1 0
					endif
					if 30 = 3
						ADD_ITEM_NUM_INPAK 3750031 1 1 0
					endif
					if 30 = 4
						ADD_ITEM_NUM_INPAK 3850031 1 1 0
					endif
				endif
			endif
		else
			add_sys_msg_start
			add_sys_msg ±³°ü¿Õ¼ä²»×ã£¡
			set_sys_msg_mode 4 0 0
			msg_info
		endif
endproc 
