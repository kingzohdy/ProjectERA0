

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b不要追逐成功，追求优秀，成功就追逐着你。
	
	task_add_option 1098 1 104
	task_add_option 1109 1 105
	task_add_option 1110 1 106
	task_add_option 2009 1 113
	task_add_option 2015 1 116
	task_add_option 2022 1 117
	task_add_option 1111 1 118
	task_add_option 1112 1 119

;修改30~40
	task_add_option 2005 1 145
;	task_add_option 2018 1 146
;	task_add_option 2019 1 147
	task_add_option 2021 1 148 
	task_add_option 2014 1 149
;20~30调整

;	task_add_option 1501 1 164	
;	task_add_option 1502 1 165	
;铜鼓村修改

	task_add_option 1301 1 166	
;精灵修改
	task_add_option 1082 1 167	
	task_add_option 3110 1 169	
;1~10修改
;	task_add_option 1077 1 170	

;;;;
	task_add_option 1801 1 171	
	task_add_option 1802 1 172	
	task_add_option 3030 1 173

	strvar Ret
	task_stat 1301 0 Ret
	if Ret = 2
		
		option 300 时间城太大了，能否送我一程？
	endif
	
	strvar Ret1
	IS_WORLD_CITY_OWNER Ret1
	if Ret1 = 1
		option 250 #进入中控中心地图
	endif
	

	option 500 我想询问混乱元素的有关事宜
	
	task_stat 4446 1 20
	if 20 = 2
		option 600 城主阁下，我想为孩子们尽点心力！
	endif
    TALK               
endproc 

proc 500
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b时间城以“神源”为能源悬浮于空中，每年都需要消耗大量的元素石为神源注入能量。但是，元素石的产量开始供不应求了。
	add_talk #b#b据研究表明，混乱元素身上蕴含着大量的能量，只要你为时间城带回这些能源，就可以获得银币作为奖励。
	add_talk #b#b感谢对时间城所做的一切！
	add_talk #b#b
	add_talk #b#b#c186506提示：混乱元素会在每天的11:00-16:00和18:00-23:00各随机出现一次，每次出现8分钟。#n
	add_talk #b#b#c186506提示：混乱元素会随机出现在银风雪原、阿努比斯神殿和十三号矿洞中的某个比较偏僻的地点。#n
	add_talk #b#b#c186506提示：用于收集能量的蒸汽核心可以通过商运活动获得。#n

		option 501 上交1个满载的蒸汽核心
		option 502 #关闭
		
	talk
endproc 
proc 501
	strvar Rate
	var Rate 0
	strvar INum
	
	GET_VAILD_ITEM_NUM_INPAK 5300963 1 20
	if 20 > 0
		GET_PACK_SPACE 0 INum
		if INum > 0
			dec_item_num_inpak 5300963 1 30
			if 30 = 0
				Rand Rate 2
				if Rate = 1
					ADD_BIND_ITEM_NUM_INPAK 5052872 1 1 0
				else
					ADD_BIND_ITEM_NUM_INPAK 5052871 1 1 0
				endif
			endif
		else
			DIALOG 1
			add_talk $_npc$:
			add_talk #b#b少年，你的背包满了，去清理一下背包再来换取奖励吧。
			add_talk #b#b
			add_talk #b#b#c186506提示：要换取奖励，背包中至少要有一个空闲的格子。#n
			talk
		endif	
	else
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b请使用蒸汽核心在混乱元素处收集到能量再来找我吧。	
		talk
	endif
	
endproc 
proc 600
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b原来是大名鼎鼎的$_player$啊！你愿意照看幼儿园的孤儿？太感谢你了，我的朋友！	
	add_talk #b#b他们日复一日的在狭小的幼儿园里“探险”，梦想长大以后能变得足够强壮，去保护这个世界所有的善良，去游览那些本该由父母陪伴他们前往的风景胜地…由您来照顾时间城幼儿园的小朋友，实在是再好不过了！

		option 601 #多谢城主，我定当竭尽全力照顾好孩子们！
	talk
endproc 
proc 601
	task_fini 4446 1	
endproc
PROC 104
	task_do_option 1098 1
ENDPROC
PROC 105
	task_do_option 1109 1
