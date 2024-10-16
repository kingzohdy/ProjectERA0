;npc拼酒
;4501 任务ID
;304134 NPCID

proc 1
	GET_TASK_VAR 4501 4 32 12 res
	;消耗绑银
	if @res = 1 and @12 = 0
		choose 18 207 208 你是来蹭酒喝的吧？除了第一次拼酒以外，其他的每次都要消耗2000绑银。点击确定开始拼酒！
		return
	endif
	
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b没有人想和差劲的家伙交朋友！尤其是胆量，其次是酒量！
	
	TASK_STAT 4501 1 70
	if @70 = 2
		option 206 开始拼酒
	endif
	
	TASK_STAT 4573 1 71
	if @71 = 2
		option 210 开始答题
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		option 220 开始答题
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		option 230 开始答题
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		option 240 开始答题
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		option 250 开始答题
	endif
		;TASK_ADD_OPTION 4501 0 103
		;TASK_ADD_OPTION 4517 0 104
		;TASK_ADD_OPTION 4573 0 105
		;TASK_ADD_OPTION 4574 0 106
		;TASK_ADD_OPTION 4575 0 107
		;TASK_ADD_OPTION 4576 0 108
		;TASK_ADD_OPTION 4577 0 109
	talk
endproc


proc 2
	get_trig_task 1 2

	if 2 = 4501
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b没有人想和差劲的家伙交朋友！尤其是胆量，其次是酒量！

		option 206 开始拼酒
		talk
	endif
	
	if 2 = 4573
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b没有人想和差劲的家伙交朋友！尤其是胆量，其次是酒量！

		option 210 开始答题
		talk
	endif
	
	if 2 = 4574
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b没有人想和差劲的家伙交朋友！尤其是胆量，其次是酒量！

		option 220 开始答题
		talk
	endif
	
	if 2 = 4575
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b没有人想和差劲的家伙交朋友！尤其是胆量，其次是酒量！

		option 230 开始答题
		talk
	endif
	
	if 2 = 4576
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b没有人想和差劲的家伙交朋友！尤其是胆量，其次是酒量！

		option 240 开始答题
		talk
	endif
	
	if 2 = 4577
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b没有人想和差劲的家伙交朋友！尤其是胆量，其次是酒量！

		option 250 开始答题
		talk
	endif
endproc

;proc 103
;	TASK_DO_OPTION 4501 0
;endproc

;proc 104
;	TASK_DO_OPTION 4517 0
;endproc

;proc 105
;	TASK_DO_OPTION 4573 0
;endproc

;proc 106
;	TASK_DO_OPTION 4574 0
;endproc

;proc 107
;	TASK_DO_OPTION 4575 0
;endproc

;proc 108
;	TASK_DO_OPTION 4576 0
;endproc

;proc 109
;	TASK_DO_OPTION 4577 0
;endproc

proc 100
	add_sys_msg_start
	add_sys_msg 你已经喝醉了
	set_sys_msg_mode 4 0 0
	msg_info
	
	;通知客户端进行屏幕模糊
	pinjiu_screen_vague 1
	
	PLAYER_CLOSE_DIALOG1 0 1
	
	strvar npcmid
	GET_MAP_NPC_ID  304134 npcmid 1
	MON_SPEECH  @npcmid 0 2 1 小样，跟我拼酒，找虐呢！
	
	SET_TASK_VAR 4501 0 32 60 10
	SET_TASK_VAR 4501 4 32 1 16
	SET_TASK_VAR 4501 8 32 0 26
	SET_TASK_VAR 4501 12 32 0 27
	SET_TASK_VAR 4501 16 32 0 28
endproc

;玩家赢了
proc 101
	add_sys_msg_start
	add_sys_msg $_npc$已经醉的不省人事了！你赢了！
	set_sys_msg_mode 4 0 0
	msg_info
	
	task_fini 4501 1
endproc

;玩家输了
proc 102
	add_sys_msg_start
	add_sys_msg 乌姆可是出了名的千杯不醉呢，这才喝到哪啊，看来你太小看对手了！
	set_sys_msg_mode 4 0 0
	msg_info
	
	SET_TASK_VAR 4501 0 32 60 10
	SET_TASK_VAR 4501 4 32 1 16
	SET_TASK_VAR 4501 8 32 0 26
	SET_TASK_VAR 4501 12 32 0 27
	SET_TASK_VAR 4501 16 32 0 28
endproc

;再来一杯
proc 201
	;每次点击“再来一杯”只有 x% 会变，每次点数值随机 1-12（最初是60%）
	rand 20 12
	add 20 1
	
	GET_TASK_VAR 4501 0 32 11 num
	add num @20
	SET_TASK_VAR 4501 0 32 @num 10
	
	if @num > 100
		call 100
	else
		call 206
	endif
endproc

;不喝了，来看看结果吧！
proc 202
	strvar jiuliangval
	;随机酒量值
	GET_TASK_VAR 4501 16 32 25 jiuliangval
	GET_TASK_VAR 4501 0 32 11 num	
	if @jiuliangval < @num and @num <= 100
	;玩家赢了
		call 101
	else
	;玩家输了
		call 102
	endif
endproc


proc 206

	strvar num res jiuliang flag
	GET_TASK_VAR 4501 0 32 11 num
	if @num = 0
		SET_TASK_VAR 4501 0 32 60 10
		var num 60
	endif
	
	;idx 12 记录npc随机酒量标志   flag 为 1时，表示已经取得  ,0时，则马上取
	GET_TASK_VAR 4501 12 32 21 flag 
	if @flag = 0
		;npc酒量  90-97
		rand 14 8
		add 14 90
		
		if @14 < 95 and @14 >= 90
			;乌姆的情绪不高，看样子似乎很容易就会喝醉
			SET_TASK_VAR 4501 8 32 1 10
		elseif @14 >= 95 and @14 <= 97
			;乌姆酒兴大增，越喝越来劲，看样子似乎能够千杯不醉
			SET_TASK_VAR 4501 8 32 2 10
		else
			SET_TASK_VAR 4501 8 32 0 10
		endif
		
		SET_TASK_VAR 4501 12 32 1 22
		;idx 为 16 设置酒量值
		SET_TASK_VAR 4501 16 32 @14 23
	endif
	
	
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b一杯酒下肚，你已经有$@num$%的醉意，还要继续喝吗？
	
	;idx 8 该显示哪句   
	GET_TASK_VAR 4501 8 32 15 jiuliang
	if @jiuliang = 1
		add_talk #b#b乌姆的情绪不高，看样子似乎很容易就会喝醉
	elseif @jiuliang = 2
		add_talk #b#b乌姆酒兴大增，越喝越来劲，看样子似乎能够千杯不醉
	else
	endif
	
	option 201 再来一杯
	option 202 #不喝了，来看看结果吧！
	
	talk
