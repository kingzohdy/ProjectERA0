;神父
 
proc 1	
	dialog 1
	
	get_wedding_sth 4 60 61 ;是否可以领取“永恒钻戒”
	get_wedding_sth 2 40 41 ;是否领取了“永恒钻戒”
	get_wedding_sth 5 70 71 ;是否可以领取“浪漫烟花”
	get_wedding_sth 3 50 51 ;是否领取了“浪漫烟花”
	get_wedding_sth 6 80 81 ;是否建立了夫妻关系
	get_wedding_sth 8 90 91 ;是否可以举行“婚礼庆典”
	get_wedding_sth 9 30 31 ;是否可以举行“婚礼蜜游”
	get_wedding_sth 7 100 101 ;是否建立了情侣关系
	get_wedding_sth 10 140 141 ;双方是否领取了“永恒钻戒”
	get_wedding_sth 1 110 111
	
	add_talk $_npc$:
	add_talk #b#b爱是包容，爱是理解。
	add_talk #b#b#cFF0080以上帝之名，我愿在此引导每一对新人步入婚姻的殿堂，见证着他们的幸福与永恒。#n
	add_talk #b#b愿天下有情人终成眷属，阿门。
	
	if @61 = 1 and @60 = 1 and @41 = 1 and @40 = 0 and 141 = 1 and 140 = 0
		add_talk #b#b
		add_talk #b#b#c186506恭喜新郎新娘正式结为夫妻，现在有请新郎新娘到我的跟前领取见证你们爱情的“永恒钻戒”#n
	endif
	if @71 = 1 and @70 = 1 and @51 = 1 and @50 = 0
		add_talk #b#b
		add_talk #b#b#c186506婚礼庆典正在进行中，新郎新娘别忘了在我这领取“浪漫烟花”#n
	endif
	
	if @41 = 1 and @40 = 1
		if 141 = 1 and 140 = 0 
			;对方还未领取永恒婚戒
			add_talk #b#b
			add_talk #b#b#c186506新郎新娘都领取“永恒钻戒”后，即可继续举行婚礼仪式#n
		endif
	endif
	
	if @41 = 1 and @40 = 1
		if 141 = 1 and 140 = 1
			if ( @91 = 1 and @90 = 0 ) or ( @30 = 0 and @31 = 1 )
				add_talk #b#b
				add_talk #b#b#c186506现在有请新郎或新娘开启婚礼仪式:#n
				if @91 = 1 and @90 = 0
					add_talk #b#b#c186506仪式一：【婚礼庆典】，与来宾一同欢庆这永恒的时刻#n#c005ebe（订制了“简约或豪华婚礼”可举行）#n
				endif
				if @31 = 1 and @30 = 0
					add_talk #b#b#c186506仪式二：【婚礼蜜游】，把幸福的祝愿传达给每位世人#n#c005ebe（仅限订制了“豪华婚礼”可举行）#n
				endif
			endif
		endif
	endif
	
	if @61 = 1 and @60 = 1
		;结婚成功，可以开始领取永恒婚戒了
		if @41 = 1 and @40 = 0
			;还没有领取
			option 240 #请授予我象征爱和承诺的【永恒钻戒】
		endif
	endif
	
	if @71 = 1 and @70 = 1
		;开始婚礼庆典了，可以领取浪漫烟花了
		if @51 = 1 and @50 = 0
			;还没有领取过
			option 250 #领取【浪漫烟花】，点燃爱的花火
		endif
	endif

	;非夫妻关系，可以向神父申请婚礼
	if @81 = 1 and @80 = 0
		option 200 #订制【婚礼套餐】
		option 211 #请神父为我们主持婚礼
	endif
	
	;是夫妻关系，可以找神父举行活动
	if @81 = 1 and @80 = 1 and @41 = 1 and @40 = 1
		if 141 = 1 and 140 = 1
			if @91 = 1 and @90 = 0
				option 220 #举行【婚礼庆典】
			endif
			if @31 = 1 and @30 = 0
				option 230 #举行【婚礼蜜游】
			endif
		endif
	endif

	if (@81 = 1 and @80 = 1) or (@101 = 1 and @100 = 1)
		option 300 #我想要解除伴侣关系
	endif
	
	talk
