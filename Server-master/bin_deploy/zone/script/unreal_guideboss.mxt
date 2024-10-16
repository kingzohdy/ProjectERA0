;幻境-BOSS地图NPC向导
proc 1
	is_captain 10 0
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b下一个未知的世界就在眼前,你准备好了吗?
	;判断角色是否是队长
	if @10 = 0
		is_last_unreal_boss_map 0 20
		;不是最后一个BOSS地图
		if @20 = 0
		option 100 #前往下一层
		endif
	endif
	option 300 #返回时间城
	talk	
endproc 

proc 100
	is_captain 10 0
	if @10 = 0 
		UNREAL_ENTER 0 2
	else
		add_sys_msg_start
		add_sys_msg	可惜了，您现在已不是队长，无法将队伍传入到下一层了。
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
endproc

proc 300
	choose 0 301 302 确定返回时间城吗?
endproc

proc 301
	TRANSMIT 1000 40765 35339 0
endproc

proc 302

endproc


