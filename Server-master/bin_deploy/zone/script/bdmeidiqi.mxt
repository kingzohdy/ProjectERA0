


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b曾经我也有一个梦想，离开这个冰冷的地方，到鸟语花香里寻找一个温柔乡，现在我所有的梦都碎了，都醒了，只剩下这个港口，冷冰冰的在这矗着，你说我能离开吗？我不能离开！为什么？
	add_talk #b#b因为我的父亲，我父亲的父亲，他们都在这儿，他们从小就教导我人不能忘本，不能抛家……我的父亲为了我而丧命，我的妹妹为了我而被绑票，我不能就这样一走了之！
		task_add_option 1368 1 100
		task_add_option 1369 1 101
;		task_add_option 1370 1 102
;		task_add_option 1371 1 103
		task_add_option 1372 1 104
		task_add_option 1373 1 105
		task_add_option 1374 1 106
;		task_add_option 1375 1 107
		task_add_option 1378 1 108

		strvar Ret
		task_stat 1372 1 Ret
		if Ret = 2
			option 150 说服美第其

		endif
	
	talk
	
endproc 

PROC 100
	task_do_option 1368 1
ENDPROC
PROC 101
	task_do_option 1369 1
ENDPROC
;PROC 102
;	task_do_option 1370 1
;ENDPROC
;PROC 103
;	task_do_option 1371 1
;ENDPROC
PROC 104
	task_do_option 1372 1
ENDPROC
PROC 105
	task_do_option 1373 1
ENDPROC
PROC 106
	task_do_option 1374 1
ENDPROC
;PROC 107
;	task_do_option 1375 1
;ENDPROC
PROC 108
	task_do_option 1378 1
ENDPROC

proc 2

	get_trig_task 1 2
	if 2 = 1378
		task_fini 1378 1
	endif
	if 2 = 1372
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b曾经我也有一个梦想，离开这个冰冷的地方，到鸟语花香里寻找一个温柔乡，现在我所有的梦都碎了，都醒了，只剩下这个港口，冷冰冰的在这矗着，你说我能离开吗？我不能离开！为什么？
		add_talk #b#b因为我的父亲，我父亲的父亲，他们都在这儿，他们从小就教导我人不能忘本，不能抛家……我的父亲为了我而丧命，我的妹妹为了我而被绑票，我不能就这样一走了之！
			option 150 说服美第其
		talk
	endif
endproc 
proc 190
	return
endproc 
proc 150
	dialog 1
	add_talk $_npc$:
	add_talk #b#b多谢您的好意，我自己的妹妹，我自己来救，不需要您多费心。

		option 151 那你怎么还不去？
		option 200 你妹妹被劫走多久了？

	talk
endproc 
	
proc 151
	dialog 1
	add_talk $_npc$:
	add_talk #b#b我不能冒险，我不能再承受失败，如果再次失败，我妹妹就可能有生命危险！

		option 152 再次失败？你之前曾经失败过？
		option 160 也许她现在就已经有生命危险了？

	talk
endproc 
proc 152
	dialog 1
	add_talk $_npc$:
	add_talk #b#b是，我曾经找了很多人去救我的妹妹，但他们把我妹妹转移到了一个隐秘的地方，让我无法找到她。

		option 153 或者他们已经杀了她！
		option 170 他们究竟为什么要抓美莉纱？

	talk
endproc 
proc 153
	dialog 1
	add_talk $_npc$:
	add_talk #b#b不会的，美莉纱没有死！
		option 154 你怎么知道她没死？
	talk
endproc 
proc 154
	dialog 1
	add_talk $_npc$:
	add_talk #b#b她是我妹妹，如果她死了，我不会感应不到！请您别再浪费我的时间了，如果您只是想找个人说话的话可以回阳帆港，我想哈罗德一定很乐意与您畅谈。
		option 190 #打扰了
	talk
endproc 
	
proc 160
	dialog 1
	add_talk $_npc$:
	add_talk #b#b不会的，我确信美莉纱还活着，我能感觉到，她就在呼啸海湾，就在我身边！而且我很快就会救出她，多谢您的帮忙，再见。
		option 190 #再见
	talk
endproc 
proc 170
	dialog 1
	add_talk $_npc$:
	add_talk #b#b无论他们想要什么，我都会拿去交换美莉纱，但是他们甚至没有送来勒索信。
		option 171 你去找过她吗？
	talk
	
endproc
proc 171
	dialog 1
	add_talk $_npc$:
	add_talk #b#b当然，呼啸海湾这么大，要找一个人可谓是大海捞针，谈何容易！
		option 172 那现在要怎么办呢？
	talk

endproc
proc 172
	dialog 1
	add_talk $_npc$:
	add_talk #b#b等。
		option 190 #……
	talk
endproc
	
	
proc 200
	dialog 1
	add_talk $_npc$:
	add_talk #b#b今天是第二十七天。

		option 201 你怎么看似一点也不紧张啊？
	talk
endproc
proc 201
	dialog 1
	add_talk $_npc$:
	add_talk #b#b紧张？紧张能把我妹妹救回来吗？紧张能让我的父亲生还吗？紧张能让我拥有无上的力量吗？
		option 202 所以，你就出卖良心勾结海盗？
	talk
endproc
proc 202
	dialog 1
	add_talk $_npc$:
	add_talk #b#b我没有，你胡说八道，我没有勾结罗勒.朗辛！我没有！
		option 203 我有说你勾结罗勒.朗辛吗？
	talk
endproc
	
proc 203
	dialog 1
	add_talk $_npc$:
	add_talk #b#b我……阳帆港人人都知道罗勒.朗辛是海盗头子。
		option 204 朗辛可是个大色魔，可怜的莉纱……
	talk
endproc
	
proc 204
	dialog 1
	add_talk $_npc$:
	add_talk #b#b莉纱不会怎么样的，他答应过我，不会动莉纱一根汗毛！
		option 205 #肯说实话啦？
	talk
endproc
proc 205
	
	task_fini 1372 1
endproc
	
	
	