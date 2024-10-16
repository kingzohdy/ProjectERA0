
proc 2
	strvar rNum
	rand rNum 1000
	ID_PLAYER_BASE_ATT 0 1 1 41 1
	mul 41 @41
	
	if @rNum >= 0 and @rNum < 150
		add_sys_msg_start
		add_sys_msg 你消耗了一个<盛典火焰石>点燃飞天礼炮
		set_sys_msg_mode 3 0 0
		msg_info
			
		strvar x y
		GET_PLAYER_MAPINFO 1 1 1 x y
		effect 0 1 playEffct($@x$,$@y$,0,"effect\\entity\\9513_16.ent",15)
		
		GET_ID_PLAYER_INFO 0 19 13
		DEV 13 10000
		MUL 13 20
		ADD_EXP 0 @13 123
		if 123 < 0
			add_sys_msg_start
			add_sys_msg 您当前存储的经验值已经达到上限，请提升等级，否则无法继续获得经验
			set_sys_msg_mode 3 0 0
			msg_info
		else
			add_sys_msg_start
			add_sys_msg 你获到了$@13$点经验
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	elseif @rNum >= 150 and @rNum < 300
		add_sys_msg_start
		add_sys_msg 你消耗了一个<盛典火焰石>点燃飞天礼炮
		set_sys_msg_mode 3 0 0
		msg_info
		
		strvar x y
		GET_PLAYER_MAPINFO 1 1 1 x y
		effect 0 1 playEffct($@x$,$@y$,0,"effect\\entity\\352117.ent",15)
		
		dev 41 10
		ADD_MONEY 0 @41 21 123 ;单人加金钱
		if 123 < 0
			add_sys_msg_start
			add_sys_msg 您包裹中的绑银已经达到上限
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	elseif @rNum >= 300 and @rNum < 488
		add_sys_msg_start
		add_sys_msg 你消耗了一个<盛典火焰石>点燃飞天礼炮
		set_sys_msg_mode 3 0 0
		msg_info
		
		strvar x y
		GET_PLAYER_MAPINFO 1 1 1 x y
		effect 0 1 playEffct($@x$,$@y$,0,"effect\\entity\\352102.ent",15)
		
		add_sys_msg_start
		add_sys_msg 因为你的存在，世界更加炫丽多彩
		set_sys_msg_mode 3 0 0
		msg_info
	elseif @rNum >= 488 and @rNum < 688		
		add_sys_msg_start
		add_sys_msg 你消耗了一个<盛典火焰石>点燃飞天礼炮
		set_sys_msg_mode 3 0 0
		msg_info
		
		strvar aflag
		GET_PLAYER_ID 1 aflag
		if @aflag = -1
			return
		else
			;给目标添加状态
			TARGET_ADD_STATUS 1 @aflag 2104 1 60 90
			add_sys_msg_start
			add_sys_msg 你是电你是光，你是炫丽的烟花~~
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	elseif @rNum >= 688 and @rNum < 968
		add_sys_msg_start
		add_sys_msg 你消耗了一个<盛典火焰石>点燃飞天礼炮
		set_sys_msg_mode 3 0 0
		msg_info
		
		strvar x y
		GET_PLAYER_MAPINFO 1 1 1 x y
		effect 0 1 playEffct($@x$,$@y$,0,"effect\\entity\\9513_16.ent",15)
		
		strvar ret1
		GET_PACK_SPACE 0 ret1
		if @ret1 = 0
			add_sys_msg_start
			add_sys_msg 您的背包已经装不下更多东西了
			set_sys_msg_mode 3 0 0
			msg_info
			return
		endif
		
		ADD_BIND_ITEM_NUM_INPAK 5060050 10 1 14 
		if @14 = 0
			add_sys_msg_start
			add_sys_msg 烟花绚烂中英雄纪念币飞入你的背包
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	elseif @rNum >= 968 and @rNum < 988
		add_sys_msg_start
		add_sys_msg 你消耗了一个<盛典火焰石>点燃飞天礼炮
		set_sys_msg_mode 3 0 0
		msg_info
		
		strvar x y
		GET_PLAYER_MAPINFO 1 1 1 x y
		effect 0 1 playEffct($@x$,$@y$,0,"effect\\entity\\9513_16.ent",15)
		
		strvar ret1
		GET_PACK_SPACE 0 ret1
		if @ret1 = 0
			add_sys_msg_start
			add_sys_msg 您的背包已经装不下更多东西了
			set_sys_msg_mode 3 0 0
			msg_info
			return
		endif
		
		ADD_BIND_ITEM_NUM_INPAK 5060050 30 1 14 
		if @14 = 0
			add_sys_msg_start
			add_sys_msg 烟花绚烂中一大波英雄纪念币飞入你的背包
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	elseif @rNum >= 988 and @rNum < 992
		add_sys_msg_start
		add_sys_msg 你消耗了一个<盛典火焰石>点燃飞天礼炮
		set_sys_msg_mode 3 0 0
		msg_info
		
		strvar x y
		GET_PLAYER_MAPINFO 1 1 1 x y
		effect 0 1 playEffct($@x$,$@y$,0,"effect\\entity\\9513_16.ent",15)
		
		strvar ret1
		GET_PACK_SPACE 0 ret1
		if @ret1 = 0
			add_sys_msg_start
			add_sys_msg 您的背包已经装不下更多东西了
			set_sys_msg_mode 3 0 0
			msg_info
			return
		endif
		
		ADD_BIND_ITEM_NUM_INPAK 3651001 1 1 14 
		if @14 = 0
			add_sys_msg_start
			add_sys_msg $_player$ 正兴高采烈地燃放飞天礼炮，突然发现一个背饰：<武生靠旗>出现在眼前，羡煞众人！
			set_sys_msg_mode 18 0 0
			SVR_MSG_INFO
		endif
	elseif @rNum >= 992 and @rNum < 996
		add_sys_msg_start
		add_sys_msg 你消耗了一个<盛典火焰石>点燃飞天礼炮
		set_sys_msg_mode 3 0 0
		msg_info
		
		strvar x y
		GET_PLAYER_MAPINFO 1 1 1 x y
		effect 0 1 playEffct($@x$,$@y$,0,"effect\\entity\\9513_16.ent",15)
		
		strvar ret1
		GET_PACK_SPACE 0 ret1
		if @ret1 = 0
			add_sys_msg_start
			add_sys_msg 您的背包已经装不下更多东西了
			set_sys_msg_mode 3 0 0
			msg_info
			return
		endif
		
		ADD_BIND_ITEM_NUM_INPAK 3651003 1 1 14 
		if @14 = 0
			add_sys_msg_start
			add_sys_msg $_player$ 正兴高采烈地燃放飞天礼炮，突然发现一个背饰：<重弦>出现在眼前，羡煞众人！
			set_sys_msg_mode 18 0 0
			SVR_MSG_INFO
		endif
	elseif @rNum >= 996 and @rNum < 1000
		add_sys_msg_start
		add_sys_msg 你消耗了一个<盛典火焰石>点燃飞天礼炮
		set_sys_msg_mode 3 0 0
		msg_info
		
		strvar x y
		GET_PLAYER_MAPINFO 1 1 1 x y
		effect 0 1 playEffct($@x$,$@y$,0,"effect\\entity\\9513_16.ent",15)
		
		strvar ret1
		GET_PACK_SPACE 0 ret1
		if @ret1 = 0
			add_sys_msg_start
			add_sys_msg 您的背包已经装不下更多东西了
			set_sys_msg_mode 3 0 0
			msg_info
			return
		endif
		
		ADD_BIND_ITEM_NUM_INPAK 3650001 1 1 14 
		if @14 = 0
			add_sys_msg_start
			add_sys_msg $_player$ 正兴高采烈地燃放飞天礼炮，突然发现一个背饰：<铃铛猫>出现在眼前，羡煞众人！
			set_sys_msg_mode 18 0 0
			SVR_MSG_INFO
		endif
	else
	endif		
endproc