endproc

proc 200
	dialog 1
	add_talk $_npc$:
	add_talk #b#b#cFF0080爱情本无贵贱，唯有真心无价。
	add_talk #b#b若要海誓山盟，勿忘守候今生。#n
	add_talk #b#b
	add_talk #b#b请许我以上帝之名传达，#c186506每对新人只许同时订制1份套餐#n，一但选择不得攀比怨尤不得懊悔退换。
	
	option 201 #【裸婚套餐】介绍
	option 202 #【简约套餐】介绍
	option 203 #【豪华套餐】介绍
	
	talk
endproc

proc 201
	dialog 1
	add_talk #b#b#b#b#b#b#b《#c186506裸婚套餐#n》清单
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b#c186506仪式一：婚礼宣誓#n
	add_talk #b#b#b 新人宣誓婚约诺言后即可结为夫妻
	add_talk #b#b#b#b#b#b#b#c186506仪式二：授予戒指#n
	add_talk  结为夫妻后新人可向“神父”领取“#c6d00d2永恒钻戒#n”
	
	option 204 #订制【裸婚套餐】（费用：90金币）
	option 400 #【永恒钻戒】介绍
	option 200 #我想再了解下其它婚礼套餐
	talk
endproc

proc 400
	dialog 1
	add_talk $_npc$:
	call 411
	option 201 #我想再了解下《裸婚套餐》
	talk
endproc

proc 202
	dialog 1
	add_talk #b#b#b#b#b#b#b《#c005ebe简约套餐#n》清单
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b#c186506仪式一：婚礼宣誓#n
	add_talk #b#b#b 新人宣誓婚约诺言后即可结为夫妻
	add_talk #b#b#b#b#b#b#b#c186506仪式二：授予戒指#n
	add_talk  结为夫妻后新人可向“神父”领取“#c6d00d2永恒钻戒#n”
	add_talk #b#b#b#b#b#b#b#c186506仪式三：婚礼庆典#n
	add_talk  举行婚礼庆典后可向“神父”领取“#c005ebe浪漫烟花#n”

	option 205 #订制【简约套餐】（费用：1314金币）
	option 402 #【永恒钻戒】介绍
	option 403 #【浪漫烟花】介绍
	option 200 #我想再了解下其它婚礼套餐
	talk
endproc

proc 402
	dialog 1
	add_talk $_npc$:
	call 411
	option 202 #我想再了解下《简约套餐》
	talk
endproc

proc 403
	dialog 1
	add_talk $_npc$:
	call 412
	option 202 #我想再了解下《简约套餐》
	talk
endproc

proc 203
	dialog 1
	add_talk #b#b#b#b#b#b#b《#c6d00d2豪华套餐#n》清单
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b#c186506仪式一：婚礼宣誓#n
	add_talk #b#b#b 新人宣誓婚约诺言后即可结为夫妻
	add_talk #b#b#b#b#b#b#b#c186506仪式二：授予戒指#n
	add_talk  结为夫妻后新人可向“神父”领取“#c6d00d2永恒钻戒#n”
	add_talk #b#b#b#b#b#b#b#c186506仪式三：婚礼庆典#n
	add_talk  举行婚礼庆典后可向“神父”领取“#c005ebe浪漫烟花#n”
	add_talk #b#b#b#b#b#b#b#c186506项目四：婚礼蜜游#n
	add_talk 举行婚礼蜜游后将乘“#c005ebe婚礼跑车#n”环游时间城
	
	option 206 #订制【豪华套餐】（费用：5200金币）
	option 404 #【永恒钻戒】介绍
	option 405 #【浪漫烟花】介绍
	option 406 #【婚礼跑车】介绍
	option 200 #我想再了解下其它婚礼套餐
	talk
endproc

proc 404
	dialog 1
	add_talk $_npc$:
	call 411
	option 203 #我想再了解下《豪华套餐》
	talk
endproc

proc 405
	dialog 1
	add_talk $_npc$:
	call 412
	option 203 #我想再了解下《豪华套餐》
	talk
endproc

proc 406
	dialog 1
	add_talk $_npc$:
	call 413
	option 203 #我想再了解下《豪华套餐》
	talk
