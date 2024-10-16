;新手副本2.2

PROC 1

	NEW_MON 218000 2 22499 12791 0 0 0 1 2
	NEW_MON 218000 1 20817 11686 0 0 0 1 2
	NEW_MON 218000 2 17196 9787 0 0 0 1 2 
	NEW_MON 218000 2 18147 8980 0 0 0 1 2
	NEW_MON 218000 1 17912 10014 0 0 0 1 2
	NEW_MON 218000 2 17273 8880 100 0 0 1 2
	NEW_MON 200009 5 24672 7315 300 0 0 1 2
	NEW_MON 200009 5 26272 7315 300 0 0 1 2
	NEW_MON 200009 5 25472 6515 300 0 0 1 2
	NEW_MON 200009 5 25472 8115 300 0 0 1 2

	
		
	    NEW_TMP_ITEM 1420013 20750 11098 0 0 0 1
	    NEW_TMP_ITEM 1420013 24330 7703 0 0 0 1
	    NEW_NPC 1 302006 17966 8774 45 luoge.mac
	;10为杀怪数，11为阶段标志，12为延时计数,13为死亡怪物id
	VAR 10 0
	SET_PWORLD_VAR 0 32 @10 1
	VAR 11 0 
	SET_PWORLD_VAR 4 32 @11 1
	VAR 12 0 
	SET_PWORLD_VAR 8 32 @12 1
	var 13 0
	set_pworld_var 12 32 @13 1
	var 14 0
	set_pworld_var 16 32 @14 1
	var 15 0
	set_pworld_var 20 32 @15 1

	;新增
	var 16 0 
	set_pworld_var 24 8 @16 1
	;风行者数量
	var 17 0
	set_pworld_var 25 8 @17 1
	
	var 18 0
	set_pworld_var 26 32 @18 1
ENDPROC

PROC 3
	strvar x y m mid
		
;	GET_PLAYER_MAPINFO 0 0 0 x y 
	get_pworld_var 26 32 1 18 
	GET_ID_PLAYER_INFO @18 3 x
	GET_ID_PLAYER_INFO @18 4 y
	GET_POINT_DIST @x @y 17966 8774 m
	
	
	if m <= 1000	
		get_pworld_var 24 8 1 16
		if 16 = 0

			get_pworld_var 25 8 1 17 
		;	GET_MON_NUM_BY_DEFID 218000 20
		;	GET_MAP_NPC_ID 302006 mid 1
			if 17 = 6
				;杀死全部的风行者
				ADD_SYS_MSG_START
				ADD_SYS_MSG 接下来我们就要去挑战哥布林王了，你准备好了就点我！
				set_sys_msg_mode 3 0 0
				MAP_MSG_INFO
			else
			
				ADD_SYS_MSG_START
				ADD_SYS_MSG 把周围的风行者都杀光，然后我就带你去挑战哥布林王！
				set_sys_msg_mode 3 0 0
				MAP_MSG_INFO
			endif
			
			var 16 1
			set_pworld_var 24 8 @16 1
		endif
	else
		var 16 0
		set_pworld_var 24 8 @16 1
	endif
	
	; ADD_SYS_MSG_START
	; ADD_SYS_MSG $@m$ $@16$ $@17$
	; set_sys_msg_mode 1 0 0
	; MAP_MSG_INFO
	
	
	
	GET_PWORLD_VAR 0 32 1 10
	;if @10 = 10
	;get_pworld_var 16 32 1 14
		;add 14 1
		;set_pworld_var 16 32 @14 1
		;if @14 = 3
			;ADD_SYS_MSG_START
			;ADD_SYS_MSG 点击飞龙前往飞羽峰深处，击杀哥布林王
			;set_sys_msg_mode 3 0 0
			;MAP_MSG_INFO
			;var 14 0
			;set_pworld_var 16 32 @14 1
		;endif
	;endif

	get_pworld_var 4 32 1 11
	get_pworld_var 8 32 1 12
	IF 11 = 2
		ADD 12 1
		SET_PWORLD_VAR 8 32 @12 1
		GET_PWORLD_VAR 8 32 1 12
			if 12 = 1
				new_npc 1 333003 26349 7823 0 zhu30out.mac
				ADD_SYS_MSG_START
				ADD_SYS_MSG 副本已完成，可从传送门离开，副本将在20秒后关闭
				set_sys_msg_mode 4 0 0
				MAP_MSG_INFO
			endif
			if 12 = 6
				ADD_SYS_MSG_START
				ADD_SYS_MSG 副本已完成，可从传送门离开，副本将在10秒后关闭
				set_sys_msg_mode 4 0 0
				MAP_MSG_INFO
			endif
			if 12 = 11
					COMPEL_CLOSE_PWORLD 1
					VAR 11 3
			endif
		SET_PWORLD_VAR 8 32 @12 1
		SET_PWORLD_VAR 4 32 @11 1
	ENDIF	



ENDPROC

PROC 4
	
	GET_PWORLD_VAR 0 32 1 10
	GET_PWORLD_VAR 4 32 1 11
	ADD 10 1      
	SET_PWORLD_VAR 0 32 @10 1
		;IF 10 = 10 and 11 = 0
			;NEW_NPC 1 302006 17966 8774 45 luoge.mac
			;NEW_TMP_ITEM 1420000 18800 8892 0 0 0 1
			;ADD_SYS_MSG_START
			;ADD_SYS_MSG 点击飞龙前往飞羽峰深处，击杀哥布林王
			;set_sys_msg_mode 3 0 0
			;MAP_MSG_INFO
			;var 11 1
			;SET_PWORLD_VAR 4 32 @11 1
		;ENDIF
	
		;if 10 = 30 and 11 = 1
			;new_mon 282008 1 26191 7396 0 0 0 0 23
			;var 11 2
			;SET_PWORLD_VAR 4 32 @11 1
		;endif

	GET_MON_INFO 0 13 0 0 0 0 19 20
	set_pworld_var 12 32 @13 1
	get_pworld_var 20 32 1 15
		if 13 = 218000
			
			add 17 1
			set_pworld_var 25 8 @17 1
		
		endif
	
	
	
		if @13 = 200009
			add 15 1
			set_pworld_var 20 32 @15 1
		endif
		get_pworld_var 20 32 1 15
		if @15 = 20 and @11 = 0
			new_mon 282008 1 26191 7396 0 0 0 0 23
			var 11 1
			SET_PWORLD_VAR 4 32 @11 1
		endif
		if @13 = 282008 and @11 = 1
			var 11 2
			SET_PWORLD_VAR 4 32 @11 1
			add 19 100
			add 20 100
			TASK_STAT 1062 1 30
			;if 30 = 2
			;NEW_MON 293026 1 @19 @20 0 0 0 1 2
			;ADD_SYS_MSG_START
			;ADD_SYS_MSG 哥布林王的坐骑上掉下一个金光闪闪的宝箱
			;set_sys_msg_mode 4 0 0
			;MAP_MSG_INFO
			;endif
		endif
	
ENDPROC

proc 5
	effect 2 0 playEffct(24100,12800,"effect\\entity\\9597_16.ent")
	
	
	
	GET_PLAYER_ID 1 18
	set_pworld_var 26 32 @18 1
	
	
endproc