;巡逻守卫


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b噼…噼噼…
	add_talk #b#b#b#b无发现可疑入侵
	add_talk #b#b噼…
	add_talk #b#b#b#b信不信由你，反正我是信了。
	add_talk #b#b噼噼…
		
	task_stat 4148 1 20
	if 20 = 2
		option 150 #大马丁让我来领取“采集场黑名单”
	endif
	talk
endproc 	

proc 150
	task_stat 4148 1 20
	if 20 = 2
		add_item_num_inpak 5301002 1 0 0
	endif
	
endproc
