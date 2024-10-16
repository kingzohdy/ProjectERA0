

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b人生苦短，$_player$，若没什么重要的事情，请不要打扰我寻找黄金遗迹。若是你能提供关于黄金遗迹的线索，我会给你非常丰厚的报酬。
	
	
;20~30 修改
	
;	task_add_option 1426 1 108
;	task_add_option 1427 1 109


	
;	TASK_STAT 1409 1 20
;	if 1 = 0 and 20 = 2
;		option 179 跟杰尼狄普对话
;	endif

	TASK_STAT 1424 1 20 
	if 1 = 0 and 20 = 2
		option 400 #获取戒指

	endif
	task_stat 1426 1 21
	if 21 = 2
		option 410 #跟杰尼狄普对话
	endif
	
	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300915 1 num1
	if num1 > 0
		TASK_STAT 3305 1 Ret
		if Ret = 2
			option 700 曲别针换别墅
		endif
	endif

	talk
endproc 
proc 410
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b奥…奥需瑞…你…你不是已经…
		add_talk #b#b<杰尼狄普看到你的出现，惊慌的后退了几退，随即又似乎想起了什么，眼中闪过一丝狠戾>
		
			option 411 #好久不见了！
		talk
endproc 
;proc 411
;	task_fini 1426 1
;endproc

proc 179
	
;		GET_PLAYER_ID 0 79
;		GET_ID_PLAYER_STATUS_INFO @79 1099 80 1 0 0 0
		DIALOG 1
		add_talk $_npc$:
;		if 80 = 0
		
			add_talk #b#b是你在原野猎人中引起的骚动？
			add_talk #b#b<杰尼狄普优雅的扶了扶礼帽，冷冷的说>
			add_talk #b#b候赛恩曾经是辛克雷的一员，因为某些愚蠢的理由他被叛了组织，我没去追究他的责任已经算是网开一面了，如果你没有其他事情，请不要耽误我的时间。

			option 180 #知道了
;		else
;			add_talk 你……是谁？
;		
;		endif
		
		talk

endproc 

;PROC 108
;	task_do_option 1426 1
;ENDPROC
;PROC 109
;	task_do_option 1427 1
;ENDPROC

;proc 180
;	task_fini 1409 1
;	return
;endproc


proc 400
	GET_PLAYER_ID 1 54
	GET_ID_PLAYER_STATUS_INFO @54 1099 60 3 0 0 0

	if 60 = 0

		TARGET_ADD_STATUS 1 @54 1099 2 0 53
		add_item_num_inpak 5300294 1 1 0

		GET_PLAYER_MAPINFO 1 2 3 56 57
		call_mon 1 204008 500 10 @56 @57 1

	else
	
		add_sys_msg_start
		add_sys_msg 哪来的狂徒，光天化日之下还想抢劫不成？
		set_sys_msg_mode 4 0 0
		msg_info
	
	endif

endproc

proc 2
	get_trig_task 1 2

;	if 2 = 1427
;
;		task_fini 1427 1
;		add_sys_msg_start
;		add_sys_msg 杰尼狄普，竟然敬酒不吃吃罚酒，看来有必要好好教训教训你。
;		set_sys_msg_mode 4 0 0
;		msg_info
;		CALL_MON 1 204090 500 30 3400 2231 1
;		del_npc 304008 1
		
;	endif 
;	if 2 = 1439
;		add_sys_msg_start
;		add_sys_msg 杰尼狄普突然向远方跑去，快追！别让他跑了！
;		set_sys_msg_mode 3 0 0
;		msg_info
;		
;		call_mon 1 304037 0 0 3300 2200 1 
;		CALL_MON_BY_PATROL 1 204091 0 60 3300 2200 72 0 30
;
;		del_npc 304008 1
;	endif
endproc



proc 700
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b世人都说黄金之城是传说，我却不以为然，所有的传说必然有蓝本参照，虽然你手上这本小说是杜撰的，但我想总归有点什么线索是真实存在的。
	add_talk #b#b怎么，你也想从书中找到黄金之城？哈，我用了大半辈子的时间都没找到，就凭你？
		option 701 #交换

	talk
	
endproc
proc 701
	
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300915 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300915 1 1
			
		;	rand Rad 100
		;	if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300930 1 0 0
		;	endif
		;	if @Rad >= 80 and @Rad < 95
		;		ADD_ITEM_NUM_INPAK 5300917 1 0 0
		;	endif
		;	if @Rad >= 95 and @Rad < 99
		;		ADD_ITEM_NUM_INPAK 5300918 1 0 0
		;	endif
		;	if @Rad = 99
		;		ADD_ITEM_NUM_INPAK 5300919 1 0 0
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