;302033

	
proc 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b记得有次在狂杀角斗场被对手杠上了，为了捍卫尊严与荣耀！我选择信春哥！
	add_talk #b#b就这样经过无数次的推倒爬起后！终于把对手永远践踏在脚下！
	add_talk #b#b结果从此深深的爱上了那洒脱的快感..
	add_talk #b#b现在本店隆重推出【#cff4e00以旧换新#n】服务！没错！你没看错！就是人人都爱！既实惠又划算的【#cff4e00以旧换新#n】服务！！

	task_add_option 1601 1 101
	task_add_option 1602 1 102
	
	task_add_option 1701 1 103
	task_add_option 1702 1 104
	task_add_option 1700 1 105
	
	task_add_option 1749 1 106
	task_add_option 1600 1 107
		
	; OPTION 110 #(买卖)打开商店 
	task_stat 1749 1 20
	if 20 = 2
		option 150 还要考试呀？
	endif
	
	strvar LV
	GET_PLAYER_BASE_ATT 1 1 LV 1
	
	if @LV < 50
		option 401 #(买卖)【战场荣誉】商店
	endif
	if @LV >= 50 and @LV < 60
		option 402 #(买卖)【战场荣誉】商店
	endif
	if @LV >= 60 and @LV < 70
		option 403 #(买卖)【战场荣誉】商店
	endif
	if @LV >= 70 and @LV < 80
		option 404 #(买卖)【战场荣誉】商店
	endif
	if @LV >= 80
		option 405 #(买卖)【战场荣誉】商店
	endif
	
	option 240 #以旧换新-兑换面饰
	
	;option 200 以旧换新【#c6d00d2卓越的 午夜魅影#n】
	; option 200 以旧换新-卓越【午夜魅影】面饰
	; option 220 以旧换新-史诗【午夜魅影】面饰
	;option 210 以旧换新【#c6d00d2卓越的 歌剧面饰#n】
	; option 210 以旧换新-卓越【歌剧迷踪】面饰
	; option 230 以旧换新-卓越【歌剧迷踪】面饰
	
	talk
endproc

PROC 240
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b你想兑换哪个等级的面饰？
	
	option 241 #以旧换新-兑换【64级】面饰
	option 242 #以旧换新-兑换【74级】面饰
	talk
ENDPROC

PROC 241
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b你想兑换哪种【64级】的面饰？
	
	option 203 #以旧换新-卓越【午夜魅影】面饰
	option 223 #以旧换新-史诗【喙刃魅影】面饰
	talk
ENDPROC

PROC 242
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b你想兑换哪种【74级】的面饰？
	
	option 213 #以旧换新-卓越【歌剧迷踪】面饰
	option 233 #以旧换新-史诗【梦境迷踪】面饰
	talk
ENDPROC

PROC 101
	task_do_option 1601 1
ENDPROC
PROC 102
	task_do_option 1602 1
ENDPROC
PROC 103
	task_do_option 1701 1
ENDPROC
PROC 104
	task_do_option 1702 1
ENDPROC
PROC 105
	task_do_option 1700 1
ENDPROC
PROC 106
	task_do_option 1749 1
ENDPROC
PROC 107
	task_do_option 1600 1
ENDPROC

proc 110
	 OPEN_NPC_SHOP
endproc

proc 2
	get_trig_task 1 2
	if 2 = 1602
		task_fini 1602 1
	endif
	if 2 = 1702
		task_fini 1702 1
	endif
	
	if 2 = 1749
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b油画不装框，好比将军在澡堂,再强的勇士也离不开精良的装备！
			
			option 150 还要考试呀？
		talk
	endif
endproc
proc 150
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b为了证明你有认真在听我说话，我决定出点小题目考验考验你，如果你现在感觉还不太了解强化、转星的过程，就去克尔泽那儿研究研究再来吧！
		
		option 151 我准备好了，请提问吧！
				
	talk
endproc 
proc 151
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b强化装备后增加10%属性的紫色星星叫什么？

		option 152 A：北极星
		option 152 B：南极星
		option 153 C：完美星
		option 152 D：牛X星

				
	talk
endproc 
proc 152
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c186506装备强化每颗星会增加1%-10%的基础属性，当随机到最高值10%时，强化后的星星就会变成紫色，俗称完美星。#n
		
		option 151 重新回答
	
	talk
endproc 
proc 153
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b不用强化石就能给装备加星的操作是什么？

		option 161 A：做梦

		option 160 B：转星
		option 161 C：祈祷
		option 161 D：抢劫
				
	talk
endproc 
proc 161
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c186506当你拥有一件强化过的装备时，可以免费将它转移到另一件相同的部件上！这个过程叫做“转星”！#n
	add_talk #b#b#c186506比如说：你拥有一件+9的41级武器，你可以把他转移到没有加星的51级、61级、71级武器上。转移成功后，你的高级武器就会变成+9，而且不需要任何消耗！#n
	add_talk #b#b#c186506注：只有相同部件的装备，才可以进行转星哟！#n

		option 151 重新回答
	
	talk
endproc

proc 160
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b转星时，应该怎么放加过星的装备，以及高级装备（待转星装备）呢？
	
		option 170 A：左边放强化过的，右边放待转星的
		option 171 B：左边放待转星的，右边放强化过的
		option 171 C、左边放强化过的，右边放星辉宝钻
		option 171 D、左边放待转星的，右边放星辉宝钻
				
	talk
endproc 
proc 171
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c186506左边的装备会覆盖掉右边装备上的星级，所以，转星的时候一定要谨慎哪！#n

		option 151 重新回答
	
	talk
endproc 

proc 170
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b去哪获得可以转星的装备呢？
	
		option 181 A：怪物掉落的装备有直接加星的
		option 181 B：从其他玩家的摊位上购买
		option 181 C：自己用星辉宝钻强化
		option 180 #D：以上说法都对
				
	talk

endproc 
proc 181
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c186506其他2条也对哟，如果你有不绑定的完美星的装备，还可以拿去换银币哟，这可是一条赚钱的好途径呢！#n

		option 151 重新回答
	
	talk

endproc 

proc 180
	task_fini 1749 1
endproc 

proc 200
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b#b换购条款
;	add_talk ★超值换购★ 
	add_talk 可获得#c186506全新#n的#c6d00d2卓越的#n【午夜魅影】面饰！
	add_talk 新面饰将有几率获得更好的【#cff4e00附加属性#n】！
	; add_talk #b#b
	add_talk ★换购所需★ 
	strvar num
	GET_ITEM_NUM_INPAK 2455007 1 num
	if num = 0
		add_talk 面饰：卓越的【午夜魅影】 #cF80B0B 0 #n/1
	else
		add_talk 面饰：卓越的【午夜魅影】 #c186506 $@num$ #n/1
	endif
	GET_ID_PLAYER_INFO 0 23 50
	if 50 < 500
		add_talk 货币：【荣誉点数】#cF80B0B $@50$ #n/500
	else
		add_talk 货币：【荣誉点数】#c186506 $@50$ #n/500			
	endif
;	add_talk #b#b
	add_talk ★换购需知★ 
	add_talk 新面饰将全数保留装备的【#cff4e00强化星级#n】
	add_talk 新面饰将全数保留装备的【#cff4e00基础属性#n】
	add_talk 新面饰将全数保留装备的【#cff4e00洗炼属性#n】
	add_talk 新面饰将全数保留装备的【#cff4e00附魔属性#n】
	add_talk 新面饰将全数保留装备的【#cff4e00镶嵌宝石#n】
	add_talk #b#b#b#b#b#b#b#b#b#b#b#b#b#b——梅杰荣誉出品

		option 203 #同意换购
		; option 210 #确定兑换歌剧迷踪
	
	talk