ENDPROC
PROC 106
	task_do_option 1110 1
ENDPROC
PROC 107
	task_do_option 4100 1
ENDPROC

PROC 113
	task_do_option 2009 1
ENDPROC

PROC 116
	task_do_option 2015 1
ENDPROC
PROC 117
	task_do_option 2022 1
ENDPROC
PROC 118
	task_do_option 1111 1
ENDPROC
PROC 119
	task_do_option 1112 1
ENDPROC


PROC 145
	task_do_option 2005 1
ENDPROC
PROC 146
	task_do_option 2018 1
ENDPROC
PROC 147 
	task_do_option 2019 1
ENDPROC
PROC 148
	task_do_option 2021 1
ENDPROC
PROC 149
	task_do_option 2014 1
ENDPROC

PROC 157
	task_do_option 4104 1
ENDPROC
PROC 158
	task_do_option 4119 1
ENDPROC

;PROC 164
;	task_do_option 1501 1
;ENDPROC

;PROC 165
;	task_do_option 1502 1
;ENDPROC
PROC 166
	task_do_option 1301 1
ENDPROC
PROC 167
	task_do_option 1082 1
ENDPROC
PROC 169
	task_do_option 3110 1
ENDPROC
;PROC 170
;	task_do_option 1077 1
;ENDPROC
PROC 171
	task_do_option 1801 1
ENDPROC
PROC 172
	task_do_option 1802 1
ENDPROC
PROC 173
	task_do_option 3030 1
ENDPROC
proc 250
	strvar Ret2
	IS_WORLD_CITY_OWNER Ret2
	if Ret2 = 0
		add_sys_msg_start
		add_sys_msg 你已经不是公会成员了，不能进入此地图。
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif

	strvar Rad
	rand Rad 6
	switch Rad
		
		case 0
			TRANSMIT 4404 19700 21100 0
		endcase
		case 1
			TRANSMIT 4404 19700 20600 0
		endcase
		case 2
			TRANSMIT 4404 19500 20500 0
		endcase
		case 3
			TRANSMIT 4404 19300 20700 0
		endcase
		case 4
			TRANSMIT 4404 19300 20700 0
		endcase
		case 5
			TRANSMIT 4404 20100 21000 0
		endcase
	endswitch
endproc 


proc 2
	get_trig_task 1 2

	if 2 = 1098
		task_fini 1098 1
	endif
	if 2 = 1110
		task_fini 1110 1
	endif
	if 2 = 1111
		task_fini 1111 1
	endif 

	if 2 = 2009
		task_fini 2009 1
	endif
	if 2 = 2012
		task_fini 2012 1
	endif
	if 2 = 2015
		task_fini 2015 1
	endif
	if 2 = 2022
		task_fini 2022 1
	endif
	if 2 = 2025
		task_fini 2025 1
	endif
	if 2 = 2030
		task_fini 2030 1
	endif

	if 2 = 2123
		task_fini 2123 1
	endif

	if 2 = 2137
		task_fini 2137 1
	endif
;	if 2 = 3110
;		task_fini 3110 1
;	endif

;	if 2 = 1502
;		task_fini 1502 1
;	endif
	if 2 = 1301
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b不要追逐成功，追求优秀，成功就追逐着你。
		
			task_fini 1301 1
			option 300 时间城太大了，能否送我一程？
		
		talk
	endif
	if 2 = 3101
		task_fini 3101 1
	endif
;	if 2 = 3000
;		task_fini 3000 1
;	endif
	if 2 = 1802
		task_fini 1802 1
	endif
endproc
proc 300
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b当然可以。
	add_talk #b#b不过，我是直接将您送到目的地，中途不会停的哟！
	add_talk #b#b您确定要乘坐直达班车吗？
		option 301 #请直接将我送到目的地吧！
		option 302 #不用啦，我自己过去，还能逛逛时间城！
	talk

endproc 
proc 301
;	NEW_OWNER_TMP_ITEM 1460073 26500 38700 0 0 0 313 1095 1 3600 1
	AUTO_MOVE 20 2100 4200 3900 1460073 0 3081 1 3600

endproc 
proc 302
	return
endproc 




