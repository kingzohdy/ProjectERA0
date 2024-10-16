;竹子

proc 2

	ADD_ITEM_NUM_INPAK 5300380 1 1 0
		
endproc
proc 6
	
	GET_ITEM_NUM_INPAK 5300380 0 10
	if 10 >= 1
		PLAYER_COLLECT_BREAK
		add_sys_msg_start
		add_sys_msg 你已经有一个新鲜的竹节了
		set_sys_msg_mode 3 0 0
		msg_info
	endif


endproc 