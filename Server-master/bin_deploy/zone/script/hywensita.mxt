;牧牛任务,温斯特
;温斯特 
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b想体验牛仔的生活吗？想要赚取外快吗？要成为人人欣羡的斗牛勇士吗？
	task_add_option 2000 1 100
	option 102 赶牛秘诀
;	TASK_STAT 2000 1 10
;	if 10 = 2
;		OPTION 101 领取牧牛
;	endif
	talk
endproc 
proc 2
	get_trig_task 1 2 
	if 2 = 2000 
		call 101 
	endif
endproc
proc 100
	task_do_option 2000 1
endproc
proc 101
;	NEW_CONVOY_MON 213018 2639 5225 500 0 0 0 360 2000 1 2
	add_sys_msg_start
	add_sys_msg 用赶牛鞭抽打牧牛，使其移动
	set_sys_msg_mode 4 0 0
	msg_info	
endproc
proc 102
	DIALOG 1
;	add_talk $_npc$:
	add_talk 赶牛其实很简单，步骤如下：
	add_talk #b#b#c186506拿起赶牛鞭，用力抽打牛的屁股，它就会朝着你面对的方向奔跑。#n
	add_talk #b
	add_talk 赶牛其实很困难，注意要点：
	add_talk #b#b#c186506 1、要不断调整方向#n
	add_talk #b#b#c186506 2、要在规定的时间内将牛送至农场#n
	option 1 返回
	talk
endproc
