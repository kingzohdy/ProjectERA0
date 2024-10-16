;npc 杜康

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b在此中秋佳节之际，还有什么能够比月饼更能传达情意的呢？你投之以月饼，我报之以美酒，还有什么比这更深情惬意的呢？
	add_talk #b#b
	add_talk #b#b#c186506友情提示：活动期间，可每天可在月饼大使处领取月饼礼盒。#n
		
	strvar res
	GET_TASK_VAR 4631 4 32 12 res
	
	strvar er1 er2 er3 er4
	TASK_STAT 4631 er1 er2
	if @er1 = 0 and @er2 = 2
		if @res = 0 and @12 = 0
			option 206 开始拼酒
		else
			option 200 #给块月饼，咱们再来一次！
		endif
	endif
	
	task_stat 4632 er3 er4
	;未接任务
	if @er3 = 0 and @er4 = 0 and @er1 = 0 and @er2 = 0
		GET_PLAYER_BASE_ATT 13 0 19 0
		if @13 = 0 and @19 >= 40
			option 200 #共尝月饼
		endif
	endif
		
	strvar num
	TASK_STAT 4632 13 14
	if @13 = 0 and @14 = 2
		rand num 14
		switch num
			case 0
				option 400 #开始对诗
			endcase
			case 1
				option 401 #开始对诗
			endcase
			case 2
				option 402 #开始对诗
			endcase
			case 3
				option 403 #开始对诗
			endcase
			case 4
				option 404 #开始对诗
			endcase
			case 5
				option 405 #开始对诗
			endcase
			case 6
				option 406 #开始对诗
			endcase
			case 7
				option 407 #开始对诗
			endcase
			case 8
				option 408 #开始对诗
			endcase
			case 9
				option 409 #开始对诗
			endcase
			case 10
				option 410 #开始对诗
			endcase
			case 11
				option 411 #开始对诗
			endcase
			case 12
				option 412 #开始对诗
			endcase
			case 13
				option 413 #开始对诗
			endcase
			case 14
				option 414 #开始对诗
			endcase
		endswitch
	endif
	
		task_stat 4631 60 70
		if @60 = 0 and @70 = 1
			TASK_ADD_OPTION 4631 0 305
		endif
		
		task_stat 4632 80 90
		if @80 = 0 and @90 = 1
			TASK_ADD_OPTION 4632 0 306
		endif
		
	talk
endproc 

proc 305
	TASK_DO_OPTION 4631 0
endproc

proc 306
	TASK_DO_OPTION 4632 0
endproc

proc 200	
	dialog 3
	add_talk $_npc$:
	add_talk #b#b很高兴你愿意与我分享如此美味的月饼，我不挑食哦，任意口味的月饼都可以，不过，我不建议你将富贵月饼赠与我，因为富贵月饼里面有一张藏宝图，那里埋藏着无数的宝石碎片与银币。
	add_talk #b#b
	add_talk #b#b将需要提交的物品拖至下方的框内
	
	NPC_COMMIT_OPT 300
	talk
endproc

proc 300
	strvar ret1
	GET_PACK_SPACE 0 ret1
	if @ret1 = 0
		add_sys_msg_start
		add_sys_msg 背包空间不足，无法换取
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	strvar itemId pos index ret ret2
	strvar iflag
	var iflag 0
	GET_COMMIT_ITEM_INFO 1 itemId
	if itemId = -1
		add_sys_msg_start
		add_sys_msg 您还没有提交物品
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	if @itemId = 5030109 or @itemId = 5012130 or @itemId = 5012131 or @itemId = 5012132
		var iflag 1
	endif
	
	if @itemId = 5012134 or @itemId = 5012135 or @itemId = 5012133
		var iflag 1
	endif
	
	if @iflag = 0
		add_sys_msg_start
		add_sys_msg 中秋佳节不收礼，只是想吃几块美味月饼而已
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	GET_COMMIT_ITEM_INFO 2 pos
	GET_COMMIT_ITEM_INFO 3 index

	DEC_ITEM_NUM_BYIDX @itemId 1 @pos @index ret
	if ret = 0		
		TASK_STAT 4631 15 70
		if @70 = 2 and @15 = 0
			strvar res
			GET_TASK_VAR 4631 4 32 12 res
			if @res = 1 and @12 = 0
				SET_TASK_VAR 4631 4 32 0 16
			endif
			;这个非常重要，因为提交对话框提交完毕。又会把属性DIALOG_STAT_COMMIT_OPT赋值回去，
			;这里把stSEnv.pstPlayer->stOnline.cDialogStatOld = DIALOG_STAT_OPT;  为3强制赋值
			SET_DLG_OLD_STAT 3
			call 1
		elseif @70 = 0 and @15 = 0
			add_sys_msg_start
			add_sys_msg 杜康已将回赠放在了你的背包中
			set_sys_msg_mode 3 0 0
			msg_info
			
			strvar rd
			rand rd 100
			if @rd >= 0 and @rd < 50
				ADD_BIND_ITEM_NUM_INPAK 5030111 1 1 13
				strvar npcmid
				GET_MAP_NPC_ID  302173 npcmid 1
				MON_SPEECH  @npcmid 0 0 1 你赠我以月饼，我回你以诗签，希望咱们能够逍遥快活一生！
			else
				ADD_BIND_ITEM_NUM_INPAK 5030112 1 1 23
				strvar npcmid
				GET_MAP_NPC_ID  302173 npcmid 1
				MON_SPEECH  @npcmid 0 0 1 你赠我以月饼，我回你以酒樽，希望咱们能够逍遥快活一生！
			endif
			SET_DLG_OLD_STAT 3
			call 1
		else
		endif
	endif
