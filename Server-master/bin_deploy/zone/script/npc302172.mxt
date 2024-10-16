;npc 朗才

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b锦里开芳宴，兰缸艳早年
	add_talk #b#b褥彩遥分地，繁光远缀天
	add_talk #b#b接汉疑名霎，依楼似月悬
	add_talk #b#b别有千金笑，来映九枝前
	add_talk #b#b
	add_talk #b#b欢迎来参加中秋灯会，灯谜基本上都很简单，只要稍微琢磨一下就能够猜到答案，即使乱猜也有四分之一的机会猜对！
	add_talk #b#b
	add_talk #b#b#c186506友情提示：活动期间，可每天可在月饼大使处领取月饼礼盒。#n
	
	;task_stat 4630 80 90
	;if @80 = 0 and @90 = 0
		GET_PLAYER_BASE_ATT 13 0 12 0
		if @13 = 0 and @12 >= 40
			;option 200 #提交月饼—获得灯谜
			option 206 我有多余的冰皮月饼
		endif
	;endif
	
	strvar ret num
	task_stat 4630 1 ret
	if Ret = 2
		rand num 20
		switch num
			case 0
				option 400 开始猜谜
			endcase
			case 1
				option 401 开始猜谜
			endcase
			case 2
				option 402 开始猜谜
			endcase
			case 3
				option 403 开始猜谜
			endcase
			case 4
				option 404 开始猜谜
			endcase
			case 5
				option 405 开始猜谜
			endcase
			case 6
				option 406 开始猜谜
			endcase
			case 7
				option 407 开始猜谜
			endcase
			case 8
				option 408 开始猜谜
			endcase
			case 9
				option 409 开始猜谜
			endcase
			case 10
				option 410 开始猜谜
			endcase
			case 11
				option 411 开始猜谜
			endcase
			case 12
				option 412 开始猜谜
			endcase
			case 13
				option 413 开始猜谜
			endcase
			case 14
				option 414 开始猜谜
			endcase
			case 15
				option 415 开始猜谜
			endcase
			case 16
				option 416 开始猜谜
			endcase
			case 17
				option 417 开始猜谜
			endcase
			case 18
				option 418 开始猜谜
			endcase
			case 19
				option 419 开始猜谜
			endcase
			case 20
				option 420 开始猜谜
			endcase
		endswitch
	endif
		
		task_stat 4630 60 70
		if @60 = 0 and @70 = 1
			TASK_ADD_OPTION 4630 0 201
		endif
	talk
endproc 

proc 201
	TASK_DO_OPTION 4630 0
endproc

proc 200
	dialog 3
	add_talk $_npc$:
	add_talk #b#b今生只爱冰皮月饼！
	add_talk #b#b
	add_talk #b#b将需要提交的物品拖至下方的框内
		
	NPC_COMMIT_OPT 300
	talk
endproc

proc 300
	strvar itemId pos index ret 
	GET_COMMIT_ITEM_INFO 1 itemId
	if itemId = -1
		add_sys_msg_start
		add_sys_msg 您还没有提交物品
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	if @itemId != 5012131
		add_sys_msg_start
		add_sys_msg 虽然这世上月饼众多，但我只取冰皮月饼这一瓢饮！
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	GET_COMMIT_ITEM_INFO 2 pos
	GET_COMMIT_ITEM_INFO 3 index

	DEC_ITEM_NUM_BYIDX @itemId 1 @pos @index ret
	if ret = 0
		ADD_BIND_ITEM_NUM_INPAK 5030110 1 1 13
		add_sys_msg_start
		add_sys_msg 朗才将一张写着灯谜的小纸条塞到了你手中。
		set_sys_msg_mode 3 0 0
		msg_info
	endif
	PLAYER_CLOSE_DIALOG 0 0
endproc

proc 206
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b 听说你有多余的冰皮月饼，真是太好了，你愿意和我换吗？我这里有好玩的甜肉月饼，好吃的纳凉月饼，热闹的火爆月饼，美丽的欢喜月饼，总有一样你会喜欢的，对吧？想要什么自己拿吧！
	
	GET_PLAYER_BASE_ATT 13 0 12 0
	if @13 = 0 and @12 >= 40
		option 202 冰皮月饼换甜肉月饼
		option 203 冰皮月饼换纳凉月饼
		option 204 冰皮月饼换火爆月饼
		option 205 冰皮月饼换欢喜月饼
	endif
	
	talk
