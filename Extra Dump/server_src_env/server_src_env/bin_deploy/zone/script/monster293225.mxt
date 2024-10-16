;;;;;冰雪下的女孩  美莉莎



proc 2
	GET_PLAYER_ID 30 0
	PLAYER_RIDE_ON @30 35 36
	if 35 = 0
		add_sys_msg_start
		add_sys_msg 地上躺着的这可是个小美眉啊，请先下马再抱人家吧！
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif


;	NEW_OWNER_TMP_ITEM 1460062 0 0 0 0 0 0 0 0 0 1
	APPLY_TMP_ITEM 1460077 0 1
	
endproc
proc 6
	GET_PLAYER_ID 30 0
	PLAYER_RIDE_ON @30 35 36
	if 35 = 0
	
		PLAYER_COLLECT_BREAK
		add_sys_msg_start
		add_sys_msg 地上躺着的这可是个小美眉啊，请先下马再抱人家吧！
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	

endproc 








