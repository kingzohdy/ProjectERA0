;跨服公会战副本    ,枢纽怪 2,2

proc 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b枢纽是决定战斗胜负的关键！
	add_talk #b
	add_talk 战场胜负规则#c0181f9（战场结束时）：
	add_talk #b#b#c186506当场中只剩一个公会枢纽，枢纽存在的公会阵营胜出，最先被摧毁枢纽的公会为第3名。
	add_talk #b#b#c186506当场中剩余枢纽数＞1，则按照枢纽血量多少排列名次，血量最多的公会胜出。
	add_talk #b
	add_talk 枢纽紧急保护措施：
	add_talk #b#b#c6d00d2初级保护：受到的伤害减少50%，持续15秒
	add_talk #b#b#c6d00d2高级保护：受到的伤害减少95%，持续15秒
	option 100 #初级保护
	option 200 #高级保护

	option 400 #缩短冷却时间
	option 300 #随便看看,再见
	talk

;;	add_sys_msg_start
;;	add_sys_msg xyz
;;	set_sys_msg_mode 1 0 0
;;	map_msg_info

endproc



proc 100
	; 初级保护
;	call 301
	strvar cost
	var cost 0
	if cost <= 0
		call 101
	else
		choose 1 101 302 开启初级保护消耗0 金币，枢纽受到的伤害减少50%，持续15秒，确认开启？
	endif

endproc

proc 101
	; 初级保护


	;状态id
	set_call_var 100 3211

	;状态等级
	set_call_var 101 1

	;持续时间
	set_call_var 102 15

	;消耗
	set_call_var 103 0

	;冷却时间
	set_call_var 104 600

	;货币类型
	set_call_var 105 60
	
	call 301
endproc
proc 200
	; 高级保护
;	call 301
	strvar cost
	var cost 50
	if cost <= 0
		call 201
	else
		choose 1 201 302 开启高级保护消耗50 金币，枢纽受到的伤害减少95%，持续15秒，确认开启？
	endif

endproc

proc 201
	; 高级保护


	;状态id
	set_call_var 100 3211

	;状态等级
	set_call_var 101 2

	;持续时间
	set_call_var 102 15

	;消耗
	set_call_var 103 50

	;冷却时间
	set_call_var 104 600

	;货币类型
	set_call_var 105 60
	
	call 301
endproc

proc 301
	;获取传递来的各个变量
	; 状态id
	strvar id
	var id @100
	; 状态等级
	strvar level
	var level @101
	; 持续时间
	strvar time
	var time @102
	; 消耗
	strvar cost
	var cost @103
	; 冷却时间
	strvar cool
	var cool @104
	; 货币类型
	strvar moneytype
	var moneytype @105
;301的后一部分在file中
	;判断冷却时间
	strvar sec sectip
	get_time_sec sec
	;camp2buffcool13,阵营2buff冷却时间
	var 13 0
	get_pworld_var 12 32 1 13
	if 13 > @sec
		sub 13 @sec
		add_sys_msg_start
		add_sys_msg 该操作还有$@13$秒的冷却时间,请稍候再试   
		set_sys_msg_mode 4 0 0
		msg_info
		return 
	endif
	
	;判断金币数量
	dec_money_test 0 @cost @moneytype 2
	if 2 = 0
		;扣金币
		dec_money 0 @cost @moneytype 2
		;加状态
		;TARGET_ADD_STATUS   #目标类型 #memid #statusID #statusLV #statusTM #返回值
		strvar memid
		get_mon_info 1 0 memid 0 0 0 0 0
		TARGET_ADD_STATUS 2 @memid @id @level @time 1
		;设置冷却时间
		add cool @sec
		;camp2buffcool13,阵营2buff冷却时间
		var 13 @cool
		set_pworld_var 12 32 @13 1
		add_sys_msg_start
		add_sys_msg $_ClanName$的$_Player$开启了枢纽保护 
		set_sys_msg_mode 4 0 0
		map_msg_info
		if cost > 0
			add_pworld_resource 0 1 5 1
		endif		
	else
		add_sys_msg_start
		add_sys_msg 金币不足,无法进行此操作   	
		set_sys_msg_mode 4 0 0
		msg_info
	endif
	
	
endproc
proc 302
endproc
proc 400
	choose 1 401 302 减少枢纽保护冷却时间300秒,消耗200 金币,一场战斗中只能使用一次,确认购买?
endproc
proc 401
	;判断冷却时间
	strvar sec sectip
	get_time_sec sec
	;camp2buffcoolreduce32,阵营2buff缩短冷却时间使用次数
	var 32 0
	get_pworld_var 88 32 1 32
	if 32 > 0
		add_sys_msg_start
		add_sys_msg 一场战斗中,此操作只能执行一次   
		set_sys_msg_mode 4 0 0
		msg_info
		return 
	endif
	;camp2buffcool13,阵营2buff冷却时间
	var 13 0
	get_pworld_var 12 32 1 13
	if 13 <= @sec
		add_sys_msg_start
		add_sys_msg 枢纽保护已冷却,不需要再进行此操作   
		set_sys_msg_mode 4 0 0
		msg_info
		return 
	endif
	sub 13 @sec
	sub 13 300
	if 13 < 0
		var 13 0
	endif
	add 13 @sec
	
	;判断金币数量
	dec_money_test 0 200 60 2
	if 2 = 0
		;扣金币
		dec_money 0 200 60 2
		
		;设置计数
		add 32 1
		;camp2buffcoolreduce32,阵营2buff缩短冷却时间使用次数
		
		set_pworld_var 88 32 @32 1
		;设置冷却时间
		;camp2buffcool13,阵营2buff冷却时间
		
		set_pworld_var 12 32 @13 1
		add_sys_msg_start
		add_sys_msg $_ClanName$的$_Player$将公会枢纽保护冷却时间缩短了300秒 
		set_sys_msg_mode 4 0 0
		map_msg_info
		add_pworld_resource 0 1 5 1
	else
		add_sys_msg_start
		add_sys_msg 金币不足,无法进行此操作   	
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc
