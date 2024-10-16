proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b铜鼓的弓箭是魔鬼最害怕的武器！！！
	task_add_option 1325 1 101
	task_add_option 1326 1 102
	task_add_option 1334 1 103


	task_add_option 1327 1 104
;	task_add_option 3004 1 105
	

	
		OPTION 201 #进入战鼓村
;		option 208 #重置战鼓村
		
	strvar Ret num1 num2
	GET_ITEM_NUM_INPAK 5300766 1 num1
	if num1 > 0
		TASK_STAT 3304 1 Ret
		if Ret = 2
			option 500 曲别针换别墅
		endif	
	endif
	GET_ITEM_NUM_INPAK 5300913 1 num2
	if num2 > 0
		TASK_STAT 3305 1 Ret
		if Ret = 2
			option 700 曲别针换别墅
		endif
	endif	
	
;	strvar ret
;	task_stat 3004 1 ret
;	if ret = 2
;		option 200 #我想要宠物，请送我过去吧！
;	endif
	talk
endproc


proc 500
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b刺客传我已经有了，不过你的这本看起来比我的要新一些，和我换吧，虽然我有些吃亏。
	
	option 501 #交换

	talk
endproc

proc 501
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300766 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300766 1 1
			
		;	rand Rad 100
		;	if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300783 1 0 0
		;	endif
		;	if @Rad >= 80 and @Rad < 95
		;		ADD_ITEM_NUM_INPAK 5300784 1 0 0
		;	endif
		;	if @Rad >= 95 and @Rad < 99
		;		ADD_ITEM_NUM_INPAK 5300767 1 0 0
		;	endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300768 1 0 0
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

proc 200
	AUTO_MOVE 20 1000 24200 13200 1460049 0 3081 1 3600
endproc 
proc 208
	RESET_PWORLD 104
endproc
proc 101
    task_do_option 1325 1
endproc
proc 102
    task_do_option 1326 1
endproc
proc 103
    task_do_option 1334 1
endproc
proc 104
    task_do_option 1327 1
endproc
proc 105
    task_do_option 3004 1
endproc
proc 201
	ENTER_PWORLD 104 0 0
endproc

proc 2
	get_trig_task 1 600

	if 600 = 1326
		dialog 1
		add_talk $_npc$:
		add_talk 铜鼓的弓箭是魔鬼最害怕的武器！！！
		OPTION 201 #进入战鼓村
		talk
	endif
	if 600 = 3004
		dialog 1
		add_talk $_npc$:
		add_talk #b#b铜鼓的弓箭是魔鬼最害怕的武器！！！
			option #200 我想要宠物，请送我过去吧！
		talk
	endif
endproc

proc 700
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b刺客传我已经有了，不过你的这本看起来比我的要新一些，和我换吧，虽然我有些吃亏。
		option 701 #交换

	talk
	
endproc
proc 701
	
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300913 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300913 1 1
			
		;	rand Rad 100
		;	if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300928 1 0 0
		;	endif
		;	if @Rad >= 80 and @Rad < 95
		;		ADD_ITEM_NUM_INPAK 5300915 1 0 0
		;	endif
		;	if @Rad >= 95 and @Rad < 99
		;		ADD_ITEM_NUM_INPAK 5300916 1 0 0
		;	endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300917 1 0 0
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