endproc
proc 203

	dialog 3
	add_talk #b#b#b#b#b#b#b#b#b换购条款
	add_talk 可获得#c186506全新#n的#c6d00d2卓越的#n【午夜魅影】面饰！
	add_talk 新面饰将有几率获得更好的【#cff4e00附加属性#n】！
	add_talk ★换购所需★ 
	strvar num
	GET_ITEM_NUM_INPAK 2455007 1 num
	if num = 0
		add_talk 面饰：卓越的【午夜魅影】 #cF80B0B 0 #n/1
	else
		add_talk 面饰：卓越的【午夜魅影】 #c186506 $@num$ #n/1
	endif
	GET_ID_PLAYER_INFO 0 23 50
	if 50 < 500
		add_talk 货币：【荣誉点数】#cF80B0B $@50$ #n/500
	else
		add_talk 货币：【荣誉点数】#c186506 $@50$ #n/500			
	endif
;	add_talk #b#b
	add_talk ★换购需知★ 
	add_talk 新面饰将全数保留装备的【#cff4e00强化星级#n】
	add_talk 新面饰将全数保留装备的【#cff4e00基础属性#n】
	add_talk 新面饰将全数保留装备的【#cff4e00洗炼属性#n】
	add_talk 新面饰将全数保留装备的【#cff4e00附魔属性#n】
	add_talk 新面饰将全数保留装备的【#cff4e00镶嵌宝石#n】
	add_talk #b#b#b#b#b#b#b#b#b#b#b#b#b#b——梅杰荣誉出品	
	add_talk #b#c186506请放入需要换购的面饰：卓越的【午夜魅影】#n
                                                    
		NPC_COMMIT_OPT 204
	
	talk
endproc 

proc 204
	strvar m_ID m_Pos1 m_Pos2 m_Col Ret RetTask
	GET_COMMIT_ITEM_INFO 1 m_ID
	if m_ID = -1
		add_sys_msg_start
		add_sys_msg 请放入需要换购的卓越的【午夜魅影】面饰
		set_sys_msg_mode 1 0 0
		msg_info		
		return 
	endif
	if m_ID = 2455007
	
		strvar Moy
		DEC_MONEY_TEST 0 500 76 Moy
		if Moy = -1
			ADD_SYS_MSG_START
			ADD_SYS_MSG 你的荣誉点数不足，无法换购
			SET_SYS_MSG_MODE 1 0 0
			MSG_INFO
			return
		else
			DEC_MONEY 0 500 76 10
		;	DEC_ITEM_NUM_BYIDX 2455007 1 @m_Pos1 @m_Pos2 RetTask
			COMMIT_ARM_WASH_ADDATTR 10
			
			PLAYER_CLOSE_DIALOG 0 0
			ADD_SYS_MSG_START
			ADD_SYS_MSG 你成功换购了卓越的【午夜魅影】面饰！
			SET_SYS_MSG_MODE 1 0 0
			MSG_INFO
			call 203
			; add_sys_msg_start
			; add_sys_msg $@m_ID$ $@10$
			; set_sys_msg_mode 1 0 0
			; msg_info
		endif
		
	else
		add_sys_msg_start
		add_sys_msg 请放入需要换购的卓越的【午夜魅影】面饰
		set_sys_msg_mode 1 0 0
		msg_info
	endif
endproc

; proc 201 
	; choose 2 202 301 新面饰只重置旧面饰的附加属性，是否确认换购？
; endproc 
proc 301
endproc
; proc 202
	; PKG_IS_UNLOCK 20 30
	; if 20 = 0 and 30 = 0
		; return
	; endif
	; strvar num
	; GET_ITEM_NUM_INPAK 2455007 1 num
	; if num > 0
		; strvar Moy
		; DEC_MONEY_TEST 0 1000 76 Moy
		; if Moy = -1
			; ADD_SYS_MSG_START
			; ADD_SYS_MSG 您的荣誉点不足，无法兑换
			; SET_SYS_MSG_MODE 1 0 0
			; MSG_INFO
			; return
		; else
			; DEC_MONEY 0 1000 76 10
			
			; dec_item_num_inpak 2455007 1 0
			; ADD_BIND_ITEM_NUM_INPAK 2455007 1 1 0
		; endif
	; else
		; ADD_SYS_MSG_START
		; ADD_SYS_MSG 您的背包中没有午夜魅影，无法兑换
		; SET_SYS_MSG_MODE 1 0 0
		; MSG_INFO
	; endif
