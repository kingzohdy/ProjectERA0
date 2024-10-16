;封印石三，刷第三波怪
proc 2
	get_pworld_var 12 32 1 14
	get_pworld_var 88 32 1 32
	If 14 = 0

		;dec_item_num_inpak 5300050 1 0
			
		new_mon 231000 3 3887 2546 600 0 0 0 2
		new_mon 231000 2 3687 2246 600 0 0 0 2
		var 14 1
		add 32 1
		set_pworld_var 88 32 @32 1
		set_pworld_var 12 32 @14 1
		add_sys_msg_start
	        add_sys_msg 酋长苏醒：封印解除 $@32$/4
	        set_sys_msg_mode 3 0 0
	        map_msg_info
     
	endif
	
endproc
