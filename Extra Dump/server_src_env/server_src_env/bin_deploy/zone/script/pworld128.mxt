;50级英雄副本

PROC 1
	add_sys_msg_start
	add_sys_msg 世人只知黄金的光彩夺目，却看不见矿井深处无数淘金者的森森枯骨
	set_sys_msg_mode 4 0 0
	map_msg_info
	


	var 21 0
	set_pworld_var 0 32 @21 1
	var 22 0
	set_pworld_var 4 32 @22 1
	var 23 0
	set_pworld_var 8 32 @23 1
	var 24 0
	set_pworld_var 16 32 @24 1
	var 25 0
	set_pworld_var 32 32 @25 1
	var 10 0
	set_pworld_var 12 32 @10 1
	var 11 0
	set_pworld_var 20 32 @11 1
	VAR 12 0
	set_pworld_var 24 32 @12 1
	var 26 0
	set_pworld_var 36 32 @26 1
	var 27 0
	set_pworld_var 40 32 @27 1
	var 28 0
	set_pworld_var 44 32 @28 1

	new_mon_by_dir 294008 1 11974 19233 0 0 0 82 1 27
	set_pworld_var 40 32 @27 1
	new_mon_by_dir 294008 1 20268 21142 0 0 0 114 1 28
	set_pworld_var 44 32 @28 1
	;add_sys_msg_start
	;add_sys_msg 27=$@27$ 28=$@28$ 
	;set_sys_msg_mode 4 0 0
	;map_msg_info
	

	NEW_MON 206464 1 12339 21357 0 296 0 1 2
	NEW_MON 206464 1 13071 21283 0 297 0 1 2
	NEW_MON 206464 1 20839 21424 0 298 0 1 2
	NEW_MON 206464 1 12154 18401 0 299 0 1 2

	new_mon 204019 1 23831 10835 0 0 0 1 2



	get_map_mon_id 206454 13 1
	set_pworld_var 28 32 @13 1
	get_pworld_var 28 32 1 13
	TARGET_ADD_STATUS 3 @13 1095 1 7200 2
	get_map_mon_id 206455 13 1
	set_pworld_var 28 32 @13 1
	get_pworld_var 28 32 1 13
	TARGET_ADD_STATUS 3 @13 1095 1 7200 2
	get_map_mon_id 206457 13 1
	set_pworld_var 28 32 @13 1
	get_pworld_var 28 32 1 13
	TARGET_ADD_STATUS 3 @13 1095 1 7200 2
	get_map_mon_id 206463 13 1
	set_pworld_var 28 32 @13 1
	get_pworld_var 28 32 1 13
	TARGET_ADD_STATUS 3 @13 1095 1 7200 2
	

	;将两个BOSS设定到组22
	GET_MAP_MON_ID 206454 2 1
	SET_MON_GROUP 1 @2 22
	GET_MAP_MON_ID 206455 2 1
	SET_MON_GROUP 1 @2 22

	;将所有组22 的怪物链接起来
	LINK_MON 1 22
	if 1 < 0
	add_sys_msg_start
	add_sys_msg 怪物加入怪物链失败
	set_sys_msg_mode 4 0 0
	map_msg_info
	endif

	



ENDPROC

PROC 3
	

	;get_pworld_var 12 32 1 10
	;get_pworld_var 0 32 1 21
	;get_pworld_var 24 32 1 26
	;add_sys_msg_start
	;add_sys_msg 21=$@21$ 
	;set_sys_msg_mode 4 0 0
	;map_msg_info
	
	get_map_mon_id 206454 13 1
	set_pworld_var 28 32 @13 1
	get_pworld_var 28 32 1 13
	TARGET_ADD_STATUS 3 @13 1095 1 7200 2
	get_map_mon_id 206455 13 1
	set_pworld_var 28 32 @13 1
	get_pworld_var 28 32 1 13
	TARGET_ADD_STATUS 3 @13 1095 1 7200 2

	get_pworld_var 0 32 1 21
	get_pworld_var 36 32 1 26
	if 21 = 2
		add 26 1
		set_pworld_var 36 32 @26 1
		if 26 = 1
			add_sys_msg_start
			add_sys_msg 来访者身份验证，数据检查中。。。
			set_sys_msg_mode 4 0 0
			map_msg_info
		endif
		if 26 = 2
			add_sys_msg_start
			add_sys_msg 身份验证失败，启动防御模式
			set_sys_msg_mode 4 0 0
			map_msg_info
			del_npc 332001 1
			NEW_MON 206456 1 23256 8010 0 0 0 1 13
			set_pworld_var 28 32 @13 1
			get_pworld_var 28 32 1 13
			TARGET_ADD_STATUS 3 @13 1095 1 7200 2
		endif



	endif

	


ENDPROC

PROC 4
	;get_pworld_var 24 32 1 12
	;add 12 1
	;set_pworld_var 24 32 @12 1

	GET_MON_INFO 0 10 0 0 0 0 1 1
	set_pworld_var 12 32 @10 1
	get_pworld_var 24 32 1 12

	if 10 = 206463
	get_pworld_var 40 32 1 27
	del_id_mon @27 1
	;if 1 = 0
	;add_sys_msg_start
	;add_sys_msg 27=$@27$,怪物删除失败
	;set_sys_msg_mode 4 0 0
	;map_msg_info
	;endif
	endif
	
	if 10 = 206454
	PWORLD_LINKED_MON_INFO 2 22 1
	if @2 < 1
		new_npc 1 333003 12807 10558 0 zhu50out.mac
		PWORLD_BOX_START 0
		pworld_over
		var 21 10
			set_pworld_var 0 32 @21 1
			OPEN_PWORLD_AWARD_UI 
			
		add_sys_msg_start
			add_sys_msg 副本已完成，可通过传送门离开
			set_sys_msg_mode 4 0 0
			map_msg_info
	endif
	endif

	if 10 = 206455
	PWORLD_LINKED_MON_INFO 2 22 1
	if @2 < 1
		new_npc 1 333003 12807 10558 0 zhu50out.mac
		PWORLD_BOX_START 0
		pworld_over
		var 21 10
			set_pworld_var 0 32 @21 1
			OPEN_PWORLD_AWARD_UI 
			
		add_sys_msg_start
			add_sys_msg 副本已完成，可通过传送门离开
			set_sys_msg_mode 4 0 0
			map_msg_info
	endif
	endif
		
	if 10 = 206457
	get_pworld_var 44 32 1 28
	del_id_mon @28 1
	endif
	
	
	get_pworld_var 32 32 1 25
	GET_MON_INFO 0 30 0 0 0 0 1 1
	If 30 = 206451
	mul 25 1
	endif
	
ENDPROC

proc 5
	effect 2 0 playEffct(6800,1700,"effect\\entity\\9599_16.ent")
endproc