; endproc 



proc 210
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b#b换购条款
;	add_talk ★超值换购★ 
	add_talk 可获得#c186506全新#n的#c6d00d2卓越的#n【歌剧迷踪】面饰！
	add_talk 新面饰将有几率获得更好的【#cff4e00附加属性#n】！
	; add_talk #b#b
	add_talk ★换购所需★ 
	strvar num
	GET_ITEM_NUM_INPAK 2455008 1 num
	if num = 0
		add_talk 面饰：卓越的【歌剧迷踪】 #cF80B0B 0 #n/1
	else
		add_talk 面饰：卓越的【歌剧迷踪】 #c186506 $@num$ #n/1
	endif
	GET_ID_PLAYER_INFO 0 23 50
	if 50 < 2000
		add_talk 货币：【荣誉点数】#cF80B0B $@50$ #n/2000
	else
		add_talk 货币：【荣誉点数】#c186506 $@50$ #n/2000			
	endif
;	add_talk #b#b
	add_talk ★换购需知★ 
	add_talk 新面饰将全数保留装备的【#cff4e00强化星级#n】
	add_talk 新面饰将全数保留装备的【#cff4e00基础属性#n】
	add_talk 新面饰将全数保留装备的【#cff4e00洗炼属性#n】
	add_talk 新面饰将全数保留装备的【#cff4e00附魔属性#n】
	add_talk 新面饰将全数保留装备的【#cff4e00镶嵌宝石#n】
	add_talk #b#b#b#b#b#b#b#b#b#b#b#b#b#b——梅杰荣誉出品
	
	;	option 201 #确定兑换午夜魅影
		option 213 #同意换购
	
	talk
endproc
; proc 211 


	; choose 2 212 301 新面饰只重置旧面饰的附加属性，是否确认换购？
; endproc 

; proc 212

	; PKG_IS_UNLOCK 20 30
	; if 20 = 0 and 30 = 0
		; return
	; endif

	; strvar num
	; GET_ITEM_NUM_INPAK 2455008 1 num
	; if num > 0
		; strvar Moy
		; DEC_MONEY_TEST 0 3000 76 Moy
		; if Moy = -1
			; ADD_SYS_MSG_START
			; ADD_SYS_MSG 您的荣誉点不足，无法兑换
			; SET_SYS_MSG_MODE 1 0 0
			; MSG_INFO
			; return
		; else
			; DEC_MONEY 0 3000 76 10
			; if 10 = 0
				
				; dec_item_num_inpak 2455008 1 0
				; ADD_BIND_ITEM_NUM_INPAK 2455008 1 1 0
			; endif
		; endif
	; else
		; ADD_SYS_MSG_START
		; ADD_SYS_MSG 您的背包中没有歌剧迷踪，无法兑换
		; SET_SYS_MSG_MODE 1 0 0
		; MSG_INFO
	
	; endif
; endproc 

proc 213
	dialog 3
	add_talk #b#b#b#b#b#b#b#b#b换购条款
	add_talk 可获得#c186506全新#n的#c6d00d2卓越的#n【歌剧迷踪】面饰！
	add_talk 新面饰将有几率获得更好的【#cff4e00附加属性#n】！
	add_talk ★换购所需★ 
	strvar num
	GET_ITEM_NUM_INPAK 2455008 1 num
	if num = 0
		add_talk 面饰：卓越的【歌剧迷踪】 #cF80B0B 0 #n/1
	else
		add_talk 面饰：卓越的【歌剧迷踪】 #c186506 $@num$ #n/1
	endif
	GET_ID_PLAYER_INFO 0 23 50
	if 50 < 2000
		add_talk 货币：【荣誉点数】#cF80B0B $@50$ #n/2000
	else
		add_talk 货币：【荣誉点数】#c186506 $@50$ #n/2000			
	endif
