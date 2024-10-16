

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b我看到混沌的力量在你血液中沸腾！

	task_add_option 2017 1 103
	task_add_option 2018 1 104
;	task_add_option 4204 1 105
	task_add_option 4205 1 106

	task_add_option 4224 1 109
	task_add_option 4301 1 112
;50级
	task_add_option 2201 1 114
	task_add_option 4400 1 115

	task_add_option 2015 1 119
	task_add_option 2016 1 120

;20~30修改
	task_add_option 4102 1 123
	task_add_option 1028 1 125
	task_add_option 1029 1 126
	task_add_option 4103 1 127
	task_add_option 4121 1 128
	task_add_option 4104 1 130
;;;;;;
	task_add_option 1101 1 131
	task_add_option 4322 1 132
	task_add_option 4425 1 133
	
;;;转职
	task_add_option 3401 1 134
;;;;
	task_add_option 1077 1 135
	task_add_option 1081 1 136
;	task_add_option 3403 1 137
	
	
	
	option 12 #学习技能
;	option 13 #技能配件镶嵌
;	option 14 #技能配件摘取
	option 200 #——【转换职业，称霸四方】——
;	
;	strvar LV
;	GET_PLAYER_ID 1 50
;	ID_PLAYER_BASE_ATT @50 1 1 LV 1 
;	if @LV >= 20
;		option 300 #重置天赋
;	endif
	strvar Ret
	task_stat 1081 1 Ret	
	if Ret = 2
		
;		option 400 时间城太大了，能否送我一程？
	endif
	
	
	strvar rum
	GET_ITEM_NUM_INPAK 5300965 1 rum
	task_stat 3025 1 Ret
	if Ret = 2 and rum = 0
		option 500 #我是替孩子们来拿红包的

	endif
	
	task_stat 1077 1 20
	if 20 = 2
		GET_MAP_NPC_ID 306000 40 1
		mon_speech @40 2 0 1 听说你将被派往地面执行任务？虽然那里布满危险，也许你会随时丧命……但是要记住，那是我们的家乡！
		
		task_fini 1077 0
	endif
	talk
endproc 

proc 12
	open_ui 1	
endproc
proc 13
	open_ui 25
endproc
proc 14
	open_ui 26
endproc

proc 500
	task_stat 3025 1 Ret
	if Ret = 2
		strvar rum
		GET_ITEM_NUM_INPAK 5300965 1 rum
		if rum = 0
			ADD_ITEM_NUM_INPAK 5300965 1 1 20
		endif
	endif
endproc 





proc 300
	strvar geni
	RESET_ALL_GENIUS geni
	if @geni = 0
		GET_PLAYER_ID 0 30
		effect_id 0 @30 playMontion(%d,"9554_16")
		add_sys_msg_start
		add_sys_msg 成功重置天赋！
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc

PROC 103
	task_do_option 2017 1
ENDPROC
PROC 104
	task_do_option 2018 1
ENDPROC

PROC 106
	task_do_option 4205 1
ENDPROC
PROC 109
	task_do_option 4224 1
ENDPROC

PROC 112
	task_do_option 4301 1
ENDPROC

PROC 114
	task_do_option 2201 1
ENDPROC
PROC 115
	task_do_option 4400 1
ENDPROC

PROC 119
	task_do_option 2015 1
ENDPROC
PROC 120
	task_do_option 2016 1
ENDPROC
PROC 123
	task_do_option 4102 1
ENDPROC
PROC 125
	task_do_option 1028 1
ENDPROC
PROC 126
	task_do_option 1029 1
ENDPROC
PROC 127
	task_do_option 4103 1
ENDPROC
PROC 128
	task_do_option 4121 1
ENDPROC

PROC 130
	task_do_option 4104 1
ENDPROC
PROC 131
	task_do_option 1101 1
ENDPROC
PROC 132
	task_do_option 4322 1
ENDPROC
PROC 133
	task_do_option 4425 1
ENDPROC


PROC 134
	task_do_option 3401 1
ENDPROC
PROC 135
	task_do_option 1077 1
ENDPROC
PROC 136
	task_do_option 1081 1
ENDPROC
PROC 137
	task_do_option 3403 1
ENDPROC



proc 2
	get_trig_task 1 2
	if 2 = 1095
		task_fini 1095 1
	endif  

	if 2 = 1029
		task_fini 1029 1
	endif 

