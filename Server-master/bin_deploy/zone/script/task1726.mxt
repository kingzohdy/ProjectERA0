
proc 3
;提交删除临时机械
	DEL_ROLE_TMP_ITEM 0 1
	DEL_ROLE_UNUSED_TMP_ITEM 0 1460018 1
		add_sys_msg_start
		add_sys_msg 陌生的气息让马儿仓皇逃去，转眼不见踪迹……
		set_sys_msg_mode 3 0 0
		msg_info
endproc 
proc 4

	GET_ITEM_NUM_INPAK 5300480 1 10
	DEC_ITEM_NUM_INPAK 5300480 @10 1

endproc

proc 5
;放弃任务删除临时机械
	DEL_ROLE_TMP_ITEM 0 1
	DEL_ROLE_UNUSED_TMP_ITEM 0 1460018 1
		add_sys_msg_start
		add_sys_msg 陌生的气息让马儿仓皇逃去，转眼不见踪迹……
		set_sys_msg_mode 3 0 0
		msg_info
endproc



