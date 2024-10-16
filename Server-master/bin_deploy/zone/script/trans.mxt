;新手村1传送员脚本
;50--地图定义ID 60--地图实例ID 70--坐标X 80--坐标Y

proc 1
	GET_PLAYER_MAPINFO  1 2 50 10 11
			
	dialog 1
	add_talk $_npc$:
	add_talk #b#b在这里看不到你的朋友？这里应征的人太多？想去其他位面的天空岛看看吗？选择你想去的地方吧，我可以免费送你过去！
	if 50 = 2000
		option 11 #天空岛二线
		option 12 #天空岛三线
		option 13 #天空岛四线
		option 14 #再见
	endif
	if 50 = 2001
		option 10 #天空岛一线
		option 12 #天空岛三线
		option 13 #天空岛四线
		option 14 #再见
	endif
	if 50 = 2002
		option 10 #天空岛一线
		option 11 #天空岛二线
		option 13 #天空岛四线
		option 14 #再见
	endif
	if 50 = 2003
		option 10 #天空岛一线
		option 11 #天空岛二线
		option 12 #天空岛三线
		option 14 #再见
	endif
	
	;option 15 #传送到铜鼓村
	talk
endproc
	
proc 10
	transmit 2000 17037 22834 1
endproc


proc 11
	transmit 2001 17037 22834 1
endproc
	
proc 12
	transmit 2002 17037 22834 1
endproc
	
proc 13
	transmit 2003 17037 22834 1
endproc
	
proc 14
	return
endproc

;proc 15
;	get_player_base_att 1 2 3 4
;	if 3 > 9
;		transmit 2100 4181 3890 1
;	else
;		add_sys_msg_start
;		add_sys_msg 你还太嫩了,继续修炼再来找我吧!
;		set_sys_msg_mode 4 0 0
;		msg_info
;	endif
;endproc

