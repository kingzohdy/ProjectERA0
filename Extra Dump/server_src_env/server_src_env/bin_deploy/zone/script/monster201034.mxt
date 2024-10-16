;祭祖神殿 ,德斯顿.阿萨曼,真身
proc 1
	strvar memid
	GET_MON_INFO 1 0 memid 0 0 0 0 0
	;TARGET_ADD_STATUS 3 @memid 3028 1 25920000 0 
	;TARGET_ADD_STATUS 3 @memid 3029 1 25920000 0 
	;TARGET_ADD_STATUS 3 @memid 3030 1 25920000 0 
	TARGET_ADD_STATUS 3 @memid 1095 1 25920000 0
	
	var 10 0
	set_mon_var 0 32 @10 0
endproc
proc 2
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;掉落包

	

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	range_set 4 5
	range_set 4 8
	new_npc 1 333045 14729 13113 0 pw105out.mac
	;level31,进度阶段
	var 31 4
	set_pworld_var 84 32 @31 1
	pworld_over
	OPEN_PWORLD_AWARD_UI
endproc
proc 4
	is_mon_fighting 1 2
	if 2 < 1
		return
	endif
	
	get_mon_var 0 32 0 10
	if 10 > 0
		return
	endif
	var 10 1
	set_mon_var 0 32 @10 0
	strvar ret defid memid hp mapdef map x y
	GET_MON_INFO ret defid memid hp mapdef map x y
	
	MON_SPEECH  @memid 1 2 0 附近的奴仆们！听从我的召唤,速度前来黑暗祭坛铲除这些愚蠢的自治联盟吧!
;	add_sys_msg_start
;	add_sys_msg ::boss4fighttip:: 
;	set_sys_msg_mode 1 0 0
;	map_msg_info
endproc