endproc

proc 204
	choose 1  207 210 订制裸婚套餐需要#cff4e00支付90金币#n\n是否确认订制该套餐
endproc

proc 205
	choose 1  208 210 订制简约套餐需要#cff4e00支付1314金币#n\n是否确认订制该套餐
endproc

proc 206
	choose 1  209 210 订制豪华套餐需要#cff4e00支付5200金币#n\n是否确认订制该套餐
endproc

proc 210
endproc

proc 207
	order_wedding_meal_check 10
	if @10 = 1
		;可以订制
		DEC_MONEY_TEST 0 90 60 12
		if @12 = 0
			GET_PACK_SPACE 0 30
			if @30 > 0
				DEC_MONEY 0 90 60 13
				if @13 = 0
					set_wedding_sth 1 1 11
					if @11 = 1
						ADD_BIND_ITEM_NUM_INPAK 5251132 1 1 31
						
						;提示#订制玩家的名字#成功订制了#套餐类型#，可向“神父”申请举行该套餐的婚礼
						broadcast_wedding_tip 4 0
						
						; add_sys_msg_start
						; add_sys_msg 你成功订制了裸婚套餐，可向“神父”申请举行该套餐的婚礼
						; set_sys_msg_mode 3 0 0
						; MSG_INFO
						return
					endif
				endif
			else
				add_sys_msg_start
				add_sys_msg 你的背包已满，请至少留出一个空位
				set_sys_msg_mode 3 0 0
				MSG_INFO
				return
			endif
		else
			add_sys_msg_start
			add_sys_msg 你持有的金币不足支付裸婚套餐的费用，暂时无法订制
			set_sys_msg_mode 3 0 0
			MSG_INFO
			return
		endif
	endif
endproc

proc 208
	order_wedding_meal_check 10
	if @10 = 1
		;可以订制
		DEC_MONEY_TEST 0 1314 60 12
		if @12 = 0
			GET_PACK_SPACE 0 30
			if @30 > 0
				DEC_MONEY 0 1314 60 13
				if @13 = 0
					set_wedding_sth 1 2 11
					if @11 = 1
						ADD_BIND_ITEM_NUM_INPAK 5251133 1 1 31
						
						;提示#订制玩家的名字#成功订制了#套餐类型#，可向“神父”申请举行该套餐的婚礼
						broadcast_wedding_tip 4 0
						
						; add_sys_msg_start
						; add_sys_msg 你成功订制了简约套餐，可向“神父”申请举行该套餐的婚礼
						; set_sys_msg_mode 3 0 0
						; MSG_INFO
						return
					endif
				endif
			else
				add_sys_msg_start
				add_sys_msg 你的背包已满，请至少留出一个空位
				set_sys_msg_mode 3 0 0
				MSG_INFO
				return
			endif
		else
			add_sys_msg_start
			add_sys_msg 你持有的金币不足支付简约套餐的费用，暂时无法订制
			set_sys_msg_mode 3 0 0
			MSG_INFO
			return
		endif
	endif
endproc

proc 209
	order_wedding_meal_check 10
	if @10 = 1
		;可以订制
		DEC_MONEY_TEST 0 5200 60 12
		if @12 = 0
			GET_PACK_SPACE 0 30
			if @30 > 0
				DEC_MONEY 0 5200 60 13
				if @13 = 0
					set_wedding_sth 1 3 11
					if @11 = 1
						ADD_BIND_ITEM_NUM_INPAK 5251134 1 1 31
						
						;提示#订制玩家的名字#成功订制了#套餐类型#，可向“神父”申请举行该套餐的婚礼
						broadcast_wedding_tip 4 0
						
						; add_sys_msg_start
						; add_sys_msg 你成功订制了豪华套餐，可向“神父”申请举行该套餐的婚礼
						; set_sys_msg_mode 3 0 0
						; MSG_INFO
						return
					endif
				endif
			else
				add_sys_msg_start
				add_sys_msg 你的背包已满，请至少留出一个空位
				set_sys_msg_mode 3 0 0
				MSG_INFO
				return
			endif
		else
			add_sys_msg_start
			add_sys_msg 你持有的金币不足支付豪华套餐的费用，暂时无法订制
			set_sys_msg_mode 3 0 0
			MSG_INFO
			return
		endif
	endif
