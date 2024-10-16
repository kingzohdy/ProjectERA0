;候姆

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b我漂泊大半辈子，居无定所，现在最想要的就是一个稳定的家园，所以干起了这份工作。
	add_talk #b#b#Name#，在外拼搏的时候别忘了回家，虽然外面的世界太精彩，但这里始终是你的避风港，别像我这样老了才领悟。
	add_talk #b#b所以你买房么？
		option 100 进入家园
		option 200 家园查询及购买
		option 300 家园介绍
		option 400 金豆豆兑换
	
	talk
endproc

proc 100
	strvar ret
	home_enter ret
	if ret != 0 
		add_sys_msg_start
		add_sys_msg 你没有购买家园，暂时无法进入
		set_sys_msg_mode 3 0 0
		msg_info
	endif
	; OPEN_UI 96
endproc

proc 200
	home_ui 0
endproc

proc 300
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b在我这购买一套家园，你不需要面对残酷的厮杀。
	add_talk #b#b#c005ebe现已开盘200套家园，只要提供【地契】和一定的装修费即可拥有一套家园！#n
	add_talk #b#b每人只能#c186506限购一套#n，先到先得！
	add_talk #b#b#cF80B0B如果你10天未进入家园，系统将自动回收家园，包括装饰、菜园及小宠物已成长的等级。#n
		option 301 家园住宅介绍
		option 302 家园菜地介绍
		option 303 家园花园介绍
		option 1 返回
	
	talk
endproc

proc 301
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b住宅内可摆放各种桌椅床等家具，使空旷的家园变得更加温馨。

		option 300 返回
	
	talk
endproc

proc 302
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b菜地可以种植蔬果出售赚取绑银，可以收获家园萌宠所需的生活物资，更能挖取到稀有的金豆豆。
	add_talk #b#b金豆豆可以在时间城-家园管理员【候姆】处兑换时装及相关物品
		option 300 返回
	
	talk
endproc

proc 303
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b花园可以用来寄养小宠物，当小宠物喂养到10级后（不包含10级），将会不定时的产出宠物蛋！
		option 300 返回
	
	talk
endproc

proc 400
	OPEN_NPC_SHOP
endproc

