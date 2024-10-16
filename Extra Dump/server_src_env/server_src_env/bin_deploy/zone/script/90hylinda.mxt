; 琳达


proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b紫薯为什么要叫紫薯呢？为什么煮紫薯的水是绿的呢？既然如此，为什么不叫绿薯或碧薯或青薯呢？
	
		task_add_option 2125 1 100
		task_add_option 2126 1 101
		task_add_option 2127 1 102
		task_add_option 2128 1 103
		
		strvar Ret
		task_stat 2128 1 Ret
		if Ret = 2
			option 150 让我选吃哪种？
		endif
		
		strvar Ret1
		task_stat 2109 1 Ret1
		if Ret1 = 2
			option 180 请问您听说过啃脸男吗？

		endif
		strvar ret2 num
		task_stat 2126 1 ret2
		GET_ITEM_NUM_INPAK 5300901 1 num
		if ret2 = 2 and num = 0
			option 190 #刚刚煮的被别人捞走了，再给我一块吧
		endif
	talk
endproc

PROC 100
	task_do_option 2125 1
ENDPROC
PROC 101
	task_do_option 2126 1
ENDPROC
PROC 102
	task_do_option 2127 1
ENDPROC
PROC 103
	task_do_option 2128 1
ENDPROC


proc 2
	get_trig_task 1 2
	if 2 = 2128
		dialog 1
		add_talk $_npc$:
		add_talk #b#b紫薯为什么要叫紫薯呢？为什么煮紫薯的水是绿的呢？既然如此，为什么不叫绿薯或碧薯或青薯呢？
			option 150 让我选吃哪种？
		talk
	endif
endproc

proc 150
	dialog 1
	add_talk $_npc$:
	add_talk #b#b对啊，你来选，吃“蓝色”紫薯，还是“绿色”紫薯！$_player$，我可就把小命交到你的手上了，你千万要慎重选择哟！

		option 151 绿色紫薯
		option 152 蓝色紫薯

	talk

endproc 
proc 151
	dialog 1
	add_talk $_npc$:
	add_talk #b#b也好，总归是绿色食品嘛，应该是有益无害的哈，那就它吧。

		option 153 #你说什么就是什么吧。

	talk
endproc
proc 152
	dialog 1
	add_talk $_npc$:
	add_talk #b#b惨绿惨绿的汤汤水水看起来像毒药，而这蓝色嘛，天也是蓝的，海水也是蓝的，应该不会有什么问题哈。

		option 153 #你说什么就是什么吧。

	talk
endproc
proc 153
;	ADD_ITEM_NUM_INPAK 5300903 1 1 0
	task_fini 2128 1
	return
endproc 

proc 180
	dialog 1
	add_talk $_npc$:
	add_talk #b#b当然听说过，当初还是我把这个消息带回兴东镇的呢，我可是镇上第一个知道这个消息的人！我还特地把相关的新闻都收集起来做了一份剪报……

		option 181 哇，好厉害呀，我能看看吗？

	talk
endproc
proc 181
	dialog 1
	add_talk $_npc$:
	add_talk #b#b也不是很厉害啦，一般般啦，你想借，当然可以，不过……可千万别弄坏了哟。

		option 182 #放心吧，我一定会好好保管它！
	talk
endproc
proc 182
	
	ADD_ITEM_NUM_INPAK 5300895 1 1 0
endproc 

proc 190
	ADD_ITEM_NUM_INPAK 5300901 1 1 0
endproc 