endproc

proc 400
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b当然可以，您可是个大人物呢。
	add_talk #b#b不过，我是直接将您送到目的地，中途不会停的哟！
	add_talk #b#b您确定要乘坐直达班车吗？
		option 401 #请直接将我送到目的地吧！
		option 402 #不用啦，我自己过去，还能逛逛时间城！
	talk
endproc
proc 401
	
;	NEW_OWNER_TMP_ITEM 1460070 20900 17100 0 0 0 310 1095 1 3600 1
	AUTO_MOVE 0 1000 27000 24900 1460070 0 3081 1 3600
endproc 
proc 402
	return
endproc 

proc 200
	DIALOG 1
		add_talk #b#b#b#b#b#b#b#b#c186506《转换准备》
		add_talk #b#b#b#b#b 确保身上无穿着任何装备#n
		add_talk #b#b#b#b#b#b#b#b#cF80B0B《转换注意》
		add_talk #b 转换后将保留天赋值，并自动替换职业天赋
		add_talk  转换后将清除职业技能，并自动习得新职业技能
		add_talk #b#b 转换后将清除【末世巅峰争霸赛】排名#n
		add_talk #b#b
		add_talk #b#b#b#b#b#b#b#b#c005ebe《转换建议》
		add_talk 适当准备好新职业的装备再转换职业更有利于上手#n
		
		strvar lv job
		GET_PLAYER_BASE_ATT 0 0 lv job
		
		if lv < 70
			add_talk #b#b
			add_talk #b#b#c186506——【角色达到70级可选择转换职业】——#n
		endif
		
		if lv >= 70
			if job != 17
				option 201 #查看【守护者】职业
			endif
			if job != 33
				option 202 #查看【征服者】职业
			endif
			if job != 20
				option 203 #查看【圣佑者】职业
			endif
			if job != 36
				option 204 #查看【驭雷师】职业
			endif
			if job != 19
				option 205 #查看【猎杀者】职业
			endif
			if job != 35
				option 206 #查看【毁灭者】职业
			endif
			if job != 18
				option 207 #查看【聚能者】职业
			endif
			if job != 34
				option 208 #查看【炼魂师】职业
			endif
		endif
		
	talk	
endproc

proc 201
	DIALOG 1
		add_talk $_npc$:
		add_talk #b#b他们是冒险时的领导者，战斗时的守护者。
		add_talk #b#b他们拥有最高的防御力，是最坚强的壁垒，可在短时间内大大提升自己的防御力，并利用范围技能与其超强的抵抗伤害能力保护他的队友。
		add_talk #c005ebe擅长武器：刀、盾
		add_talk 职业特点：高生命值、高防御力#n
		
		call 210
		
		strvar lv job
		GET_PLAYER_BASE_ATT 0 0 lv job
		
		if lv >= 70
			if job != 17
				option 211 #转换成【守护者】职业
			endif
		endif
		
		option 200 #返回
		
	talk	
endproc

proc 202
	DIALOG 1
		add_talk $_npc$:
		add_talk #b#b他们是狂暴的斗士，天生的杀戮者。
		add_talk #b#b他们对战斗有着异于常人的敏锐，通过各种牵制技能来左右敌人，然后以其惊人的潜能瞬间激发强大的伤害，给予敌人致命的伤害。
		add_talk #c005ebe擅长武器：主手剑、副手剑
		add_talk 职业特点：单体高伤害、牵制技能#n
		
		call 210
		
		strvar lv job
		GET_PLAYER_BASE_ATT 0 0 lv job
		
		if lv >= 70
			if job != 33
				option 212 #转换成【征服者】职业
			endif
		endif
		
		option 200 #返回
		
	talk	
endproc

proc 203
	DIALOG 1
		add_talk $_npc$:
		add_talk #b#b他们是团队的精神领袖，是天使的化身。
		add_talk #b#b他们拥有大量的恢复技能，以超强的治疗能力保护身边的队友，并且给予他们强大的祝福辅助，只要有他们在，队友们是战场上永远的胜利者。
		add_talk #c005ebe擅长武器：圣杖、圣器
		add_talk 职业特点：群体治疗技能、辅助技能#n
		
		call 210
		
		strvar lv job
		GET_PLAYER_BASE_ATT 0 0 lv job
		
		if lv >= 70
			if job != 20
				option 213 #转换成【圣佑者】职业
			endif
		endif
		
		option 200 #返回
		
	talk	
