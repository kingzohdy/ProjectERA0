proc 1
	dialog 1
	add_talk $_npc$:
	; add_talk #b#b亲爱的玩家您好！我是$_npc$！
	add_talk #b#b为了答谢大家对《零纪元》的厚爱和支持，您可以从#c005ebe公测特权金卡#n中收集3张#c005ebe黄金礼券#n和我兑换#c005ebe黄金礼包#n，或狂野摩托坐骑，赶快行动吧！
	; add_talk #b#b
	add_talk #b#b当然您也可以从#c005ebe公测特权金卡#n中收集4张#c005ebe白金卡#n和我兑换#c005ebe白金福袋#n，高级宠物、金券、宝石等更多虚拟宝物！
	add_talk #b#b
	add_talk #b#b#c186506宠物券：开启多彩宝石礼包获得
	add_talk #b#b坐骑券：开启白金福袋有机会获得
	add_talk #b#b时装券：开启多彩宝石礼包或白金福袋获得#n
		
		option 400 #(买卖)打开商店
		
;		option 500 #我要兑换黄金礼包
;		option 520 我要换幸运刮刮乐福袋
		
;		option 600 #我要兑换白金福袋
	talk
endproc

proc 500
	strvar it1
	GET_ITEM_NUM_INPAK 5054006 1 it1
	
	if it1 >= 3
		strvar num
		GET_PACK_SPACE 0 num 
		if num > 0
			DEC_ITEM_NUM_INPAK 5054006 3 15
			if 15 = 0
				ADD_BIND_ITEM_NUM_INPAK 5052910 1 1 0                   
			endif 
		else                  
			add_sys_msg_start
			add_sys_msg 背包空间不足！
			set_sys_msg_mode 4 0 0
			msg_info
		
		endif
	else
		add_sys_msg_start
		add_sys_msg 你携带的黄金礼券数量不足！
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc


proc 520
	dialog 1
	add_talk $_npc$:
	add_talk #b#b您可以通过四张刮刮乐来和我兑换刮刮乐礼包
	add_talk #b#b
;	add_talk #b#b第一种：官网+17173+多玩+腾讯
;	add_talk #b#b第二种：官网+17173+多玩+新浪
;	add_talk #b#b第三种：官网+17173+多玩+网易
;	add_talk #b#b第四种：官网+腾讯+新浪+网易
		
		option 501 #我用第一种组合换福袋
		option 502 #我用第二种组合换福袋
		option 503 #我用第三种组合换福袋
		option 504 #我用第四种组合换福袋
		
	talk
endproc

proc 501
	strvar it1 it2 it3 it4 it5 it6
	GET_ITEM_NUM_INPAK 5054000 1 it1
	GET_ITEM_NUM_INPAK 5054003 1 it4
	GET_ITEM_NUM_INPAK 5054004 1 it5
	GET_ITEM_NUM_INPAK 5054005 1 it6
	
	if it1 > 0 and it4 > 0 and it5 > 0 and it6 > 0
		strvar num
		GET_PACK_SPACE 0 num 
		if num > 0
			DEC_ITEM_NUM_INPAK 5054000 1 1
			DEC_ITEM_NUM_INPAK 5054003 1 1
			DEC_ITEM_NUM_INPAK 5054004 1 1
			DEC_ITEM_NUM_INPAK 5054005 1 1
			                  
			ADD_BIND_ITEM_NUM_INPAK 5052911 1 1 0  
		else                  
			add_sys_msg_start
			add_sys_msg 背包空间不足！
			set_sys_msg_mode 4 0 0
			msg_info
		
		endif
		
	else
		add_sys_msg_start
		add_sys_msg 你携带的物品数量不足！
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc 

