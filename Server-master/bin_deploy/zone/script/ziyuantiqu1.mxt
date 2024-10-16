


proc 1
	DIALOG 1
	
	add_talk $_npc$:
	add_talk #b#b您好，我是为公会开发生态的艾莉亚！当然，开发生态无非为了更好的采集资源，所以我们这里还承办了采集车的制造呢。
	add_talk #b#b
		option 200 #提取采集车资源放入背包
			
	strvar Ret
	task_stat 3675 1 Ret
	if Ret = 2
		option 250 我是来领取整理好的资源的
	endif
	talk
endproc 

proc 200
	LAYDOWN_CLAN_ORE 0 0
endproc
proc 250
	DIALOG 1	
	add_talk $_npc$:
 	add_talk #b#b啊，找到你要的资源了，堆积的东西实在太多，让你久等非常的抱歉。
		option 251 #领取整理好的资源
	talk
endproc 
proc 251
	add_item_num_inpak 5300872 1 1 0
endproc 