;	add_talk #b#b
	add_talk ★换购需知★ 
	add_talk 新面饰将全数保留装备的【#cff4e00强化星级#n】
	add_talk 新面饰将全数保留装备的【#cff4e00基础属性#n】
	add_talk 新面饰将全数保留装备的【#cff4e00洗炼属性#n】
	add_talk 新面饰将全数保留装备的【#cff4e00附魔属性#n】
	add_talk 新面饰将全数保留装备的【#cff4e00镶嵌宝石#n】
	add_talk #b#b#b#b#b#b#b#b#b#b#b#b#b#b——梅杰荣誉出品
	add_talk #b#c186506请放入需要换购面饰：卓越的【歌剧迷踪】#n
                                                    
		NPC_COMMIT_OPT 214
	
	talk
endproc

proc 214
	strvar m_ID m_Pos1 m_Pos2 m_Col Ret RetTask
	GET_COMMIT_ITEM_INFO 1 m_ID
	if m_ID = -1
		add_sys_msg_start
		add_sys_msg 请放入需要换购的卓越的【歌剧迷踪】面饰
		set_sys_msg_mode 1 0 0
		msg_info		
		return 
	endif
	if m_ID = 2455008
	
		strvar Moy
		DEC_MONEY_TEST 0 2000 76 Moy
		if Moy = -1
			ADD_SYS_MSG_START
			ADD_SYS_MSG 你的荣誉点数不足，无法换购
			SET_SYS_MSG_MODE 1 0 0
			MSG_INFO
			return
		else
			DEC_MONEY 0 2000 76 10
		;	DEC_ITEM_NUM_BYIDX 2455007 1 @m_Pos1 @m_Pos2 RetTask
			COMMIT_ARM_WASH_ADDATTR 1
			PLAYER_CLOSE_DIALOG 0 0
			
			PLAYER_CLOSE_DIALOG 0 0
			ADD_SYS_MSG_START
			ADD_SYS_MSG 你成功换购了卓越的【歌剧迷踪】面饰！
			SET_SYS_MSG_MODE 1 0 0
			MSG_INFO
			call 213
		endif
	else
		add_sys_msg_start
		add_sys_msg 请放入需要换购的卓越的【歌剧迷踪】面饰
		set_sys_msg_mode 1 0 0
		msg_info
	endif
endproc

proc 401
	OPEN_SHOP_BY_ID 10020
	
	task_fini 2267 1
endproc
proc 402
	OPEN_SHOP_BY_ID 10021
	
	task_fini 2267 1
endproc
proc 403
	OPEN_SHOP_BY_ID 10022
	
	task_fini 2267 1
endproc
proc 404
	OPEN_SHOP_BY_ID 10023
	
	task_fini 2267 1
endproc
proc 405
	OPEN_SHOP_BY_ID 10024
	
	task_fini 2267 1
endproc

proc 220
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b#b换购条款 
	add_talk 可获得#c186506全新#n的#cff0080史诗的#n【喙刃魅影】面饰！
	add_talk 新面饰将有几率获得更好的【#cff4e00附加属性#n】！
	; add_talk #b#b
	add_talk ★换购所需★ 
	strvar num
	GET_ITEM_NUM_INPAK 2411603 1 num
	if num = 0
		add_talk 面饰：史诗的【喙刃魅影】 #cF80B0B 0 #n/1
	else
		add_talk 面饰：史诗的【喙刃魅影】 #c186506 $@num$ #n/1
	endif
	GET_ID_PLAYER_INFO 0 23 50
	if 50 < 500
		add_talk 货币：【荣誉点数】#cF80B0B $@50$ #n/500
	else
		add_talk 货币：【荣誉点数】#c186506 $@50$ #n/500			
	endif
	add_talk ★换购需知★ 
	add_talk 新面饰将全数保留装备的【#cff4e00强化星级#n】
	add_talk 新面饰将全数保留装备的【#cff4e00基础属性#n】
	add_talk 新面饰将全数保留装备的【#cff4e00洗炼属性#n】
	add_talk 新面饰将全数保留装备的【#cff4e00附魔属性#n】
	add_talk 新面饰将全数保留装备的【#cff4e00镶嵌宝石#n】
	add_talk #b#b#b#b#b#b#b#b#b#b#b#b#b#b——梅杰荣誉出品

		option 223 #同意换购
	
	talk
