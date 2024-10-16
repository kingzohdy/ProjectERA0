;抓宠地图彩虹岛传送员 317005


proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b一个给力的宠物会让你打怪、做任务、下副本事半功倍，即使出门在外也是防身之利器。
	task_add_option 3001 1 102	
	
	task_add_option 3002 1 103	
	task_add_option 3007 1 104
	task_add_option 3008 1 105
	task_add_option 3009 1 106
	
	task_add_option 3000 1 107
;	task_add_option 1502 1 130
	
	task_add_option 3010 1 121
	task_add_option 3011 1 108
	task_add_option 3012 1 109
	task_add_option 3013 1 120
	task_add_option 3014 1 122
	task_add_option 3015 1 123
	task_add_option 3004 1 124
	
	task_add_option 1082 1 125
	task_add_option 1351 1 126
	task_add_option 1084 1 127
	
	task_add_option 3016 1 128
	task_add_option 3017 1 129

	strvar Ret
	task_stat 1082 1 Ret
	GET_ID_PLAYER_INFO 0 8 lv
	if Ret = 2
		
;		option 500 时间城太大了，能否送我一程？
	endif
	
	
	
	task_stat 3001 1 21
	if 21 = 2
		option 301 开始答题
	endif
	task_stat 3014 1 22
	if 22 = 2
		option 201 开始答题
	endif
	task_stat 3015 1 23
	if 23 = 2
		option 401 开始答题
	endif
	
	task_stat 3016 1 24
	task_stat 3017 1 25
	if 24 = 2
		option 430 #宠物转星-提交宠物蛋
	endif
	if 25 = 2
		option 440 #宠物吞噬-提交宠物蛋
	endif
	
	
	OPTION 800 #(买卖)打开宠物商店
	if 24 = 3
		OPTION 600 #宠物转星  （可转移宠物的强化星级）
	endif
	if 25 = 3
		OPTION 601 #宠物吞噬  （可吸收宠物的经验、幻化资质）
	endif
	
	if lv >= 70
		option 610 #宠物进阶  （可晋升为强大的战宠）
	endif
	
	if lv >= 50
		option 611 #宠物进化（可提升宠物成长潜力）
	endif
	
	strvar lv
	task_stat 3003 0 50
	; GET_ID_PLAYER_INFO 0 8 lv (移到第1个strvar定义）
	if 50 = 3 
	;	OPTION 110 #去彩虹岛抓宠物啦！
	endif
	
	TASK_STAT 3002 0 60
;	TASK_STAT 3008 0 61
	TASK_STAT 3007 0 62
		
	if 60 = 1 or 60 = 3

		if 62 != 1 
			if 62 != 3 
				option 320 #去彩虹试炼谷找凯拉
			endif
		endif

	endif
	talk

endproc
proc 600
	open_ui 81
endproc
proc 601
	open_ui 82
endproc
proc 610
	open_ui 83
endproc
proc 611
	open_ui 90
endproc
proc 430
	dialog 3
	add_talk $_npc$:
	add_talk #b#b很高兴你愿意帮助我，能与你分享成果是我的荣幸，你只需要在下面的框内入上一颗宠物蛋即可，品种不限！
	add_talk #b#b
	add_talk 获得宠物蛋的方式有很多种：
	add_talk #b#b最省钱的方式：#c186506去彩虹岛捕捉#n
	add_talk #b#b最省力的方式：#c186506流浪汉处捐献开锦囊#n
	add_talk #b#b最省心的方式：#c186506在摊位上购买#n
	add_talk #b#b#c6d00d2
	add_talk #b#b
;	add_talk #b#b将宠物蛋拖至下方的框内

		NPC_COMMIT_OPT 431
	talk
endproc
proc 431
	strvar itemId pos index ret flag
	GET_COMMIT_ITEM_INFO 1 itemId
	if itemId = -1
		add_sys_msg_start
		add_sys_msg 您还没有提交物品
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	GET_COMMIT_ITEM_INFO 5 50
	;是否是宠物蛋
	if 50 = -1
		add_sys_msg_start
		add_sys_msg 你提交的好像不是宠物蛋！
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	GET_COMMIT_ITEM_INFO 2 pos
	GET_COMMIT_ITEM_INFO 3 index
	DEC_ITEM_NUM_BYIDX @itemId 1 @pos @index ret
	if ret = 0
		task_stat 3016 1 24
		if 24 = 2
			task_fini 3016 1
		endif
	endif
	PLAYER_CLOSE_DIALOG 0 0

endproc 
proc 440
	dialog 3
	add_talk $_npc$:
	add_talk #b#b很高兴你愿意帮助我，能与你分享成果是我的荣幸，你只需要在下面的框内入上一颗宠物蛋即可，品种不限！
	add_talk #b#b
	add_talk 获得宠物蛋的方式有很多种：
	add_talk #b#b最省钱的方式：#c186506去彩虹岛捕捉#n
	add_talk #b#b最省力的方式：#c186506流浪汉处捐献开锦囊#n
	add_talk #b#b最省心的方式：#c186506在摊位上购买#n
	add_talk #b#b#c6d00d2
	add_talk #b#b
;	add_talk #b#b将宠物蛋拖至下方的框内

		NPC_COMMIT_OPT 441
	talk
endproc
proc 441
	strvar itemId pos index ret flag
	GET_COMMIT_ITEM_INFO 1 itemId
	if itemId = -1
		add_sys_msg_start
		add_sys_msg 您还没有提交物品
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	GET_COMMIT_ITEM_INFO 5 50
	;是否是宠物蛋
	if 50 = -1
		add_sys_msg_start
		add_sys_msg 你提交的好像不是宠物蛋！
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	GET_COMMIT_ITEM_INFO 2 pos
	GET_COMMIT_ITEM_INFO 3 index
	DEC_ITEM_NUM_BYIDX @itemId 1 @pos @index ret
	if ret = 0
		task_stat 3017 1 25

		if 25 = 2
			task_fini 3017 1
		endif
	
	endif
	PLAYER_CLOSE_DIALOG 0 0

endproc 


PROC 800
	
	TASK_STAT 3009 1 22
	if 22 = 2
		task_fini 3009 1
	endif

	OPEN_NPC_SHOP
ENDPROC


proc 301
	dialog 1
	add_talk $_npc$:
	add_talk #b#b这个世界上，能够与你同甘共苦、生死相随、不离不弃、死心塌地、相伴终生、携手同游的是__？
	add_talk #b#b<请选择正确的答案>
		option 303  选项A：宠物
		option 304  选项B：宇宙超人
		option 304  选项C：凹凸曼
		option 304  选项D：梦中情人
	talk
endproc 
proc 302
	return
endproc 
proc 303
	dialog 1
	add_talk $_npc$:
	add_talk  #b#b这个世界上愿意为你豁命挡箭、两肋插刀，替你承受伤痛的是__？
	add_talk #b#b<请选择正确的答案>
		option 304  选项A：宇宙超人
		option 304  选项B：凹凸曼
		option 312  #选项C：宠物
		option 304  选项D：梦中情人
		
	talk
endproc

proc 304
	dialog 1
	add_talk $_npc$:
	add_talk #b#b咦，这可不是博学的人应该选的答案哟，好好想想再重新回答吧。
		option 301 重新开始
		option 302 #放弃
		
	talk
endproc

proc 312
	TASK_FINI 3001 0
endproc

proc 102
	task_do_option 3001 1
endproc
proc 103
	task_do_option 3002 1
endproc
proc 104
	task_do_option 3007 1
endproc
proc 105
	task_do_option 3008 1
endproc
proc 106
	task_do_option 3009 1
endproc
proc 107
	task_do_option 3000 1
endproc
proc 108
	task_do_option 3011 1
endproc
proc 109
	task_do_option 3012 1
endproc

proc 120
	task_do_option 3013 1
endproc 
proc 121
	task_do_option 3010 1
endproc 
proc 122
	task_do_option 3014 1
endproc 
proc 123
	task_do_option 3015 1
endproc 
proc 124
	task_do_option 3004 1
endproc 
proc 125
	task_do_option 1082 1
endproc  
proc 126
	task_do_option 1351 1
endproc  
proc 127
	task_do_option 1084 1
endproc  
proc 128
	task_do_option 3016 1
endproc  
proc 129
	task_do_option 3017 1
endproc 

proc 110

	get_player_base_att 1 2 3 4
	if 3 >= 20
		transmit 4400 23725 27202 1
	else
		add_sys_msg_start
		add_sys_msg 实在抱歉,您的等级未达到20级,请您过段时间再来吧...
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc


proc 320

		ENTER_PWORLD 202 23725 27202 1
endproc

proc 500
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b当然可以，您可是个大人物呢。
	add_talk #b#b不过，我是直接将您送到目的地，中途不会停的哟！
	add_talk #b#b您确定要乘坐直达班车吗？
		option 501 #请直接将我送到目的地吧！
		option 502 #不用啦，我自己过去，还能逛逛时间城！
	talk
endproc
proc 501
	
;	NEW_OWNER_TMP_ITEM 1460072 29400 26600 0 0 0 312 1095 1 3600 1
	AUTO_MOVE 0 1000 26500 38800 1460072 0 3081 1 3600
endproc 
proc 502
	return
endproc 

proc 2
	get_trig_task 1 2
	if 2 = 1084
		task_fini 1084 1
	endif

	if 2 = 1082
	;	DIALOG 1
	;	add_talk $_npc$:
	;	add_talk #b#b我这里有各种精灵的魂哦，都是上等的品种！
			task_fini 1082 1
	;		option 500 时间城太大了，能否送我一程？
	;	talk
	endif
	
	if 2 = 3002
		TASK_FINI 3002 0
		dialog 1
		add_talk $_npc$:
		add_talk #b#b莉莉要送你去见凯拉，你准备好了吗？
		
			option 320 #去找凯拉
			option 321 #以后再去
		talk
	endif
	if 2 = 3001
		dialog 1
		add_talk $_npc$:
		add_talk #b#b莉莉要出题了，你准备好了吗？
		
			option 301 开始答题
		talk
	endif
	if 2 = 3014
		call 201
	endif
	if 2 = 3015
		call 401
	endif	

	if 2 = 3016
	
	endif
	if 2 = 3017
	
	endif
	

endproc 
proc 201
	dialog 1
	add_talk $_npc$:
	add_talk #b#b宠物的资质有什么用？<请选择正确的答案>

		option 290 A、好看
		option 204 B、资质越高的宠物战斗力越高
		option 290 C、资质越高的宠物战斗力越低
		option 290 D、不清楚	
	talk
endproc 
proc 290
	dialog 1
	add_talk $_npc$:
	add_talk 回答错误！
	add_talk #b#b
	add_talk 正确答案是：
	add_talk #b#b#c186506资质越高的宠物战斗力越高#n
	add_talk #b#b#b
	add_talk #b#b#c0181f9资质影响宠物战斗力、各项属性成长；可在时间城的【拉奥多】处选择【宠物幻化】选项，消耗【幻化果实】来改变宠物的资质。#n

		option 201 重新回答此题
	talk
endproc

;proc 202
;	dialog 1
;	add_talk $_npc$:
;	add_talk #b#b以下哪项是宠物融合的条件？
;	add_talk #b#b<请选择正确的答案>
;
;		option 291 A、≥10级，优秀、杰出、完美资质，品种相同
;		option 291 B、≥50级，优秀、杰出、完美资质，品种不同
;		option 291 C、≥40级，资质不限，品种：不限
;		option 203 D、≥50级，优秀、杰出、完美资质，品种相同
;
;	talk
;endproc 
;proc 291
;	dialog 1
;	add_talk $_npc$:
;	add_talk 回答错误！
;	add_talk #b#b
;	add_talk 正确答案是：
;	add_talk #b#b#c186506等级≥50级，资质：优秀、杰出、完美，品种：相同#n
;	add_talk #b#b#b
;	add_talk #b#b#c0181f9在达到融合条件的前提下，选中你要与之融合的宠物，在屏幕中间的目标头像上右击，会出现融合选项#n
;
;		option 202 重新回答此题
;
;	talk
;endproc
;;;;;;;;;;;;;;;;;;;;;;;;----------------------------------------------------------
proc  202
	dialog 1
	add_talk $_npc$:
	add_talk #b#b宠物的替身率有什么用？
	add_talk #b#b<请选择正确的答案>

		option 292 A、摆设
		option 292 B、提高宠物攻击
		option 204 C、替主人承受伤害
		option 292 D、不清楚

	talk
endproc 
proc 292
	dialog 1
	add_talk $_npc$:
	add_talk 回答错误！
	add_talk #b#b
	add_talk 正确答案是：
	add_talk #b#b#c186506替主人承受伤害#n

	add_talk #b#b#b
	add_talk #b#b#c0181f9高替身率的宠物是ＰＫ必备，不同的宠物品种，替身率不尽相同，替身率不会随着宠物的等级、资质改变，可以通过宠物技能书来提高宠物的替身率#n

		option 202 重新回答此题

	talk
endproc 

;;;;;;;;;;;;;;;;;;;;;;;;----------------------------------------------------------
proc 204
	dialog 1
	add_talk $_npc$:
	add_talk #b#b宠物的天赋技能如何开启？
	add_talk #b#b<请选择正确的答案>

		option 205 A、孵化宠物时有几率获得
		option 293 B、用技能书学习
		option 293 C、升级就会获得
		option 293 D、不清楚

	talk
endproc 
proc 293
	dialog 1
	add_talk $_npc$:
	add_talk 回答错误！
	add_talk #b#b
	add_talk 正确答案是：
	add_talk #b#b#c186506孵化宠物时有几率获得#n

	add_talk #b#b#b
	add_talk #b#b#c0181f9宠物的天赋是与生俱来的，无法通过后天途径获得，只能在孵化宠物蛋时自然领悟。
		option 204 重新回答此题

	talk
endproc 

proc 205
	dialog 1
	add_talk $_npc$:
	add_talk #b#b宠物技能如何获得？
	add_talk #b#b<请选择正确的答案>

		option 296 A、大声念：俺把你来蒙
		option 206 B、升级时领悟或通过技能书领悟
		option 296 C、送去宠物乐园学习
		option 296 D、不清楚

	talk
endproc

proc 296
	dialog 1
	add_talk $_npc$:
	add_talk 回答错误！
	add_talk #b#b
	add_talk 正确答案是：
	add_talk #b#b#c186506通过升级或宠物技能书领悟#n

	add_talk #b#b#b
	add_talk #b#b#c0181f9你还可以在宠物训练员-拉奥多[时间城]对宠物技能进行重置，使宠物遗忘旧的技能，领悟新的技能。此项操作不会使技能的数量增加或减少。
	
		option 205 重新回答此题

	talk
endproc 

proc 206
	dialog 1
	add_talk $_npc$:
	add_talk #b#b以下哪项操作能够改变宠物的资质？<请选择正确的答案>

		option 295 A、宠物放生
		option 295 B、宠物强化
		option 212 C、在拉奥多处进行：宠物幻化
		option 295 D、不清楚
	talk
endproc

proc 295
	dialog 1
	add_talk $_npc$:
	add_talk 回答错误！
	add_talk #b#b
	add_talk 正确答案是：
	add_talk #b#b#c186506在拉奥多处进行：宠物幻化#n

	add_talk #b#b#b
	add_talk #b#b#c0181f9幻化的结果被【保存】才会生效，当幻化结果出现降低时，可以【继续幻化】或【取消】保存，这两种选择都不会使宠物的属性降低。
		option 206 重新回答此题

	talk
endproc 
;proc 208
;	dialog 1
;	add_talk $_npc$:
;	add_talk #b#b宠物强化过程中，使用“归元宝玉”道具，重置已强化过的星星，在未保存新的强化结果时关闭强化面板，被重置的星星会消失吗？
;
;
;		option 212 A、不会消失，随时可以接着强化
;		option 220 B、会消失，关掉面板就没了
;		option 220 C、不清楚
;	talk
;endproc
;proc 220
;	dialog 1
;	add_talk $_npc$:
;	add_talk 回答错误！
;	add_talk #b#b
;	add_talk 正确答案是：
;	add_talk #b#b#c186506不会消失，随时可以接着强化#n
;
;	add_talk #b#b#b
;	add_talk #b#b#c0181f9使用“归元宝玉”清除已强化过的星星后，这颗星星就会一直空置，直到你再次保存强化结果！并不会受到时间的限制，也不会因为关闭了强化面板而恢复重置前的强化结果！#n
;
;		option 208 重新回答此题
;
;	talk
;endproc
proc 212
	dialog 1
	add_talk $_npc$:
	add_talk #b#b以下哪项操作可使你获得更优秀的宠物？
	add_talk #b#b<请选择正确的答案>

		option 297 A、乞讨
		option 213 B、参加有宠物奖励的活动
		option 297 C、向上帝许愿
		option 297 D、不清楚
	talk
endproc
proc 213
	dialog 1
	add_talk $_npc$:
	add_talk #b#b你真的是聪明绝顶，这么快就将如何抓宠驯宠领悟的融会贯通，你可以通过宠物战斗力排行榜来查看别人的宠物，希望某一天我能在宠物战斗力排行榜首页看到你的名字！
		TASK_FINI 3014 1
		option 230 #谢谢夸奖
	talk
endproc 
proc 230
	return
endproc

proc 297
	dialog 1
	add_talk $_npc$:
	add_talk 回答错误！
	add_talk #b#b
	add_talk 正确答案是：
	add_talk #b#b#c186506参加有宠物奖励的活动#n

	add_talk #b#b#b
	add_talk #b#b#c0181f9除了在捕捉宠物外，很多活动都可以获得宠物蛋，比如【龙蛋谷】-霸王龙、【盘山道】-野人酋长、【商运声望】-基尔特商会亨利……请多留意活动助手【快捷键 H 】#n

		option 212 重新回答此题

	talk
endproc 


;;;;;;;;;;;;;;;3015


proc 401
	dialog 1
	add_talk $_npc$:
	add_talk #b#b宠物忠诚度太低，会导致宠物无法出战，用什么方法可以恢复宠物的忠诚度？
	add_talk #b#b<请选择正确的答案>

		option 421 A、放生
		option 403 B、使用可提升忠诚度的道具：驯兽糖果
		option 421 C、融合
		option 421 D、不清楚
	talk
endproc

proc 421
	dialog 1
	add_talk $_npc$:
	add_talk 回答错误！
	add_talk #b#b
	add_talk 正确答案是：
	add_talk #b#b#c186506使用可清除疲劳度的道具：驯兽道具#n

	add_talk #b#b#b
	add_talk #b#b#c0181f9你可以通过自动喂养，让宠物的忠诚度始终保持在能够出战的状态，具体步骤如下：#n
	add_talk #b#b#c0181f9第一步：打开宠物面板【快捷键 Ｘ】#n
	add_talk #b#b#c0181f9第二步：点击【自动喂养】按钮#n
	add_talk #b#b#c0181f9第三步：将背包中的驯兽糖果拖动到自动喂养界面中第3行文字后的空图标栏#n
	add_talk #b#b#c0181f9第四步：修改“忠诚度低于”后面的数字改成 80#n
	add_talk #b#b#c0181f9第五步：保存设置结果，点击【确定】按钮#n

		option 401 重新回答此题

	talk
endproc 
proc 403
	dialog 1
	add_talk $_npc$:
	add_talk #b#b去哪儿捕捉其他种类的宠物？
	add_talk #b#b<请选择正确的答案>

		option 404 A、打死怪物有几率掉落“可捕捉”的宠物
		option 422 B、秋天的时候把宠物种田里
		option 422 C、这个问题太难了
		option 422 D、不清楚
	talk
endproc

proc 422
	dialog 1
	add_talk $_npc$:
	add_talk 回答错误！
	add_talk #b#b
	add_talk 正确答案是：
	add_talk #b#b#c186506打死怪物有几率掉落“可捕捉”的宠物#n

	add_talk #b#b#b
	add_talk #b#b#c0181f9可捕捉成宠物的怪物头顶有紫色的“可捕捉”字样；怪物血量越高，捕捉成功率越低。#n

		option 403 重新回答此题

	talk
endproc 

proc 404
	dialog 1
	add_talk $_npc$:
	add_talk #b#b捕捉宠物的道具怎么获得？
	add_talk #b#b<请选择正确的答案>

		option 423 A、在心里默念“我要抓宠”
		option 423 B、怪物掉落
		option 405 #C、在莉莉的商店里购买
		option 423 D、不清楚
	talk
endproc
proc 405
	TASK_FINI 3015 1
endproc

proc 423
	dialog 1
	add_talk $_npc$:
	add_talk 回答错误！
	add_talk #b#b
	add_talk 正确答案是：
	add_talk #b#b#c186506在莉莉的商店里购买#n

	add_talk #b#b#b
	add_talk #b#b#c0181f9每次捕捉，无论成功与否，都会消耗一个道具。#n

		option 404 重新回答此题

	talk
endproc 













