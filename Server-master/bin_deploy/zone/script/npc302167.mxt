;粽老npc

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b天载悠悠，成习俗，天中端午。
	add_talk #b#b逢佳节，粼粼波上，百舟竞渡。
	add_talk #b#b万户家中缠米粽……什么，你不会包粽子？让老头儿我来指点你一二。
	
;	TASK_ADD_OPTION 4486 0 103
	TASKCYCLE_ADD_OPTION 139 1 120

	option 130 包粽子的材料从哪里得？
	option 125 我们来包粽子吧！
	
	GET_PLAYER_DAILY_FLAG 0 15 10
	if 10 = 0
		option 134 领取粽叶
	endif
	
	talk
	
endproc 


proc 103
	TASK_DO_OPTION 4486 0
endproc

PROC 120	
	TASKCYCLE_DO_OPTION 139 1
ENDPROC

proc 125
	dialog 1
	add_talk $_npc$:
	add_talk #b#b#c186506“糯米”#n：困难与专家难度的主线副本（#c186506永夜镇、黄金矿井、森林结界、铁翼基地、太阳神殿#n）与活动副本（#c186506盘山道、龙蛋谷、筑梦空间#n）里的BOSS皆有几率掉落。
	add_talk #b##b#c186506“红豆”#n：在#L屈圆@@[302168,1000]#n的商店中就有出售，而#c186506“黯然消魂馅”#n则需要在#c186506商城#n中购买。
	add_talk #b#b“粽叶”：只需要隔段时间来我这领就好了。
	option 131 制作相思粽(１粽叶+１红豆)
	option 132 制作情义粽(１粽叶+１糯米+１鲜肉)
	option 133 制作吉祥粽(１粽叶+１黯然消魂馅)
	option 1 返回
	talk
endproc

PROC 130
	dialog 1
	add_talk $_npc$:
	add_talk #b#b#c186506相思粽#n：１粽叶+１红豆
	add_talk #b#b#c0181f9情义粽#n：１粽叶+１糯米+１鲜肉
	add_talk #b#b#c6d00d2吉祥粽#n：１粽叶+１黯然消魂馅
	add_talk #b#b
	add_talk #b#b你不知道到哪能弄到材料？认真听好哟：
	add_talk #b#b#c186506“糯米”#n：困难与专家难度的主线副本（#c186506永夜镇、黄金矿井、森林结界、铁翼基地、太阳神殿#n）与活动副本（#c186506盘山道、龙蛋谷、筑梦空间#n）里的BOSS皆有几率掉落。
	add_talk #b##b#c186506“红豆”#n：在#L屈圆@@[302168,1000]#n的商店中就有出售，而#c186506“黯然消魂馅”#n则需要在#c186506商城#n中购买。
	add_talk #b#b“粽叶”：只需要隔段时间来我这领就好了。
	
	option 160 #多谢指点，我现在就去找材料
	talk
ENDPROC


PROC 131
	dialog 1
	add_talk $_npc$:
	add_talk #b#b#c186506相思粽可以为你瞬间恢复生命；#c0181f9情义粽可以为你瞬间恢复魔法；#ccd00d2吉祥粽里藏着无尽的宝藏#n

    add_talk #b#b制作#c186506相思粽#n所需材料：#c186506１粽叶+１红豆#n

	GET_ITEM_NUM_INPAK 5060041 1 10
	GET_ITEM_NUM_INPAK 5060043 1 12
	
	add_talk #b#b当前拥有#cff4e00粽叶：$@10$个#n
	add_talk #b#b当前拥有#cff4e00红豆：$@12$个#n

	option 170 制作相思粽 1个
	option 172 制作相思粽 10个

	option 132 制作情义粽
	option 133 制作吉祥粽

	option 1 返回
		
	talk
ENDPROC

PROC 132
	dialog 1
	add_talk $_npc$:
	add_talk #b#b#c186506相思粽可以为你瞬间恢复生命；#c0181f9情义粽可以为你瞬间恢复魔法；#ccd00d2吉祥粽里藏着无尽的宝藏#n

    add_talk #b#b制作#c0181f9情义粽#n所需材料：#c0181f9１粽叶+１糯米+１鲜肉#n

	GET_ITEM_NUM_INPAK 5060041 1 10
	GET_ITEM_NUM_INPAK 5060042 1 11
	GET_ITEM_NUM_INPAK 5060044 1 12
	
	
	add_talk #b#b当前拥有#cff4e00粽叶：$@10$个#n
	add_talk #b#b当前拥有#cff4e00糯米：$@11$个#n
	add_talk #b#b当前拥有#cff4e00鲜肉：$@12$个#n

	option 180 制作情义粽 1个
	option 182 制作情义粽 10个

	option 131 制作相思粽
	option 133 制作吉祥粽

	option 1 返回
		
	talk
