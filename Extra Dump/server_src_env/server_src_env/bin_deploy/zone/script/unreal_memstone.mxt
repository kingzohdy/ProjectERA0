;彩虹幻境-记忆石NPC

proc 1
	get_unreal_layer 1 10
	DIALOG 1
	add_talk $_npc$:
	var 20 @10
	MOD @20 10
	if @10 != 1 and @20 = 1
	add_talk #b#b你当前所在位置-幻境第$@10$层，点击"开启当前传送点",可以保存当前幻境位置,下次您可以带领您的队伍,直接传送到此位置。
	option 100 #开启当前传送点
	else
	add_talk #b#b我是记忆之门.也许在幻境的某一层,我能帮你记下来...
	endif
	option 200 #返回时间城
	talk	
endproc

proc 100
	save_unreal_layer 1
endproc

proc 200
	choose 0 201 202 确定返回时间城吗?
endproc

proc 201
	is_captain 20 0
	if @20 = 0
		TEAM_TRANS_INSAMEMAPINST 1000 40765 35339 0
	else
		TRANSMIT 1000 40765 35339 0
	endif
endproc

proc 202

endproc
