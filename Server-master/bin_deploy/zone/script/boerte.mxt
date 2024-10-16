

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b这旁边的就是永夜的入口，康斯坦丁牧师就是从那里进入永夜的，那已经是很久以前的事了，勇敢的冒险者，请召集你的兄弟和朋友们进去协助康斯坦丁牧师对抗黑暗吧。
	add_talk #b#b等消灭了亚库里斯之后，我邀请你去月亮山脉的瀑上镇去游玩，那里有很多很多稀有矿石，还有各式各样新奇的玩意，如果你愿意，还可以长住在那。
	

;20~30

	task_add_option 1420 1 104
	task_add_option 1421 1 105
	task_add_option 1499 1 106
	task_add_option 1418 1 107
	task_add_option 1416 1 108
	task_add_option 2241 1 109
	task_add_option 1419 1 110


	option 300 #进入永夜镇
;	option 301 #重置永夜镇

;	task_stat 1416 1 20
;	if 1 = 0 and 20 = 2
;		GET_ITEM_NUM_INPAK 5300291 1 30
;		if 30 < 1
;			option 400 获取天悯之链
;		endif
;	endif
	
	
	talk
endproc 


PROC 104
	task_do_option 1420 1
ENDPROC
PROC 105
	task_do_option 1421 1
ENDPROC
PROC 106
	task_do_option 1499 1
ENDPROC
PROC 107
	task_do_option 1418 1
ENDPROC
PROC 108
	task_do_option 1416 1
ENDPROC
PROC 109
	task_do_option 2241 1
	TASK_FINI 2241 1

ENDPROC
PROC 110
	task_do_option 1419 1
ENDPROC


proc 300
;	enter_pworld 102 0 1
;		OPEN_PWORLD_UI 102
		OPEN_PWORLD_UI 1
	
endproc
proc 301

	RESET_PWORLD 102
		add_sys_msg_start
		add_sys_msg 副本重置成功
		set_sys_msg_mode 4 0 0
		msg_info
endproc
proc 400
	
    DIALOG 1
   	ADD_TALK $_npc$:       
    ADD_TALK #b#b这天悯之链是康斯坦丁牧师所赠，怎能轻易借给你？

	option 401 正是康斯坦丁牧师让我来的。
	TALK
endproc
proc 401
    DIALOG 1
   	ADD_TALK $_npc$:       
    ADD_TALK 康斯坦丁牧师现在的情况如何？

	option 402 他现在正为恶魔的呢喃侵扰。
	TALK
endproc
proc 402
    DIALOG 1
   	ADD_TALK $_npc$:       
    ADD_TALK 可是康斯坦丁牧师曾嘱咐过我，无论发生什么事都不能将天悯之链摘下!

	option 403 天悯之链能助他摆脱堕落的侵扰。
	TALK
endproc
proc 403
    DIALOG 1
   	ADD_TALK $_npc$:       
    ADD_TALK #c676464<博尔特毅然摘下颈上的天悯之链，递到你面前>#n
	ADD_TALK 请收好，请一定要解救康斯坦丁牧师脱险，我会永远感激你，朋友。


	option 404 #有了天悯之链康斯坦丁牧师一定会平安无事
	TALK
endproc

proc 404

;	add_item_num_inpak 5300291 1 1 0
	add_sys_msg_start
	add_sys_msg 获得了天悯之链！
	set_sys_msg_mode 4 0 0
	msg_info
endproc

proc 2

	get_trig_task 1 2

	if 2 = 2241
		task_fini 2241 1
	endif
	if 2 = 1419
		task_fini 1419 1
	endif
endproc
