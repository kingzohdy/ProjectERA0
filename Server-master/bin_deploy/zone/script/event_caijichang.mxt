proc 1
	add_sys_msg_start
	add_sys_msg 资源采集场已经开放,获得采集许可证的玩家可以使用许可证进入!
	set_sys_msg_mode 0 0 0
	SVR_MSG_INFO
	
	
endproc 

proc 2
	add_sys_msg_start
	add_sys_msg 采集场已经关闭,获得采集许可证的玩家,请等待下次开启!
	set_sys_msg_mode 0 0 0
	SVR_MSG_INFO


endproc 