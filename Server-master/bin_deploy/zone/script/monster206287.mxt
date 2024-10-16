;70副本,副本死亡机器人5
;死亡事件
proc 2
	strvar x y
	GET_MON_INFO 1 0 0 0 0 0 x y
	var 10 0
	new_mon_by_dir 206323 1 @x @y 0 0 0 @10 1 2	
endproc