endproc
proc 223

	dialog 3
	add_talk #b#b#b#b#b#b#b#b#b换购条款 
	add_talk 可获得#c186506全新#n的#cff0080史诗的#n【喙刃魅影】面饰！
	add_talk 新面饰将有几率获得更好的【#cff4e00附加属性#n】！
	add_talk ★换购所需★ 
	strvar num
	GET_ITEM_NUM_INPAK 2411603 1 num
	if num = 0
		add_talk 面饰：史诗的【喙刃魅影】 #cF80B0B 0 #n/1
	else
		add_talk 面饰：史诗的【喙刃魅影】 #c186506 $@num$ #n/1
	endif
	GET_ID_PLAYER_INFO 0 23 50
	if 50 < 500
		add_talk 货币：【荣誉点数】#cF80B0B $@50$ #n/500
	else
		add_talk 货币：【荣誉点数】#c186506 $@50$ #n/500			
	endif
	add_talk ★换购需知★ 
	add_talk 新面饰将全数保留装备的【#cff4e00强化星级#n】
	add_talk 新面饰将全数保留装备的【#cff4e00基础属性#n】
	add_talk 新面饰将全数保留装备的【#cff4e00洗炼属性#n】
	add_talk 新面饰将全数保留装备的【#cff4e00附魔属性#n】
	add_talk 新面饰将全数保留装备的【#cff4e00镶嵌宝石#n】
	add_talk #b#b#b#b#b#b#b#b#b#b#b#b#b#b——梅杰荣誉出品
	add_talk #b#c186506请放入需要换购的面饰：史诗的【喙刃魅影】#n
                                                    
		NPC_COMMIT_OPT 224
	
	talk
endproc 

proc 224
	strvar m_ID m_Pos1 m_Pos2 m_Col Ret RetTask
	GET_COMMIT_ITEM_INFO 1 m_ID
	if m_ID = -1
		add_sys_msg_start
		add_sys_msg 请放入需要换购的史诗的【喙刃魅影】面饰！
		set_sys_msg_mode 1 0 0
		msg_info		
		return 
	endif
	if m_ID = 2411603
	
		strvar Moy
		DEC_MONEY_TEST 0 500 76 Moy
		if Moy = -1
			ADD_SYS_MSG_START
			ADD_SYS_MSG 你的荣誉点数不足，无法换购
			SET_SYS_MSG_MODE 1 0 0
			MSG_INFO
			return
		else
			DEC_MONEY 0 500 76 10
			COMMIT_ARM_WASH_ADDATTR 10
			PLAYER_CLOSE_DIALOG 0 0
			
			PLAYER_CLOSE_DIALOG 0 0
			ADD_SYS_MSG_START
			ADD_SYS_MSG 你成功换购了史诗的【喙刃魅影】面饰！
			SET_SYS_MSG_MODE 1 0 0
			MSG_INFO
			call 223
		endif
		
	else
		add_sys_msg_start
		add_sys_msg 请放入需要换购的史诗的【喙刃魅影】面饰
		set_sys_msg_mode 1 0 0
		msg_info
	endif
endproc