endproc

;冰皮月饼换甜肉月饼
proc 202
	strvar ret1
	GET_PACK_SPACE 0 ret1
	if @ret1 = 0
		add_sys_msg_start
		add_sys_msg 背包空间不足，无法换取
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	GET_ITEM_NUM_INPAK 5012131 13 15
	if @13 = 0 and @15 >= 1
		DEC_ITEM_NUM_INPAK 5012131 1 14
		if @14 = 0
			ADD_BIND_ITEM_NUM_INPAK 5012130 1 1 13
		endif
	else
		add_sys_msg_start
		add_sys_msg 背包中没有冰皮月饼！
		set_sys_msg_mode 3 0 0
		msg_info
	endif
	call 206
endproc

;冰皮月饼换纳凉月饼
proc 203
	strvar ret1
	GET_PACK_SPACE 0 ret1
	if @ret1 = 0
		add_sys_msg_start
		add_sys_msg 背包空间不足，无法换取
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	GET_ITEM_NUM_INPAK 5012131 13 15
	if @13 = 0 and @15 >= 1
		DEC_ITEM_NUM_INPAK 5012131 1 14
		if @14 = 0
			ADD_BIND_ITEM_NUM_INPAK 5012132 1 1 13
		endif
	else
		add_sys_msg_start
		add_sys_msg 背包中没有冰皮月饼！
		set_sys_msg_mode 3 0 0
		msg_info
	endif
	call 206
endproc

;冰皮月饼换火爆月饼
proc 204
	strvar ret1
	GET_PACK_SPACE 0 ret1
	if @ret1 = 0
		add_sys_msg_start
		add_sys_msg 背包空间不足，无法换取
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	GET_ITEM_NUM_INPAK 5012131 13 15
	if @13 = 0 and @15 >= 1
		DEC_ITEM_NUM_INPAK 5012131 1 14
		if @14 = 0
			ADD_BIND_ITEM_NUM_INPAK 5012134 1 1 13
		endif
	else
		add_sys_msg_start
		add_sys_msg 背包中没有冰皮月饼！
		set_sys_msg_mode 3 0 0
		msg_info
	endif
	call 206
endproc

;冰皮月饼换欢喜月饼
proc 205
	strvar ret1
	GET_PACK_SPACE 0 ret1
	if @ret1 = 0
		add_sys_msg_start
		add_sys_msg 背包空间不足，无法换取
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	GET_ITEM_NUM_INPAK 5012131 13 15
	if @13 = 0 and @15 >= 1
		DEC_ITEM_NUM_INPAK 5012131 1 14
		if @14 = 0
			ADD_BIND_ITEM_NUM_INPAK 5012135 1 1 13
		endif
	else
		add_sys_msg_start
		add_sys_msg 背包中没有冰皮月饼！
		set_sys_msg_mode 3 0 0
		msg_info
	endif
	call 206
endproc

proc 400
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b嫦娥下凡
	add_talk #b#b（打一花名）
	
		option 500 #月季
		option 501 #牡丹
		option 501 #桂花
		option 501 #芙蓉
		
	talk
endproc

proc 401
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b明月照我还
	add_talk #b#b（打一人名）
	
		option 501 #陆有亮
		option 500 #归有光
		option 501 #孔明月
		option 501 #秦天明
		
	talk
endproc

proc 402
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b举杯邀明月
	add_talk #b#b（打一礼貌用语）
	
		option 501 #关照
		option 501 #借光
		option 500 #赏光
		option 501 #光临
		
	talk
endproc

proc 403
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b夜夜看落花
	add_talk #b#b（打一礼貌用语）
	
		option 501 #惠存
		option 501 #久仰
		option 501 #高见
		option 500 #多谢
		
	talk
endproc

proc 404
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b十五的月亮
	add_talk #b#b（打一成语）
	
		option 501 #不明不白
		option 501 #黑白分明
		option 500 #正大光明
		option 501 #明心见性
		
	talk
endproc

proc 405
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b女大十八变，越变越好看
	add_talk #b#b（打一成语）
	
		option 501 #变化无穷
		option 500 #成人之美
		option 501 #千变万化
		option 501 #美轮美奂
		
	talk
endproc

proc 406
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b在家日子好过
	add_talk #b#b（打一人事用语）
	
		option 500 #出差
		option 501 #下班
		option 501 #转业
		option 501 #闲聊
		
	talk
endproc

proc 407
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b早不说晚不说
	add_talk #b#b（打一字）
	
		option 501 #中
		option 500 #许
		option 501 #乐
		option 501 #况
		
	talk
endproc

proc 408
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b重逢
	add_talk #b#b（打一字）
	
		option 501 #桑
		option 501 #念
		option 500 #观
		option 501 #千
		
	talk
endproc

proc 409
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b语言不生动
	add_talk #b#b（打一字）
	
		option 501 #道
		option 501 #画
		option 501 #音
		option 500 #评
		
	talk
endproc

proc 410
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b同心改革为祖国
	add_talk #b#b（打一字）
	
		option 501 #明
		option 501 #倡
		option 500 #中
		option 501 #香
		
	talk
endproc

proc 411
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b一弯月照枝头亮，两颗星悬天下明
	add_talk #b#b（打一字）
	
		option 501 #甬
		option 500 #秋
		option 501 #新
		option 501 #意
		
	talk
endproc

proc 412
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b心已决断不宜迟
	add_talk #b#b（打一字）
	
		option 500 #快
		option 501 #忌
		option 501 #谊
		option 501 #尽
		
	talk
endproc

proc 413
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b休把旁人抬太高
	add_talk #b#b（打一字）
	
		option 501 #自
		option 500 #乐
		option 501 #尹
		option 501 #荷
		
	talk
endproc

proc 414
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b天秋月又满
	add_talk #b#b（打一水果）
	
		option 501 #葡萄
		option 501 #蜜桃
		option 500 #桂圆
		option 501 #橙子
		
	talk
endproc

proc 415
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b驼背公公，力大无穷，爱驮东西，车水马龙
	add_talk #b#b（打一物）
	
		option 501 #猴子
		option 501 #骆驼
		option 501 #袋鼠
		option 500 #桥
		
	talk
endproc

proc 416
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b头戴大红花，身穿什锦衣，好象当家人，一早催人起
	add_talk #b#b（打一动物）
	
		option 501 #新郎倌
		option 501 #周扒皮
		option 500 #公鸡
		option 501 #芙火鸡
		
	talk
endproc

proc 417
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b小货郎，不挑担，背着针，到处窜
	add_talk #b#b（打一动物）
	
		option 501 #老鼠
		option 500 #刺猬
		option 501 #野猪
		option 501 #河马
		
	talk
endproc

proc 418
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b像条带，一盘菜，下了水，跑得快
	add_talk #b#b（打一动物）
	
		option 500 #带鱼
		option 501 #黄缮
		option 501 #水蛇
		option 501 #巨蟒
		
	talk
endproc

proc 419
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b小小诸葛亮，独从中军帐，摆下八卦阵，专捉飞来将
	add_talk #b#b（打一动物）
	
		option 501 #壁虎
		option 500 #蜘蛛
		option 501 #蟾蜍
		option 501 #青蛙
		
	talk
endproc

proc 420
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b两叶花四朵，颜色白又黄，一年开一次，八月放异香
	add_talk #b#b（打一植物）
	
		option 501 #豆花
		option 501 #莲花
		option 500 #桂花
		option 501 #校花
		
	talk
endproc
proc 500
	TASK_FINI 4630 0
	
	strvar npcID
	GET_MAP_NPC_ID 302172 npcID 0
	mon_speech @npcID 2 0 1 恭喜你，答对了！
	
endproc

proc 501
	COMMIT_TASK 4630 0
	
	add_sys_msg_start 
	add_sys_msg 未能猜出真正的谜底，无法获得奖励！
	set_sys_msg_mode 4 0 0
	msg_info
	
	strvar npcID
	GET_MAP_NPC_ID 302172 npcID 0
	mon_speech @npcID 2 0 1 不对不对！
endproc
