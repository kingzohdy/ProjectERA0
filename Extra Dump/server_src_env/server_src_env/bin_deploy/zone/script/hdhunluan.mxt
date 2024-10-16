;混乱元素脚本，20-满载蒸汽核心数量 30-扣除道具返回标志 40-蒸汽核心数量

proc 1
	DIALOG 1
	add_talk $_npc$:
	GET_VAILD_ITEM_NUM_INPAK 5300963 1 20

	if 20 >= 10
		call 141
	else
		call 140
	endif

	talk
endproc 

proc 140
	add_talk #b#b似乎是一种元素生物。你能感觉得到，这小小的身躯中蕴含着巨大的能量。抓紧时间用蒸汽核心收集能量吧。
	add_talk #b#b
	add_talk #b#b#c186506提示：蒸汽核心可以通过商运活动获得。#n
	
	strvar num
	rand num 5
	switch @num
		case 0
			option 150 我来收集能量的
			option 154 #蒸汽核心是什么
			option 155 #你们从哪里来的
			option 156 #我见过黑暗物质
			option 157 #我只是个路过的
		endcase
		case 1
			option 154 #蒸汽核心是什么
			option 150 我来收集能量的
			option 155 #你们从哪里来的
			option 156 #我见过黑暗物质
			option 157 #我只是个路过的
		endcase
		case 2
			option 154 #蒸汽核心是什么
			option 155 #你们从哪里来的
			option 150 我来收集能量的
			option 156 #我见过黑暗物质
			option 157 #我只是个路过的
		endcase
		case 3
			option 154 #蒸汽核心是什么
			option 155 #你们从哪里来的
			option 156 #我见过黑暗物质
			option 150 我来收集能量的
			option 157 #我只是个路过的
		endcase
		case 4
			option 154 #蒸汽核心是什么
			option 155 #你们从哪里来的
			option 156 #我见过黑暗物质
			option 157 #我只是个路过的
			option 150 我来收集能量的
		endcase
	endswitch
endproc 

proc 141
	add_talk #b#b满载的蒸汽核心中透出澎湃的能量，非常不稳定，同时携带多个蒸汽核心极其危险。
	add_talk #b#b
	add_talk #b#b#c186506提示：同一时间只能携带10个【满载的蒸汽核心】，请上交后再继续收集能量吧。#n
endproc 

proc 150
	
	
	strvar Mnum
	var Mnum 0
	GET_VAILD_ITEM_NUM_INPAK 5300963 1 Mnum
	;满载的蒸汽核心数量
	strvar Znum
	var Znum 0
	GET_VAILD_ITEM_NUM_INPAK 5060030 1 Znum
	;蒸汽核心数量
	
	strvar num
	var num 10
	SUB num @Mnum
	
	if Znum = 0
		call 153
		return
	else
		if @Znum >= @num
			dec_item_num_inpak 5060030 @num 30
			if 30 = 0
				add_item_num_inpak 5300963 @num 1 0
				
				add_sys_msg_start
				add_sys_msg 蒸汽核心已经充满能量，请迅速回到时间城大行政官处上交
				set_sys_msg_mode 3 0 0
				msg_info
				
				call 151
			endif
		else
			if @num > @Znum
				dec_item_num_inpak 5060030 @Znum 30
				if 30 = 0
					add_item_num_inpak 5300963 @Znum 1 0
					
					add_sys_msg_start
					add_sys_msg 蒸汽核心已经充满能量，请迅速回到时间城大行政官处上交
					set_sys_msg_mode 3 0 0
					msg_info
					
					call 151
				endif
			endif
		endif
	endif
endproc 

proc 151
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b收集能量过后，混乱元素似乎变小了一点。
	add_talk #b#b
	add_talk #b#b#c186506提示：混乱元素会在8分钟后消失，请迅速赶回时间城，向#L大行政官@@[306013,1000]#n#c186506上交满载的蒸汽核心。在混乱元素出现期间，可以重复上交，无次数限制。#n

	talk
endproc 

proc 153
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b你没有蒸汽核心，不能向混乱元素收集能量。
	add_talk #b#b
	add_talk #b#b#c186506提示：蒸汽核心可以在商运活动中获得。#n
	add_talk #b#b#c186506提示：混乱元素会在8分钟后消失，请抓紧时间。#n

	talk
endproc 

proc 154
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b蒸汽核心是一种能量收集装置，利用它可以收集我体内的高密能量。
	add_talk #b#b正如我所说，你可以通过完成商运的任务来获得这种装置。
	add_talk #b#b最近时间城的大行政官似乎高价在收购装置收集的能量。

	talk
endproc

proc 155
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b我们来到这个世界已经很久了。
	add_talk #b#b早在世纪末日的混沌中，地壳裂缝产生了扭曲空间，我们便从另一个空间带到这个星球。

	talk
endproc

proc 156
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b黑暗物质？更应该说他们也是混乱元素吧。
	add_talk #b#b已经记不起多久前了，突然出现的黑暗议会，抓走了我们的同胞。
	add_talk #b#b最终这些同胞被黑暗魔法侵蚀，成了现在的黑暗物质。

	talk
endproc
	
	
	
	