endproc

;拼酒
;4631 任务ID
proc 100
	add_sys_msg_start
	add_sys_msg 你已经喝醉了
	set_sys_msg_mode 4 0 0
	msg_info
	
	;通知客户端进行屏幕模糊
	pinjiu_screen_vague 1
	
	PLAYER_CLOSE_DIALOG1 0 1
	
	strvar npcmid
	GET_MAP_NPC_ID  302173 npcmid 1
	MON_SPEECH  @npcmid 0 2 1 小样，跟我拼酒，找虐呢！
	
	SET_TASK_VAR 4631 0 32 60 10
	SET_TASK_VAR 4631 4 32 1 16
	SET_TASK_VAR 4631 8 32 0 26
	SET_TASK_VAR 4631 12 32 0 27
	SET_TASK_VAR 4631 16 32 0 28
endproc

;玩家赢了
proc 101
	add_sys_msg_start
	add_sys_msg $_npc$已经醉的不省人事了！你赢了！
	set_sys_msg_mode 4 0 0
	msg_info
	
	task_fini 4631 1
endproc

;玩家输了
proc 102
	add_sys_msg_start
	add_sys_msg $_npc$可是出了名的千杯不醉呢，这才喝到哪啊，看来你太小看对手了！
	set_sys_msg_mode 4 0 0
	msg_info
	
	SET_TASK_VAR 4631 0 32 60 10
	SET_TASK_VAR 4631 4 32 1 16
	SET_TASK_VAR 4631 8 32 0 26
	SET_TASK_VAR 4631 12 32 0 27
	SET_TASK_VAR 4631 16 32 0 28
	
	PLAYER_CLOSE_DIALOG1 0 1
endproc

;再来一杯
proc 201
	;每次点击“再来一杯”只有 x% 会变，每次点数值随机 1-12（最初是60%）
	rand 20 12
	add 20 1
	
	GET_TASK_VAR 4631 0 32 11 num
	add num @20
	SET_TASK_VAR 4631 0 32 @num 10
	
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
	GET_TASK_VAR 4631 16 32 25 jiuliangval
	GET_TASK_VAR 4631 0 32 11 num	
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
	GET_TASK_VAR 4631 0 32 11 num
	if @num = 0
		SET_TASK_VAR 4631 0 32 60 10
		var num 60
	endif
	
	;idx 12 记录npc随机酒量标志   flag 为 1时，表示已经取得  ,0时，则马上取
	GET_TASK_VAR 4631 12 32 21 flag 
	if @flag = 0
		;npc酒量  90-97
		rand 14 8
		add 14 90
		
		if @14 < 95 and @14 >= 90
			;杜康的情绪不高，看样子似乎很容易就会喝醉
			SET_TASK_VAR 4631 8 32 1 10
		elseif @14 >= 95 and @14 <= 97
			;杜康酒兴大增，越喝越来劲，看样子似乎能够千杯不醉
			SET_TASK_VAR 4631 8 32 2 10
		else
			SET_TASK_VAR 4631 8 32 0 10
		endif
		
		SET_TASK_VAR 4631 12 32 1 22
		;idx 为 16 设置酒量值
		SET_TASK_VAR 4631 16 32 @14 23
	endif
	
	
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b一杯酒下肚，你已经有$@num$%的醉意，还要继续喝吗？
	
	;idx 8 该显示哪句   
	GET_TASK_VAR 4631 8 32 15 jiuliang
	if @jiuliang = 1
		add_talk #b#b杜康的情绪不高，看样子似乎很容易就会喝醉
	elseif @jiuliang = 2
		add_talk #b#b杜康酒兴大增，越喝越来劲，看样子似乎能够千杯不醉
	else
	endif
	
	option 201 再来一杯
	option 202 #不喝了，来看看结果吧！
	
	talk
endproc



proc 400
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b明月几时有
	add_talk #b#b（请接下句）
	
		option 500 #把酒问青天
		option 501 #把酒问室友
		option 501 #把酒问女神
		option 501 #把酒问基友
		
	talk
endproc

proc 401
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b但愿人长久
	add_talk #b#b（请接下句）
	
		option 501 #光棍不再有
		option 500 #千里共婵娟
		option 501 #千里啪啪啪
		option 501 #再闷一杯酒 
		
	talk
endproc

