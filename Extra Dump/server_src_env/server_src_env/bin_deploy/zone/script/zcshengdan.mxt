;;;;圣诞NPC


proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b喔呵呵！一年一度的圣诞节又到了！大家节日快乐啊！
	add_talk #b#b今年又是准备了不少礼物！姜饼、圣诞糖果、雪球、圣诞烟花、圣诞魔棒应有尽有！当然少不了圣诞时装和驯鹿拉车！
	add_talk #b#b这些都能从圣诞礼箱里开到哦！喔呵呵呵！
		
	task_add_option 3121 15 100

;	task_add_option 3122 1 101
;	task_add_option 3123 1 102
;	task_add_option 3124 1 103
;	task_add_option 3125 1 104
;	task_add_option 3126 1 105
;	task_add_option 3127 1 106
;	task_add_option 3128 1 107
;	task_add_option 3129 1 108

	TASKCYCLE_ADD_OPTION 121 12 110
	
	option 200 #【圣诞大派送】
	option 300 #【被抢走的礼箱】
	

	talk
endproc
PROC 100
	task_do_option 3121 1
ENDPROC

PROC 110	
	TASKCYCLE_DO_OPTION 121 1
ENDPROC
proc 2
	get_trig_task 1 2
	strvar Mid
	GET_PLAYER_ID 1 Mid
	if 2 = 3121		
		
		ADD_BIND_ITEM_NUM_INPAK 5012105 5 0 0
	endif

endproc 

PROC 200
	dialog 1
	add_talk $_npc$:
	add_talk #b#b喔呵呵，为了这次圣诞节，老夫可是费尽心思准备了好多礼物！
	add_talk #b#b只要等老夫整理好就会把礼箱堆到时间城的大圣诞树下，想要礼物的孩子可不要错过啦！
	add_talk #b#b
	add_talk #c005ebe圣诞大派送时间，时间城的大圣诞树下将堆满大量【圣诞红礼箱】#n
	add_talk #b#b#c186506活动日期：2013年12月25日-2014年1月3日
	add_talk #b#b活动时间：10:00-22:00（每间隔2小时）#n
	
	option 201 #大圣诞树都在哪？
	option 1 #返回
	
	talk
ENDPROC

PROC 201
	dialog 1
	add_talk $_npc$:
	add_talk #b#b时间城里可是有#c18650610棵#n大圣诞树哟！
	add_talk #b#b
	add_talk #b#b第1棵~#L大圣诞树@@(1000,29504,33330)#n
	add_talk #b#b第2棵~#L大圣诞树@@(1000,29448,29696)#n
	add_talk #b#b第3棵~#L大圣诞树@@(1000,23301,29856)#n
	add_talk #b#b第4棵~#L大圣诞树@@(1000,23349,33149)#n
	add_talk #b#b第5棵~#L大圣诞树@@(1000,38795,29661)#n
	add_talk #b#b第6棵~#L大圣诞树@@(1000,39694,31108)#n
	add_talk #b#b第7棵~#L大圣诞树@@(1000,37960,32411)#n
	add_talk #b#b第8棵~#L大圣诞树@@(1000,38847,34238)#n
	add_talk #b#b第9棵~#L大圣诞树@@(1000,38432,43607)#n
	add_talk #b#b第10棵~#L大圣诞树@@(1000,21414,46004)#n
	
	
	option 1 #返回
	
	talk
ENDPROC

PROC 300
	dialog 1
	add_talk $_npc$:
	add_talk #b#b唔…今年圣诞节【黑暗物质】又跑出来捣乱了~
	add_talk #b#b打算为变装派对准备的【#c005ebe圣诞黄礼箱#n】被抢走一大批~如今只好把礼箱重新抢回来。
	add_talk #b#b唔…要是谁抢到箱子就归谁好了，喔呵呵。
	add_talk #b#b
	add_talk #c005ebe黑暗来袭活动期间，【黑暗物质】将大量涌现在银风雪原#n
	add_talk #b#b#c186506活动时间：10:30-10:40、15:15-15:25#n

	option 1 #返回
	
	talk
ENDPROC