ENDPROC

PROC 133
	dialog 1
	add_talk $_npc$:
	add_talk #b#b#c186506相思粽可以为你瞬间恢复生命；#c0181f9情义粽可以为你瞬间恢复魔法；#ccd00d2吉祥粽里藏着无尽的宝藏#n

    add_talk #b#b制作#ccd00d2吉祥粽#n所需材料：#ccd00d2１粽叶+１黯然消魂馅#n

	GET_ITEM_NUM_INPAK 5060041 1 10
	GET_ITEM_NUM_INPAK 5060045 1 11
	
	add_talk #b#b当前拥有#cff4e00粽叶：$@10$个#n
	add_talk #b#b当前拥有#cff4e00黯然消魂馅：$@11$个#n

	option 190 制作吉祥粽 1个
	option 192 制作吉祥粽 10个

	option 131 制作相思粽
	option 132 制作情义粽

	option 1 返回
		
	talk
ENDPROC


proc 134	
	GET_DAYONLINE 1 10
	
	var 11 @10
	if 10 >= 3600
		dev 10 3600
		MOD 11 3600
		dev 11 60
	else
		var 10 0
		dev 11 60
	endif
	
	dialog 1
	add_talk $_npc$:
	add_talk #b#b每天早上6点重置在线时间，当天在线时间可累计。
	add_talk #b#b
	add_talk #b#b您今日在线时间为：$@10$时$@11$分
	add_talk #b#b
	add_talk #b#b累计#c186506在线1小时#n可领取粽叶：15个
	
    option 150 #领取
	option 1 #返回
	talk
endproc

;领取
proc 150	
	GET_DAYONLINE 1 10
	
	GET_PLAYER_DAILY_FLAG 0 15 20
	;0表示未设置
	if 20 = 1
		PLAYER_CLOSE_DIALOG1 0 0
		return
	endif

	if 10 < 3600
		dialog 1
		add_talk $_npc$:
		add_talk #b#b对不起，您今日在线时间不足１小时，不能领取奖励。
		option 134 返回
		talk
	else
		GET_PACK_SPACE 0 11
		if 11 < 1
			add_sys_msg_start
			add_sys_msg 你拿不下更多东西了，先去仓库管理员那儿清理一下背包吧！
			set_sys_msg_mode 3 0 0
			msg_info
		else
			strvar Ret 
			SET_PLAYER_DAILY_FLAG Ret 15 1
			if Ret = 0				
				ADD_ITEM_NUM_INPAK 5060041 15 1 14
			endif
		endif
		PLAYER_CLOSE_DIALOG1 0 0
	endif
endproc

;5012123  相思棕 1个
proc 170
	GET_ITEM_NUM_INPAK 5060041 1 10
	GET_ITEM_NUM_INPAK 5060043 1 30
	if @10 < 1
		add_sys_msg_start
		add_sys_msg 缺少粽叶！可前往粽老[时间城]领取，也可通过端午节任务获得！
		set_sys_msg_mode 4 0 0
		msg_info
		return 
	endif
	
	if @30 < 1
		add_sys_msg_start
		add_sys_msg 缺少红豆！可前往屈圆[时间城]的商店中购买！
		set_sys_msg_mode 4 0 0
		msg_info
		return 
    endif
	
	if @10 >= 1 and @30 >= 1
		GET_PACK_SPACE 0 11
		if 11 < 1
			add_sys_msg_start
			add_sys_msg 你拿不下更多东西了，先去仓库管理员那儿清理一下背包吧！
			set_sys_msg_mode 3 0 0
			msg_info
		else
			ADD_BIND_ITEM_NUM_INPAK 5012123 1 1 13
			DEC_ITEM_NUM_INPAK 5060041 1 0
			DEC_ITEM_NUM_INPAK 5060043 1 0
			add_sys_msg_start
			add_sys_msg 制作一个相思棕成功！
			set_sys_msg_mode 4 0 0
			msg_info
			call 131
		endif
	else
		call 131
		add_sys_msg_start
		add_sys_msg 材料不足！粽老的“包粽子的材料从哪里得？”选项可查看材料出处
		set_sys_msg_mode 4 0 0
		msg_info
	endif
	
