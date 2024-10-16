;70副本,密码机器人npc
proc 1
	get_pworld_var 12 32 1 13
	if 13 != 0
		return
	endif
	DIALOG 1
;	add_talk $_npc$:
	add_talk #b#b#c676464<你仔细研究着这个制作精良的高科技，发现它似乎没有一丝破绽，连个类似锁匙孔的凹陷都没有，正当你一愁莫展时，发现上面印着一行小字>#n
	add_talk #b#b细节决定成败，注意生活中的每一个细节，你就会找到成功的钥匙。
	add_talk #b#b#b#b#b#b#b#b#b#b#b#b#b#b#b#b#b——薇薇安
	option 101 #开始破译密码
	talk
endproc
proc 101
	;副本阶段,主要用于tick时要触发的proc
	
	get_pworld_var 12 32 1 13
	if 13 = 0
		var 13 1
		set_pworld_var 12 32 @13 1 
		var 10 45
		new_mon_by_dir 261041 1 20638 29712 0 0 0 @10 1 2
	endif
	call 1
endproc