endproc

proc 204
	DIALOG 1
		add_talk $_npc$:
		add_talk #b#b他们是掌控雷电的大师，是严谨的仲裁者。
		add_talk #b#b他们拥有不俗的魔法伤害，给予敌人有效的打击。除此他们还是团队中必不可少的协助者，利用各种诅咒魔法有力地削弱折磨敌人。
		add_talk #c005ebe擅长武器：圣杖、圣器
		add_talk 职业特点：远程魔法伤害、诅咒技能#n
		
		call 210
		
		strvar lv job
		GET_PLAYER_BASE_ATT 0 0 lv job
		
		if lv >= 70
			if job != 36
				option 214 #转换成【驭雷师】职业
			endif
		endif
		
		option 200 #返回
		
	talk	
endproc

proc 205
	DIALOG 1
		add_talk $_npc$:
		add_talk #b#b他们是一切罪恶的仇敌，极具魅力的杀手。
		add_talk #b#b他们使用双手枪作为自己的武器， 通过埋设地雷，精确的射击牵制着敌人，扼杀在措手不及中，是队伍重要的游动部署。
		add_talk #c005ebe擅长武器：右手枪、左手枪
		add_talk 职业特点：埋设地雷、远程物理伤害#n
		
		call 210
		
		strvar lv job
		GET_PLAYER_BASE_ATT 0 0 lv job
		
		if lv >= 70
			if job != 19
				option 215 #转换成【猎杀者】职业
			endif
		endif
		
		option 200 #返回
		
	talk	
endproc

proc 206
	DIALOG 1
		add_talk $_npc$:
		add_talk #b#b他们是爆破的专家，战场的屠戮者。
		add_talk #b#b他们精通各种机枪，强大的火力支援成为了队友的依赖，超强的群体杀伤能力让人不容忽视，顷刻间摧毁一切是他们留给世人的印象。
		add_talk #c005ebe擅长武器：机枪、准镜
		add_talk 职业特点：远程范围物理伤害#n
		
		call 210
		
		strvar lv job
		GET_PLAYER_BASE_ATT 0 0 lv job
		
		if lv >= 70
			if job != 35
				option 216 #转换成【毁灭者】职业
			endif
		endif
		
		option 200 #返回
		
	talk	
endproc

proc 207
	DIALOG 1
		add_talk $_npc$:
		add_talk #b#b他们掌握非凡的范围魔法，是死亡的使者。
		add_talk #b#b身着华丽法袍的他们对战斗有着独特的理解，拥有着卓越的大范围攻击魔法，结合着减速效果使他们无所不能。
		add_talk #c005ebe擅长武器：魔杖、魔具
		add_talk 职业特点：远程范围魔法伤害、减速技能#n
		
		call 210
		
		strvar lv job
		GET_PLAYER_BASE_ATT 0 0 lv job
		
		if lv >= 70
			if job != 18
				option 217 #转换成【聚能者】职业
			endif
		endif
		
		option 200 #返回
		
	talk	
endproc

proc 208
	DIALOG 1
		add_talk $_npc$:
		add_talk #b#b毁灭就是他们的代名词。
		add_talk #b#b他们强大的破坏法术攻击是来自地狱的挽歌，是团队中必不可少的利刃，从天而降的地狱之火挥手间让敌人灰飞烟灭。
		add_talk #c005ebe擅长武器：魔杖、魔具
		add_talk 职业特点：远程单体魔法伤害#n
		
		call 210
		
		strvar lv job
		GET_PLAYER_BASE_ATT 0 0 lv job
		
		if lv >= 70
			if job != 34
				option 218 #转换成【炼魂师】职业
			endif
		endif
		
		option 200 #返回
		
	talk	
endproc

proc 211
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 0
	mul lv 100
	
	; choose 2 221 220 转换职业需要$@lv$银币和1本“转换之书”\n是否确认转换成【守护者】？
	choose 2 221 220 转换职业只需要支付$@lv$银币\n是否确认转换成【守护者】？
endproc
proc 212
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 0
	mul lv 100
	
	; choose 2 222 220 转换职业需要$@lv$银币和1本“转换之书”\n是否确认转换成【征服者】？
	choose 2 222 220 转换职业只需要支付$@lv$银币\n是否确认转换成【征服者】？
