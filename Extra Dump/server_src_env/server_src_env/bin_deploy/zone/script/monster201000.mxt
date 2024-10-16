;
proc 2

	TASK_STAT 4024 1 20
	if 20 = 2 

		ADD_ITEM_NUM_INPAK 5300022 1 1 12

		add_sys_msg_start
		add_sys_msg 你获得了一把钥匙！
		set_sys_msg_mode 4 0 0
		msg_info
	endif


endproc
	