


proc 1
	dialog 1
	add_talk 能量灌注及激活说明： 
	add_talk #b#b每人每天可无限次给我灌注能量
	add_talk #b#b每次灌注能量都会获得好处
;	add_talk #b#b#c186506使用“纯净能量棒”能增加较多能量，获得较多奖励；使用“聚焦能量管”增加较少能量，获得较少奖励。#n
	
	add_talk #b#b只有会长有权限激活BOSS，BOSS存在3个小时
	add_talk #b#b只能周日06：00-24：00激活；若会长未能及时激活，则在24：00自动激活。
	add_talk #b#b
	is_in_my_clan_city 50
	if 50 = 1
		GET_CLAN_BABY_FEED 50
		if 50 >= 0 and 50 <= 100
			add_talk #b#b当前品质：普通
		endif
		if 50 > 100 and 50 <= 2000
			add_talk #b#b当前品质：#c186506良好#n
		endif
		if 50 > 2000 and 50 <= 5000
			add_talk #b#b当前品质：#c005ebe优秀#n
		endif
		if 50 > 5000
			add_talk #b#b当前品质：#c6d00d2杰出
		endif
		add_talk #b#b#c6d00d2每周一早上6：00重置BOSS品质
		;	option 150 使用“纯净能量棒”灌注能量
			option 160 使用“聚焦能量管”灌注能量
			
		strvar ret
		GET_CLAN_BUILDING_LV ret 0 9
		if ret >= 3
			option 200 #激活BOSS（只有会长有权限操作）
		endif
	endif
		
	talk
	
endproc 

proc 150
	; strvar ret
	; GET_CLAN_BUILDING_LV ret 0 9
	; if ret < 3
		
		; add_sys_msg_start
		; add_sys_msg 由于公会等级过低，未能达到参与BOSS养成计划的需求，请提升公会等级后再试！
		; set_sys_msg_mode 3 0 0
		; msg_info
		; return
	; endif

	; dialog 1
	; add_talk #b#b每天可以无限次的给我灌注能量，每次都可获得奖励！
	; add_talk #b#b
	; add_talk #b#b#c186506纯净能量棒出处：
	; add_talk #b#b
	
		; option 151 使用“纯净能量棒”：  1个
		; option 152 使用“纯净能量棒”：  5个
		; option 153 使用“纯净能量棒”：  10个
		; option 154 使用“纯净能量棒”：  50个
		; option 155 使用“纯净能量棒”：  100个
		; option 1 返回
	; talk
endproc

proc 159
	strvar ret
	task_stat 3018 1 ret
	if 1 = 0 and ret = 2
		task_fini 3018 1 
	endif
endproc 

proc 151
	strvar num
	GET_ITEM_NUM_INPAK 5060036 1 num
	if num >= 1
		DEC_ITEM_NUM_INPAK 5060036 1 30
		if 30 = 0
			CLAN_BABY_FEED 1 20
			if 20 = 0
				GET_ID_PLAYER_INFO 0 19 50
			;	mul 50 2
				dev 50 10000
				
				ADD_EXP 0 @50 1
			;	ADD_ID_PLAYER_CONTRI 0 0 10 0 0
				call 159
			endif
		endif
	else
		call 156
	endif
	
	
	; add_sys_msg_start
	; add_sys_msg $@num$
	; set_sys_msg_mode 1 0 0
	; msg_info
	
endproc
proc 152
	strvar num
	GET_ITEM_NUM_INPAK 5060036 1 num
	if num >= 5
		DEC_ITEM_NUM_INPAK 5060036 5 30
		if 30 = 0
			CLAN_BABY_FEED 5 20
			if 20 = 0
				GET_ID_PLAYER_INFO 0 19 50
				dev 50 10000
				mul 50 5
				ADD_EXP 0 @50 1
			;	ADD_ID_PLAYER_CONTRI 0 0 10 0 0
				call 159
			endif
		endif
	else
		call 156
	endif
endproc 
proc 153
	strvar num
	GET_ITEM_NUM_INPAK 5060036 1 num
	if num >= 10
		DEC_ITEM_NUM_INPAK 5060036 10 30
		if 30 = 0
			CLAN_BABY_FEED 10 20
			if 20 = 0
				GET_ID_PLAYER_INFO 0 19 50
				dev 50 10000
				mul 50 10
				ADD_EXP 0 @50 1
			;	ADD_ID_PLAYER_CONTRI 0 0 10 0 0
				call 159
			endif
		endif
	else
		call 156
	endif
endproc 
proc 154
	strvar num
	GET_ITEM_NUM_INPAK 5060036 1 num
	if num >= 50
		DEC_ITEM_NUM_INPAK 5060036 50 30
		if 30 = 0
			CLAN_BABY_FEED 50 20
			if 20 = 0
			
				GET_ID_PLAYER_INFO 0 19 50
				dev 50 10000
				mul 50 50
				ADD_EXP 0 @50 1
			;	ADD_ID_PLAYER_CONTRI 0 0 10 0 0
				call 159
			endif
		endif
	else
		call 156
	endif
endproc 
proc 155
	strvar num
	GET_ITEM_NUM_INPAK 5060036 1 num
	if num >= 100
		DEC_ITEM_NUM_INPAK 5060036 100 30
		if 30 = 0
			CLAN_BABY_FEED 100 20
			if 20 = 0
				GET_ID_PLAYER_INFO 0 19 50
				dev 50 10000
				mul 50 100
				ADD_EXP 0 @50 1
			;	ADD_ID_PLAYER_CONTRI 0 0 10 0 0
				call 159
			endif
		endif
	else
		call 156
	endif
endproc 
proc 156
	add_sys_msg_start
	add_sys_msg 你没有足够的纯净能量棒
	set_sys_msg_mode 3 0 0
	msg_info
endproc 


proc 160
	strvar ret
	GET_CLAN_BUILDING_LV ret 0 9
	if ret < 3
		
		add_sys_msg_start
		add_sys_msg 由于公会等级过低，未能达到参与BOSS养成计划的需求，请提升公会等级后再试！
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	
	
	GET_NOW_TIME 0 0 0 20 15 0 0
	if 50 = 1 and 20 = 1 and 15 < 6
		add_sys_msg_start
		add_sys_msg 每周日激活BOSS后至下周一早上6：00前不可为BOSS灌注能量
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	GET_CLAN_BABY_STAT 50
	if 50 != 0
	
		add_sys_msg_start
		add_sys_msg 每周日激活BOSS后至下周一早上6：00前不可为BOSS灌注能量
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	
	dialog 1
	add_talk #b#b每天可以无限次的给我灌注能量，每次都可获得奖励！
	add_talk #b#b
	add_talk #b#b#c186506聚焦能量管出处：完成#L宠趣任务@@[306042,4400]#n获得
	add_talk #b#b

	
		option 161 使用“聚焦能量管”：  1个
		option 162 使用“聚焦能量管”：  5个
		option 163 使用“聚焦能量管”：  10个
		option 164 使用“聚焦能量管”：  50个
		option 165 使用“聚焦能量管”：  100个
		option 1 返回
	talk
endproc

proc 161

	GET_NOW_TIME 0 0 0 20 15 0 0
	if 50 = 1 and 20 = 1 and 15 < 6
		add_sys_msg_start
		add_sys_msg 每周日激活BOSS后至下周一早上6：00前不可为BOSS灌注能量
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	GET_CLAN_BABY_STAT 50
	if 50 != 0
		return
	endif
	strvar num
	GET_ITEM_NUM_INPAK 5060037 1 num
	if num >= 1
		DEC_ITEM_NUM_INPAK 5060037 1 30
		if 30 = 0
			CLAN_BABY_FEED 1 20
			if 20 = 0
			
				GET_ID_PLAYER_INFO 0 19 50
				dev 50 10000
			;	mul 50 2
				
				ADD_EXP 0 @50 1
			;	ADD_ID_PLAYER_CONTRI 0 0 10 0 0
				call 159
			endif
		endif
	else
		call 166
	endif	
endproc
proc 162

	GET_NOW_TIME 0 0 0 20 15 0 0
	if 50 = 1 and 20 = 1 and 15 < 6
		add_sys_msg_start
		add_sys_msg 每周日激活BOSS后至下周一早上6：00前不可为BOSS灌注能量
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	GET_CLAN_BABY_STAT 50
	if 50 != 0
	
		add_sys_msg_start
		add_sys_msg 每周日激活BOSS后至下周一早上6：00前不可为BOSS灌注能量
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	strvar num
	GET_ITEM_NUM_INPAK 5060037 1 num
	if num >= 5
		DEC_ITEM_NUM_INPAK 5060037 5 30
		if 30 = 0
			CLAN_BABY_FEED 5 20
			if 20 = 0
				GET_ID_PLAYER_INFO 0 19 50
				dev 50 10000
			;	mul 50 2
				mul 50 5
				ADD_EXP 0 @50 1
			;	ADD_ID_PLAYER_CONTRI 0 0 10 0 0
				call 159
			endif
		endif
	else
		call 166
	endif	
endproc
proc 163

	GET_NOW_TIME 0 0 0 20 15 0 0
	if 50 = 1 and 20 = 1 and 15 < 6
		add_sys_msg_start
		add_sys_msg 每周日激活BOSS后至下周一早上6：00前不可为BOSS灌注能量
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	GET_CLAN_BABY_STAT 50
	if 50 != 0
	
		add_sys_msg_start
		add_sys_msg 每周日激活BOSS后至下周一早上6：00前不可为BOSS灌注能量
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	strvar num
	GET_ITEM_NUM_INPAK 5060037 1 num
	if num >= 10
		DEC_ITEM_NUM_INPAK 5060037 10 30
		if 30 = 0
			CLAN_BABY_FEED 10 20
			if 20 = 0
				GET_ID_PLAYER_INFO 0 19 50
				dev 50 10000
			;	mul 50 2
				mul 50 10
				ADD_EXP 0 @50 1
			;	ADD_ID_PLAYER_CONTRI 0 0 10 0 0
				call 159
			endif
		endif
	else
		call 166
	endif	
endproc
proc 164

	GET_NOW_TIME 0 0 0 20 15 0 0
	if 50 = 1 and 20 = 1 and 15 < 6
		add_sys_msg_start
		add_sys_msg 每周日激活BOSS后至下周一早上6：00前不可为BOSS灌注能量
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	GET_CLAN_BABY_STAT 50
	if 50 != 0
	
		add_sys_msg_start
		add_sys_msg 每周日激活BOSS后至下周一早上6：00前不可为BOSS灌注能量
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	strvar num
	GET_ITEM_NUM_INPAK 5060037 1 num
	if num >= 50
		DEC_ITEM_NUM_INPAK 5060037 50 30
		if 30 = 0
			CLAN_BABY_FEED 50 20
			if 20 = 0
				GET_ID_PLAYER_INFO 0 19 50
				dev 50 10000
			;	mul 50 2
				mul 50 50
				ADD_EXP 0 @50 1
			;	ADD_ID_PLAYER_CONTRI 0 0 10 0 0
				call 159
			endif
		endif
	else
		call 166
	endif	
endproc
proc 165

	GET_NOW_TIME 0 0 0 20 15 0 0
	if 50 = 1 and 20 = 1 and 15 < 6
		add_sys_msg_start
		add_sys_msg 每周日激活BOSS后至下周一早上6：00前不可为BOSS灌注能量
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	GET_CLAN_BABY_STAT 50
	if 50 != 0
	
		add_sys_msg_start
		add_sys_msg 每周日激活BOSS后至下周一早上6：00前不可为BOSS灌注能量
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	strvar num
	GET_ITEM_NUM_INPAK 5060037 1 num
	if num >= 100
		DEC_ITEM_NUM_INPAK 5060037 100 30
		if 30 = 0
			CLAN_BABY_FEED 100 20
			if 20 = 0
				GET_ID_PLAYER_INFO 0 19 50
				dev 50 10000
			;	mul 50 2
				mul 50 100
				ADD_EXP 0 @50 1
			;	ADD_ID_PLAYER_CONTRI 0 0 10 0 0
				call 159
			endif
		endif
	else
		call 166
	endif	
endproc

proc 166
	add_sys_msg_start
	add_sys_msg 你没有足够的聚焦能量管
	set_sys_msg_mode 3 0 0
	msg_info
endproc 

proc 200
	
	GET_CLAN_BABY_STAT 20
	if 20 = 0
		clan_baby_call 10
		if 10 = 0
			EFFECT 4 1 SetMessageTips(415)
			
			GET_ID_PLAYER_INFO 0 18 50
			
			add_sys_msg_start
			add_sys_msg 英明神武的会长大人已亲自前往公会城市激活BOSS-冲击波，请大家尽快前往，击杀后会获得非常丰厚的奖励哟！
			set_sys_msg_mode 14 0 0
			CLAN_MSG_INFO 1 @50

		endif
		; GET_CUR_MAX_LEVEL 50 60
		; if 50 = 0 and 60 < 60
			; GET_CLAN_BABY_FEED 40
			; if 40 >= 0 and 40 <= 100
				; new_life_mon 208100 1 29600 10100 1 1 1 1 10800 1 1 
			; endif
			; if 40 > 100 and 40 <= 2000
				; new_life_mon 208101 1 29600 10100 1 1 1 1 10800 1 1 
			; endif
			; if 40 > 2000 and 40 <= 5000
				; new_life_mon 208102 1 29600 10100 1 1 1 1 10800 1 1 
			; endif
			; if 40 > 5000
				; new_life_mon 208103 1 29600 10100 1 1 1 1 10800 1 1 
			; endif
		; endif
		; if 50 = 0 and 60 >= 60 and 60 < 70 
			; GET_CLAN_BABY_FEED 40
			; if 40 >= 0 and 40 <= 100
				; new_life_mon 208104 1 29600 10100 1 1 1 1 10800 1 1 
			; endif
			; if 40 > 100 and 40 <= 2000
				; new_life_mon 208105 1 29600 10100 1 1 1 1 10800 1 1 
			; endif
			; if 40 > 2000 and 40 <= 5000
				; new_life_mon 208106 1 29600 10100 1 1 1 1 10800 1 1 
			; endif
			; if 40 > 5000
				; new_life_mon 208107 1 29600 10100 1 1 1 1 10800 1 1 
			; endif
		; endif
		; if 50 = 0 and 60 >= 70 and 60 < 80 
			; GET_CLAN_BABY_FEED 40
			; if 40 >= 0 and 40 <= 100
				; new_life_mon 208108 1 29600 10100 1 1 1 1 10800 1 1 
			; endif
			; if 40 > 100 and 40 <= 2000
				; new_life_mon 208109 1 29600 10100 1 1 1 1 10800 1 1 
			; endif
			; if 40 > 2000 and 40 <= 5000
				; new_life_mon 208110 1 29600 10100 1 1 1 1 10800 1 1 
			; endif
			; if 40 > 5000
				; new_life_mon 208111 1 29600 10100 1 1 1 1 10800 1 1 
			; endif
		; endif
		; if 50 = 0 and 60 >= 80
			; GET_CLAN_BABY_FEED 40
			; if 40 >= 0 and 40 <= 100
				; new_life_mon 208112 1 29600 10100 1 1 1 1 10800 1 1 
			; endif
			; if 40 > 100 and 40 <= 2000
				; new_life_mon 208113 1 29600 10100 1 1 1 1 10800 1 1 
			; endif
			; if 40 > 2000 and 40 <= 5000
				; new_life_mon 208114 1 29600 10100 1 1 1 1 10800 1 1 
			; endif
			; if 40 > 5000
				; new_life_mon 208115 1 29600 10100 1 1 1 1 10800 1 1 
			; endif
		; endif
	else
		
		add_sys_msg_start
		add_sys_msg 无效请求，请前往史明威处了解活动规则！
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc 