endproc

proc 211
	dialog 1
	add_talk #b#b#b#b#b#b#b#b#c005ebe【婚礼准备】
	add_talk #b#b
	add_talk ★、开始婚礼时，需要订制方出示的“套餐清单”
	add_talk ★、对有婚史者，将自动回收已有的“永恒钻戒”
	add_talk ★、举行婚礼时，新郎新娘双方都需要在婚礼现场#n
	
	option 299 #开始婚礼仪式
	
	talk
endproc

proc 299
	begin_wedding_check 20
	if @20 = 1 ;可以开始婚礼
		broadcast_wedding_tip 1 21
		if @21 = 1
			oath_ui 22  ;双方弹出誓言面板
		endif
	endif
endproc

;婚礼庆典
proc 220
	begin_wedding_celebration_check 10
	if @10 = 1 ;可以开始婚礼庆典了
		set_wedding_sth 4 0 80
		;记录婚礼庆典的信息，如名字和庆典开始的时间等
		if @80 = 1
			;提示#新郎名字#与#新娘名字#在阳光海滩举行了隆重的婚礼庆典！并且激活领取浪漫烟花选项
			broadcast_wedding_tip 2 11
			call 1
			
			NEW_MAP_MON 352120 1 4403 23278 15083 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 22298 15887 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 23695 15238 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 22829 14503 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 23513 15128 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 23050 14526 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 23765 15682 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 22531 14560 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 23634 15917 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 22357 14756 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 23397 16051 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 22866 16066 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 22559 16005 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 22260 15593 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 22273 15032 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 22244 15304 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 23136 16082 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 23783 15429 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 23273 14857 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 23223 14650 0 0 0 0 1800 0 0
			
			NEW_MAP_MON 352127 1 4403 23275 14961 0 0 0 0 1800 0 0
			NEW_MAP_MON 352127 1 4403 23273 14748 0 0 0 0 1800 0 0
			NEW_MAP_MON 352127 1 4403 23147 14577 0 0 0 0 1800 0 0
			NEW_MAP_MON 352127 1 4403 22940 14504 0 0 0 0 1800 0 0
			NEW_MAP_MON 352127 1 4403 22668 14518 0 0 0 0 1800 0 0
			NEW_MAP_MON 352127 1 4403 22426 14653 0 0 0 0 1800 0 0
			NEW_MAP_MON 352127 1 4403 22300 14890 0 0 0 0 1800 0 0
			NEW_MAP_MON 352127 1 4403 22253 15168 0 0 0 0 1800 0 0
			NEW_MAP_MON 352127 1 4403 22248 15443 0 0 0 0 1800 0 0
			NEW_MAP_MON 352127 1 4403 22277 15734 0 0 0 0 1800 0 0
			NEW_MAP_MON 352127 1 4403 22420 15958 0 0 0 0 1800 0 0
			NEW_MAP_MON 352127 1 4403 22715 16042 0 0 0 0 1800 0 0
			NEW_MAP_MON 352127 1 4403 23004 16077 0 0 0 0 1800 0 0
			NEW_MAP_MON 352127 1 4403 23274 16068 0 0 0 0 1800 0 0
			NEW_MAP_MON 352127 1 4403 23522 15987 0 0 0 0 1800 0 0
			NEW_MAP_MON 352127 1 4403 23714 15796 0 0 0 0 1800 0 0
			NEW_MAP_MON 352127 1 4403 23779 15539 0 0 0 0 1800 0 0
			NEW_MAP_MON 352127 1 4403 23763 15323 0 0 0 0 1800 0 0
			NEW_MAP_MON 352127 1 4403 23611 15169 0 0 0 0 1800 0 0
			NEW_MAP_MON 352127 1 4403 23393 15108 0 0 0 0 1800 0 0
			
			NEW_MAP_MON 352123 1 4403 22119 14854 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 21824 14988 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 21525 15076 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 21228 15016 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 20992 14841 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 20709 14802 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 20425 14900 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 20181 15080 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 19963 15292 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 19772 15510 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 19609 15765 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 19495 16013 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 19379 16275 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 19298 16520 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 19217 16834 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 19155 17138 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 19114 17478 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 19101 17820 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 19103 18162 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 19141 18469 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 19199 18743 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 19267 19031 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 19366 19285 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 19495 19539 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 19678 19758 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 19897 19942 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 20155 20053 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 20457 20120 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 20742 20129 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 21011 20065 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 21283 19909 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 21515 19748 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 21728 19502 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 21870 19208 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 21933 18919 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 22102 18696 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 22307 18505 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 22569 18329 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 22839 18173 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 23058 17916 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 23206 17641 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 23196 17325 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 23110 17019 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 23177 16683 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 23340 16415 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 21773 18817 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 21684 18728 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 21599 18601 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 21550 18461 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 21533 18335 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 21564 18170 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 21626 18037 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 21726 17916 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 21853 17829 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 21997 17781 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 22147 17767 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 22300 17796 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 22431 17867 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 22542 17954 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 22633 18087 0 0 0 0 1800 0 0
			
			NEW_MAP_MON 352126 1 4403 26367 10497 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 26021 10497 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 25705 10605 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 25427 10798 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 25227 11068 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 25121 11389 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 25074 11742 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 24989 12089 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 24813 12412 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 24607 12662 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 24363 12908 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 24111 13117 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 23898 13362 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 23676 13606 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 23468 13833 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 23267 14077 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 23822 14590 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 24008 14355 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 24191 14122 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 24375 13865 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 24572 13629 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 24814 13368 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 25036 13148 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 25275 12945 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 25538 12735 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 25869 12643 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 26244 12624 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 26599 12460 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 26872 12183 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 27053 11867 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 27107 11508 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 27091 11138 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 27115 11325 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 27103 11702 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 26988 12052 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 26754 12355 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 26440 12579 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 26060 12655 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 25687 12649 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 25403 12851 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 25159 13046 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 24939 13261 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 24691 13516 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 24479 13754 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 24289 14004 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 24105 14245 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 23919 14479 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 23368 13955 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 23569 13725 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 23783 13481 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 24005 13226 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 24236 13014 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 24492 12784 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 24719 12541 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 24936 12269 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 25079 11930 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 25084 11565 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 25157 11226 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 25315 10917 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 25547 10681 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 25854 10529 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 26204 10487 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 23154 14195 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 23749 14737 0 0 0 0 1800 0 0
		endif
	endif