endproc
proc 213
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 0
	mul lv 100
	
	; choose 2 223 220 转换职业需要$@lv$银币和1本“转换之书”\n是否确认转换成【圣佑者】？
	choose 2 223 220 转换职业只需要支付$@lv$银币\n是否确认转换成【圣佑者】？
endproc
proc 214
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 0
	mul lv 100
	
	; choose 2 224 220 转换职业需要$@lv$银币和1本“转换之书”\n是否确认转换成【驭雷师】？
	choose 2 224 220 转换职业只需要支付$@lv$银币\n是否确认转换成【驭雷师】？
endproc
proc 215
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 0
	mul lv 100
	
	; choose 2 225 220 转换职业需要$@lv$银币和1本“转换之书”\n是否确认转换成【猎杀者】？
	choose 2 225 220 转换职业只需要支付$@lv$银币\n是否确认转换成【猎杀者】？
endproc
proc 216
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 0
	mul lv 100
	
	; choose 2 226 220 转换职业需要$@lv$银币和1本“转换之书”\n是否确认转换成【毁灭者】？
	choose 2 226 220 转换职业只需要支付$@lv$银币\n是否确认转换成【毁灭者】？
endproc
proc 217
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 0
	mul lv 100
	
	; choose 2 227 220 转换职业需要$@lv$银币和1本“转换之书”\n是否确认转换成【聚能者】？
	choose 2 227 220 转换职业只需要支付$@lv$银币\n是否确认转换成【聚能者】？
endproc
proc 218
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 0
	mul lv 100
	
	; choose 2 228 220 转换职业需要$@lv$银币和1本“转换之书”\n是否确认转换成【炼魂师】？
	choose 2 228 220 转换职业只需要支付$@lv$银币\n是否确认转换成【炼魂师】？
endproc

proc 221
	strvar num ret ret1 ret2 ret3 gezi
	GET_ITEM_NUM_INPAK 5030020 0 num
	GET_PACK_SPACE 0 gezi
	career_alter_check ret
	if ret = 0
		; if num >= 1
			if gezi > 0
				strvar lv job yinbi
				GET_PLAYER_BASE_ATT 0 0 lv job
				mul lv 100
				GET_ID_PLAYER_INFO 0 14  yinbi
				if yinbi >= @lv
					; DEC_ITEM_NUM_INPAK 5030020 1 ret1
					DEC_MONEY 0 @lv 20 ret2
					if ret2 = 0; and ret1 = 0
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5030070 1 1 ret3
						if ret3 = 0
							career_alter 0 17
						endif
					endif
				else
					add_sys_msg_start
					add_sys_msg 背包中的银币不够支付转换职业的费用，暂时无法转换职业
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			else
				add_sys_msg_start
				add_sys_msg 背包不够格子装放赠送的【技能书大全】，暂时无法转换职业
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		; else
			; add_sys_msg_start
			; add_sys_msg 背包中找不到转换职业需要的【转换之书】，暂时无法转换职业
			; set_sys_msg_mode 3 0 0
			; msg_info
		; endif
	endif	
endproc

proc 222
	strvar num ret ret1 ret2 ret3 gezi
	GET_ITEM_NUM_INPAK 5030020 0 num
	GET_PACK_SPACE 0 gezi
	career_alter_check ret
	if ret = 0
		; if num >= 1
			if gezi > 0
				strvar lv job yinbi
				GET_PLAYER_BASE_ATT 0 0 lv job
				mul lv 100
				GET_ID_PLAYER_INFO 0 14  yinbi
				if yinbi >= @lv
					; DEC_ITEM_NUM_INPAK 5030020 1 ret1
					DEC_MONEY 0 @lv 20 ret2
					if ret2 = 0; and ret1 = 0
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5030070 1 1 ret3
						if ret3 = 0
							career_alter 0 33
						endif
					endif
				else
					add_sys_msg_start
					add_sys_msg 背包中的银币不够支付转换职业的费用，暂时无法转换职业
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			else
				add_sys_msg_start
				add_sys_msg 背包不够格子装放赠送的【技能书大全】，暂时无法转换职业
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		; else
			; add_sys_msg_start
			; add_sys_msg 背包中找不到转换职业需要的【转换之书】，暂时无法转换职业
			; set_sys_msg_mode 3 0 0
			; msg_info
		; endif
	endif	
