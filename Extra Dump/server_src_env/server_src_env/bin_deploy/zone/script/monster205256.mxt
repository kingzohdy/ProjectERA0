;70副本,充能机器人
;死亡事件
proc 2
	strvar x y
	GET_MON_INFO 1 0 0 0 0 0 x y
	var 10 0
	new_mon_by_dir 206325 1 @x @y 0 0 0 @10 1 2
	TARGET_ADD_STATUS 3 @2 3065 1 10800 1
endproc
