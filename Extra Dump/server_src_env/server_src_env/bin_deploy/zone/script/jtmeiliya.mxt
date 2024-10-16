
;梅利亚
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b不要问为什么，这没法解释，你必须自己去体验，我们所经历的完全是另外一个层次……也许在数年、数十年后，你也会达到这个层次，但首先，你需要加入我们，需要对阿拜多斯神表现出绝对的忠诚！
	task_add_option 1562 1 103
	task_add_option 1563 1 104
	task_add_option 1564 1 105
	task_add_option 1565 1 106
	task_add_option 1566 1 107
	task_add_option 1567 1 108
	strvar Ret
	task_stat 1565 1 Ret
	if Ret = 2
		option 150 开始答题
	
	endif
	talk
endproc 

PROC 103
	task_do_option 1562 1
ENDPROC
PROC 104
	task_do_option 1563 1
ENDPROC
PROC 105
	task_do_option 1564 1
ENDPROC
PROC 106
	task_do_option 1565 1
ENDPROC
PROC 107
	task_do_option 1566 1
ENDPROC
PROC 108
	task_do_option 1567 1
ENDPROC

proc 2
	get_trig_task 1 2
 
	if 2 = 1567
		task_fini 1567 1
	endif 

endproc



proc 150
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b你是$_player$吗？
		option 151 是
		option 300 #不是
	talk
endproc
proc 300
	strvar Mid Hp
	GET_PLAYER_ID 1 Mid 
	EFFECT_ID 2 @Mid playMontion(%d,"8000_5")
	
	GET_ID_PLAYER_INFO 0 2 Hp
	if Hp > 1000
		ADD_PLAYER_HP 0 -1000 1
		return
	else
		sub @Hp 1
		mul @Hp -1
		ADD_PLAYER_HP 0 @Hp 1
		
	endif
endproc
proc 151
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b奴隶能获得自由吗？
                  
		option 300 #能
		option 152 不能
	talk
endproc
proc 152
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b你愿意成为血色追随者的一员？
                  
		option 153 愿意
		option 300 #不愿意
	talk
endproc
proc 153
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b血是红色的吗？
 
		option 154 是
		option 300 #不是
	talk
endproc
proc 154
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b有生之年你会背叛我们吗？

		option 300 #会
		option 155 不会
	talk
endproc
proc 155
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b你愿意为了血色计划献出生命？

		option 156 愿意
		option 300 #不愿意
	talk
endproc
proc 156
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b你愿意将灵魂奉献给阿拜多斯？

		option 157 愿意
		option 300 #不愿意
	talk
endproc
proc 157
	task_fini 1565 1
endproc