endproc

proc 223
	strvar num ret ret1 ret2 ret3 gezi
	GET_ITEM_NUM_INPAK 5030020 0 num
	GET_PACK_SPACE 0 gezi
	career_alter_check ret
	if ret = 0
		; if num >= 1
			if gezi > 0
				strvar lv job yinbi
				GET_PLAYER_BASE_ATT 0 0 lv job
				mul lv 100
				GET_ID_PLAYER_INFO 0 14  yinbi
				if yinbi >= @lv
					; DEC_ITEM_NUM_INPAK 5030020 1 ret1
					DEC_MONEY 0 @lv 20 ret2
					if ret2 = 0; and ret1 = 0
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5030070 1 1 ret3
						if ret3 = 0
							career_alter 0 20
						endif
					endif
				else
					add_sys_msg_start
					add_sys_msg 背包中的银币不够支付转换职业的费用，暂时无法转换职业
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			else
				add_sys_msg_start
				add_sys_msg 背包不够格子装放赠送的【技能书大全】，暂时无法转换职业
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		; else
			; add_sys_msg_start
			; add_sys_msg 背包中找不到转换职业需要的【转换之书】，暂时无法转换职业
			; set_sys_msg_mode 3 0 0
			; msg_info
		; endif
	endif	
endproc

proc 224
	strvar num ret ret1 ret2 ret3 gezi
	GET_ITEM_NUM_INPAK 5030020 0 num
	GET_PACK_SPACE 0 gezi
	career_alter_check ret
	if ret = 0
		; if num >= 1
			if gezi > 0
				strvar lv job yinbi
				GET_PLAYER_BASE_ATT 0 0 lv job
				mul lv 100
				GET_ID_PLAYER_INFO 0 14  yinbi
				if yinbi >= @lv
					; DEC_ITEM_NUM_INPAK 5030020 1 ret1
					DEC_MONEY 0 @lv 20 ret2
					if ret2 = 0; and ret1 = 0
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5030070 1 1 ret3
						if ret3 = 0
							career_alter 0 36
						endif
					endif
				else
					add_sys_msg_start
					add_sys_msg 背包中的银币不够支付转换职业的费用，暂时无法转换职业
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			else
				add_sys_msg_start
				add_sys_msg 背包不够格子装放赠送的【技能书大全】，暂时无法转换职业
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		; else
			; add_sys_msg_start
			; add_sys_msg 背包中找不到转换职业需要的【转换之书】，暂时无法转换职业
			; set_sys_msg_mode 3 0 0
			; msg_info
		; endif
	endif	
endproc

proc 225
	strvar num ret ret1 ret2 ret3 gezi
	GET_ITEM_NUM_INPAK 5030020 0 num
	GET_PACK_SPACE 0 gezi
	career_alter_check ret
	if ret = 0
		; if num >= 1
			if gezi > 0
				strvar lv job yinbi
				GET_PLAYER_BASE_ATT 0 0 lv job
				mul lv 100
				GET_ID_PLAYER_INFO 0 14  yinbi
				if yinbi >= @lv
					; DEC_ITEM_NUM_INPAK 5030020 1 ret1
					DEC_MONEY 0 @lv 20 ret2
					if ret2 = 0; and ret1 = 0
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5030070 1 1 ret3
						if ret3 = 0
							career_alter 0 19
						endif
					endif
				else
					add_sys_msg_start
					add_sys_msg 背包中的银币不够支付转换职业的费用，暂时无法转换职业
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			else
				add_sys_msg_start
				add_sys_msg 背包不够格子装放赠送的【技能书大全】，暂时无法转换职业
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		; else
			; add_sys_msg_start
			; add_sys_msg 背包中找不到转换职业需要的【转换之书】，暂时无法转换职业
			; set_sys_msg_mode 3 0 0
			; msg_info
		; endif
	endif	
endproc

