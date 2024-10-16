
proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b嘿，朋友，来我这里看看，我这里出售的物品可是别的地方买也不买到的，当然啦，你也需要一定的威望，无名小众在我们这可获得不了什么东西，不过你也不用担心，只要你在基尔特的声望达到了友善以上就可以喽。
	add_talk #b#b
	add_talk #b#b#c186506完成【商运】和【赏善惩罚】都能获得基尔特商会的声望。#n

	OPTION 101 #(买卖)声望商店

	STRVAR sta sta_2 sta_3
	
	TASK_STAT 4600 1 sta
	if @sta = 2
		option 200 开始答题
	endif
	TASK_STAT 4614 1 sta_2
	if @sta_2 = 2
		option 600 区区整理难不到我
	endif
	TASK_STAT 4615 1 sta_3
	if @sta_3 = 2
		option 700 进入逆往之间整理
	endif

	task_stat 4601 1 10
	if 10 = 2
		option 900 #你想要什么防具呢？
	endif
	task_stat 4602 1 11
	if 11 = 2
		option 910 #你想要什么武器呢？
	endif
	task_stat 4603 1 12
	if 12 = 2
		option 920 #你想要什么宠物蛋呢？
	endif
	
	task_add_option 4618 1 102
	
	TASKCYCLE_ADD_OPTION 145 1 800
	
	talk
endproc
	
PROC 101
	OPEN_NPC_SHOP
ENDPROC

PROC 102
	TASK_DO_OPTION 4618 0
ENDPROC

proc 200
	STRVAR sta not not_1 num

	TASK_STAT 4600 1 sta
	if @sta = 2
	
		;答对次数
		GET_TASK_VAR 4600 0 32 1 not

		if @not >= 3
			TASK_FINI 4600 1
			PLAYER_CLOSE_DIALOG1 0 0
		else
			GET_TASK_VAR 4600 4 32 1 not_1
			if @not_1 = 1
				;答对次数清0
				SET_TASK_VAR 4600 0 32 0 1
				;标记清0
				SET_TASK_VAR 4600 4 32 0 1
			endif
		
			;20道题
			rand num 20
			switch @num
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
			endswitch
		endif
	endif
endproc

proc 300
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b请仔细阅读以下题目，并认真作答，如果答错一题，将重新开始答题，如果答对三道，则完成任务！
	add_talk #b#b
	add_talk #b#b#c186506看到世界频道有人在低价叫卖东西，并且还要加聊天工具的，你怎么办？#n
	
	STRVAR num
	rand num 4
	if @num = 0
		option 401 果断加之，有便宜不占是笨蛋
		option 400 点击小地图右上角的BUG反馈举报他
		option 401 赶紧联系，索要电话，发展成长期合作关系
		option 401 问问能不能再便宜点
	elseif @num = 1
		option 400 点击小地图右上角的BUG反馈举报他
		option 401 赶紧联系，索要电话，发展成长期合作关系
		option 401 问问能不能再便宜点
		option 401 果断加之，有便宜不占是笨蛋
	elseif @num = 2
		option 401 赶紧联系，索要电话，发展成长期合作关系
		option 401 问问能不能再便宜点
		option 401 果断加之，有便宜不占是笨蛋
		option 400 点击小地图右上角的BUG反馈举报他
	else
		option 401 问问能不能再便宜点
		option 401 果断加之，有便宜不占是笨蛋
		option 400 点击小地图右上角的BUG反馈举报他
		option 401 赶紧联系，索要电话，发展成长期合作关系
	endif
	talk
endproc

proc 301
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b请仔细阅读以下题目，并认真作答，如果答错一题，将重新开始答题，如果答对三道，则完成任务！
	add_talk #b#b
	add_talk #b#b#c186506如果有人跟你说登陆某非官方网站能够领取大量游戏道具、金币、银币、经验、宠物，你会怎么做？#n
	
	STRVAR num
	rand num 4
	if @num = 0
	option 402 先问问他能不能多给一些
	option 400 点击小地图右上角的BUG反馈举报他
	option 402 截图，并群发告之好友，动员好友一起去领
	option 402 果断去领
	elseif @num = 1
	option 400 点击小地图右上角的BUG反馈举报他
	option 402 截图，并群发告之好友，动员好友一起去领
	option 402 果断去领
	option 402 先问问他能不能多给一些
	elseif @num = 2
	option 402 截图，并群发告之好友，动员好友一起去领
	option 402 果断去领
	option 402 先问问他能不能多给一些
	option 400 点击小地图右上角的BUG反馈举报他
	else
	option 402 果断去领
	option 402 先问问他能不能多给一些
	option 400 点击小地图右上角的BUG反馈举报他
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
	
	STRVAR num
	rand num 4
	if @num = 0
	option 404 重新建一个号，并在世界上大骂盗号贼
	option 404 盗就盗了，无所谓
	option 400 绑定安全邮箱，及时认证，绑定密保卡
	option 404 设法去盗别人的号
	elseif @num = 1
	option 404 盗就盗了，无所谓
	option 400 绑定安全邮箱，及时认证，绑定密保卡
	option 404 设法去盗别人的号
	option 404 重新建一个号，并在世界上大骂盗号贼
	elseif @num = 2
	option 400 绑定安全邮箱，及时认证，绑定密保卡
	option 404 设法去盗别人的号
	option 404 重新建一个号，并在世界上大骂盗号贼
	option 404 盗就盗了，无所谓
	else
	option 404 设法去盗别人的号
	option 404 重新建一个号，并在世界上大骂盗号贼
	option 404 盗就盗了，无所谓
	option 400 绑定安全邮箱，及时认证，绑定密保卡
	endif
	talk
endproc

proc 303
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b请仔细阅读以下题目，并认真作答，如果答错一题，将重新开始答题，如果答对三道，则完成任务！
	add_talk #b#b
	add_talk #b#b#c186506如果发现被盗后账号密码无法取回或被恶意绑定密保卡，怎么办？#n
	
	STRVAR num
	rand num 4
	if @num = 0
	option 406 一个一个试，总有一个是对的
	option 406 反正也不值几个钱，重新再练一个号
	option 406 被恶意绑定了我也没办法
	option 400 到论坛“问题反馈区”发帖向GM求助
	elseif @num = 1	
	option 406 反正也不值几个钱，重新再练一个号
	option 406 被恶意绑定了我也没办法
	option 400 到论坛“问题反馈区”发帖向GM求助
	option 406 一个一个试，总有一个是对的
	elseif @num = 2
	option 406 被恶意绑定了我也没办法
	option 400 到论坛“问题反馈区”发帖向GM求助
	option 406 一个一个试，总有一个是对的
	option 406 反正也不值几个钱，重新再练一个号
	else
	option 400 到论坛“问题反馈区”发帖向GM求助
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
	
	STRVAR num
	rand num 4
	if @num = 0
	option 400 www.ljy0.com
	option 408 www.ljy1.com
	option 408 www.ljy2.com
	option 408 www.ljy3.com
	elseif @num = 1
	option 408 www.ljy1.com
	option 408 www.ljy2.com
	option 408 www.ljy3.com
	option 400 www.ljy0.com
	elseif @num = 2
	option 408 www.ljy2.com
	option 408 www.ljy3.com
	option 400 www.ljy0.com
	option 408 www.ljy1.com
	else
	option 408 www.ljy3.com
	option 400 www.ljy0.com
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
	
	STRVAR num
	rand num 4
	if @num = 0
	option 410 世界新闻
	option 400 礼包活动、福利活动、最新活动、游戏资料、攻略
	option 410 比cctv还全面的新闻信息，比娱乐网站还多的视频
	option 410 策划生活照
	elseif @num = 1
	option 400 礼包活动、福利活动、最新活动、游戏资料、攻略
	option 410 比cctv还全面的新闻信息，比娱乐网站还多的视频
	option 410 策划生活照
	option 410 世界新闻
	elseif @num = 2
	option 410 比cctv还全面的新闻信息，比娱乐网站还多的视频
	option 410 策划生活照
	option 410 世界新闻
	option 400 礼包活动、福利活动、最新活动、游戏资料、攻略
	else
	option 410 策划生活照
	option 410 世界新闻
	option 400 礼包活动、福利活动、最新活动、游戏资料、攻略
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
	
	STRVAR num
	rand num 4
	if @num = 0
	option 412 直接给他，这么好的事百年难得一遇啊，当然要赶紧
	option 412 把朋友的号一起给他
	option 412 考虑一下
	option 400 记下ID到论坛“问题反馈区”去举报他！
	elseif @num = 1
	option 412 把朋友的号一起给他
	option 412 考虑一下
	option 400 记下ID到论坛“问题反馈区”去举报他！
	option 412 直接给他，这么好的事百年难得一遇啊，当然要赶紧
	elseif @num = 2
	option 412 考虑一下
	option 400 记下ID到论坛“问题反馈区”去举报他！
	option 412 直接给他，这么好的事百年难得一遇啊，当然要赶紧
	option 412 把朋友的号一起给他
	else
	option 400 记下ID到论坛“问题反馈区”去举报他！
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
	
	STRVAR num
	rand num 4
	if @num = 0
	option 400 http://passport.wanyouyl.com
	option 414 http://www.ljy0.com
	option 414 http://bbs.ljy0.com
	option 414 http://pay.wanyouyl.com
	elseif @num = 1
	option 414 http://www.ljy0.com
	option 414 http://bbs.ljy0.com
	option 414 http://pay.wanyouyl.com
	option 400 http://passport.wanyouyl.com
	elseif @num = 2
	option 414 http://bbs.ljy0.com
	option 414 http://pay.wanyouyl.com
	option 400 http://passport.wanyouyl.com
	option 414 http://www.ljy0.com
	else
	option 414 http://pay.wanyouyl.com
	option 400 http://passport.wanyouyl.com
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
	
	STRVAR num
	rand num 4
	if @num = 0
	option 400 零纪元官网：http://www.ljy0.com
	option 400 零纪元官方论坛：http://bbs.ljy0.com
	option 400 各大正规媒体网站（如17173、新浪、多玩、网易）
	option 416 游戏内世界频道发的收购XX礼包的信息
	elseif @num = 1
	option 400 零纪元官方论坛：http://bbs.ljy0.com
	option 400 各大正规媒体网站（如17173、新浪、多玩、网易）
	option 416 游戏内世界频道发的收购XX礼包的信息
	option 400 零纪元官网：http://www.ljy0.com
	elseif @num = 2
	option 400 各大正规媒体网站（如17173、新浪、多玩、网易）
	option 416 游戏内世界频道发的收购XX礼包的信息
	option 400 零纪元官网：http://www.ljy0.com
	option 400 零纪元官方论坛：http://bbs.ljy0.com
	else
	option 416 游戏内世界频道发的收购XX礼包的信息
	option 400 零纪元官网：http://www.ljy0.com
	option 400 零纪元官方论坛：http://bbs.ljy0.com
	option 400 各大正规媒体网站（如17173、新浪、多玩、网易）
	endif
	talk
endproc

proc 309
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b请仔细阅读以下题目，并认真作答，如果答错一题，将重新开始答题，如果答对三道，则完成任务！
	add_talk #b#b
	add_talk #b#b#c186506游戏中自己的朋友突然向你索要金币、银币、装备，你会怎么做？#n
	
	STRVAR num
	rand num 4
	if @num = 0
	option 418 直接给他
	option 418 难得好友开中，不太好驳他面子，二话不说直接给
	option 400 仔细询问，通过其他方式（电话）联系，确认清楚
	option 418 直接下线
	elseif @num = 1
	option 418 难得好友开中，不太好驳他面子，二话不说直接给
	option 400 仔细询问，通过其他方式（电话）联系，确认清楚
	option 418 直接下线
	option 418 直接给他
	elseif @num = 2
	option 400 仔细询问，通过其他方式（电话）联系，确认清楚
	option 418 直接下线
	option 418 直接给他
	option 418 难得好友开中，不太好驳他面子，二话不说直接给
	else
	option 418 直接下线
	option 418 直接给他
	option 418 难得好友开中，不太好驳他面子，二话不说直接给
	option 400 仔细询问，通过其他方式（电话）联系，确认清楚
	endif
	talk
endproc

proc 310
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b请仔细阅读以下题目，并认真作答，如果答错一题，将重新开始答题，如果答对三道，则完成任务！
	add_talk #b#b
	add_talk #b#b#c186506有人在游戏里加你好友，然后告诉你去XXX网站领取金币、宝宝、地狱骑士坐骑，你会怎么做？#n
	
	STRVAR num
	rand num 4
	if @num = 0
	option 420 哇，这么好的事，当然是赶紧去领了！
	option 420 把账号密码告诉他，让他帮忙去领！
	option 400 骗子！记下ID去论坛“问题反馈区”举报他
	option 420 立即转发给好友，呼朋唤友叫公会一起去领
	elseif @num = 1
	option 420 把账号密码告诉他，让他帮忙去领！
	option 400 骗子！记下ID去论坛“问题反馈区”举报他
	option 420 立即转发给好友，呼朋唤友叫公会一起去领
	option 420 哇，这么好的事，当然是赶紧去领了！
	elseif @num = 2
	option 400 骗子！记下ID去论坛“问题反馈区”举报他
	option 420 立即转发给好友，呼朋唤友叫公会一起去领
	option 420 哇，这么好的事，当然是赶紧去领了！
	option 420 把账号密码告诉他，让他帮忙去领！
	else
	option 420 立即转发给好友，呼朋唤友叫公会一起去领
	option 420 哇，这么好的事，当然是赶紧去领了！
	option 420 把账号密码告诉他，让他帮忙去领！
	option 400 骗子！记下ID去论坛“问题反馈区”举报他
	endif
	talk
endproc

proc 311
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b请仔细阅读以下题目，并认真作答，如果答错一题，将重新开始答题，如果答对三道，则完成任务！
	add_talk #b#b
	add_talk #b#b#c186506关于龙蛋谷，哪项说法是正确的？#n
	
	STRVAR num
	rand num 4
	if @num = 0
	option 422 我自己去，独吞所有经验
	option 422 不组小号，小号会拉低经验
	option 400 动态副本的经验只跟自己的等级有关
	option 422 使用副本中的“火焰刀”“手雷”会减少经验
	elseif @num = 1
	option 422 不组小号，小号会拉低经验
	option 400 动态副本的经验只跟自己的等级有关
	option 422 使用副本中的“火焰刀”“手雷”会减少经验
	option 422 我自己去，独吞所有经验
	elseif @num = 2
	option 400 动态副本的经验只跟自己的等级有关
	option 422 使用副本中的“火焰刀”“手雷”会减少经验
	option 422 我自己去，独吞所有经验
	option 422 不组小号，小号会拉低经验
	else
	option 422 使用副本中的“火焰刀”“手雷”会减少经验
	option 422 我自己去，独吞所有经验
	option 422 不组小号，小号会拉低经验
	option 400 动态副本的经验只跟自己的等级有关
	endif
	talk
endproc

proc 312
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b请仔细阅读以下题目，并认真作答，如果答错一题，将重新开始答题，如果答对三道，则完成任务！
	add_talk #b#b
	add_talk #b#b#c18650664级面饰进阶的材料叫什么名字？#n
	
	STRVAR num
	rand num 4
	if @num = 0
	option 424 炽铜微粒、炽铜碎块、炽铜合金
	option 424 坚钢微粒、坚钢碎块、坚钢合金
	option 400 松珀碎块、松珀合金
	option 424 黑耀微粒、黑耀碎块、黑耀合金
	elseif @num = 1	
	option 424 坚钢微粒、坚钢碎块、坚钢合金
	option 400 松珀碎块、松珀合金
	option 424 黑耀微粒、黑耀碎块、黑耀合金
	option 424 炽铜微粒、炽铜碎块、炽铜合金
	elseif @num = 2	
	option 400 松珀碎块、松珀合金
	option 424 黑耀微粒、黑耀碎块、黑耀合金
	option 424 炽铜微粒、炽铜碎块、炽铜合金
	option 424 坚钢微粒、坚钢碎块、坚钢合金
	else
	option 424 黑耀微粒、黑耀碎块、黑耀合金
	option 424 炽铜微粒、炽铜碎块、炽铜合金
	option 424 坚钢微粒、坚钢碎块、坚钢合金
	option 400 松珀碎块、松珀合金
	endif
	talk
endproc

proc 313
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b请仔细阅读以下题目，并认真作答，如果答错一题，将重新开始答题，如果答对三道，则完成任务！
	add_talk #b#b
	add_talk #b#b#c18650669项链进阶的材料叫什么名字？#n
	
	STRVAR num
	rand num 4
	if @num = 0
	option 426 炽铜微粒、炽铜碎块、炽铜合金
	option 426 坚铜微粒、坚铜碎块、坚铜合金
	option 400 松珀碎块、松珀合金
	option 426 黑耀微粒、黑耀碎块、黑耀合金
	elseif @num = 1
	option 426 坚铜微粒、坚铜碎块、坚铜合金
	option 400 松珀碎块、松珀合金
	option 426 黑耀微粒、黑耀碎块、黑耀合金
	option 426 炽铜微粒、炽铜碎块、炽铜合金
	elseif @num = 2	
	option 400 松珀碎块、松珀合金
	option 426 黑耀微粒、黑耀碎块、黑耀合金
	option 426 炽铜微粒、炽铜碎块、炽铜合金
	option 426 坚铜微粒、坚铜碎块、坚铜合金
	else
	option 426 黑耀微粒、黑耀碎块、黑耀合金
	option 426 炽铜微粒、炽铜碎块、炽铜合金
	option 426 坚铜微粒、坚铜碎块、坚铜合金
	option 400 松珀碎块、松珀合金
	endif
	talk
endproc

proc 314
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b请仔细阅读以下题目，并认真作答，如果答错一题，将重新开始答题，如果答对三道，则完成任务！
	add_talk #b#b
	add_talk #b#b#c186506宠物技能重置、升级通过哪位NPC进行？#n
	
	STRVAR num
	rand num 4
	if @num = 0
	option 428 宠物导师-莉莉
	option 400 宠物训练员-拉奥多
	option 428 技能训练师-康拉德
	option 428 铸造大师-泰戈
	elseif @num = 1
	option 400 宠物训练员-拉奥多
	option 428 技能训练师-康拉德
	option 428 铸造大师-泰戈
	option 428 宠物导师-莉莉
	elseif @num = 2
	option 428 技能训练师-康拉德
	option 428 铸造大师-泰戈
	option 428 宠物导师-莉莉
	option 400 宠物训练员-拉奥多
	else
	option 428 铸造大师-泰戈
	option 428 宠物导师-莉莉
	option 400 宠物训练员-拉奥多
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
	
	STRVAR num
	rand num 4
	if @num = 0
	option 430 博学士-大马丁
	option 430 地心引路人-凡尔纳
	option 430 坐骑大师-阿诺德
	option 400 宝鉴-哈利波大
	elseif @num = 1
	option 430 地心引路人-凡尔纳
	option 430 坐骑大师-阿诺德
	option 400 宝鉴-哈利波大
	option 430 博学士-大马丁
	elseif @num = 2
	option 430 坐骑大师-阿诺德
	option 400 宝鉴-哈利波大
	option 430 博学士-大马丁
	option 430 地心引路人-凡尔纳
	else
	option 400 宝鉴-哈利波大
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
	
	STRVAR num
	rand num 4
	if @num = 0
	option 400 装备解封、进阶大师-惠杰丰
	option 432 装备改造师-斯迪芬奇
	option 432 装备强化大师-克尔泽
	option 432 公会管理员-斯蒂芬斯
	elseif @num = 1
	option 432 装备改造师-斯迪芬奇
	option 432 装备强化大师-克尔泽
	option 432 公会管理员-斯蒂芬斯
	option 400 装备解封、进阶大师-惠杰丰
	elseif @num = 2
	option 432 装备强化大师-克尔泽
	option 432 公会管理员-斯蒂芬斯
	option 400 装备解封、进阶大师-惠杰丰
	option 432 装备改造师-斯迪芬奇
	else
	option 432 公会管理员-斯蒂芬斯
	option 400 装备解封、进阶大师-惠杰丰
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
	
	STRVAR num
	rand num 4
	if @num = 0
	option 400 装备改造师-斯迪芬奇
	option 434 装备强化大师-克尔泽
	option 434 装备解封、进阶大师-惠杰丰
	option 434 高级荣誉商人-梅杰
	elseif @num = 1
	option 434 装备强化大师-克尔泽
	option 434 装备解封、进阶大师-惠杰丰
	option 434 高级荣誉商人-梅杰
	option 400 装备改造师-斯迪芬奇
	elseif @num = 2
	option 434 装备解封、进阶大师-惠杰丰
	option 434 高级荣誉商人-梅杰
	option 400 装备改造师-斯迪芬奇
	option 434 装备强化大师-克尔泽
	else
	option 434 高级荣誉商人-梅杰
	option 400 装备改造师-斯迪芬奇
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
	
	STRVAR num
	rand num 4
	if @num = 0
	option 436 跑到副本门口，组满人后直接进去
	option 436 用意念的力量“我要进副本，我要进副本……”
	option 400 通过小地图上的“传”按钮，随时随地随心进
	option 436 去摸电门
	elseif @num = 1
	option 436 用意念的力量“我要进副本，我要进副本……”
	option 400 通过小地图上的“传”按钮，随时随地随心进
	option 436 去摸电门
	option 436 跑到副本门口，组满人后直接进去
	elseif @num = 2
	option 400 通过小地图上的“传”按钮，随时随地随心进
	option 436 去摸电门
	option 436 跑到副本门口，组满人后直接进去
	option 436 用意念的力量“我要进副本，我要进副本……”
	else
	option 436 去摸电门
	option 436 跑到副本门口，组满人后直接进去
	option 436 用意念的力量“我要进副本，我要进副本……”
	option 400 通过小地图上的“传”按钮，随时随地随心进
	endif
	talk
endproc

proc 400
	STRVAR sta num
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		GET_TASK_VAR 4600 0 32 1 not
		add not 1
		SET_TASK_VAR 4600 0 32 @not 1
		
		if @not >= 3
			TASK_FINI 4600 1
			PLAYER_CLOSE_DIALOG1 0 0
		else
			call 200
		endif
	endif
endproc

proc 401
	STRVAR sta
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		;1的话，表示一定要把答对次数清0
		SET_TASK_VAR 4600 4 32 1 1

		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b#c0181f9很多骗术都是利用人们的贪小便宜心理，无论是现实生活还是网络世界里，都奉行一个流传千百年的真理：天下没有免费的午餐！#n
		
		option 500 重新开始答题
	endif
	
	talk
endproc

proc 402
	STRVAR sta
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		;1的话，表示一定要把答对次数清0
		SET_TASK_VAR 4600 4 32 1 1

		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b#c0181f9常见骗术：有些盗号贼做出一个仿真的“木马网站”，当你点击的时候就通过木马盗取您的账号，损害您的利益！对付这类骗术，请认准零纪元唯一官网：www.ljy0.com，您可以官网上查询到所有的零纪元官方推出的活动。#n
		
		option 500 重新开始答题
	endif
		
	talk
endproc

proc 404
	STRVAR sta
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		;1的话，表示一定要把答对次数清0
		SET_TASK_VAR 4600 4 32 1 1

		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b#c0181f9无论何时何地，都要提高自己的防骗防盗意识，防患于未然！通过官网的“账号管理”按钮，登陆个人中心，根据“安全提示”里的内容，填写安全邮箱，认证安全邮箱，绑定密保卡，让自己远离盗号！#n
	
		option 500 重新开始答题
	endif
	
	talk
endproc

proc 406
	STRVAR sta
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		;1的话，表示一定要把答对次数清0
		SET_TASK_VAR 4600 4 32 1 1

		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b#c0181f9如果您遇到这样的情况，请到论坛“问题反馈区”发帖向GM求助。希望您永远不会遇到这样的情况，但是盗号贼就像老鼠一样，即使人人喊打也不会灭绝，所以，保护好账号安全尤为重要，通过官网的“账号管理”按钮，登陆个人中心，绑定密保卡，验证安全邮箱吧！#n
		
		option 500 重新开始答题
	endif
	
	talk
endproc

proc 408
	STRVAR sta
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		;1的话，表示一定要把答对次数清0
		SET_TASK_VAR 4600 4 32 1 1

		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b#c0181f9防火防骗防盗号，最关键的一条，请认准官网：www.ljy0.com#n
		
		option 500 重新开始答题
	endif
	
	talk
endproc

proc 410
	STRVAR sta
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		;1的话，表示一定要把答对次数清0
		SET_TASK_VAR 4600 4 32 1 1

		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b#c0181f9常见骗术：某玩家在世界频道发布的“收XXX礼包”让你去百度XXX礼包……这种99.99％是骗子！请不要相信官网上没有出现过的“礼包”活动，想要了解最新最全面的福利与活动，请登陆零纪元官网：www.ljy0.com#n
		
		option 500 重新开始答题
	endif
	
	talk
endproc

proc 412
	STRVAR sta
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		;1的话，表示一定要把答对次数清0
		SET_TASK_VAR 4600 4 32 1 1

		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b#c0181f9现在盗号技术非常发达，那些设置简单的密码很容易被破解，所以，请不要把自己的账号透露给陌生人，更不要相信那些非官方发布的中奖邮件、密语及聊天信息，官方人员更不会向您索要密码类的个人信息。#n
		
		option 500 重新开始答题
	endif
	
	talk
endproc

proc 414
	STRVAR sta
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		;1的话，表示一定要把答对次数清0
		SET_TASK_VAR 4600 4 32 1 1

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
	endif
	
	talk
endproc

proc 416
	STRVAR sta
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		;1的话，表示一定要把答对次数清0
		SET_TASK_VAR 4600 4 32 1 1

		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b#c0181f9在零纪元里，所有的“礼包”里领取的物品都是绑定的，不可交易的哦，所以，那些在世界上收XX礼包的大多是骗子哟，特别是那些让你去百度而非去官网查找最新活动信息的！请认准零纪元唯一官网：www.ljy0.com#n
		
		option 500 重新开始答题
	endif
	
	talk
endproc


proc 418
	STRVAR sta
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		;1的话，表示一定要把答对次数清0
		SET_TASK_VAR 4600 4 32 1 1

		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b#c0181f9常见骗术：盗号贼盗得一个游戏账号，在卷主被盗者号上所有财产之前，先假借号主人的名义向号中所有好友、公会成员借钱借物，然后连同号主人的财产一并转移！所以，无论是现实中还是零纪元里，如果有人向你借钱借物，记得确认对方是否真正为好友本人哟！#n
		
		option 500 重新开始答题
	endif
	
	talk
endproc

proc 420
	STRVAR sta
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		;1的话，表示一定要把答对次数清0
		SET_TASK_VAR 4600 4 32 1 1

		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b#c0181f9常见骗术：在游戏中发布虚假信息（领取金币、宝宝、坐骑等稀有物品），让您登陆一个非官方网站，网站上挂有木马，记录下您的账号信息！防火防骗防盗号，切勿轻信中奖信息！请记住零纪元唯一官网：www.ljy0.com#n
		
		option 500 重新开始答题
	endif
	
	talk
endproc

proc 422
	STRVAR sta
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		;1的话，表示一定要把答对次数清0
		SET_TASK_VAR 4600 4 32 1 1

		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b#c0181f9龙蛋谷与盘山道、筑梦空间一样，属于动态副本，动态副本的经验只与自己的等级有关，跟队友的等级无关，带小号能够让副本中的怪物难度（等级）降低，让副本过程更容易、更迅速，但不会拉低你所获得的经验，因为它只取决于你自身的等级与队伍中的人数！
		add_talk #b#b#c0181f9另外，使用副本中的“火焰刀”“手雷”也并不会让你所获得的经验减少，唯一能让你获得的经验减少的途径是：有队友离开了！无关乎他的等级与职业！#n
		
		option 500 重新开始答题
	endif
	
	talk
endproc

proc 424
	STRVAR sta
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		;1的话，表示一定要把答对次数清0
		SET_TASK_VAR 4600 4 32 1 1

		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b#c0181f9面饰、首饰（项链、戒指、护符）的进阶材料都是松珀合金，它通过失落之域怪物掉落的松珀碎块合成。#n
		
		option 500 重新开始答题
	endif
		
	talk
endproc

proc 426
	STRVAR sta
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		;1的话，表示一定要把答对次数清0
		SET_TASK_VAR 4600 4 32 1 1

		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b#c0181f9面饰、首饰（项链、戒指、护符）的进阶材料都是松珀合金，它通过失落之域怪物掉落的松珀碎块合成。#n
		
		option 500 重新开始答题
	endif
	
	talk
endproc

proc 428
	STRVAR sta
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		;1的话，表示一定要把答对次数清0
		SET_TASK_VAR 4600 4 32 1 1

		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b#c0181f9宠物训练员-拉奥多，他可以帮助宠物进行技能重置、升级，宠物资质提升，宠物属性强化。他就在时间城下方自由港附近。#n
		
		option 500 重新开始答题
	endif
	
	talk
endproc

proc 430
	STRVAR sta
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		;1的话，表示一定要把答对次数清0
		SET_TASK_VAR 4600 4 32 1 1

		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b#c0181f9宝鉴-哈利波大，她就在时间城上方，公会管理员斯蒂芬斯不远处，从40级开始，就可以在她那儿接受“熔炉秘宝”任务，紫色炉子有几率获得本职业当前等级段的紫装，金色炉子必然会获得本职业的紫装、重铸石，有一定几率会获得粉装。
		add_talk #b#b#c0181f9当放弃、交付“熔炉秘宝”任务时（非金色熔炉），会必然获得一张金熔炉凭证，积攒到一定数量时，就可以在哈利波大处将白、绿、蓝、紫色熔炉，换成金色熔炉！#n
		
		option 500 重新开始答题
	endif
	
	talk
endproc

proc 432
	STRVAR sta
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		;1的话，表示一定要把答对次数清0
		SET_TASK_VAR 4600 4 32 1 1

		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b#c0181f9你可以在“惠合承”处查看装备进阶的相关问题，以及合成装备所需要的材料出处，合成方式。
		add_talk #b#b#c0181f9装备进阶则在“装备解封、进阶大师-惠杰丰”处进行。#n
	
		option 500 重新开始答题
	endif
	
	talk
endproc

proc 434
	STRVAR sta
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		;1的话，表示一定要把答对次数清0
		SET_TASK_VAR 4600 4 32 1 1

		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b#c0181f9装备属性转移在“装备改造师-斯迪芬奇”处进行，转移所需要的转移石，可从地宫里的怪物身上获得。现在紫装属性可以转移到相同部位、等级的粉装上哟，转移所需要的道具可在商城-角色道具-成长页面中购买。#n
		
		option 500 重新开始答题
	endif
	
	talk
endproc

proc 436
	STRVAR sta
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		;1的话，表示一定要把答对次数清0
		SET_TASK_VAR 4600 4 32 1 1

		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b#c0181f9仔细研究屏幕右上角小地图上的“传”字按钮哟，那是进入副本的快捷方式。除此之外还有战场报名按钮，金币交易按钮、活动助手按钮，它们让游戏变得更方便！#n
		
		option 500 重新开始答题
	endif
	
	talk
endproc

proc 500
	STRVAR sta
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		;答对次数清0
		SET_TASK_VAR 4600 0 32 0 1
		;1的话，表示一定要把答对次数清0
		SET_TASK_VAR 4600 4 32 1 1
		call 200
	endif
	
endproc

PROC 600
	ENTER_PWORLD 514 0 0
	;进入副本
ENDPROC

PROC 700
	ENTER_PWORLD 515 0 0
	;进入副本
ENDPROC

PROC 800
	TASKCYCLE_DO_OPTION 145 1
ENDPROC

proc 900
	strvar ret
	
	dialog 3
	add_talk $_npc$:
	get_task_var 4601 0 8 1 ret	
	if ret = 0
		add_talk #b#b我想要一件白色#c186506野战服#n（12级战士胸甲），可以不？
	endif
	if ret = 1
		add_talk #b#b我想要一件白色#c186506亚麻衣#n（12级法师胸甲），可以不？
	endif
	if ret = 2
		add_talk #b#b我想要一件白色#c186506狂战装#n（12级枪手胸甲），可以不？
	endif
	if ret = 3
		add_talk #b#b我想要一件白色#c186506祷告装#n（12级祭司胸甲），可以不？
	endif
	if ret = 4
		add_talk #b#b我想要一件白色#c186506精铁头盔#n（13级战士头盔），可以不？
	endif
	if ret = 5
		add_talk #b#b我想要一件白色#c186506符文头巾#n（13级法师头盔），可以不？
	endif
	if ret = 6
		add_talk #b#b我想要一件白色#c186506优雅软帽#n（13级枪手头盔），可以不？
	endif
	if ret = 7
		add_talk #b#b我想要一件白色#c186506十字纹饰#n（13级祭司头盔），可以不？
	endif
	if ret = 8
		add_talk #b#b我想要一件白色#c186506武装面具#n（14级战士面饰），可以不？
	endif
	if ret = 9
		add_talk #b#b我想要一件白色#c186506元素护饰#n（14级法师面饰），可以不？
	endif
	if ret = 10
		add_talk #b#b我想要一件白色#c186506热诚面饰#n（14级枪手面饰），可以不？
	endif
	if ret = 11
		add_talk #b#b我想要一件白色#c186506教徒面具#n（14级祭司面饰），可以不？
	endif
	if ret = 12
		add_talk #b#b我想要一件白色#c186506精铁肩甲#n（15级战士肩甲），可以不？
	endif
	if ret = 13
		add_talk #b#b我想要一件白色#c186506符文肩甲#n（15级法师肩甲），可以不？
	endif
	if ret = 14
		add_talk #b#b我想要一件白色#c186506野火II型#n（15级枪手肩甲），可以不？
	endif
	if ret = 15
		add_talk #b#b我想要一件白色#c186506十字肩甲#n（15级祭司肩甲），可以不？
	endif
	if ret = 16
		add_talk #b#b我想要一件白色#c186506战士靴#n（16级战士鞋子），可以不？
	endif
	if ret = 17
		add_talk #b#b我想要一件白色#c186506符文靴#n（16级法师鞋子），可以不？
	endif
	if ret = 18
		add_talk #b#b我想要一件白色#c186506冒险靴#n（16级枪手鞋子），可以不？
	endif
	if ret = 19
		add_talk #b#b我想要一件白色#c186506主教靴#n（16级法师鞋子），可以不？
	endif
	
		add_talk #b#b将需要提交的物品拖至下方的框内
		
		NPC_COMMIT_OPT 901

	talk
endproc  

proc 901
	strvar ID fie gri ret
	GET_COMMIT_ITEM_INFO 1 ID
	
	GET_COMMIT_ITEM_INFO 2 fie
	;位置
	GET_COMMIT_ITEM_INFO 3 gri
	;格子
	
	get_task_var 4601 0 8 1 ret 
	if ret = 0
		if ID = 2019902
			DEC_ITEM_NUM_BYIDX 2019902 1 @fie @gri 1
			task_fini 4601 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的野战服？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 1
		if ID = 2029902
			DEC_ITEM_NUM_BYIDX 2029902 1 @fie @gri 1
			task_fini 4601 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的亚麻衣？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 2
		if ID = 2039902
			DEC_ITEM_NUM_BYIDX 2039902 1 @fie @gri 1
			task_fini 4601 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的狂战装？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 3
		if ID = 2049902
			DEC_ITEM_NUM_BYIDX 2049902 1 @fie @gri 1
			task_fini 4601 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的祷告装？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 4
		if ID = 2119902
			DEC_ITEM_NUM_BYIDX 2119902 1 @fie @gri 1
			task_fini 4601 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的精铁头盔？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 5
		if ID = 2129902
			DEC_ITEM_NUM_BYIDX 2129902 1 @fie @gri 1
			task_fini 4601 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的符文头巾？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 6
		if ID = 2139902
			DEC_ITEM_NUM_BYIDX 2139902 1 @fie @gri 1
			task_fini 4601 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的优雅软帽？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 7
		if ID = 2149902
			DEC_ITEM_NUM_BYIDX 2149902 1 @fie @gri 1
			task_fini 4601 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的十字纹饰？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 8
		if ID = 2419902
			DEC_ITEM_NUM_BYIDX 2419902 1 @fie @gri 1
			task_fini 4601 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的武装面具？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 9
		if ID = 2429902
			DEC_ITEM_NUM_BYIDX 2429902 1 @fie @gri 1
			task_fini 4601 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的元素护饰？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 10
		if ID = 2439902
			DEC_ITEM_NUM_BYIDX 2439902 1 @fie @gri 1
			task_fini 4601 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的热诚面饰？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 11
		if ID = 2449902
			DEC_ITEM_NUM_BYIDX 2449902 1 @fie @gri 1
			task_fini 4601 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的教徒面具？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 12
		if ID = 2219902
			DEC_ITEM_NUM_BYIDX 2219902 1 @fie @gri 1
			task_fini 4601 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的精铁肩甲？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 13
		if ID = 2229902
			DEC_ITEM_NUM_BYIDX 2229902 1 @fie @gri 1
			task_fini 4601 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的符文肩甲？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 14
		if ID = 1279902
			DEC_ITEM_NUM_BYIDX 1279902 1 @fie @gri 1
			task_fini 4601 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的野火II型？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 15
		if ID = 2249902
			DEC_ITEM_NUM_BYIDX 2249902 1 @fie @gri 1
			task_fini 4601 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的十字肩甲？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 16
		if ID = 2619902
			DEC_ITEM_NUM_BYIDX 2619902 1 @fie @gri 1
			task_fini 4601 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的战士靴？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 17
		if ID = 2629902
			DEC_ITEM_NUM_BYIDX 2629902 1 @fie @gri 1
			task_fini 4601 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的符文靴？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 18
		if ID = 2639902
			DEC_ITEM_NUM_BYIDX 2639902 1 @fie @gri 1
			task_fini 4601 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的冒险靴？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 19
		if ID = 2649902
			DEC_ITEM_NUM_BYIDX 2649902 1 @fie @gri 1
			task_fini 4601 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的主教靴？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
endproc

proc 910
	strvar ret
	
	dialog 3
	add_talk $_npc$:
	get_task_var 4602 0 8 1 ret	
	if ret = 0
		add_talk #b#b我想要一把白色#c186506金刚刀#n（11级战士主手武器），可以不？
	endif
	if ret = 1
		add_talk #b#b我想要一把白色#c186506精金刀#n（21级战士主手武器），可以不？
	endif
	if ret = 2
		add_talk #b#b我想要一把白色#c186506骑士刀#n（31级战士主手武器），可以不？
	endif
	if ret = 3
		add_talk #b#b我想要一把白色#c186506血腥锋芒#n（41级战士主手武器），可以不？
	endif
	if ret = 4
		add_talk #b#b我想要一把白色#c186506复仇利刃#n（51级战士主手武器），可以不？
	endif
	if ret = 5
		add_talk #b#b我想要一把白色#c186506秘银盾#n（17级战士副手盾牌），可以不？
	endif
	if ret = 6
		add_talk #b#b我想要一把白色#c186506白金盾#n（27级战士副手盾牌），可以不？
	endif
	if ret = 7
		add_talk #b#b我想要一把白色#c186506龙骨盾#n（37级战士副手盾牌），可以不？
	endif
	if ret = 8
		add_talk #b#b我想要一把白色#c186506狮鹫之盾#n（47级战士副手盾牌），可以不？
	endif
	if ret = 9
		add_talk #b#b我想要一把白色#c186506力量庇佑#n（57级战士副手盾牌），可以不？
	endif
	if ret = 10
		add_talk #b#b我想要一把白色#c186506赤杨杖#n（11级法师主手武器），可以不？
	endif
	if ret = 11
		add_talk #b#b我想要一把白色#c186506黑木杖#n（21级法师主手武器），可以不？
	endif
	if ret = 12
		add_talk #b#b我想要一把白色#c186506熔岩杖#n（31级法师主手武器），可以不？
	endif
	if ret = 13
		add_talk #b#b我想要一把白色#c186506战火之杖#n（41级法师主手武器），可以不？
	endif
	if ret = 14
		add_talk #b#b我想要一把白色#c186506光明权杖#n（51级法师主手武器），可以不？
	endif
	if ret = 15
		add_talk #b#b我想要一把白色#c186506洞察烛#n（17级法师副手法器），可以不？
	endif
	if ret = 16
		add_talk #b#b我想要一把白色#c186506血色薇#n（27级法师副手法器），可以不？
	endif
	if ret = 17
		add_talk #b#b我想要一把白色#c186506灵魂印记#n（37级法师副手法器），可以不？
	endif
	if ret = 18
		add_talk #b#b我想要一把白色#c186506精神深渊#n（47级法师副手法器），可以不？
	endif
	if ret = 19
		add_talk #b#b我想要一把白色#c186506合金指纹#n（57级法师副手法器），可以不？
	endif
	if ret = 20
		add_talk #b#b我想要一把白色#c186506铁膛II#n（11级枪手主手武器），可以不？
	endif
	if ret = 21
		add_talk #b#b我想要一把白色#c186506铁膛III#n（21级枪手主手武器），可以不？
	endif
	if ret = 22
		add_talk #b#b我想要一把白色#c186506涡轮I#n（31级枪手主手武器），可以不？
	endif
	if ret = 23
		add_talk #b#b我想要一把白色#c186506涡轮II#n（41级枪手主手武器），可以不？
	endif
	if ret = 24
		add_talk #b#b我想要一把白色#c186506涡轮III#n（51级枪手主手武器），可以不？
	endif
	if ret = 25
		add_talk #b#b我想要一把白色#c186506重锤II#n（17级枪手副手武器），可以不？
	endif
	if ret = 26
		add_talk #b#b我想要一把白色#c186506重锤III#n（27级枪手副手武器），可以不？
	endif
	if ret = 27
		add_talk #b#b我想要一把白色#c186506螺旋I#n（37级枪手副手武器），可以不？
	endif
	if ret = 28
		add_talk #b#b我想要一把白色#c186506螺旋II#n（47级枪手副手武器），可以不？
	endif
	if ret = 29
		add_talk #b#b我想要一把白色#c186506螺旋III#n（57级枪手副手武器），可以不？
	endif
	if ret = 30
		add_talk #b#b我想要一把白色#c186506铭文杖#n（11级祭司主手武器），可以不？
	endif
	if ret = 31
		add_talk #b#b我想要一把白色#c186506修士杖#n（21级祭司主手武器），可以不？
	endif
	if ret = 32
		add_talk #b#b我想要一把白色#c186506幻影杖#n（31级祭司主手武器），可以不？
	endif
	if ret = 33
		add_talk #b#b我想要一把白色#c186506祈祷之杖#n（41级祭司主手武器），可以不？
	endif
	if ret = 34
		add_talk #b#b我想要一把白色#c186506法典权杖#n（51级祭司主手武器），可以不？
	endif
	if ret = 35
		add_talk #b#b我想要一把白色#c186506星光风#n（17级祭司副手圣器），可以不？
	endif
	if ret = 36
		add_talk #b#b我想要一把白色#c186506圣律典#n（27级祭司副手圣器），可以不？
	endif
	if ret = 37
		add_talk #b#b我想要一把白色#c186506圣法阵#n（37级祭司副手圣器），可以不？
	endif
	if ret = 38
		add_talk #b#b我想要一把白色#c186506灵魂吐息#n（47级祭司副手圣器），可以不？
	endif
	if ret = 39
		add_talk #b#b我想要一把白色#c186506十字戒令#n（57级祭司副手圣器），可以不？
	endif
	
		add_talk #b#b将需要提交的物品拖至下方的框内
		
		NPC_COMMIT_OPT 911

	talk
endproc  

proc 911
	strvar ID fie gri ret
	GET_COMMIT_ITEM_INFO 1 ID
	
	GET_COMMIT_ITEM_INFO 2 fie
	;位置
	GET_COMMIT_ITEM_INFO 3 gri
	;格子
	
	get_task_var 4602 0 8 1 ret 
	if ret = 0
		if ID = 1019902
			DEC_ITEM_NUM_BYIDX 1019902 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的金刚刀？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 1
		if ID = 1019903
			DEC_ITEM_NUM_BYIDX 1019903 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的精金刀？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 2
		if ID = 1019904
			DEC_ITEM_NUM_BYIDX 1019904 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的骑士刀？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 3
		if ID = 1019905
			DEC_ITEM_NUM_BYIDX 1019905 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的血腥锋芒？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 4
		if ID = 1019906
			DEC_ITEM_NUM_BYIDX 1019906 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的复仇利刃？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 5
		if ID = 1069902
			DEC_ITEM_NUM_BYIDX 1069902 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的秘银盾？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 6
		if ID = 1069903
			DEC_ITEM_NUM_BYIDX 1069903 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的白金盾？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 7
		if ID = 1069904
			DEC_ITEM_NUM_BYIDX 1069904 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的龙骨盾？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 8
		if ID = 1069905
			DEC_ITEM_NUM_BYIDX 1069905 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的狮鹫之盾？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 9
		if ID = 1069906
			DEC_ITEM_NUM_BYIDX 1069906 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的力量庇佑？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 10
		if ID = 1109902
			DEC_ITEM_NUM_BYIDX 1109902 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的赤杨杖？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 11
		if ID = 1109903
			DEC_ITEM_NUM_BYIDX 1109903 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的黑木杖？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 12
		if ID = 1109904
			DEC_ITEM_NUM_BYIDX 1109904 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的熔岩杖？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 13
		if ID = 1109905
			DEC_ITEM_NUM_BYIDX 1109905 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的战火之杖？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 14
		if ID = 1109906
			DEC_ITEM_NUM_BYIDX 1109906 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的光明权杖？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 15
		if ID = 1189902
			DEC_ITEM_NUM_BYIDX 1189902 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的洞察烛？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 16
		if ID = 1189903
			DEC_ITEM_NUM_BYIDX 1189903 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的血色薇？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 17
		if ID = 1189904
			DEC_ITEM_NUM_BYIDX 1189904 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的灵魂印记？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 18
		if ID = 1189905
			DEC_ITEM_NUM_BYIDX 1189905 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的精神深渊？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 19
		if ID = 1189906
			DEC_ITEM_NUM_BYIDX 1189906 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的合金指纹？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 20
		if ID = 1209902
			DEC_ITEM_NUM_BYIDX 1209902 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的铁膛II？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 21
		if ID = 1209903
			DEC_ITEM_NUM_BYIDX 1209903 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的铁膛III？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 22
		if ID = 1209904
			DEC_ITEM_NUM_BYIDX 1209904 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的涡轮I？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 23
		if ID = 1209905
			DEC_ITEM_NUM_BYIDX 1209905 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的涡轮II？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 24
		if ID = 1209906
			DEC_ITEM_NUM_BYIDX 1209906 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的涡轮III？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 25
		if ID = 1289902
			DEC_ITEM_NUM_BYIDX 1289902 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的重锤II？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 26
		if ID = 1289903
			DEC_ITEM_NUM_BYIDX 1289903 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的重锤III？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 27
		if ID = 1289904
			DEC_ITEM_NUM_BYIDX 1289904 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的螺旋I？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 28
		if ID = 1289905
			DEC_ITEM_NUM_BYIDX 1289905 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的螺旋II？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 29
		if ID = 1289906
			DEC_ITEM_NUM_BYIDX 1289906 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的螺旋III？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 30
		if ID = 1509902
			DEC_ITEM_NUM_BYIDX 1509902 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的铭文杖？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 31
		if ID = 1509903
			DEC_ITEM_NUM_BYIDX 1509903 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的修士杖？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 32
		if ID = 1509904
			DEC_ITEM_NUM_BYIDX 1509904 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的幻影杖？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 33
		if ID = 1509905
			DEC_ITEM_NUM_BYIDX 1509905 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的祈祷之杖？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 34
		if ID = 1509906
			DEC_ITEM_NUM_BYIDX 1509906 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的法典权杖？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 35
		if ID = 1589902
			DEC_ITEM_NUM_BYIDX 1589902 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的星光风？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 36
		if ID = 1589903
			DEC_ITEM_NUM_BYIDX 1589903 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的圣律典？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
	endif
	endif
	if ret = 37
		if ID = 1589904
			DEC_ITEM_NUM_BYIDX 1589904 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的圣法阵？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 38
		if ID = 1589905
			DEC_ITEM_NUM_BYIDX 1589905 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的灵魂吐息？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 39
		if ID = 1589906
			DEC_ITEM_NUM_BYIDX 1589906 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是白色品质的十字戒令？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
endproc

proc 920
	strvar ret
	
	dialog 3
	add_talk $_npc$:
	get_task_var 4603 0 8 1 ret	
	if ret = 0
		add_talk #b#b我想要一只#c186506嘟嘟鸟宠物蛋#n，可以不？
	endif
	if ret = 1
		add_talk #b#b我想要一只#c186506野猪宠物蛋#n，可以不？
	endif
	if ret = 2
		add_talk #b#b我想要一只#c186506孢子人宠物蛋#n，可以不？
	endif
	if ret = 3
		add_talk #b#b我想要一只#c186506沼泽巨鳄宠物蛋#n，可以不？
	endif
	if ret = 4
		add_talk #b#b我想要一只#c186506秃鹫宠物蛋#n，可以不？
	endif
	if ret = 5
		add_talk #b#b我想要一只#c186506幽光幼龙宠物蛋#n，可以不？
	endif
	if ret = 6
		add_talk #b#b我想要一只#c186506剑齿虎宠物蛋#n，可以不？
	endif
	if ret = 7
		add_talk #b#b我想要一只#c186506蝎子宠物蛋#n，可以不？
	endif
	
		add_talk #b#b将需要提交的物品拖至下方的框内
		
		NPC_COMMIT_OPT 921

	talk
endproc  

proc 921
	strvar ID fie gri ret
	GET_COMMIT_ITEM_INFO 1 ID
	
	GET_COMMIT_ITEM_INFO 2 fie
	;位置
	GET_COMMIT_ITEM_INFO 3 gri
	;格子
	
	get_task_var 4603 0 8 1 ret 
	if ret = 0
		if ID = 5610100
			DEC_ITEM_NUM_BYIDX 5610100 1 @fie @gri 1
			task_fini 4603 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是嘟嘟鸟宠物蛋？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 1
		if ID = 5610101
			DEC_ITEM_NUM_BYIDX 5610101 1 @fie @gri 1
			task_fini 4603 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是野猪宠物蛋？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 2
		if ID = 5610102
			DEC_ITEM_NUM_BYIDX 5610102 1 @fie @gri 1
			task_fini 4603 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是孢子人宠物蛋？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 3
		if ID = 5611116
			DEC_ITEM_NUM_BYIDX 5611116 1 @fie @gri 1
			task_fini 4603 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是沼泽巨鳄宠物蛋？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 4
		if ID = 5610104
			DEC_ITEM_NUM_BYIDX 5610104 1 @fie @gri 1
			task_fini 4603 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是秃鹫宠物蛋？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 5
		if ID = 5610106
			DEC_ITEM_NUM_BYIDX 5610106 1 @fie @gri 1
			task_fini 4603 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是幽光幼龙宠物蛋？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 6
		if ID = 5610107
			DEC_ITEM_NUM_BYIDX 5610107 1 @fie @gri 1
			task_fini 4603 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是剑齿虎宠物蛋？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 7
		if ID = 5610105
			DEC_ITEM_NUM_BYIDX 5610105 1 @fie @gri 1
			task_fini 4603 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg 你确定这货是蝎子宠物蛋？不带这么忽悠人的！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
endproc

proc 999
	add_sys_msg_start
;	add_sys_msg 成功上交物品，任务完成
	set_sys_msg_mode 1 0 0
	msg_info
	PLAYER_CLOSE_DIALOG 0 0
endproc
