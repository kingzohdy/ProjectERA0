;海滩NPC米尔斯

proc 1
	get_crayfish_num_oneday 0 10
	dialog 1
	add_talk $_npc$:
    add_talk #b#b亲，欢迎来到阳光海滩哦！！#004
    add_talk #b#b亲，从这儿向右边走，就是沙滩了哦！#003
    add_talk #b#b亲，那儿有无数的帅哥美女哟！#002
    add_talk #b#b亲，还有每天1小时的充足日光浴经验哟！#009
	task_add_option 2246 1 100
	get_role_var 6 8 0 20
	if 20 = 1
		option 200 #(买卖)打开商店
	endif
	if @10 < 5
		option 300 上交美味小龙虾
	endif
	
	option 400 回收珍珠
	
	talk
	
endproc 

proc 3
	GET_TRIG_TASK 0 10
	if @10 = 2246
		set_role_var 6 8 1 0
	endif
endproc

PROC 100
	task_do_option 2246 1
ENDPROC

proc 200
	OPEN_NPC_SHOP
endproc

proc 300
	get_crayfish_num_oneday 0 10
	var 11 5
	sub 11 @10
	dialog 1
	add_talk $_npc$:
    add_talk #b#b什么，你有美味小龙虾，快快快，赶紧拿来，我会给你大把的奖励。
    add_talk #b#b不过，听说小龙虾吃多了会让身材变形，不然，你就给我5个吧，我的意思是，每天5个哟。
    add_talk #b#b
	if 10 = 5
		add_talk #b#b#c186506已经吃了5个美味小龙虾，真想再吃一个，哦，不行，为了我的魔鬼身材，我要Hold住！
	else
		add_talk #b#b#c186506你今天才给了我$@10$个，剩余的$@11$个，不打算给我了吗？
	endif
	if 11 >= 1
		option 320 提交1只
	endif
	if 11 = 5
		option 330 提交5只
	endif
	option 1 返回
	talk
endproc

;提交1只
proc 320
	;设置选项提交数
	var 100 1
	GET_ITEM_NUM_INPAK 5052201 1 2
	;判断背包中的物品跟 选项提交数的关系
	if 2 < @100
		;背包中小龙虾数量 大于0 才冒泡提示
		if 2 > 0 
			;npc冒泡
			strvar Mid
			GET_MAP_NPC_ID 307004 Mid 1
			MON_SPEECH @Mid 1 0 1 欺骗美女感情你不觉得惭愧吗？我的美味小龙虾…
		endif
		;提示
		add_sys_msg_start
		add_sys_msg 小龙虾数量不足，请先去沙滩捕捉再来提交
		set_sys_msg_mode 4 0 0
		msg_info
		PLAYER_CLOSE_DIALOG1 0 0
		return
	endif
	;获取当天已经提交的数量 10
	GET_CRAYFISH_NUM_ONEDAY 0 10
	;计算当天还需要提交的数量 11
	var 11 5
	sub 11 @10
	dialog 1
	add_talk $_npc$:
	;判断当天还需要提交的数量11 跟 选项操作提交数的关系100
	if 100 <= @11
		commit_crayfish_num_once 0 @100 40
		DEC_ITEM_NUM_INPAK 5052201 @100 0
		ADD_EXP 0 @40 0
		;12 此次操作后当天还需要提交的数量
		var 12 @11
		sub 12 @100
		add_talk #b#b 成功提交：$@100$只
		add_talk #b#b 获得经验：$@40$
		if 12 > 0 
			add_talk #b#b 还可提交：$@12$只
		endif
		if 100 <= @12
			option 320 继续提交
			option 300 返回
		else
			option 350 #关闭面板
		endif
	endif
	talk
endproc

;提交5只
proc 330
	;设置选项提交数
	var 100 5
	GET_ITEM_NUM_INPAK 5052201 1 2
	;判断背包中的物品跟 选项提交数的关系
	if 2 < @100
		;背包中小龙虾数量 大于0 才冒泡提示
		if 2 > 0
			;npc冒泡
			strvar Mid
			GET_MAP_NPC_ID 307004 Mid 1
			MON_SPEECH @Mid 1 0 1 欺骗美女感情你不觉得惭愧吗？我的美味小龙虾…
		endif
		;提示
		add_sys_msg_start
		add_sys_msg 小龙虾数量不足，请先去沙滩捕捉再来提交
		set_sys_msg_mode 4 0 0
		msg_info
		PLAYER_CLOSE_DIALOG1 0 0
		return
	endif
	;获取当天已经提交的数量 10
	GET_CRAYFISH_NUM_ONEDAY 0 10
	;计算当天还需要提交的数量 11
	var 11 5
	sub 11 @10
	dialog 1
	add_talk $_npc$:
	;判断当天还需要提交的数量11 跟 选项操作提交数的关系100
	if 100 <= @11
		commit_crayfish_num_once 0 @100 40
		DEC_ITEM_NUM_INPAK 5052201 @100 0
		ADD_EXP 0 @40 0
		;12 此次操作后当天还需要提交的数量
		var 12 @11
		sub 12 @100
		add_talk #b#b 成功提交：$@100$只
		add_talk #b#b 获得经验：$@40$
		if 12 > 0 
			add_talk #b#b 还可提交：$@12$只
		endif
		if 100 <= @12
			option 320 继续提交
		endif
	endif
	option 350 #关闭面板
	talk