proc 226
	strvar num ret ret1 ret2 ret3 gezi
	GET_ITEM_NUM_INPAK 5030020 0 num
	GET_PACK_SPACE 0 gezi
	career_alter_check ret
	if ret = 0
		; if num >= 1
			if gezi > 0
				strvar lv job yinbi
				GET_PLAYER_BASE_ATT 0 0 lv job
				mul lv 100
				GET_ID_PLAYER_INFO 0 14  yinbi
				if yinbi >= @lv
					; DEC_ITEM_NUM_INPAK 5030020 1 ret1
					DEC_MONEY 0 @lv 20 ret2
					if ret2 = 0; and ret1 = 0
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5030070 1 1 ret3
						if ret3 = 0
							career_alter 0 35
						endif
					endif
				else
					add_sys_msg_start
					add_sys_msg 背包中的银币不够支付转换职业的费用，暂时无法转换职业
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			else
				add_sys_msg_start
				add_sys_msg 背包不够格子装放赠送的【技能书大全】，暂时无法转换职业
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		; else
			; add_sys_msg_start
			; add_sys_msg 背包中找不到转换职业需要的【转换之书】，暂时无法转换职业
			; set_sys_msg_mode 3 0 0
			; msg_info
		; endif
	endif	
endproc

proc 227
	strvar num ret ret1 ret2 ret3 gezi
	GET_ITEM_NUM_INPAK 5030020 0 num
	GET_PACK_SPACE 0 gezi
	career_alter_check ret
	if ret = 0
		; if num >= 1
			if gezi > 0
				strvar lv job yinbi
				GET_PLAYER_BASE_ATT 0 0 lv job
				mul lv 100
				GET_ID_PLAYER_INFO 0 14  yinbi
				if yinbi >= @lv
					; DEC_ITEM_NUM_INPAK 5030020 1 ret1
					DEC_MONEY 0 @lv 20 ret2
					if ret2 = 0; and ret1 = 0
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5030070 1 1 ret3
						if ret3 = 0
							career_alter 0 18
						endif
					endif
				else
					add_sys_msg_start
					add_sys_msg 背包中的银币不够支付转换职业的费用，暂时无法转换职业
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			else
				add_sys_msg_start
				add_sys_msg 背包不够格子装放赠送的【技能书大全】，暂时无法转换职业
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		; else
			; add_sys_msg_start
			; add_sys_msg 背包中找不到转换职业需要的【转换之书】，暂时无法转换职业
			; set_sys_msg_mode 3 0 0
			; msg_info
		; endif
	endif	
endproc

proc 228
	strvar num ret ret1 ret2 ret3 gezi
	GET_ITEM_NUM_INPAK 5030020 0 num
	GET_PACK_SPACE 0 gezi
	career_alter_check ret
	if ret = 0
		; if num >= 1
			if gezi > 0
				strvar lv job yinbi
				GET_PLAYER_BASE_ATT 0 0 lv job
				mul lv 100
				GET_ID_PLAYER_INFO 0 14  yinbi
				if yinbi >= @lv
					; DEC_ITEM_NUM_INPAK 5030020 1 ret1
					DEC_MONEY 0 @lv 20 ret2
					if ret2 = 0; and ret1 = 0
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5030070 1 1 ret3
						if ret3 = 0
							career_alter 0 34
						endif
					endif
				else
					add_sys_msg_start
					add_sys_msg 背包中的银币不够支付转换职业的费用，暂时无法转换职业
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			else
				add_sys_msg_start
				add_sys_msg 背包不够格子装放赠送的【技能书大全】，暂时无法转换职业
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		; else
			; add_sys_msg_start
			; add_sys_msg 背包中找不到转换职业需要的【转换之书】，暂时无法转换职业
			; set_sys_msg_mode 3 0 0
			; msg_info
		; endif
	endif
endproc

proc 210
	strvar lv job yinbi
	GET_PLAYER_BASE_ATT 0 0 lv job
	
	add_talk #b#b
	if lv >= 70
		mul lv 100
		GET_ID_PLAYER_INFO 0 14  yinbi
		if yinbi >= @lv
			add_talk 转换费用：#c186506$@lv$#n银币（等级越高，费用越高）
		endif
		if yinbi < @lv
			add_talk 转换费用：#cF80B0B$@lv$#n银币（等级越高，费用越高）
		endif
		; add_talk 需要道具：转换职业需要提供【#c6d00d2转换之书#n】
	endif
	add_talk #b#b
	add_talk  #c005ebe——成功转换职业将赠送1本【技能书大全】——#n
	add_talk #b#b
	add_talk #b#b#b#b#c186506（每次转换职业需要间隔1小时）#n
endproc