endproc

proc 230
	begin_honeymoon_travel_check 10
	if @10 = 1 ;可以开始密游了
		set_wedding_sth 5 0 81   ;记录密游的信息
		if @81 = 1
			broadcast_wedding_tip 3 11
			if @11 = 1
				NEW_MACHINE_PATROL_SPOUSE 1460090 1130 23127 14948 16
			endif
		endif
	endif
endproc

proc 240
	strvar ret zhi num
	get_wedding_sth 2 ret 40
	if @40 = 1 and @ret = 0
		GET_PACK_SPACE 0 30
		if @30 > 0
			GET_ROLE_VAR 72 8 0 zhi
			if zhi = 0
				ADD_BIND_ITEM_NUM_INPAK 2911000 1 1 31
				
				add_sys_msg_start
				add_sys_msg 你从神父手上接过了象征着爱与承诺的“永恒钻戒”
				set_sys_msg_mode 3 0 0
				MSG_INFO
				
				add zhi 1
				SET_ROLE_VAR 72 8 @zhi 0
				
				if @31 = 0
					set_wedding_sth 2 0 0
				endif
			endif
			if zhi > 0
				GET_ITEM_NUM_INPAK 2911000 0 num
				DEC_ITEM_NUM_INPAK 2911000 @num 0
				
				ADD_BIND_ITEM_NUM_INPAK 2911000 1 1 31
				
				add_sys_msg_start
				add_sys_msg 你交出了铭刻着曾经的“永恒钻戒”，从神父手上接过新的“永恒钻戒”
				set_sys_msg_mode 3 0 0
				MSG_INFO
				
				if @31 = 0
					set_wedding_sth 2 0 0
				endif
			endif
		else
			add_sys_msg_start
			add_sys_msg 你的背包已满，请至少留出一个空位装放“永恒钻戒”
			set_sys_msg_mode 3 0 0
			MSG_INFO
			return
		endif
	else
		add_sys_msg_start
		add_sys_msg 你已经领取过“永恒钻戒”了
		set_sys_msg_mode 3 0 0
		MSG_INFO
		return
	endif
endproc

;领取【浪漫烟花】，点燃爱的花火
proc 250
	strvar ret
	get_wedding_sth 3 ret 40
	if @40 = 1 and @ret = 0
		GET_PACK_SPACE 0 30
		if @30 > 0
			ADD_BIND_ITEM_NUM_INPAK 5251136 1 1 31
			
			add_sys_msg_start
			add_sys_msg 你从神父手上接过了意喻着浪漫与璀璨的“浪漫烟花”
			set_sys_msg_mode 3 0 0
			MSG_INFO
			
			if @31 = 0
				set_wedding_sth 3 0 0
			endif
		else
			add_sys_msg_start
			add_sys_msg 你的背包已满，请至少留出一个空位装放“浪漫烟花”
			set_sys_msg_mode 3 0 0
			MSG_INFO
			return
		endif
	else
		add_sys_msg_start
		add_sys_msg 你已经领取过“浪漫烟花”了
		set_sys_msg_mode 3 0 0
		MSG_INFO
		return
	endif
endproc

proc 300
	dialog 1
	add_talk #b#b#b#b#b#b#b#b#c005ebe【分手准备】
	add_talk #b#b
	add_talk ★、解除关系时，提出方单方决定即可分手
	add_talk ★、对方离线时，双方将直接自动解除关系
	add_talk ★、解除关系前，需提出方提供“无情剪刀”#n
	add_talk #b#b
	add_talk #cF80B0B注意：如果订制了婚礼套餐，分手后套餐将会废弃#n
	
	option 301 #我已经决定分手这个选择
	
	talk
endproc

proc 301
	remove_relation_ui 0
endproc

proc 411
	; add_talk #b#b【永恒钻戒】是爱与承诺的象征，戒指戴在无名指意味着爱情的暖流将随血脉汇入心田，传说无名指与心脏是血脉连通最多的手指，故作“爱情之脉”。
	add_talk #b#b【永恒钻戒】是爱与承诺的象征，把戒指戴在无名指上意味着今生今世永不分开。
	add_talk #b#b请把双手的中指向下弯曲背对相靠，剩下手指的指尖相应对碰，这时再把指尖一对一分开，我想你就会明白了。
	add_talk #b#b因为无名指既是代表夫妻。
	add_talk #b#b
	add_talk #c186506【永恒钻戒】
	add_talk 装备条件：夫妻
	add_talk 戒指用途：装备后可获得“我想你”夫妻技能，施放技能可马上传送到伴侣的身旁。#n
endproc

proc 412
	add_talk #b#b订制了简约或豪华套餐的新人，宣誓仪式结束后可向我申请【婚礼庆典】，并领取1份【浪漫烟花】。
	add_talk #b#b庆典中新人可点燃烟花，与好友来宾一同庆贺这永恒的一刻。
	add_talk #b#b
	add_talk #c186506【浪漫烟花】：
	add_talk 领取条件：1. 婚礼订制了简约或豪华套餐
	add_talk #b#b#b#b#b2. 成功申请了【婚礼庆典】
	add_talk 使用条件：只能在婚礼现场使用
	add_talk 烟花馈赠：采集烟花燃烧残留的【烟花星尘】可获得珍稀宝贝#n
endproc

proc 413
	add_talk #b#b订制了豪华套餐的新人，宣誓仪式结束后可向我申请【婚礼蜜游】，乘坐【婚礼跑车】环游时间城。
	add_talk #b#b环游中新人可在跑车上向大家抛送【浪漫花球】，愿每个人都能早日遇上命中的另一半。
	add_talk #b#b
	add_talk #c186506【浪漫花球】：
	add_talk 使用方法：只能使用【婚礼跑车】的技能抛送花球
	add_talk 花球馈赠：采集抛送在地上的【浪漫花球】可获得珍稀宝贝#n
endproc
