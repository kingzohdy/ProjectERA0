
proc 1

;	add_sys_msg_start
;	add_sys_msg 春节活动开始啦！在时间城中央找到春节使者乐天，参加各种有趣的活动吧！
;	set_sys_msg_mode 6 0 0
;	svr_msg_info

	NEW_MAP_LIFE_NPC 1 1000 307023 29400 33300 0 1814400 zcletian.mac
	
endproc 
proc 2
	GET_MAP_INST 1000 10
	DEL_NPC 307023 15
	add_sys_msg_start
	add_sys_msg 春节活动已经圆满结束！在此预祝大家在2013年里人品爆发！ROLL点100！满载而归！！
	set_sys_msg_mode 6 0 0
	svr_msg_info
	
endproc
