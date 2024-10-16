;贵重物资

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
	TARGET_ADD_STATUS 1 @memID 1330 2 1800 ret
	if ret = 0
		ADD_SYS_MSG_START
		ADD_SYS_MSG 你成功找到一个【贵重物资】，赶紧搬回去给巴顿领取奖励！
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	endif
endproc
