;·âÓ¡Ê¯¶þ£¬Ë¢µÚ¶þ²¨¹Ö
proc 2
	get_pworld_var 8 32 1 13
	get_pworld_var 88 32 1 32
	If 13 = 0

		;dec_item_num_inpak 5300049 1 0
		
		new_mon 231000 3 3887 2546 600 0 0 0 2
		new_mon 231000 2 3687 2246 600 0 0 0 2
		var 13 1
		add 32 1
		set_pworld_var 88 32 @32 1
		set_pworld_var 8 32 @13 1
		add_sys_msg_start
	        add_sys_msg Çõ³¤ËÕÐÑ£º·âÓ¡½â³ý $@32$/4
	        set_sys_msg_mode 3 0 0
	        map_msg_info
	endif
	
endproc
	