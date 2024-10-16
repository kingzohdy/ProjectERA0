

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b盘山道是给诸位勇士练习作战能力、协作能力的地方，真诚合作你会有更多收获！
;	add_talk #b#b
;	add_talk #c6d00d2关于“一条龙”的玩法：#n
;	add_talk #b#b#c6d00d2击杀#cff0080盘山道#n#c6d00d2最终BOSS会获得#cff0080同心协力#n#c6d00d2状态（效果：20分钟内#cff0080杀怪经验提升300％　#n#c6d00d2）并且可以和慧然果子、独悟果子的多倍经验共存，所以，最好的去处就是#cff0080经验多多的筑梦空间#n
	add_talk #b#b
	add_talk #c186506特别提示：
	add_talk #b#b#c186506当离开队伍后，同心协力状态会消失
	add_talk #b#b#c186506队伍人数越多，你所获得的经验越多
	add_talk #b#b#c186506#c186506队友等级越低，副本难度越低
	add_talk #b#b#c186506经验获得只与个人等级有关，跟队友等级无关


;;;;;;;;;;;;;;;;;;;;;;;;;;

	task_add_option 1111 1 114

	
	task_add_option 3321 1 116
	task_add_option 3322 1 117

	TASK_STAT 12020 1 20
	if 1 = 0 and 20 = 2
	  	option 150 (环次)药水送来了
	endif
	TASK_STAT 12220 1 20
	if 1 = 0 and 20 = 2
	  	option 160 (环次)给你带来了药水
	endif
;;;;;;;;;;;;;	

;	OPTION 210 #(买卖)勇气徽章商店
	OPTION 200 #进入盘山道副本
	OPTION 210 关于“一条龙”的玩法介绍
	
	talk
endproc 
proc 201 
	RESET_PWORLD 101
endproc
proc 210
	dialog 1
;	add_talk $_npc$:
	add_talk 关于“一条龙”的玩法：
	add_talk #b#b
	add_talk #b#b#c6d00d2一条龙是指：盘山道 + 筑梦空间#n
	add_talk #b#b
	add_talk #b#b盘山道：6人组队击杀酋长，会获得#cff4e00同心协力#n状态，此状态可以和慧然果子、独悟果子的多倍经验共存。

	add_talk #b#b
	add_talk #b#b#cff4e00同心协力效果：20分钟杀怪经验提升300％#n
	add_talk #b#b
	add_talk #b#b筑梦空间：怪物简单、集中的经验副本
	add_talk #b#b
	add_talk #b#b#c186506特别提醒：在获得同心协力状态后离开队伍，同心协力状态会消失#n
	
		option 1 返回
	
	talk
endproc

PROC 114
	task_do_option 1111 1
ENDPROC

PROC 116
	task_do_option 3321 1
ENDPROC
PROC 117
	task_do_option 3322 1
ENDPROC

PROC 150
    	DIALOG 1
   	ADD_TALK $_npc$:       
    	ADD_TALK  #b#b真的谢谢你的药剂，我想我应该可以很快恢复过来了。

	OPTION 250 #(环次)不用客气！
	TALK

ENDPROC
PROC 250
	GET_ITEM_NUM_INPAK 5007000 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg 你身上没有这件物品
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 5007000 1 0
	task_fini 12020 1
	endif

ENDPROC

PROC 160

    	DIALOG 1
   	ADD_TALK $_npc$:       
    	ADD_TALK  #b#b多谢你的来访，欢迎又一个加入时间城关怀的人。

	OPTION 260 #(环次)不用客气！
	TALK

ENDPROC
PROC 260
	GET_ITEM_NUM_INPAK 5007001 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg 你身上没有这件物品
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 5007001 1 0
	task_fini 12220 1
	endif
ENDPROC

proc 200

;	GET_EVENT_STAT 411 3	
;	GET_EVENT_STAT 412 5		
;	if @411 = 1 or @412 = 1
;		TEAM_PWORLD_INFO 300 101 0 0
;		if 300 = 0
;		ENTER_PWORLD 101 0 0
;		OPEN_PWORLD_UI 101
		OPEN_PWORLD_UI 12
;		else
;			is_captain 11 0
;			if 11 = 0
;				open_pworld_ui 101 
;			else
;				add_sys_msg_start
;				add_sys_msg 你不是队长！此副本需要队长操作进入！
;				set_sys_msg_mode 4 0 0
;				msg_info 
;			endif
;		endif
;
;	else
;		add_sys_msg_start
;		add_sys_msg 活动还没开启，无法进入！
;		set_sys_msg_mode 4 0 0
;		msg_info	
;	
;	endif
endproc