proc 402
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b花间一壶酒，独酌无相亲
	add_talk #b#b（请接下句）
	
		option 501 #白日依山尽，黄河入海流
		option 501 #月既不解饮，影徒随我身
		option 500 #举杯邀明月，对影成三人
		option 501 #青山遮不住，毕竟东流去
		
	talk
endproc

proc 403
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b海上生明月，天涯共此时
	add_talk #b#b（请接下句）
	
		option 501 #片云天共远，永夜月同孤
		option 501 #灭烛怜光满，披衣觉露滋
		option 501 #明月松间照，清泉石上流
		option 500 #情人怨遥夜，意夕起相思
		
	talk
endproc

proc 404
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b明月出天山，苍茫云海间
	add_talk #b#b（请接下句）
	
		option 501 #千山鸟飞绝，万径人踪灭
		option 501 #汉下白登道，胡窥青海湾
		option 500 #长风几万里，吹度玉门关
		option 501 #愿得一人心，白首不相离
		
	talk
endproc

proc 405
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b床前明月光，凝是地上霜
	add_talk #b#b（请接下句）
	
		option 501 #举头望明月，地上鞋两双
		option 500 #举头望明月，低头思故乡
		option 501 #举头望明月，蚊子配成双
		option 501 #举头望明月，低头闻饼香
		
	talk
endproc

proc 406
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b暮云收尽溢清寒，银汉无声转玉盘
	add_talk #b#b（请接下句）
	
		option 500 #此生此夜不长好，明月明年何处看
		option 501 #此情可待成追忆，只是当时已惘然
		option 501 #相见时难别亦难，东风无力百花钱
		option 501 #羌笛何须怨杨柳,春风不度玉门关
		
	talk
endproc

proc 407
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b昔年八月十五夜，曲江池畔杏园边
	add_talk #b#b（请接下句）
	
		option 501 #今年八月十五夜，散发提壶弄扁舟
		option 500 #今年八月十五夜，湓浦沙头水馆前
		option 501 #今年八月十五夜，直挂云帆济沧海
		option 501 #今年八月十五夜，此时无声胜有声
		
	talk
endproc

proc 408
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b西北望乡何处是，东南见月几回圆
	add_talk #b#b（请接下句）
	
		option 501 #天生我材必有用，千金散尽还复来
		option 501 #朝辞白帝彩云间，千里江陵一日还
		option 500 #昨风一吹无人会，今夜清光似往年
		option 501 #凤凰台上凤凰游，凤去台空江自流
		
	talk
endproc

proc 409
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b中庭地白树栖鸦，冷露无声湿桂花
	add_talk #b#b（请接下句）
	
		option 501 #呼儿将出换美酒，与尔同销万古愁
		option 501 #今夜举杯邀明月，不知谁人伴我还
		option 501 #商女不知亡国恨，隔岸犹唱后庭花
		option 500 #今夜月明人尽望，不知秋思落谁家
		
	talk
endproc

proc 410
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b明月明月明月
	add_talk #b#b（请接下句）
	
		option 501 #美女美女美女
		option 501 #帅哥帅哥帅哥
		option 500 #争奈乍圆还缺
		option 501 #争奈无人相伴
		
	talk
endproc

proc 411
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b露从今夜白
	add_talk #b#b（请接下句）
	
		option 501 #月是国外明
		option 500 #月是故乡明
		option 501 #月是他乡明
		option 501 #月是古时明
		
	talk
endproc

proc 412
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b明月未出群山高，瑞光千丈生白毫
	add_talk #b#b（请接下句）
	
		option 500 #一杯未尽银阙涌，乱云脱坏如崩涛
		option 501 #谁为天公洗眸子，应费明河千斛水
		option 501 #遂令冷看世间人，照我湛然心不起
		option 501 #春潮带雨晚来急，野渡无人舟自横
		
	talk
endproc

proc 413
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b把酒问青天
	add_talk #b#b（请接上句）
	
		option 501 #老婆几时有
		option 500 #明月几时有
		option 501 #银币几时有
		option 501 #极品几时有
		
	talk
endproc

proc 414
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b月有阴晴圆缺
	add_talk #b#b（请接上句）
	
		option 501 #女有环肥燕瘦
		option 501 #男有高矮穷挫
		option 500 #人有悲欢离合
		option 501 #人有生老病死
		
	talk
endproc

proc 500
	TASK_FINI 4632 0
	
	strvar npcID
	GET_MAP_NPC_ID 302173 npcID 0
	mon_speech @npcID 2 0 1 真乃经典名句也！佩服佩服！
	
endproc

proc 501
	;COMMIT_TASK 4632 0
	TASK_FINI 4632 0
	
	;add_sys_msg_start 
	;add_sys_msg 未能对出诗句，无法获得奖励！
	;set_sys_msg_mode 4 0 0
	;msg_info
	
	strvar npcID
	GET_MAP_NPC_ID 302173 npcID 0
	mon_speech @npcID 2 0 1 这对仗…倒也算是工整，只是…看来我要好好推敲推敲了！
endproc
