
;贝尼苏
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b很高兴见到你，$_player$，你是来帮助我们击退敌人的吧？
	task_add_option 1581 1 101
	task_add_option 1582 1 102
	task_add_option 1583 1 103
	
	task_add_option 1584 1 104
	task_add_option 1585 1 105

	strvar Ret
	task_stat 1583 1 Ret
	if Ret = 2
	;	option 150 打开隐秘机关
	
	endif
	strvar Ret num1 num2
	GET_ITEM_NUM_INPAK 5300773 1 num1
	if num1 > 0
		TASK_STAT 3304 1 Ret
		if Ret = 2
			option 500 #曲别针换别墅
		endif	
	endif
	GET_ITEM_NUM_INPAK 5300918 1 num2
	if num2 > 0
		TASK_STAT 3305 1 Ret
		if Ret = 2
			option 700 曲别针换别墅
		endif
	endif
	talk
endproc 


PROC 101
	task_do_option 1581 1
ENDPROC
PROC 102
	task_do_option 1582 1
ENDPROC
PROC 103
	task_do_option 1583 1
ENDPROC
PROC 104
	task_do_option 1584 1
ENDPROC
PROC 105
	task_do_option 1585 1
ENDPROC


;proc 2
;	get_trig_task 1 2
 
;	if 2 = 1562
;		task_fini 1562 1
;	endif 
;endproc

proc 300
	return
endproc

proc 150
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b转动机关
		option 151 左转
		option 300 #右转
	talk
endproc 
proc 151
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b转动机关
		option 152 左转
		option 300 #右转
	talk
endproc
proc 152
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b转动机关
		option 300 #左转
		option 153 右转
	talk

endproc
proc 153
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b转动机关
		option 300 #左转
		option 154 右转
	talk
endproc

proc 154
	task_fini 1583 1
endproc



proc 500
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b你的这个是真迹吗？看着也不像，不如我拿魔法帝国年表和你换吧，啊？不要！哎，别走啊，想要什么你自己挑吧！
	
	option 501 #交换

	talk
endproc
proc 501
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300773 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300773 1 1
			
		;	rand Rad 100
		;	if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300774 1 0 0
		;	endif
		;	if @Rad >= 80 and @Rad < 95
		;		ADD_ITEM_NUM_INPAK 5300775 1 0 0
		;	endif
		;	if @Rad >= 95 and @Rad < 99
		;		ADD_ITEM_NUM_INPAK 5300776 1 0 0
		;	endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300777 1 0 0
		;	endif
		else
			add_sys_msg_start
			add_sys_msg 背包空间不足！
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg 你身上没有可以交换的物品！
		set_sys_msg_mode 1 0 0
		msg_info
	endif
endproc 

proc 700
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b你的这个是真迹吗？看着也不像，不如我拿魔法帝国年表和你换吧，啊？不要！哎，别走啊，想要什么你自己挑吧！
		option 701 #交换

	talk
	
endproc
proc 701
	
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300918 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300918 1 1
			
		;	rand Rad 100
		;	if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300919 1 0 0
		;	endif
		;	if @Rad >= 80 and @Rad < 95
		;		ADD_ITEM_NUM_INPAK 5300920 1 0 0
		;	endif
		;	if @Rad >= 95 and @Rad < 99
		;;		ADD_ITEM_NUM_INPAK 5300921 1 0 0
		;	endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300922 1 0 0
		;	endif
		else
			add_sys_msg_start
			add_sys_msg 背包空间不足！
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg 你身上没有可以交换的物品！
		set_sys_msg_mode 1 0 0
		msg_info
	endif
endproc 