proc 230
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b#b换购条款
	add_talk 可获得#c186506全新#n的#cff0080史诗的#n【梦境迷踪】面饰！
	add_talk 新面饰将有几率获得更好的【#cff4e00附加属性#n】！
	; add_talk #b#b
	add_talk ★换购所需★ 
	strvar num
	GET_ITEM_NUM_INPAK 2411604 1 num
	if num = 0
		add_talk 面饰：史诗的【梦境迷踪】 #cF80B0B 0 #n/1
	else
		add_talk 面饰：史诗的【梦境迷踪】 #c186506 $@num$ #n/1
	endif
	GET_ID_PLAYER_INFO 0 23 50
	if 50 < 2000
		add_talk 货币：【荣誉点数】#cF80B0B $@50$ #n/2000
	else
		add_talk 货币：【荣誉点数】#c186506 $@50$ #n/2000			
	endif
	add_talk ★换购需知★ 
	add_talk 新面饰将全数保留装备的【#cff4e00强化星级#n】
	add_talk 新面饰将全数保留装备的【#cff4e00基础属性#n】
	add_talk 新面饰将全数保留装备的【#cff4e00洗炼属性#n】
	add_talk 新面饰将全数保留装备的【#cff4e00附魔属性#n】
	add_talk 新面饰将全数保留装备的【#cff4e00镶嵌宝石#n】
	add_talk #b#b#b#b#b#b#b#b#b#b#b#b#b#b——梅杰荣誉出品
	
		option 233 #同意换购
	
	talk
endproc

proc 233
	dialog 3
	add_talk #b#b#b#b#b#b#b#b#b换购条款
	add_talk 可获得#c186506全新#n的#cff0080史诗的#n【梦境迷踪】面饰！
	add_talk 新面饰将有几率获得更好的【#cff4e00附加属性#n】！
	add_talk ★换购所需★ 
	strvar num
	GET_ITEM_NUM_INPAK 2411604 1 num
	if num = 0
		add_talk 面饰：史诗的【梦境迷踪】 #cF80B0B 0 #n/1
	else
		add_talk 面饰：史诗的【梦境迷踪】 #c186506 $@num$ #n/1
	endif
	GET_ID_PLAYER_INFO 0 23 50
	if 50 < 2000
		add_talk 货币：【荣誉点数】#cF80B0B $@50$ #n/2000
	else
		add_talk 货币：【荣誉点数】#c186506 $@50$ #n/2000			
	endif
	add_talk ★换购需知★ 
	add_talk 新面饰将全数保留装备的【#cff4e00强化星级#n】
	add_talk 新面饰将全数保留装备的【#cff4e00基础属性#n】
	add_talk 新面饰将全数保留装备的【#cff4e00洗炼属性#n】
	add_talk 新面饰将全数保留装备的【#cff4e00附魔属性#n】
	add_talk 新面饰将全数保留装备的【#cff4e00镶嵌宝石#n】
	add_talk #b#b#b#b#b#b#b#b#b#b#b#b#b#b——梅杰荣誉出品
	add_talk #b#c186506请放入需要换购面饰：史诗的【梦境迷踪】#n
                                                    
		NPC_COMMIT_OPT 234
	
	talk
endproc

proc 234
	strvar m_ID m_Pos1 m_Pos2 m_Col Ret RetTask
	GET_COMMIT_ITEM_INFO 1 m_ID
	if m_ID = -1
		add_sys_msg_start
		add_sys_msg 请放入需要换购的史诗的【梦境迷踪】面饰
		set_sys_msg_mode 1 0 0
		msg_info		
		return 
	endif
	if m_ID = 2411604
	
		strvar Moy
		DEC_MONEY_TEST 0 2000 76 Moy
		if Moy = -1
			ADD_SYS_MSG_START
			ADD_SYS_MSG 你的荣誉点数不足，无法换购
			SET_SYS_MSG_MODE 1 0 0
			MSG_INFO
			return
		else
			DEC_MONEY 0 2000 76 10
			COMMIT_ARM_WASH_ADDATTR 1
			PLAYER_CLOSE_DIALOG 0 0
			
			PLAYER_CLOSE_DIALOG 0 0
			ADD_SYS_MSG_START
			ADD_SYS_MSG 你成功换购了史诗的【梦境迷踪】面饰！
			SET_SYS_MSG_MODE 1 0 0
			MSG_INFO
			call 233
		endif
	else
		add_sys_msg_start
		add_sys_msg 请放入需要换购的史诗的【梦境迷踪】面饰
		set_sys_msg_mode 1 0 0
		msg_info
	endif
endproc