endproc

proc 350
	;PLAYER_CLOSE_DIALOG 0 0
endproc


proc 400
	strvar Ret Sex Level Job
	dialog 1
	add_talk $_npc$:
    add_talk #b#b你有多余的珍珠吗？给我吧！我愿意出高价购买哟！不要怕我没有钱，只怕你没有货！
    add_talk #b#b你可以通过航海大亨活动或是在盛夏商人那里获得小海蚌，然后把它养大，你就会获得好多好多珍珠！
	
	 get_player_base_att  Ret Sex Level Job
	 
		option 401 #晶莹珍珠 1000银币/个
		
	if @Level >= 80
		option 402 #莹绿珍珠 1200银币/个
		option 403 #海蓝珍珠 1400银币/个
		option 404 #暖紫珍珠 1600银币/个
	elseif @Level >= 70
		option 402 #莹绿珍珠 1200银币/个
		option 403 #海蓝珍珠 1400银币/个
	elseif @Level >= 60
		option 402 #莹绿珍珠 1200银币/个
	else
	
	endif	
	talk
endproc

proc 401
	GET_ITEM_NUM_INPAK 5060053 1 40
	if @40 < 1
		add_sys_msg_start
		add_sys_msg 背包中没有晶莹珍珠
		set_sys_msg_mode 4 0 0
		msg_info
		return 
    endif
	
	strvar num
	var num @40
	MUL num 1000
	ADD_MONEY 0 @num 20 123 
	if 123 < 0
		add_sys_msg_start
		add_sys_msg 您包裹中的银币已经达到上限
		set_sys_msg_mode 3 0 0
		msg_info
	else
		DEC_ITEM_NUM_INPAK 5060053 @40 1
		
		add_sys_msg_start
		add_sys_msg 卖出晶莹珍珠$@40$个，获得银币$@num$
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc

proc 402
	GET_ITEM_NUM_INPAK 5060054 1 40
	if @40 < 1
		add_sys_msg_start
		add_sys_msg 背包中没有莹绿珍珠
		set_sys_msg_mode 4 0 0
		msg_info
		return 
    endif
	
	strvar num
	var num @40
	MUL num 1200
	ADD_MONEY 0 @num 20 123 
	if 123 < 0
		add_sys_msg_start
		add_sys_msg 您包裹中的银币已经达到上限
		set_sys_msg_mode 3 0 0
		msg_info
	else
		DEC_ITEM_NUM_INPAK 5060054 @40 1
		
		add_sys_msg_start
		add_sys_msg 卖出莹绿珍珠$@40$个，获得银币$@num$
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc

proc 403
	GET_ITEM_NUM_INPAK 5060055 1 40
	if @40 < 1
		add_sys_msg_start
		add_sys_msg 背包中没有海蓝珍珠
		set_sys_msg_mode 4 0 0
		msg_info
		return 
    endif
	
	strvar num
	var num @40
	MUL num 1400
	ADD_MONEY 0 @num 20 123 
	if 123 < 0
		add_sys_msg_start
		add_sys_msg 您包裹中的银币已经达到上限
		set_sys_msg_mode 3 0 0
		msg_info
	else
		DEC_ITEM_NUM_INPAK 5060055 @40 1
		
		add_sys_msg_start
		add_sys_msg 卖出海蓝珍珠$@40$个，获得银币$@num$
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc

proc 404
	GET_ITEM_NUM_INPAK 5060056 1 40
	if @40 < 1
		add_sys_msg_start
		add_sys_msg 背包中没有暖紫珍珠
		set_sys_msg_mode 4 0 0
		msg_info
		return 
    endif
	
	strvar num
	var num @40
	MUL num 1600
	ADD_MONEY 0 @num 20 123 
	if 123 < 0
		add_sys_msg_start
		add_sys_msg 您包裹中的银币已经达到上限
		set_sys_msg_mode 3 0 0
		msg_info
	else
		DEC_ITEM_NUM_INPAK 5060056 @40 1
		
		add_sys_msg_start
		add_sys_msg 卖出暖紫珍珠$@40$个，获得银币$@num$
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc
