;罕见物资

proc 6
	RIDE_OFF 1
	strvar ret
	IS_CAN_BIANSHEN ret 1
	if ret != 1
		ADD_SYS_MSG_START
		ADD_SYS_MSG 你现在无法搬运箱子，可恢复正常形态后再来
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	endif
endproc

Proc 2
	strvar memID ret
	get_player_id 0 memID
	TARGET_ADD_STATUS 1 @memID 1330 3 1800 ret
	if ret = 0
		ADD_SYS_MSG_START
		ADD_SYS_MSG 你成功找到一个【罕见物资】，赶紧搬回去给巴顿领取奖励！
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
		
		add_sys_msg_start
		add_sys_msg $_player$成功找到一个【罕见物资】！为阵营作出了巨大的贡献！
		set_sys_msg_mode 18 0 0
		MAP_MSG_INFO
	endif
endproc