endproc

;5012123  相思棕 10个
proc 172 
	GET_ITEM_NUM_INPAK 5060041 1 10
	GET_ITEM_NUM_INPAK 5060043 1 30
	if @10 < 1
		add_sys_msg_start
		add_sys_msg 缺少粽叶！可前往粽老[时间城]领取，也可通过端午节任务获得！
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	if @30 < 1
		add_sys_msg_start
		add_sys_msg 缺少红豆！可前往屈圆[时间城]的商店中购买！
		set_sys_msg_mode 4 0 0
		msg_info
		return
    endif
	
	if @10 >= 10 and @30 >= 10
	    GET_PACK_SPACE 0 11
		if 11 < 1
			add_sys_msg_start
			add_sys_msg 你拿不下更多东西了，先去仓库管理员那儿清理一下背包吧！
			set_sys_msg_mode 3 0 0
			msg_info
		else
			ADD_BIND_ITEM_NUM_INPAK 5012123 10 1 13
			DEC_ITEM_NUM_INPAK 5060041 10 0
			DEC_ITEM_NUM_INPAK 5060043 10 0
			add_sys_msg_start
			add_sys_msg 制作十个相思棕成功！
			set_sys_msg_mode 4 0 0
			msg_info
			call 131
		endif
	else
		call 131
		add_sys_msg_start
		add_sys_msg 材料不足！粽老的“包粽子的材料从哪里得？”选项可查看材料出处
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc


;5012124   情义粽 1个
proc 180
	GET_ITEM_NUM_INPAK 5060041 1 10
	GET_ITEM_NUM_INPAK 5060042 1 20
	GET_ITEM_NUM_INPAK 5060044 1 30
	if @10 < 1
		add_sys_msg_start
		add_sys_msg 缺少粽叶！可前往粽老[时间城]领取，也可通过端午节任务获得！
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	if @20 < 1 
		add_sys_msg_start
		add_sys_msg 缺少糯米！可前往困难、专家难度的主线副本，或经验副本获得！
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	if @30 < 1
		add_sys_msg_start
		add_sys_msg 缺少鲜肉！可前往屈圆[时间城]的商店中购买！
		set_sys_msg_mode 4 0 0
		msg_info
		return
    endif
	
	if @10 >= 1 and @20 >= 1 and @30 >= 1
		GET_PACK_SPACE 0 11
		if 11 < 1
			add_sys_msg_start
			add_sys_msg 你拿不下更多东西了，先去仓库管理员那儿清理一下背包吧！
			set_sys_msg_mode 3 0 0
			msg_info
		else
			ADD_BIND_ITEM_NUM_INPAK 5012124 1 1 13
			DEC_ITEM_NUM_INPAK 5060041 1 0
			DEC_ITEM_NUM_INPAK 5060042 1 0
			DEC_ITEM_NUM_INPAK 5060044 1 0
			add_sys_msg_start
			add_sys_msg 制作一个情义棕成功！
			set_sys_msg_mode 4 0 0
			msg_info
			call 132
		endif
	else
		call 132
		add_sys_msg_start
		add_sys_msg 材料不足！粽老的“包粽子的材料从哪里得？”选项可查看材料出处
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc

;5012124   情义粽 10个
proc 182
	GET_ITEM_NUM_INPAK 5060041 1 10
	GET_ITEM_NUM_INPAK 5060042 1 20
	GET_ITEM_NUM_INPAK 5060044 1 30
	if @10 < 1
		add_sys_msg_start
		add_sys_msg 缺少粽叶！可前往粽老[时间城]领取，也可通过端午节任务获得！
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	if @20 < 1 
		add_sys_msg_start
		add_sys_msg 缺少糯米！可前往困难、专家难度的主线副本，或经验副本获得！
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	if @30 < 1
		add_sys_msg_start
		add_sys_msg 缺少鲜肉！可前往屈圆[时间城]的商店中购买！
		set_sys_msg_mode 4 0 0
		msg_info
		return
    endif
	
	if @10 >= 10 and @20 >= 10 and @30 >= 10
		GET_PACK_SPACE 0 11
		if 11 < 1
			add_sys_msg_start
			add_sys_msg 你拿不下更多东西了，先去仓库管理员那儿清理一下背包吧！
			set_sys_msg_mode 3 0 0
			msg_info
		else
			ADD_BIND_ITEM_NUM_INPAK 5012124 10 1 13
			DEC_ITEM_NUM_INPAK 5060041 10 0
			DEC_ITEM_NUM_INPAK 5060042 10 0
			DEC_ITEM_NUM_INPAK 5060044 10 0
			add_sys_msg_start
			add_sys_msg 制作十个情义棕成功！
			set_sys_msg_mode 4 0 0
			msg_info
			call 132
		endif
	else
		call 132
		add_sys_msg_start
		add_sys_msg 材料不足！粽老的“包粽子的材料从哪里得？”选项可查看材料出处
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc


