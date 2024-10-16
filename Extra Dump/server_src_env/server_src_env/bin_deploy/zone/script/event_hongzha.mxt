; 轰炸大战

proc 1
	add_sys_msg_start
	add_sys_msg 【轰炸大战】的实战模拟正式打响，几十批重型战车已全数候命随时出击！
	set_sys_msg_mode 1 0 0
	SVR_MSG_INFO
	
	SET_EVENT_PWORLD 0 302
endproc

proc 2
	add_sys_msg_start
	add_sys_msg 狂轰乱炸的实战模拟已圆满结束，【轰炸大战】暂告一段落
	set_sys_msg_mode 1 0 0
	SVR_MSG_INFO
endproc