endproc

proc 207
	DEC_MONEY_TEST 0 2000 21 15
	if 15 = 0
		DEC_MONEY 0 2000 21 12
		if 12 = 0		
			SET_TASK_VAR 4501 4 32 0 16
			
			call 1
		else
			add_sys_msg_start
			add_sys_msg 您的绑银不足。
			set_sys_msg_mode 4 0 0
			msg_info
			return
		endif
	endif			
endproc

proc 208
	return
endproc



;选择题
proc 200
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;答对次数
	GET_TASK_VAR @430 0 32 1 80

	if @80 >= 3
		TASK_FINI @430 1
		PLAYER_CLOSE_DIALOG1 0 0
	else
		GET_TASK_VAR @430 4 32 1 90
		if @90 = 1
			;答对次数清0
			SET_TASK_VAR @430 0 32 0 1
			;标记清0
			SET_TASK_VAR @430 4 32 0 1
		endif
	
		;20道题
		rand 12 20
		switch 12
			case 0
				call 300
			endcase

			case 1
				call 301
			endcase

			case 2
				call 302
			endcase
					
			case 3
				call 303
			endcase
					
			case 4
				call 304
			endcase
					
			case 5
				call 305
			endcase
					
			case 6
				call 306
			endcase
					
			case 7
				call 307
			endcase
					
			case 8
				call 308
			endcase
					
			case 9
				call 309
			endcase

			case 10
				call 310
			endcase
					
			case 11
				call 311
			endcase
					
			case 12
				call 312
			endcase
					
			case 13
				call 313
			endcase
			
			case 14
				call 314
			endcase
			
			case 15
				call 315
			endcase
			
			case 16
				call 316
			endcase
			
			case 17
				call 317
			endcase
			
			case 18
				call 318
			endcase
			
			case 19
				call 319
			endcase
		endswitch
	endif
endproc

proc 210 
	call 200
endproc

proc 220 
	call 200
endproc

proc 230 
	call 200
endproc

proc 240 
	call 200
endproc

proc 250
	call 200
endproc



;选项偶数的为错误的，奇数401为正确的
proc 300
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b请仔细阅读以下题目，并认真作答，如果答错一题，将重新开始答题，如果答对三道，则完成任务！
	add_talk #b#b
	add_talk #b#b#c186506看到世界频道有人在低价叫卖东西，并且还要加聊天工具的，你怎么办？#n
	
	rand 12 4
	if @12 = 0
		option 400 果断加之，有便宜不占是笨蛋
		option 401 点击小地图右上角的BUG反馈举报他
		option 400 赶紧联系，索要电话，发展成长期合作关系
		option 400 问问能不能再便宜点
	elseif @12 = 1
		option 401 点击小地图右上角的BUG反馈举报他
		option 400 赶紧联系，索要电话，发展成长期合作关系
		option 400 问问能不能再便宜点
		option 400 果断加之，有便宜不占是笨蛋
	elseif @12 = 2
		option 400 赶紧联系，索要电话，发展成长期合作关系
		option 400 问问能不能再便宜点
		option 400 果断加之，有便宜不占是笨蛋
		option 401 点击小地图右上角的BUG反馈举报他
	else
		option 400 问问能不能再便宜点
		option 400 果断加之，有便宜不占是笨蛋
		option 401 点击小地图右上角的BUG反馈举报他
		option 400 赶紧联系，索要电话，发展成长期合作关系
	endif
	talk
endproc

proc 301
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b请仔细阅读以下题目，并认真作答，如果答错一题，将重新开始答题，如果答对三道，则完成任务！
	add_talk #b#b
	add_talk #b#b#c186506如果有人跟你说登陆某非官方网站能够领取大量游戏道具、金币、银币、经验、宠物，你会怎么做？#n
	
	rand 12 4
	if @12 = 0
	option 402 先问问他能不能多给一些
	option 401 点击小地图右上角的BUG反馈举报他
	option 402 截图，并群发告之好友，动员好友一起去领
	option 402 果断去领
	elseif @12 = 1
	option 401 点击小地图右上角的BUG反馈举报他
	option 402 截图，并群发告之好友，动员好友一起去领
	option 402 果断去领
	option 402 先问问他能不能多给一些
	elseif @12 = 2
	option 402 截图，并群发告之好友，动员好友一起去领
	option 402 果断去领
	option 402 先问问他能不能多给一些
	option 401 点击小地图右上角的BUG反馈举报他
	else
	option 402 果断去领
	option 402 先问问他能不能多给一些
	option 401 点击小地图右上角的BUG反馈举报他
	option 402 截图，并群发告之好友，动员好友一起去领
	endif
	talk
endproc

proc 302
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b请仔细阅读以下题目，并认真作答，如果答错一题，将重新开始答题，如果答对三道，则完成任务！
	add_talk #b#b
	add_talk #b#b#c186506发现账号被盗后怎么办？#n
	
	rand 12 4
	if @12 = 0
	option 404 重新建一个号，并在世界上大骂盗号贼
	option 404 盗就盗了，无所谓
	option 401 绑定安全邮箱，及时认证，绑定密保卡
	option 404 设法去盗别人的号
	elseif @12 = 1
	option 404 盗就盗了，无所谓
	option 401 绑定安全邮箱，及时认证，绑定密保卡
	option 404 设法去盗别人的号
	option 404 重新建一个号，并在世界上大骂盗号贼
	elseif @12 = 2
	option 401 绑定安全邮箱，及时认证，绑定密保卡
	option 404 设法去盗别人的号
	option 404 重新建一个号，并在世界上大骂盗号贼
	option 404 盗就盗了，无所谓
	else
	option 404 设法去盗别人的号
	option 404 重新建一个号，并在世界上大骂盗号贼
	option 404 盗就盗了，无所谓
	option 401 绑定安全邮箱，及时认证，绑定密保卡
	endif
	talk
endproc

proc 303
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b请仔细阅读以下题目，并认真作答，如果答错一题，将重新开始答题，如果答对三道，则完成任务！
	add_talk #b#b
	add_talk #b#b#c186506如果发现被盗后账号密码无法取回或被恶意绑定密保卡，怎么办？#n
	
	rand 12 4
	if @12 = 0
	option 406 一个一个试，总有一个是对的
	option 406 反正也不值几个钱，重新再练一个号
	option 406 被恶意绑定了我也没办法
	option 401 到论坛“问题反馈区”发帖向GM求助
	elseif @12 = 1	
	option 406 反正也不值几个钱，重新再练一个号
	option 406 被恶意绑定了我也没办法
	option 401 到论坛“问题反馈区”发帖向GM求助
	option 406 一个一个试，总有一个是对的
	elseif @12 = 2
	option 406 被恶意绑定了我也没办法
	option 401 到论坛“问题反馈区”发帖向GM求助
	option 406 一个一个试，总有一个是对的
	option 406 反正也不值几个钱，重新再练一个号
	else
	option 401 到论坛“问题反馈区”发帖向GM求助
	option 406 一个一个试，总有一个是对的
	option 406 反正也不值几个钱，重新再练一个号
	option 406 被恶意绑定了我也没办法
	endif
	talk
endproc

proc 304
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b请仔细阅读以下题目，并认真作答，如果答错一题，将重新开始答题，如果答对三道，则完成任务！
	add_talk #b#b
	add_talk #b#b#c186506零纪元的官网是哪个？#n	
	
	rand 12 4
	if @12 = 0
	option 401 www.ljy0.com
	option 408 www.ljy1.com
	option 408 www.ljy2.com
	option 408 www.ljy3.com
	elseif @12 = 1
	option 408 www.ljy1.com
	option 408 www.ljy2.com
	option 408 www.ljy3.com
	option 401 www.ljy0.com
	elseif @12 = 2
	option 408 www.ljy2.com
	option 408 www.ljy3.com
	option 401 www.ljy0.com
	option 408 www.ljy1.com
	else
	option 408 www.ljy3.com
	option 401 www.ljy0.com
	option 408 www.ljy1.com
	option 408 www.ljy2.com
	endif
	talk
endproc

proc 305
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b请仔细阅读以下题目，并认真作答，如果答错一题，将重新开始答题，如果答对三道，则完成任务！
	add_talk #b#b
	add_talk #b#b#c186506零纪元官网上有哪些内容？#n
	
	rand 12 4
	if @12 = 0
	option 410 世界新闻
	option 401 礼包活动、福利活动、最新活动、游戏资料、攻略
	option 410 比cctv还全面的新闻信息，比娱乐网站还多的视频
	option 410 策划生活照
	elseif @12 = 1
	option 401 礼包活动、福利活动、最新活动、游戏资料、攻略
	option 410 比cctv还全面的新闻信息，比娱乐网站还多的视频
	option 410 策划生活照
	option 410 世界新闻
	elseif @12 = 2
	option 410 比cctv还全面的新闻信息，比娱乐网站还多的视频
	option 410 策划生活照
	option 410 世界新闻
	option 401 礼包活动、福利活动、最新活动、游戏资料、攻略
	else
	option 410 策划生活照
	option 410 世界新闻
	option 401 礼包活动、福利活动、最新活动、游戏资料、攻略
	option 410 比cctv还全面的新闻信息，比娱乐网站还多的视频
	endif
	talk
endproc

proc 306
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b请仔细阅读以下题目，并认真作答，如果答错一题，将重新开始答题，如果答对三道，则完成任务！
	add_talk #b#b
	add_talk #b#b#c186506游戏里有人说赠送点卡、金币，只要把号告诉他就可以，你会怎么做呢？#n
	
	rand 12 4
	if @12 = 0
	option 412 直接给他，这么好的事百年难得一遇啊，当然要赶紧
	option 412 把朋友的号一起给他
	option 412 考虑一下
	option 401 记下ID到论坛“问题反馈区”去举报他！
	elseif @12 = 1
	option 412 把朋友的号一起给他
	option 412 考虑一下
	option 401 记下ID到论坛“问题反馈区”去举报他！
	option 412 直接给他，这么好的事百年难得一遇啊，当然要赶紧
	elseif @12 = 2
	option 412 考虑一下
	option 401 记下ID到论坛“问题反馈区”去举报他！
	option 412 直接给他，这么好的事百年难得一遇啊，当然要赶紧
	option 412 把朋友的号一起给他
	else
	option 401 记下ID到论坛“问题反馈区”去举报他！
	option 412 直接给他，这么好的事百年难得一遇啊，当然要赶紧
	option 412 把朋友的号一起给他
	option 412 考虑一下
	endif
	talk
endproc

proc 307
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b请仔细阅读以下题目，并认真作答，如果答错一题，将重新开始答题，如果答对三道，则完成任务！
	add_talk #b#b
	add_talk #b#b#c186506零纪元“账号管理”的官网地址是？#n
	
	rand 12 4
	if @12 = 0
	option 401 http://passport.wanyouyl.com
	option 414 http://www.ljy0.com
	option 414 http://bbs.ljy0.com
	option 414 http://pay.wanyouyl.com
	elseif @12 = 1
	option 414 http://www.ljy0.com
	option 414 http://bbs.ljy0.com
	option 414 http://pay.wanyouyl.com
	option 401 http://passport.wanyouyl.com
	elseif @12 = 2
	option 414 http://bbs.ljy0.com
	option 414 http://pay.wanyouyl.com
	option 401 http://passport.wanyouyl.com
	option 414 http://www.ljy0.com
	else
	option 414 http://pay.wanyouyl.com
	option 401 http://passport.wanyouyl.com
	option 414 http://www.ljy0.com
	option 414 http://bbs.ljy0.com
	endif
	talk
endproc

proc 308
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b请仔细阅读以下题目，并认真作答，如果答错一题，将重新开始答题，如果答对三道，则完成任务！
	add_talk #b#b
	add_talk #b#b#c186506以下列出的各种途径中哪里能了解到零纪元最新的活动动态？#n
	
	rand 12 4
	if @12 = 0
	option 401 零纪元官网：http://www.ljy0.com
	option 401 零纪元官方论坛：http://bbs.ljy0.com
	option 401 各大正规媒体网站（如17173、新浪、多玩、网易）
	option 416 游戏内世界频道发的收购XX礼包的信息
	elseif @12 = 1
	option 401 零纪元官方论坛：http://bbs.ljy0.com
	option 401 各大正规媒体网站（如17173、新浪、多玩、网易）
	option 416 游戏内世界频道发的收购XX礼包的信息
	option 401 零纪元官网：http://www.ljy0.com
	elseif @12 = 2
	option 401 各大正规媒体网站（如17173、新浪、多玩、网易）
	option 416 游戏内世界频道发的收购XX礼包的信息
	option 401 零纪元官网：http://www.ljy0.com
	option 401 零纪元官方论坛：http://bbs.ljy0.com
	else
	option 416 游戏内世界频道发的收购XX礼包的信息
	option 401 零纪元官网：http://www.ljy0.com
	option 401 零纪元官方论坛：http://bbs.ljy0.com
	option 401 各大正规媒体网站（如17173、新浪、多玩、网易）
	endif
	talk
endproc

proc 309
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b请仔细阅读以下题目，并认真作答，如果答错一题，将重新开始答题，如果答对三道，则完成任务！
	add_talk #b#b
	add_talk #b#b#c186506游戏中自己的朋友突然向你索要金币、银币、装备，你会怎么做？#n
	
	rand 12 4
	if @12 = 0
	option 418 直接给他
	option 418 难得好友开中，不太好驳他面子，二话不说直接给
	option 401 仔细询问，通过其他方式（电话）联系，确认清楚
	option 418 直接下线
	elseif @12 = 1
	option 418 难得好友开中，不太好驳他面子，二话不说直接给
	option 401 仔细询问，通过其他方式（电话）联系，确认清楚
	option 418 直接下线
	option 418 直接给他
	elseif @12 = 2
	option 401 仔细询问，通过其他方式（电话）联系，确认清楚
	option 418 直接下线
	option 418 直接给他
	option 418 难得好友开中，不太好驳他面子，二话不说直接给
	else
	option 418 直接下线
	option 418 直接给他
	option 418 难得好友开中，不太好驳他面子，二话不说直接给
	option 401 仔细询问，通过其他方式（电话）联系，确认清楚
	endif
	talk
endproc

proc 310
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b请仔细阅读以下题目，并认真作答，如果答错一题，将重新开始答题，如果答对三道，则完成任务！
	add_talk #b#b
	add_talk #b#b#c186506有人在游戏里加你好友，然后告诉你去XXX网站领取金币、宝宝、地狱骑士坐骑，你会怎么做？#n
	
	rand 12 4
	if @12 = 0
	option 420 哇，这么好的事，当然是赶紧去领了！
	option 420 把账号密码告诉他，让他帮忙去领！
	option 401 骗子！记下ID去论坛“问题反馈区”举报他
	option 420 立即转发给好友，呼朋唤友叫公会一起去领
	elseif @12 = 1
	option 420 把账号密码告诉他，让他帮忙去领！
	option 401 骗子！记下ID去论坛“问题反馈区”举报他
	option 420 立即转发给好友，呼朋唤友叫公会一起去领
	option 420 哇，这么好的事，当然是赶紧去领了！
	elseif @12 = 2
	option 401 骗子！记下ID去论坛“问题反馈区”举报他
	option 420 立即转发给好友，呼朋唤友叫公会一起去领
	option 420 哇，这么好的事，当然是赶紧去领了！
	option 420 把账号密码告诉他，让他帮忙去领！
	else
	option 420 立即转发给好友，呼朋唤友叫公会一起去领
	option 420 哇，这么好的事，当然是赶紧去领了！
	option 420 把账号密码告诉他，让他帮忙去领！
	option 401 骗子！记下ID去论坛“问题反馈区”举报他
	endif
	talk
endproc

proc 311
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b请仔细阅读以下题目，并认真作答，如果答错一题，将重新开始答题，如果答对三道，则完成任务！
	add_talk #b#b
	add_talk #b#b#c186506关于龙蛋谷，哪项说法是正确的？#n
	
	rand 12 4
	if @12 = 0
	option 422 我自己去，独吞所有经验
	option 422 不组小号，小号会拉低经验
	option 401 动态副本的经验只跟自己的等级有关
	option 422 使用副本中的“火焰刀”“手雷”会减少经验
	elseif @12 = 1
	option 422 不组小号，小号会拉低经验
	option 401 动态副本的经验只跟自己的等级有关
	option 422 使用副本中的“火焰刀”“手雷”会减少经验
	option 422 我自己去，独吞所有经验
	elseif @12 = 2
	option 401 动态副本的经验只跟自己的等级有关
	option 422 使用副本中的“火焰刀”“手雷”会减少经验
	option 422 我自己去，独吞所有经验
	option 422 不组小号，小号会拉低经验
	else
	option 422 使用副本中的“火焰刀”“手雷”会减少经验
	option 422 我自己去，独吞所有经验
	option 422 不组小号，小号会拉低经验
	option 401 动态副本的经验只跟自己的等级有关
	endif
	talk
endproc

proc 312
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b请仔细阅读以下题目，并认真作答，如果答错一题，将重新开始答题，如果答对三道，则完成任务！
	add_talk #b#b
	add_talk #b#b#c18650664级面饰进阶的材料叫什么名字？#n
	
	rand 12 4
	if @12 = 0
	option 424 炽铜微粒、炽铜碎块、炽铜合金
	option 424 坚铜微粒、坚铜碎块、坚铜合金
	option 401 松珀碎块、松珀合金
	option 424 黑耀微粒、黑耀碎块、黑耀合金
	elseif @12 = 1	
	option 424 坚铜微粒、坚铜碎块、坚铜合金
	option 401 松珀碎块、松珀合金
	option 424 黑耀微粒、黑耀碎块、黑耀合金
	option 424 炽铜微粒、炽铜碎块、炽铜合金
	elseif @12 = 2	
	option 401 松珀碎块、松珀合金
	option 424 黑耀微粒、黑耀碎块、黑耀合金
	option 424 炽铜微粒、炽铜碎块、炽铜合金
	option 424 坚铜微粒、坚铜碎块、坚铜合金
	else
	option 424 黑耀微粒、黑耀碎块、黑耀合金
	option 424 炽铜微粒、炽铜碎块、炽铜合金
	option 424 坚铜微粒、坚铜碎块、坚铜合金
	option 401 松珀碎块、松珀合金
	endif
	talk
endproc

proc 313
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b请仔细阅读以下题目，并认真作答，如果答错一题，将重新开始答题，如果答对三道，则完成任务！
	add_talk #b#b
	add_talk #b#b#c18650669项链进阶的材料叫什么名字？#n
	
	rand 12 4
	if @12 = 0
	option 426 炽铜微粒、炽铜碎块、炽铜合金
	option 426 坚铜微粒、坚铜碎块、坚铜合金
	option 401 松珀碎块、松珀合金
	option 426 黑耀微粒、黑耀碎块、黑耀合金
	elseif @12 = 1
	option 426 坚铜微粒、坚铜碎块、坚铜合金
	option 401 松珀碎块、松珀合金
	option 426 黑耀微粒、黑耀碎块、黑耀合金
	option 426 炽铜微粒、炽铜碎块、炽铜合金
	elseif @12 = 2	
	option 401 松珀碎块、松珀合金
	option 426 黑耀微粒、黑耀碎块、黑耀合金
	option 426 炽铜微粒、炽铜碎块、炽铜合金
	option 426 坚铜微粒、坚铜碎块、坚铜合金
	else
	option 426 黑耀微粒、黑耀碎块、黑耀合金
	option 426 炽铜微粒、炽铜碎块、炽铜合金
	option 426 坚铜微粒、坚铜碎块、坚铜合金
	option 401 松珀碎块、松珀合金
	endif
	talk
endproc

proc 314
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b请仔细阅读以下题目，并认真作答，如果答错一题，将重新开始答题，如果答对三道，则完成任务！
	add_talk #b#b
	add_talk #b#b#c186506宠物技能重置、升级通过哪位NPC进行？#n
	
	rand 12 4
	if @12 = 0
	option 428 宠物导师-莉莉
	option 401 宠物训练员-拉奥多
	option 428 技能训练师-康拉德
	option 428 铸造大师-泰戈
	elseif @12 = 1
	option 401 宠物训练员-拉奥多
	option 428 技能训练师-康拉德
	option 428 铸造大师-泰戈
	option 428 宠物导师-莉莉
	elseif @12 = 2
	option 428 技能训练师-康拉德
	option 428 铸造大师-泰戈
	option 428 宠物导师-莉莉
	option 401 宠物训练员-拉奥多
	else
	option 428 铸造大师-泰戈
	option 428 宠物导师-莉莉
	option 401 宠物训练员-拉奥多
	option 428 技能训练师-康拉德
	endif
	talk
endproc

proc 315
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b请仔细阅读以下题目，并认真作答，如果答错一题，将重新开始答题，如果答对三道，则完成任务！
	add_talk #b#b
	add_talk #b#b#c186506能够获得本职业紫装、粉装的熔炉秘宝任务领取找哪个NPC？#n
	
	rand 12 4
	if @12 = 0
	option 430 博学士-大马丁
	option 430 地心引路人-凡尔纳
	option 430 坐骑大师-阿诺德
	option 401 宝鉴-哈利波大
	elseif @12 = 1
	option 430 地心引路人-凡尔纳
	option 430 坐骑大师-阿诺德
	option 401 宝鉴-哈利波大
	option 430 博学士-大马丁
	elseif @12 = 2
	option 430 坐骑大师-阿诺德
	option 401 宝鉴-哈利波大
	option 430 博学士-大马丁
	option 430 地心引路人-凡尔纳
	else
	option 401 宝鉴-哈利波大
	option 430 博学士-大马丁
	option 430 地心引路人-凡尔纳
	option 430 坐骑大师-阿诺德
	endif
	talk
endproc

proc 316
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b请仔细阅读以下题目，并认真作答，如果答错一题，将重新开始答题，如果答对三道，则完成任务！
	add_talk #b#b
	add_talk #b#b#c186506装备进阶（将紫装升级为粉装）找哪个NPC？#n
	
	rand 12 4
	if @12 = 0
	option 401 装备解封、进阶大师-惠杰丰
	option 432 装备改造师-斯迪芬奇
	option 432 装备强化大师-克尔泽
	option 432 公会管理员-斯蒂芬斯
	elseif @12 = 1
	option 432 装备改造师-斯迪芬奇
	option 432 装备强化大师-克尔泽
	option 432 公会管理员-斯蒂芬斯
	option 401 装备解封、进阶大师-惠杰丰
	elseif @12 = 2
	option 432 装备强化大师-克尔泽
	option 432 公会管理员-斯蒂芬斯
	option 401 装备解封、进阶大师-惠杰丰
	option 432 装备改造师-斯迪芬奇
	else
	option 432 公会管理员-斯蒂芬斯
	option 401 装备解封、进阶大师-惠杰丰
	option 432 装备改造师-斯迪芬奇
	option 432 装备强化大师-克尔泽
	endif
	talk
endproc

proc 317
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b请仔细阅读以下题目，并认真作答，如果答错一题，将重新开始答题，如果答对三道，则完成任务！
	add_talk #b#b
	add_talk #b#b#c186506装备属性转移（将相同等级、部位的紫装的附加属性转移到另一个装备上）找哪个NPC？#n
	
	rand 12 4
	if @12 = 0
	option 401 装备改造师-斯迪芬奇
	option 434 装备强化大师-克尔泽
	option 434 装备解封、进阶大师-惠杰丰
	option 434 高级荣誉商人-梅杰
	elseif @12 = 1
	option 434 装备强化大师-克尔泽
	option 434 装备解封、进阶大师-惠杰丰
	option 434 高级荣誉商人-梅杰
	option 401 装备改造师-斯迪芬奇
	elseif @12 = 2
	option 434 装备解封、进阶大师-惠杰丰
	option 434 高级荣誉商人-梅杰
	option 401 装备改造师-斯迪芬奇
	option 434 装备强化大师-克尔泽
	else
	option 434 高级荣誉商人-梅杰
	option 401 装备改造师-斯迪芬奇
	option 434 装备强化大师-克尔泽
	option 434 装备解封、进阶大师-惠杰丰
	endif
	talk
endproc

proc 318
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b请仔细阅读以下题目，并认真作答，如果答错一题，将重新开始答题，如果答对三道，则完成任务！
	add_talk #b#b
	add_talk #b#b#c186506最快的进入副本的方式是？#n
	
	rand 12 4
	if @12 = 0
	option 436 跑到副本门口，组满人后直接进去
	option 436 用意念的力量“我要进副本，我要进副本……”
	option 401 通过小地图上的“传”按钮，随时随地随心进
	option 436 去摸电门
	elseif @12 = 1
	option 436 用意念的力量“我要进副本，我要进副本……”
	option 401 通过小地图上的“传”按钮，随时随地随心进
	option 436 去摸电门
	option 436 跑到副本门口，组满人后直接进去
	elseif @12 = 2
	option 401 通过小地图上的“传”按钮，随时随地随心进
	option 436 去摸电门
	option 436 跑到副本门口，组满人后直接进去
	option 436 用意念的力量“我要进副本，我要进副本……”
	else
	option 436 去摸电门
	option 436 跑到副本门口，组满人后直接进去
	option 436 用意念的力量“我要进副本，我要进副本……”
	option 401 通过小地图上的“传”按钮，随时随地随心进
	endif
	talk
endproc

proc 319
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b请仔细阅读以下题目，并认真作答，如果答错一题，将重新开始答题，如果答对三道，则完成任务！
	add_talk #b#b
	add_talk #b#b#c186506今天星期几？#n
	
	strvar weekid
	;weekid : 0 ~ 6    0代表周日    1代表周一
	GET_NOW_TIME  0 0 0 weekid 0 0 0 
	
	if @weekid = 1
		rand 12 4
		if @12 = 0
			option 401 周一
			option 438 周二
			option 438 周三
			option 438 周四
			option 438 周五
			option 438 周六
			option 438 周日
		elseif @12 = 1
			option 438 周二
			option 438 周三
			option 438 周四
			option 438 周五
			option 438 周六
			option 438 周日
			option 401 周一
		elseif @12 = 2
			option 438 周三
			option 438 周四
			option 438 周五
			option 438 周六
			option 438 周日
			option 401 周一
			option 438 周二
		else
			option 438 周五
			option 438 周六
			option 438 周日
			option 401 周一
			option 438 周二
			option 438 周三
			option 438 周四
		endif
	elseif @weekid = 2
		rand 12 4
		if @12 = 0
			option 438 周一
			option 401 周二
			option 438 周三
			option 438 周四
			option 438 周五
			option 438 周六
			option 438 周日
		elseif @12 = 1
			option 401 周二
			option 438 周三
			option 438 周四
			option 438 周五
			option 438 周六
			option 438 周日
			option 438 周一
		elseif @12 = 2
			option 438 周三
			option 438 周四
			option 438 周五
			option 438 周六
			option 438 周日
			option 438 周一
			option 401 周二
		else
			option 438 周五
			option 438 周六
			option 438 周日
			option 438 周一
			option 401 周二
			option 438 周三
			option 438 周四
		endif
	elseif @weekid = 3
		rand 12 4
		if @12 = 0
			option 438 周一
			option 438 周二
			option 401 周三
			option 438 周四
			option 438 周五
			option 438 周六
			option 438 周日
		elseif @12 = 1
			option 438 周二
			option 401 周三
			option 438 周四
			option 438 周五
			option 438 周六
			option 438 周日
			option 438 周一
		elseif @12 = 2
			option 401 周三
			option 438 周四
			option 438 周五
			option 438 周六
			option 438 周日
			option 438 周一
			option 438 周二
		else
			option 438 周五
			option 438 周六
			option 438 周日
			option 438 周一
			option 438 周二
			option 401 周三
			option 438 周四
		endif
	elseif @weekid = 4
		rand 12 4
		if @12 = 0
			option 438 周一
			option 438 周二
			option 438 周三
			option 401 周四
			option 438 周五
			option 438 周六
			option 438 周日
		elseif @12 = 1
			option 438 周二
			option 438 周三
			option 401 周四
			option 438 周五
			option 438 周六
			option 438 周日
			option 438 周一
		elseif @12 = 2
			option 438 周三
			option 401 周四
			option 438 周五
			option 438 周六
			option 438 周日
			option 438 周一
			option 438 周二
		else
			option 438 周五
			option 438 周六
			option 438 周日
			option 438 周一
			option 438 周二
			option 438 周三
			option 401 周四
		endif
	elseif @weekid = 5
		rand 12 4
		if @12 = 0
			option 438 周一
			option 438 周二
			option 438 周三
			option 438 周四
			option 401 周五
			option 438 周六
			option 438 周日
		elseif @12 = 1
			option 438 周二
			option 438 周三
			option 438 周四
			option 401 周五
			option 438 周六
			option 438 周日
			option 438 周一
		elseif @12 = 2
			option 438 周三
			option 438 周四
			option 401 周五
			option 438 周六
			option 438 周日
			option 438 周一
			option 438 周二
		else
			option 401 周五
			option 438 周六
			option 438 周日
			option 438 周一
			option 438 周二
			option 438 周三
			option 438 周四
		endif
	elseif @weekid = 6
		rand 12 4
		if @12 = 0
			option 438 周一
			option 438 周二
			option 438 周三
			option 438 周四
			option 438 周五
			option 401 周六
			option 438 周日
		elseif @12 = 1
			option 438 周二
			option 438 周三
			option 438 周四
			option 438 周五
			option 401 周六
			option 438 周日
			option 438 周一
		elseif @12 = 2
			option 438 周三
			option 438 周四
			option 438 周五
			option 401 周六
			option 438 周日
			option 438 周一
			option 438 周二
		else
			option 438 周五
			option 401 周六
			option 438 周日
			option 438 周一
			option 438 周二
			option 438 周三
			option 438 周四
		endif
	elseif @weekid = 0
		rand 12 4
		if @12 = 0
			option 438 周一
			option 438 周二
			option 438 周三
			option 438 周四
			option 438 周五
			option 438 周六
			option 401 周日
		elseif @12 = 1
			option 438 周二
			option 438 周三
			option 438 周四
			option 438 周五
			option 438 周六
			option 401 周日
			option 438 周一
		elseif @12 = 2
			option 438 周三
			option 438 周四
			option 438 周五
			option 438 周六
			option 401 周日
			option 438 周一
			option 438 周二
		else
			option 438 周五
			option 438 周六
			option 401 周日
			option 438 周一
			option 438 周二
			option 438 周三
			option 438 周四
		endif
	else
		return
	endif
	
	
	talk
endproc

;选项偶数的为错误的，奇数401为正确的

proc 400
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;1的话，表示一定要把答对次数清0
	SET_TASK_VAR @430 4 32 1 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c0181f9很多骗术都是利用人们的贪小便宜心理，无论是现实生活还是网络世界里，都奉行一个流传千百年的真理：天下没有免费的午餐！#n
	
	option 500 重新开始答题
	
	talk
endproc

proc 401
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	GET_TASK_VAR @430 0 32 1 80
	add 80 1
	SET_TASK_VAR @430 0 32 @80 1
	
	if @80 >= 3
		TASK_FINI @430 1
		PLAYER_CLOSE_DIALOG1 0 0
	else
		call 200
	endif
endproc

proc 402
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;1的话，表示一定要把答对次数清0
	SET_TASK_VAR @430 4 32 1 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c0181f9常见骗术：有些盗号贼做出一个仿真的“木马网站”，当你点击的时候就通过木马盗取您的账号，损害您的利益！对付这类骗术，请认准零纪元唯一官网：www.ljy0.com，您可以官网上查询到所有的零纪元官方推出的活动。#n
	
	option 500 重新开始答题
	
	talk
endproc

proc 404
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;1的话，表示一定要把答对次数清0
	SET_TASK_VAR @430 4 32 1 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c0181f9无论何时何地，都要提高自己的防骗防盗意识，防患于未然！通过官网的“账号管理”按钮，登陆个人中心，根据“安全提示”里的内容，填写安全邮箱，认证安全邮箱，绑定密保卡，让自己远离盗号！#n
	
	option 500 重新开始答题
	
	talk
endproc

proc 406
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;1的话，表示一定要把答对次数清0
	SET_TASK_VAR @430 4 32 1 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c0181f9如果您遇到这样的情况，请到论坛“问题反馈区”发帖向GM求助。希望您永远不会遇到这样的情况，但是盗号贼就像老鼠一样，即使人人喊打也不会灭绝，所以，保护好账号安全尤为重要，通过官网的“账号管理”按钮，登陆个人中心，绑定密保卡，验证安全邮箱吧！#n
	
	option 500 重新开始答题
	
	talk
endproc

proc 408
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;1的话，表示一定要把答对次数清0
	SET_TASK_VAR @430 4 32 1 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c0181f9防火防骗防盗号，最关键的一条，请认准官网：www.ljy0.com#n
	
	option 500 重新开始答题
	
	talk
endproc

proc 410
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;1的话，表示一定要把答对次数清0
	SET_TASK_VAR @430 4 32 1 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c0181f9常见骗术：某玩家在世界频道发布的“收XXX礼包”让你去百度XXX礼包……这种99.99％是骗子！请不要相信官网上没有出现过的“礼包”活动，想要了解最新最全面的福利与活动，请登陆零纪元官网：www.ljy0.com#n
	
	option 500 重新开始答题
	
	talk
endproc

proc 412
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;1的话，表示一定要把答对次数清0
	SET_TASK_VAR @430 4 32 1 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c0181f9现在盗号技术非常发达，那些设置简单的密码很容易被破解，所以，请不要把自己的账号透露给陌生人，更不要相信那些非官方发布的中奖邮件、密语及聊天信息，官方人员更不会向您索要密码类的个人信息。#n
	
	option 500 重新开始答题
	
	talk
endproc

proc 414
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;1的话，表示一定要把答对次数清0
	SET_TASK_VAR @430 4 32 1 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c0181f9账号管理及密保卡管理的官网地址：http://passport.wanyouyl.com
	add_talk #b#b
    add_talk #b#b#c0181f9零纪元官网：http://www.ljy0.com
	add_talk #b#b
    add_talk #b#b#c0181f9零纪元官方论坛：http://bbs.ljy0.com
	add_talk #b#b
    add_talk #b#b#c0181f9零纪元官方充值中心地址：http://pay.wanyouyl.com#n
	
	option 500 重新开始答题
	
	talk
endproc

proc 416
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;1的话，表示一定要把答对次数清0
	SET_TASK_VAR @430 4 32 1 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c0181f9在零纪元里，所有的“礼包”里领取的物品都是绑定的，不可交易的哦，所以，那些在世界上收XX礼包的大多是骗子哟，特别是那些让你去百度而非去官网查找最新活动信息的！请认准零纪元唯一官网：www.ljy0.com#n
	
	option 500 重新开始答题
	
	talk
endproc


proc 418
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;1的话，表示一定要把答对次数清0
	SET_TASK_VAR @430 4 32 1 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c0181f9常见骗术：盗号贼盗得一个游戏账号，在卷主被盗者号上所有财产之前，先假借号主人的名义向号中所有好友、公会成员借钱借物，然后连同号主人的财产一并转移！所以，无论是现实中还是零纪元里，如果有人向你借钱借物，记得确认对方是否真正为好友本人哟！#n
	
	option 500 重新开始答题
	
	talk
endproc

proc 420
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;1的话，表示一定要把答对次数清0
	SET_TASK_VAR @430 4 32 1 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c0181f9常见骗术：在游戏中发布虚假信息（领取金币、宝宝、坐骑等稀有物品），让您登陆一个非官方网站，网站上挂有木马，记录下您的账号信息！防火防骗防盗号，切勿轻信中奖信息！请记住零纪元唯一官网：www.ljy0.com#n
	
	option 500 重新开始答题
	
	talk
endproc

proc 422
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;1的话，表示一定要把答对次数清0
	SET_TASK_VAR @430 4 32 1 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c0181f9龙蛋谷与盘山道、筑梦空间一样，属于动态副本，动态副本的经验只与自己的等级有关，跟队友的等级无关，带小号能够让副本中的怪物难度（等级）降低，让副本过程更容易、更迅速，但不会拉低你所获得的经验，因为它只取决于你自身的等级与队伍中的人数！
	add_talk #b#b#c0181f9另外，使用副本中的“火焰刀”“手雷”也并不会让你所获得的经验减少，唯一能让你获得的经验减少的途径是：有队友离开了！无关乎他的等级与职业！#n
	
	option 500 重新开始答题
	
	talk
endproc

proc 424
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;1的话，表示一定要把答对次数清0
	SET_TASK_VAR @430 4 32 1 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c0181f9面饰、首饰（项链、戒指、护符）的进阶材料都是松珀合金，它通过失落之域怪物掉落的松珀碎块合成。#n
	
	option 500 重新开始答题
	
	talk
endproc

proc 426
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;1的话，表示一定要把答对次数清0
	SET_TASK_VAR @430 4 32 1 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c0181f9面饰、首饰（项链、戒指、护符）的进阶材料都是松珀合金，它通过失落之域怪物掉落的松珀碎块合成。#n
	
	option 500 重新开始答题
	
	talk
endproc

proc 428
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;1的话，表示一定要把答对次数清0
	SET_TASK_VAR @430 4 32 1 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c0181f9宠物训练员-拉奥多，他可以帮助宠物进行技能重置、升级，宠物资质提升，宠物属性强化。他就在时间城下方自由港附近。#n
	
	option 500 重新开始答题
	
	talk
endproc

proc 430
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;1的话，表示一定要把答对次数清0
	SET_TASK_VAR @430 4 32 1 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c0181f9宝鉴-哈利波大，她就在时间城上方，公会管理员斯蒂芬斯不远处，从40级开始，就可以在她那儿接受“熔炉秘宝”任务，紫色炉子有几率获得本职业当前等级段的紫装，金色炉子必然会获得本职业的紫装、重铸石，有一定几率会获得粉装。
	add_talk #b#b#c0181f9当放弃、交付“熔炉秘宝”任务时（非金色熔炉），会必然获得一张金熔炉凭证，积攒到一定数量时，就可以在哈利波大处将白、绿、蓝、紫色熔炉，换成金色熔炉！#n
	
	option 500 重新开始答题
	
	talk
endproc

proc 432
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;1的话，表示一定要把答对次数清0
	SET_TASK_VAR @430 4 32 1 1
	
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c0181f9你可以在“惠合承”处查看装备进阶的相关问题，以及合成装备所需要的材料出处，合成方式。
	add_talk #b#b#c0181f9装备进阶则在“装备解封、进阶大师-惠杰丰”处进行。#n
	
	option 500 重新开始答题
	
	talk
endproc

proc 434
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;1的话，表示一定要把答对次数清0
	SET_TASK_VAR @430 4 32 1 1
	
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c0181f9装备属性转移在“装备改造师-斯迪芬奇”处进行，转移所需要的转移石，可从地宫里的怪物身上获得。现在紫装属性可以转移到相同部位、等级的粉装上哟，转移所需要的道具可在商城-角色道具-成长页面中购买。#n
	
	option 500 重新开始答题
	
	talk
endproc

proc 436
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;1的话，表示一定要把答对次数清0
	SET_TASK_VAR @430 4 32 1 1
	
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c0181f9仔细研究屏幕右上角小地图上的“传”字按钮哟，那是进入副本的快捷方式。除此之外还有战场报名按钮，金币交易按钮、活动助手按钮，它们让游戏变得更方便！#n
	
	option 500 重新开始答题
	
	talk
endproc

proc 438
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;1的话，表示一定要把答对次数清0
	SET_TASK_VAR @430 4 32 1 1
	
	DIALOG 1
	add_talk $_npc$:
	
	strvar weekid
	;weekid : 0 ~ 6    0代表周日    1代表周一
	GET_NOW_TIME  0 0 0 weekid 0 0 0 
	
	if @weekid = 1
		add_talk #b#b#c0181f9今天是周一噢！#n
	elseif @weekid = 2
		add_talk #b#b#c0181f9今天是周二噢！#n
	elseif @weekid = 3
		add_talk #b#b#c0181f9今天是周三噢！#n
	elseif @weekid = 4
		add_talk #b#b#c0181f9今天是周四噢！#n
	elseif @weekid = 5
		add_talk #b#b#c0181f9今天是周五噢！#n
	elseif @weekid = 6
		add_talk #b#b#c0181f9今天是周六噢！#n
	elseif @weekid = 0
		add_talk #b#b#c0181f9今天是周日噢！#n
	else
		return
	endif
	
	option 500 重新开始答题
	talk
endproc

proc 500
	TASK_STAT 4573 1 71
	if @71 = 2
		var 430 4573
	endif
	
	TASK_STAT 4574 1 72
	if @72 = 2
		var 430 4574
	endif
	
	TASK_STAT 4575 1 73
	if @73 = 2
		var 430 4575
	endif
	
	TASK_STAT 4576 1 74
	if @74 = 2
		var 430 4576
	endif
	
	TASK_STAT 4577 1 75
	if @75 = 2
		var 430 4577
	endif
	
	;答对次数清0
	SET_TASK_VAR @430 0 32 0 1
	;1的话，表示一定要把答对次数清0
	SET_TASK_VAR @430 4 32 1 1
	call 200
endproc