;5230043   吉祥粽 1个
proc 190 
	GET_ITEM_NUM_INPAK 5060041 1 10
	GET_ITEM_NUM_INPAK 5060045 1 20

	if @10 < 1
		add_sys_msg_start
		add_sys_msg 缺少粽叶！可前往粽老[时间城]领取，也可通过端午节任务获得！
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	if @20 < 1 
		add_sys_msg_start
		add_sys_msg 缺少黯然消魂馅！可在金币商城中购买！
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	strvar levelnum sex job
	GET_PLAYER_BASE_ATT 0 sex levelnum job
	if @10 >= 1 and @20 >= 1
		GET_PACK_SPACE 0 11
		if 11 < 1
			add_sys_msg_start
			add_sys_msg 你拿不下更多东西了，先去仓库管理员那儿清理一下背包吧！
			set_sys_msg_mode 3 0 0
			msg_info
		else
			if @levelnum >= 10 and @levelnum <= 59
				ADD_ITEM_NUM_INPAK 5230043 1 1 13
			elseif @levelnum >= 60 and @levelnum <= 69
				ADD_ITEM_NUM_INPAK 5230044 1 1 13
			elseif @levelnum >= 70 and @levelnum <= 120
				ADD_ITEM_NUM_INPAK 5230045 1 1 13
			else
				call 133
				add_sys_msg_start
				add_sys_msg 制作吉祥棕失败！
				set_sys_msg_mode 4 0 0
				msg_info
				return
			endif
			DEC_ITEM_NUM_INPAK 5060041 1 0
			DEC_ITEM_NUM_INPAK 5060045 1 0
			add_sys_msg_start
			add_sys_msg 制作一个吉祥棕成功！
			set_sys_msg_mode 4 0 0
			msg_info
			call 133
		endif
	else
		call 133
		add_sys_msg_start
		add_sys_msg 材料不足！粽老的“包粽子的材料从哪里得？”选项可查看材料出处
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc

;5230043   吉祥粽 10个
proc 192 
	GET_ITEM_NUM_INPAK 5060041 1 10
	GET_ITEM_NUM_INPAK 5060045 1 20

	if @10 < 1
		add_sys_msg_start
		add_sys_msg 缺少粽叶！可前往粽老[时间城]领取，也可通过端午节任务获得！
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	if @20 < 1 
		add_sys_msg_start
		add_sys_msg 缺少黯然消魂馅！可在金币商城中购买！
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	strvar levelnum sex job
	GET_PLAYER_BASE_ATT 0 sex levelnum job
	if @10 >= 10 and @20 >= 10
		GET_PACK_SPACE 0 11
		if 11 < 1
			add_sys_msg_start
			add_sys_msg 你拿不下更多东西了，先去仓库管理员那儿清理一下背包吧！
			set_sys_msg_mode 3 0 0
			msg_info
		else
			if @levelnum >= 10 and @levelnum <= 59
				ADD_ITEM_NUM_INPAK 5230043 10 1 13
			elseif @levelnum >= 60 and @levelnum <= 69
				ADD_ITEM_NUM_INPAK 5230044 10 1 13
			elseif @levelnum >= 70 and @levelnum <= 120
				ADD_ITEM_NUM_INPAK 5230045 10 1 13
			else
				call 133
				add_sys_msg_start
				add_sys_msg 制作吉祥棕失败！
				set_sys_msg_mode 4 0 0
				msg_info
				return
			endif
			DEC_ITEM_NUM_INPAK 5060041 10 0
			DEC_ITEM_NUM_INPAK 5060045 10 0
			add_sys_msg_start
			add_sys_msg 制作十个吉祥棕成功！
			set_sys_msg_mode 4 0 0
			msg_info
			call 133
		endif
	else
		call 133
		add_sys_msg_start
		add_sys_msg 材料不足！粽老的“包粽子的材料从哪里得？”选项可查看材料出处
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc

