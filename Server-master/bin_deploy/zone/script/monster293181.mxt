

proc 2
	strvar Rad Rad1
	var Rad 0
	rand Rad 10
	if Rad < 3
	;成功
		add_sys_msg_start
		add_sys_msg 这个瓶盖内部刻着复杂的图画，你有种强烈的感觉，就是它了！
		set_sys_msg_mode 3 0 0
		msg_info
		
		add_item_num_inpak 5300597 1 1 0
		
	endif
	if Rad >= 3 and Rad < 10
		rand Rad1 2
		if Rad1 = 0
			add_sys_msg_start
			add_sys_msg 这里没有瓶盖！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		if Rad1 = 1
			add_sys_msg_start
			add_sys_msg 这个瓶盖没什么特别！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif	

endproc



