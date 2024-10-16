;公会守护枢纽活动    ,购买buff
proc 1
	call 100
;	DIALOG 1
;	add_talk $_npc$:
;	add_talk #b#b最近出现在附近的敌人越来越多，越来越强，如果你们觉得防守吃力的话可以来我这，我能给你们提供一定的帮助。
;	add_talk #b#b机械状态显示在机械技能栏上方
;	is_pworld_over 1
;	if 1 = 0
;		option 100 #强化使用的机械
;
;	endif
;
;	talk
endproc
proc 100
	DIALOG 1
	add_talk $_npc$:
;	add_talk #b#b希望驾驶的机械更加强力吗？我这提供各种属性提升的途径。
;	add_talk #b#b5到10级的状态可在背包内使用道具重复获得
	add_talk #b#b最近出现在附近的敌人越来越多，越来越强，如果你们觉得防守吃力的话可以来我这，我能给你们提供一定的帮助。
	add_talk #b#b#c186506提示:机械状态显示在机械技能栏上方#n
	is_pworld_over 1
	if 1 = 0
		strvar level time costtype cost itemid itemnum ret statusID
		var level 0
		get_role_ride_on_machine_mem_id 20 0
		;GET_ID_MON_STATUS_INFO  #怪物实例ＩＤ #状态ID #返回标志 #等级 #叠加层数 #施加者ID #特殊信息（暂时无用）
		;GET_MACHINE_PWORLD_STATUS_INFO #返回标志 #状态ID #状态等级 #状态时间 #金钱类型 #价格 #物品id #物品数量
		var level 0
		get_id_mon_status_info @20 3167 3 level 0 0 0		
		add level 1
		GET_MACHINE_PWORLD_STATUS_INFO ret 3167 @level time costtype cost itemid itemnum removeitem removenum
		if ret = 0
			option 110 毁灭力$@level$级（购买后增加攻击力）
		endif
		var level 0
		get_id_mon_status_info @20 3168 3 level 0 0 0		
		add level 1
		GET_MACHINE_PWORLD_STATUS_INFO ret 3168 @level time costtype cost itemid itemnum removeitem removenum
		if ret = 0
			;option 120 钢甲$@level$级（购买后增加血量上限）
		endif
		var level 0
		get_id_mon_status_info @20 3169 3 level 0 0 0		
		add level 1
		GET_MACHINE_PWORLD_STATUS_INFO ret 3169 @level time costtype cost itemid itemnum removeitem removenum
		if ret = 0
			option 130 机甲盾$@level$级（购买后增加防御）
		endif
	endif
	talk
endproc
proc 110
	strvar level time costtype cost itemid itemnum ret statusID
	get_role_ride_on_machine_mem_id 20 0
	var level 0
	get_id_mon_status_info @20 3167 3 level 0 0 0		
	add level 1
	GET_MACHINE_PWORLD_STATUS_INFO ret 3167 @level time costtype cost itemid itemnum removeitem removenum
	set_money_type 1 @costtype
	if ret = 0
		choose 2 111 112 购买毁灭力$@level$级需要消耗$@cost$$_MoneyType$，确认购买？
	endif
endproc
proc 111
	strvar level time costtype cost itemid itemnum ret statusID removeitem removenum
	get_role_ride_on_machine_mem_id 20 0
	var level 0
	get_id_mon_status_info @20 3167 3 level 0 0 0		
	add level 1
	GET_MACHINE_PWORLD_STATUS_INFO ret 3167 @level time costtype cost itemid itemnum removeitem removenum
	if ret = 0
		set_call_var time @time
		set_call_var costtype @costtype
		set_call_var cost @cost
		set_call_var itemid @itemid
		set_call_var itemnum @itemnum
		set_call_var statusID 3167
		set_call_var level @level
		set_call_var removeitem @removeitem
		set_call_var removenum @removenum
		call 140
	endif
endproc
proc 112
endproc
proc 140
	strvar level time costtype cost itemid itemnum ret statusID ret1  removeitem removenum
	get_role_ride_on_machine_mem_id 20 0
	if 20 <= 0
		add_sys_msg_start
		add_sys_msg 购买失败,需要骑乘机械才能购买  
		set_sys_msg_mode 1 0 0
		msg_info
		return
	endif
	DEC_MONEY_TEST 0 @cost @costtype 1
	set_money_type 2 @costtype
	if 1 != 0
		add_sys_msg_start
		add_sys_msg $_MoneyType$不足,无法进行此操作  
		set_sys_msg_mode 1 0 0
		msg_info
		return
	endif
	
	if itemnum > 0 and itemid != 0
		add_item_num_inpak @itemid @itemnum 2 ret1
		if ret1 != 0
			add_sys_msg_start
			add_sys_msg 背包已满,无法进行此操作  
			set_sys_msg_mode 1 0 0
			msg_info
			return
		endif
		add_item_num_inpak  @itemid @itemnum 0 ret1
	endif
	DEC_ITEM_NUM_INPAK @removeitem @removenum 1
	dec_money 0  @cost @costtype 1
	TARGET_ADD_STATUS 3 @20 @statusID @level @time 1
	
	call 100
endproc
proc 120
	strvar level time costtype cost itemid itemnum ret statusID
	get_role_ride_on_machine_mem_id 20 0
	var level 0
	get_id_mon_status_info @20 3168 3 level 0 0 0		
	add level 1
	GET_MACHINE_PWORLD_STATUS_INFO ret 3168 @level time costtype cost itemid itemnum removeitem removenum
	set_money_type 1 @costtype
	if ret = 0
		choose 2 121 122 购买钢甲$@level$级需要消耗$@cost$$_MoneyType$，确认购买？
	endif
endproc
proc 121
	strvar level time costtype cost itemid itemnum ret statusID
	get_role_ride_on_machine_mem_id 20 0
	var level 0
	get_id_mon_status_info @20 3168 3 level 0 0 0		
	add level 1
	GET_MACHINE_PWORLD_STATUS_INFO ret 3168 @level time costtype cost itemid itemnum removeitem removenum
	if ret = 0
		set_call_var time @time
		set_call_var costtype @costtype
		set_call_var cost @cost
		set_call_var itemid @itemid
		set_call_var itemnum @itemnum
		set_call_var statusID 3168
		set_call_var level @level
		call 140
	endif
endproc
proc 122
endproc
proc 130
	strvar level time costtype cost itemid itemnum ret statusID
	get_role_ride_on_machine_mem_id 20 0
	var level 0
	get_id_mon_status_info @20 3169 3 level 0 0 0		
	add level 1
	GET_MACHINE_PWORLD_STATUS_INFO ret 3169 @level time costtype cost itemid itemnum removeitem removenum
	set_money_type 1 @costtype
	if ret = 0
		choose 2 131 132 购买机甲盾$@level$级需要消耗$@cost$$_MoneyType$，确认购买？
	endif
endproc
proc 131
	strvar level time costtype cost itemid itemnum ret statusID
	get_role_ride_on_machine_mem_id 20 0
	var level 0
	get_id_mon_status_info @20 3169 3 level 0 0 0		
	add level 1
	GET_MACHINE_PWORLD_STATUS_INFO ret 3169 @level time costtype cost itemid itemnum removeitem removenum
	if ret = 0
		set_call_var time @time
		set_call_var costtype @costtype
		set_call_var cost @cost
		set_call_var itemid @itemid
		set_call_var itemnum @itemnum
		set_call_var statusID 3169
		set_call_var level @level
		call 140
	endif
endproc
proc 132
endproc
