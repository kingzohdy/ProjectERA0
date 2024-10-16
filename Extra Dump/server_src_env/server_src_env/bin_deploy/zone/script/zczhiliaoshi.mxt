proc 1
	strvar LVL Cost
	;GET_ID_PLAYER_INFO #角色实例ID（0表示自己） #AttrFlag  #返回值
	GET_ID_PLAYER_INFO 0 8 LVL
	var Cost @LVL
;	mul Cost 2
	
	strvar PetMaxHP PetHP
	GET_ID_PLAYER_INFO 0 12 PetHP
	GET_ID_PLAYER_INFO 0 13 PetMaxHP
	
	
	dialog 1
	add_talk $_npc$:
	add_talk #b#b您好，朋友。以神之名起誓，吾将为您敞开援助之手，以博爱之心感化世人，阿门。
	add_talk #c186506神圣祝福——恢复人物生命值和魔法值上限，与宠物的生命值上限#n
;	add_talk #c186506罪恶救赎——消除人物身上的红名状态#n
;	add_talk #c186506神恩眷佑——将人物经验从负值恢复成正值#n
	
	if LVL < 30
		OPTION 105 #神圣的祝福（30级以下免费治疗）
	else
		OPTION 98 #神圣的祝福（需消耗$@Cost$绑银）
	;OPTION 104 #治疗当前宠物（需消耗$@PetMaxHP$银币）
	endif
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		strvar Lv2 Mu2
		GET_ID_PLAYER_INFO 0 8 Lv2	
		var Mu2 @Lv2
		mul Mu2 @Lv2
		mul Mu2 2	
;		option 150 #罪恶救赎（需消耗$@Mu2$银币）
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		strvar Lvs Mu1
		GET_ID_PLAYER_INFO 0 8 Lvs	
		
		var Mu1 @Lvs
		mul Mu1 @Lvs
		dev Mu1 2	
;		option 151 #神恩眷佑（需消耗$@Mu1$银券）

	talk
endproc
proc 150
	strvar Mid Ret Lv2 Mu2 Moy
	GET_PLAYER_ID 1 Mid
	GET_ID_PLAYER_INFO 0 8 Lv2	
	var Mu2 @Lv2
	mul Mu2 @Lv2
	mul Mu2 2
	DEC_MONEY_TEST 0 @Mu2 20 Moy
	if Moy = -1	
		add_sys_msg_start
		add_sys_msg 您背包中的银币不足！
		set_sys_msg_mode 4 0 0
		msg_info
	else
		strvar Rete
		GET_PLAYER_PKVAL_TYPE @Mid Rete
		if Rete = 1
			add_sys_msg_start
			add_sys_msg 您暂时仍不需要这个帮助，愿神永伴您的左右。
			set_sys_msg_mode 4 0 0
			msg_info
		else
			add_exp @Mid -1 Ret
			DEC_MONEY @Mid @Mu2 20 1
		endif
	endif
endproc

proc 151

	strvar Mid Ret Lv Mu1 Moy
	GET_PLAYER_ID 1 Mid
	GET_ID_PLAYER_INFO 0 8 Lv	
	var Mu1 @Lv
	mul Mu1 @Lv
	dev Mu1 2
	DEC_MONEY_TEST 0 @Mu1 20 Moy
	if Moy = -1	
		add_sys_msg_start
		add_sys_msg 您背包中的银币不足！
		set_sys_msg_mode 4 0 0
		msg_info
	else
		add_exp @Mid 0 Ret
		if Ret = -1
			add_sys_msg_start
			add_sys_msg 您暂时仍不需要这个帮助，愿神永伴您的左右。
			set_sys_msg_mode 4 0 0
			msg_info
		else
			DEC_MONEY @Mid @Mu1 20 10	
		endif
	endif
endproc 


proc 98
	strvar Cost LVL
	GET_ID_PLAYER_INFO 0 8 LVL
	var Cost @LVL

	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < @Cost
		choose 1 101 299 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	
	elseif ret < @Cost
		choose 1 101 299 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 101
	endif
endproc 
proc 299
	return
endproc

proc 101
	strvar MaxHP HP MaxMP MP LVL Ret1 Cost
	strvar PetMaxHP PetHP
	GET_ID_PLAYER_INFO 0 2 HP
	GET_ID_PLAYER_INFO 0 9 MaxHP
	GET_ID_PLAYER_INFO 0 11 MP
	GET_ID_PLAYER_INFO 0 10 MaxMP
	
	GET_ID_PLAYER_INFO 0 12 PetHP
	GET_ID_PLAYER_INFO 0 13 PetMaxHP
	GET_ID_PLAYER_INFO 0 8 LVL
	var Cost @LVL
;	mul Cost 2

			
	if @HP < @MaxHP or @MP < @MaxMP or @PetHP < @PetMaxHP
	
			;DEC_MONEY 角色ID 减少的钱 钱类型 返回标记
			
		DEC_MONEY_TEST 0 @Cost 21 Ret1	
		
		if Ret1 = 0
			strvar ret
			DEC_MONEY 0 @Cost 21 ret
			ADD_PLAYER_HP 0 999999 0
			ADD_PLAYER_MP 0 999999 0
			ADD_FIGHT_PET_HP 0 999999 0
			
			add_sys_msg_start
			add_sys_msg 你感受到了圣洁的光芒，生命值和魔法值回满！
			set_sys_msg_mode 4 0 0
			msg_info
		else
			add_sys_msg_start
			add_sys_msg 您的绑银不足，无法治疗！
			set_sys_msg_mode 4 0 0
			msg_info	
		endif
	else

		add_sys_msg_start
		add_sys_msg 你的状态良好，不需要我的援助！
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc



proc 104
	strvar Ret8 PetMaxHP PetHP
	strvar MaxHP HP MaxMP MP LVL Ret1 Cost
	
	GET_ID_PLAYER_INFO 0 12 PetHP
	GET_ID_PLAYER_INFO 0 13 PetMaxHP
	if PetHP = -1 or @PetMaxHP = -1
		add_sys_msg_start
		add_sys_msg 你未有宠物出战，无法治疗！
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	;DEC_MONEY 角色ID 减少的钱 钱类型 返回标记
	DEC_MONEY 0 @LVL 21 Ret8
	
	if Ret8 = 0
		if PetHP < @PetMaxHP	
				if Ret8 = 0
					ADD_FIGHT_PET_HP 0 999999 0
					add_sys_msg_start
					add_sys_msg 宠物生命值已回满！
					set_sys_msg_mode 4 0 0
					msg_info
				else
					return
				endif
			else
					add_sys_msg_start
					add_sys_msg 你的宠物状态良好，不需要我的援助！
					set_sys_msg_mode 4 0 0
					msg_info
			endif		
	else
		add_sys_msg_start
		add_sys_msg 您的银币不足，无法治疗！
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc

proc 105
	strvar MaxHP HP MaxMP MP LVL Ret1 Cost
	strvar PetMaxHP PetHP
	GET_ID_PLAYER_INFO 0 2 HP
	GET_ID_PLAYER_INFO 0 9 MaxHP
	GET_ID_PLAYER_INFO 0 11 MP
	GET_ID_PLAYER_INFO 0 10 MaxMP
	
	GET_ID_PLAYER_INFO 0 12 PetHP
	GET_ID_PLAYER_INFO 0 13 PetMaxHP
	GET_ID_PLAYER_INFO 0 8 LVL


			
	if @HP < @MaxHP or @MP < @MaxMP or @PetHP < @PetMaxHP



		ADD_PLAYER_HP 0 999999 0
		ADD_PLAYER_MP 0 999999 0
		ADD_FIGHT_PET_HP 0 999999 0
		
				
		add_sys_msg_start
		add_sys_msg 你感受到了圣洁的光芒，生命值和魔法值回满！
		set_sys_msg_mode 4 0 0
		msg_info
	else
		add_sys_msg_start
		add_sys_msg 你的状态良好，不需要我的援助！
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc
