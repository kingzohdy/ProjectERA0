

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b这里出售许多外面买不到的珍品哦。
	add_talk #b#b可通过#c186506奥斯卡#n和#c186506布鲁鲁#n获得万圣节糖果兑换。
	add_talk #b#b#c186506奥斯卡：#n#c0181f908:00-24:00#n，每10分钟随机出现在某些地图中，找到他并用【万圣糖果礼券】与交换便会获得7天伯爵时装或者万圣节糖果。
	add_talk #b#b#cFF4e00可以在我这里查询奥斯卡出现的位置哦。#n	
	add_talk #b#b#c186506布鲁鲁：#n#c0181f919:00-23:00#n，分别在时间城或细语丘陵、冰岛、流金沙漠、落日荒野、越雾雨林的安全区，每半小时可向他免费领取1个万圣节糖果。
	add_talk #b#b#cFF4e00每一个布鲁鲁共享糖果的领取次数。#n
	
		OPTION 110 #(买卖)打开商店
		
	strvar ret
	
		GET_EVENT_STAT ret 104
		if ret = 1
			OPTION 200 #奥斯卡跑哪去了？
		endif

	talk
endproc 


PROC 110
	OPEN_NPC_SHOP
ENDPROC

PROC 200
	strvar sj tk ql bd xy sm hy yl ld lg ms

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c186506奥斯卡#n每10分钟随机出现在某些地图中，找到他并用【万圣糖果礼盒】与他交换便会获得7天伯爵时装或者万圣节糖果。
	add_talk #c186506【奥斯卡】位置：#n
	
	GET_EVENT_VAR_BY_ID 104 0 32 0 sj
	GET_EVENT_VAR_BY_ID 104 4 32 0 tk
	GET_EVENT_VAR_BY_ID 104 8 32 0 ql
	GET_EVENT_VAR_BY_ID 104 12 32 0 bd
	GET_EVENT_VAR_BY_ID 104 16 32 0 xy
	GET_EVENT_VAR_BY_ID 104 20 32 0 sm
	GET_EVENT_VAR_BY_ID 104 24 32 0 hy
	GET_EVENT_VAR_BY_ID 104 28 32 0 yl
	GET_EVENT_VAR_BY_ID 104 32 32 0 ld
	GET_EVENT_VAR_BY_ID 104 36 32 0 lg
	GET_EVENT_VAR_BY_ID 104 40 32 0 ms
	
		if @sj = 1000
			add_talk #b#b——#c186506时间城#n
		endif
		if @bd = 2202
			add_talk #b#b——#c186506冰岛#n
		endif
		if @xy = 2200
			add_talk #b#b——#c186506银风雪原#n
		endif
		if @sm = 2300
			add_talk #b#b——#c186506流金沙漠#n
		endif
		if @hy = 2400
			add_talk #b#b——#c186506落日荒野#n
		endif
		if @yl = 2500
			add_talk #b#b——#c186506越雾雨林#n
		endif
		if @ld = 2104
			add_talk #b#b——#c186506荫川林地#n
		endif
		if @lg = 2201
			add_talk #b#b——#c186506风回裂谷#n
		endif
		if @ms = 3404
			add_talk #b#b——#c186506鸣沙陷地#n
		endif
		if @sj = 0 and @tk = 0 and @ql = 0 and @bd = 0 and @xy = 0 and @sm = 0 and @hy = 0 and @yl = 0 and @ld = 0 and @lg = 0 and @ms = 0
			add_talk #b#b#c186506没人知道奥斯卡去哪了，也许他已再次踏上了新的旅途#
		endif
	
	OPTION 1 #返回
	
	TALK
ENDPROC

