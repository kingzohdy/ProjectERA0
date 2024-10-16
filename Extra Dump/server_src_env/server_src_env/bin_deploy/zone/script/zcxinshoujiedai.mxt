

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b欢迎来到时间城，$_player$！
    add_talk #b#b为了感谢您对我们的帮助，我为您准备了大量的礼物，绝对会让您爱不释手！当然，我不会收您的钱，一分也不会收！您只需要用新手礼券来换就行了！
	add_talk #b#b如果您平时喜欢做任务的话，现在应该收集不少礼券了吧？
	
;	OPTION 199 #(买卖)新手礼券商店
	
;	OPTION 300 #超粒因子兑换微粒因子（1换3）
;	OPTION 301 #效粒因子兑换微粒因子（1换2）
;	OPTION 302 #太古灵源兑换虚空灵源（1换3）
;	OPTION 303 #远始灵源兑换虚空灵源（1换2）
	
	
;	option 300 有福利
	task_add_option 2027 1 101
	task_add_option 2028 1 102
	
	strvar RetTsk
	task_stat 3126 1 RetTsk
	if RetTsk = 2
		option 200 这是圣诞老公公给你的礼物
	endif
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;	
;	strvar month sun
;	GET_NOW_TIME month sun 0 0 0 0 0
;	if month = 5 and sun >= ?
	
;	OPTION 300 #领取10万经验
;		
;	endif
;	if month > 5 and sun >= 1
;	
;	endif
	
	talk
endproc 

;proc 300
;		ADD_EXP 0 100000 0
;endproc 



PROC 199
	OPEN_NPC_SHOP
ENDPROC

PROC 101
	task_do_option 2027 1
ENDPROC

PROC 102
	task_do_option 2028 1
ENDPROC

proc 200 
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b 耶！圣诞礼物又来了！谢谢你！$_player$！
		option 210 #交出圣诞礼物
	talk
endproc 
proc 210
	GET_ITEM_NUM_INPAK 5300844 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg 你身上没有这件物品！
		set_sys_msg_mode 1 0 0
		msg_info
	ENDIF
	if 11 >= 1
			
		dec_item_num_inpak 5300844 1 30
		if 30 != -1
			task_fini 3126 1
		endif
	endif
endproc 

proc 300
;;;;; 超粒因子兑换微粒因子
	strvar Num Num1 Ret Ret1 packNum zNum pNum BangNum
	var Num 0
	var Num1 0
	GET_ITEM_NUM_INPAK 5233512 1 Num
	GET_ITEM_NUM_INPAK 5233513 1 Num1

	var BangNum @Num
	add BangNum @Num1
	;身上有多少兑换物品总数
	
	var zNum @BangNum
	mul zNum 3
	;得出可兑换的超粒因子个数packNum	
	
	var packNum @zNum
	Dev packNum 99
	;需要多少背包格子
	
	GET_PACK_SPACE 0 pNum
	if BangNum != 0
		if pNum >= @packNum
			dec_item_num_inpak 5233512 @Num Ret
			dec_item_num_inpak 5233513 @Num1 Ret1
			if Ret = 0 or Ret1 = 0
				ADD_ITEM_NUM_INPAK 5233500 @zNum 0 1
			endif
		else
			add_sys_msg_start
			add_sys_msg 请预留$@packNum$个以上的背包空格再兑换物品！
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg 你携带的兑换物数量不足，无法兑换！
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc

proc 301
;;;;; 效粒因子兑换微粒因子
	strvar Num Num1 Ret Ret1 packNum zNum pNum BangNum
	var Num 0
	var Num1 0
	GET_ITEM_NUM_INPAK 5233501 1 Num
	GET_ITEM_NUM_INPAK 5233511 1 Num1
	
	var BangNum @Num
	add BangNum @Num1
	;身上有多少兑换物品总数
	
	var zNum @BangNum
	mul zNum 2
	;得出可兑换的超粒因子个数packNum	
	
	var packNum @zNum
	Dev packNum 99
	;需要多少背包格子
	
	GET_PACK_SPACE 0 pNum
	if BangNum != 0
		if pNum >= @packNum
			dec_item_num_inpak 5233501 @Num Ret
			dec_item_num_inpak 5233511 @Num1 Ret1
			if Ret = 0 or Ret1 = 0
				ADD_ITEM_NUM_INPAK 5233500 @zNum 0 1
			endif
		else
			add_sys_msg_start
			add_sys_msg 请预留$@packNum$个以上的背包空格再兑换物品！
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg 你携带的兑换物数量不足，无法兑换！
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc


proc 302
;;;;; 太古灵源换初生
	strvar Num Num1 Ret Ret1 packNum zNum pNum
	
	GET_ITEM_NUM_INPAK 5251117 1 Num

	;身上有多少兑换物品总数
	
	var zNum @Num
	mul zNum 3
	;得出可兑换的超粒因子个数packNum	
	
	var packNum @zNum
	Dev packNum 20
	;需要多少背包格子.
	
	GET_PACK_SPACE 0 pNum
	if Num != 0
		if pNum >= @packNum
			dec_item_num_inpak 5251117 @Num Ret
			if Ret = 0
				ADD_ITEM_NUM_INPAK 5251115 @zNum 0 1
			endif
		else
			add_sys_msg_start
			add_sys_msg 请预留$@packNum$个以上的背包空格再兑换物品！
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg 你携带的兑换物数量不足，无法兑换！
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc

proc 303
;;;;; 远始灵源
	strvar Num Num1 Ret Ret1 packNum zNum pNum
	GET_ITEM_NUM_INPAK 5251116 1 Num
	;身上有多少兑换物品总数
	
	var zNum @Num
	mul zNum 2

	;得出可兑换的超粒因子个数packNum	
	
	var packNum @zNum
	Dev packNum 20
	
	;需要多少背包格子.
	
	GET_PACK_SPACE 0 pNum
	if Num != 0
		if pNum >= @packNum
			dec_item_num_inpak 5251116 @Num Ret
			if Ret = 0
				ADD_ITEM_NUM_INPAK 5251115 @zNum 0 1
			endif
		else
			add_sys_msg_start
			add_sys_msg 请预留$@packNum$个以上的背包空格再兑换物品！
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg 你携带的兑换物数量不足，无法兑换！
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc



proc 2
	get_trig_task 1 2
	if 2 = 2028 
		task_fini 2028 1
	endif

endproc
