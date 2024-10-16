

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b这个世界上，有很多东西都比生命更重要，比如名望。可是，名望又是从哪里来的呢？
	
	TASKCYCLE_ADD_OPTION 107 1 12
	
	
	task_add_option	2248 1 103
	
	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300781 1 num1
	if num1 > 0
		TASK_STAT 3304 1 Ret
		if Ret = 2
			option 500 曲别针换别墅
		endif
	endif
	strvar Ret num2
	GET_ITEM_NUM_INPAK 5300924 1 num2
	if num2 > 0
		TASK_STAT 3305 1 Ret
		if Ret = 2
			option 700 曲别针换别墅
		endif
	endif
	talk
endproc 
PROC 12
	TASKCYCLE_DO_OPTION 107 1
ENDPROC

PROC 103
	task_do_option 2248 1
ENDPROC


proc 500
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b真没想到这本我寻觅数十年的《玛雅野史》竟然在你的手中，跟我换吧，你想要什么？别墅？
	add_talk #b#b别墅我倒是有，不过，你只能获得一周的使用权！怎么样，换么？
	add_talk #b#b这种野史可没几个人愿意收藏哟，好好考虑一下吧，过了这个村可没这个店啦！
	option 501 #交换
		
	talk
endproc
proc 501
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300781 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300781 1 1
			
		;	rand Rad 100
		;	if  @Rad < 95
		;		ADD_ITEM_NUM_INPAK 5300782 1 0 0
		;	endif
		;	if @Rad >= 95 
				ADD_ITEM_NUM_INPAK 5300785 1 0 0
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
;proc 2
;	get_trig_task 1 2
;	if 2 = 4438 
;		task_fini 4438 1
;	endif  
 ;
;endproc

proc 700
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b真没想到这本我寻觅数十年的《玛雅野史》竟然在你的手中，跟我换吧，你想要什么？别墅？
	add_talk #b#b别墅我倒是有，不过，你只能获得一周的使用权！怎么样，换么？
	add_talk #b#b这种野史可没几个人愿意收藏哟，好好考虑一下吧，过了这个村可没这个店啦！

		option 701 #交换

	talk
	
endproc
proc 701
	
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300924 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300924 1 1
			
		;	rand Rad 100
		;	if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300935 1 0 0
		;	endif
		;	if @Rad >= 80 and @Rad < 95
		;		ADD_ITEM_NUM_INPAK 5300926 1 0 0
		;	endif
		;	if @Rad >= 95 and @Rad < 99
		;		ADD_ITEM_NUM_INPAK 5300927 1 0 0
		;	endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300928 1 0 0
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


