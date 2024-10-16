;祭祖神殿 ,祭台
;出生
proc 1
	;是否祭台终点
	strvar mid hp
	GET_MON_INFO 1 0 mid hp 0 0 0 0
	sub hp 1
	mul hp -1
	;给祭台减血	
	mon_set_hp @hp @mid 1
	;祭坛的实例id
	VAR 14 @mid
	set_pworld_var 16 32 @14 1
	
	;上次tick的血量比例
	var 10 0
	set_mon_var 0 32 @10 1
endproc
;死亡
proc 2
	;祭坛的实例id
	VAR 14 0
	set_pworld_var 16 32 @14 1
endproc
;tick
proc 4
	return
	strvar mid percent
	GET_MON_INFO 1 0 mid 0 0 0 0 0
	get_mon_hp_percent @mid percent
	if percent >= 100
		get_pworld_var 8 32 1 12;
		add 12 1
		;进入祭台满血阶段
		set_pworld_var 8 32 @12 1;
		;全屏aoe
		mon_skill 10351 1
		;招boss来aoe
		return
	endif
	get_mon_var 0 32 1 10
	set_mon_var 0 32 @percent 1
	dev 10 20
	dev percent 20
	if 10 != @percent
		mul percent 20
		add_sys_msg_start
		add_sys_msg 警告!警告!!祭坛的灵魂能量超过$@percent$%，一旦祭坛的灵魂能量达到100%，影-杰克-耐罗将会完整降临!!请速度拦截出现的信仰的灵魂!!!
		set_sys_msg_mode 4 0 0
		map_msg_info
	endif
endproc