proc 502
	strvar it1 it2 it3 it4 it5 it6
	GET_ITEM_NUM_INPAK 5054000 1 it1
	GET_ITEM_NUM_INPAK 5054003 1 it4
	GET_ITEM_NUM_INPAK 5054004 1 it5
	GET_ITEM_NUM_INPAK 5054002 1 it6
	
	if it1 > 0 and it4 > 0 and it5 > 0 and it6 > 0
		strvar num
		GET_PACK_SPACE 0 num 
		if num > 0
			DEC_ITEM_NUM_INPAK 5054000 1 1
			DEC_ITEM_NUM_INPAK 5054003 1 1
			DEC_ITEM_NUM_INPAK 5054004 1 1
			DEC_ITEM_NUM_INPAK 5054002 1 1
			                  
			ADD_BIND_ITEM_NUM_INPAK 5052911 1 1 0  
		else                  
			add_sys_msg_start
			add_sys_msg 背包空间不足！
			set_sys_msg_mode 4 0 0
			msg_info
		
		endif
		
	else
		add_sys_msg_start
		add_sys_msg 你携带的物品数量不足！
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc 

proc 503
	strvar it1 it2 it3 it4 it5 it6
	GET_ITEM_NUM_INPAK 5054000 1 it1
	GET_ITEM_NUM_INPAK 5054003 1 it4
	GET_ITEM_NUM_INPAK 5054004 1 it5
	GET_ITEM_NUM_INPAK 5054001 1 it6
	
	if it1 > 0 and it4 > 0 and it5 > 0 and it6 > 0
		strvar num
		GET_PACK_SPACE 0 num 
		if num > 0
			DEC_ITEM_NUM_INPAK 5054000 1 1
			DEC_ITEM_NUM_INPAK 5054003 1 1
			DEC_ITEM_NUM_INPAK 5054004 1 1
			DEC_ITEM_NUM_INPAK 5054001 1 1
			                  
			ADD_BIND_ITEM_NUM_INPAK 5052911 1 1 0  
		else                  
			add_sys_msg_start
			add_sys_msg 背包空间不足！
			set_sys_msg_mode 4 0 0
			msg_info
		
		endif
		
	else
		add_sys_msg_start
		add_sys_msg 你携带的物品数量不足！
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc 



proc 504
	strvar it1 it2 it3 it4 it5 it6
	GET_ITEM_NUM_INPAK 5054000 1 it1
	GET_ITEM_NUM_INPAK 5054001 1 it4
	GET_ITEM_NUM_INPAK 5054002 1 it5
	GET_ITEM_NUM_INPAK 5054005 1 it6
	
	if it1 > 0 and it4 > 0 and it5 > 0 and it6 > 0
		strvar num
		GET_PACK_SPACE 0 num 
		if num > 0
			DEC_ITEM_NUM_INPAK 5054000 1 1
			DEC_ITEM_NUM_INPAK 5054001 1 1
			DEC_ITEM_NUM_INPAK 5054002 1 1
			DEC_ITEM_NUM_INPAK 5054005 1 1
			                  
			ADD_BIND_ITEM_NUM_INPAK 5052911 1 1 0  
		else                  
			add_sys_msg_start
			add_sys_msg 背包空间不足！
			set_sys_msg_mode 4 0 0
			msg_info
		
		endif
		
	else
		add_sys_msg_start
		add_sys_msg 你携带的物品数量不足！
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc 

proc 600
	strvar it1
	GET_ITEM_NUM_INPAK 5052930 1 it1
	
	if it1 >= 4
		strvar num
		GET_PACK_SPACE 0 num 
		if num > 0
			DEC_ITEM_NUM_INPAK 5052930 4 15
			if 15 = 0
				ADD_BIND_ITEM_NUM_INPAK 5052912 1 1 0                   
			endif 
		else                  
			add_sys_msg_start
			add_sys_msg 背包空间不足！
			set_sys_msg_mode 4 0 0
			msg_info
		
		endif
	else
		add_sys_msg_start
		add_sys_msg 你携带的白金卡数量不足！
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc 

proc 400
	OPEN_NPC_SHOP
endproc





