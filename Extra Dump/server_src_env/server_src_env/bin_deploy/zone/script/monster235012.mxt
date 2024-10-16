;守护蛇神 1746
 
proc 1
;怪物刷出
	STRVAR a
endproc
proc 2
;怪物死亡
		get_task_var 1746 1 8 1 a
		var a 1
		set_task_var 1746 1 8 @a 1
			add_sys_msg_start
			add_sys_msg 蛇神死亡了，任务失败了
			set_sys_msg_mode 3 0 0
			msg_info
endproc

